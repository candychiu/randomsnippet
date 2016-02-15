#include "SwaptionSkewAccessor.h"

#include <soci-odbc.h>

#include "data.h"
#include "macro.h"
#include "MappingObjects.h"
#include "TypeConverters.h"

#include <algorithm>
#include <sstream>
#include <exception>

using namespace soci;
using namespace lib::Data_details;
using namespace QuantLib;

namespace
{
using namespace lib;

struct CountPoints 
{
	size_t operator()(size_t total, SwaptionSkewData const& v) {
		return total + v.Moneyness.size();
	}
};

void add(session& sql, _DB_VolatilitySnapshot const& snapshot, std::vector<SwaptionSkewData> const& data, size_t size) {

	// insert data points
	std::vector<short> optionTenors(size);
	std::vector<short> swapTenors(size);
	std::vector<short> strikeSpreads(size);
	std::vector<double> spreads(size);

	size_t i = 0;
	for (auto iSkew = data.cbegin(); iSkew != data.cend(); ++iSkew) {
		auto m = iSkew->Moneyness.cbegin();
		auto s = iSkew->Skews.cbegin();

		for (; m != iSkew->Moneyness.cend() && s != iSkew->Skews.cend(); ++m, ++s) {
			optionTenors[i] = static_cast<short>(iSkew->OptionTenor);
			swapTenors[i] = static_cast<short>(iSkew->SwapTenor);
			strikeSpreads[i] = static_cast<short>(*m);
			spreads[i] = *s * snapshot.Factor;
			i++;
		}
	}

	statement st = (sql.prepare
		<< "INSERT INTO VolatilitySkewPoint "
		<< "VALUES (" << snapshot.Id << ", :OptionTenor, :SwapTenor, :StrikeSpread, :Spread) "
		, use(optionTenors)
		, use(swapTenors)
		, use(strikeSpreads)
		, use(spreads)
	);
	st.execute();
}

}

namespace lib
{

namespace Data_details 
{
const std::string from<SwaptionSkewAccessor>::value = 
	"FROM VolatilitySnapshot s, VolatilitySkewPoint p WHERE s.Id = p.SnapshotId AND ";
}

SwaptionSkewAccessor::SwaptionSkewAccessor()
{
}

boost::shared_ptr<std::vector<SwaptionSkewData> >
SwaptionSkewAccessor::load(VolatilitySnapshot const& snapshot) const
{
	boost::shared_ptr<std::vector<SwaptionSkewData> > data( 
		new std::vector<SwaptionSkewData>());

	DATA_TRY;

	session sql(backEndFactory(), connectionStr());

	_DB_VolatilitySnapshot dbSnapshot(snapshot);
	dbSnapshot.IsSkew = true;

	const int BATCH_SIZE = 100;
	std::vector<short> optionTenors(BATCH_SIZE);
	std::vector<short> swapTenors(BATCH_SIZE);
	std::vector<short> strikeSpreads(BATCH_SIZE);
	std::vector<double> spreads(BATCH_SIZE);

	statement st = (sql.prepare
		<< "SELECT p.OptionTenor, p.SwapTenor, p.StrikeSpread, "
		<< "     (CAST(p.Spread AS FLOAT) / CAST(s.Factor AS FLOAT)) AS Spread "
		<< from<SwaptionSkewAccessor>::value << conditions::value
		<< "ORDER BY p.OptionTenor, p.SwapTenor, p.StrikeSpread "
		, use(dbSnapshot)
		, into(optionTenors)
		, into(swapTenors)
		, into(strikeSpreads)
		, into(spreads)
	);

	// CHECK hasData is false even there's data returned by fetch()
	bool hasData = st.execute();
	//if (!hasData) return data;

	int lastOptionTenor = -1;
	int lastSwapTenor = -1;

	while (st.fetch()) {

		auto ot = optionTenors.cbegin();
		auto st = swapTenors.cbegin();
		auto ss = strikeSpreads.cbegin();
		auto s = spreads.cbegin();

		for (; ot != optionTenors.cend() && st != swapTenors.cend() &&
				ss != strikeSpreads.cend() && s != spreads.cend();
				++ot, ++st, ++ss, ++s) {

			// new skew
			if (*ot != lastOptionTenor || *st != lastSwapTenor) {
				data->push_back(SwaptionSkewData());
				data->rbegin()->OptionTenor = *ot;
				data->rbegin()->SwapTenor = *st;
			}
			data->rbegin()->Moneyness.push_back(*ss);
			data->rbegin()->Skews.push_back(*s);

			lastOptionTenor = *ot;
			lastSwapTenor = *st;
		}
	}
	
	SOCI_ODBC_CATCH;

	return data;
}

void
SwaptionSkewAccessor::add(VolatilitySnapshot const& snapshot,
	boost::shared_ptr<std::vector<SwaptionSkewData>> data) const
{
	// CHECK encountered a memory leak when bulk insert size is 0.
	size_t size = std::accumulate(data->cbegin(), data->cend(), 0, CountPoints());
	if (size == 0) return;
	
	DATA_TRY;

	session sql(backEndFactory(), connectionStr());

	{
		transaction tr(sql);

		// insert a snapshot
		_DB_VolatilitySnapshot dbSnapshot(snapshot);
		dbSnapshot.IsSkew = true;

		sql.once 
			<< "INSERT INTO VolatilitySnapshot (QuoteType, SourceId, Date, ConvertedFromId, Product, Factor, IsSkew) "
			<< "OUTPUT inserted.Id "
			<< "VALUES (:QuoteType, :SourceId, :Date, :ConvertedFromId, :Product, :Factor, :IsSkew) "
			, into(dbSnapshot.Id)
			, use(dbSnapshot);

		::add(sql, dbSnapshot, *data, size);

		tr.commit();
	}

	SOCI_ODBC_CATCH;

}

void 
SwaptionSkewAccessor::update(VolatilitySnapshot const& snapshot,
		boost::shared_ptr<std::vector<SwaptionSkewData>> data) const
{
	// CHECK encountered a memory leak when bulk insert size is 0.
	size_t size = std::accumulate(data->cbegin(), data->cend(), 0, CountPoints());
	if (size == 0) return;
	
	DATA_TRY;

	session sql(backEndFactory(), connectionStr());

	{
		transaction tr(sql);

		// insert a snapshot
		_DB_VolatilitySnapshot dbSnapshot(snapshot);
		dbSnapshot.IsSkew = true;

		// find the snapshot
		sql.once << "SELECT s.Id " 
			<< from<SwaptionSkewAccessor>::value << conditions::value 
			, use(dbSnapshot)
			, into(dbSnapshot.Id);
		if (!dbSnapshot.validId()) {
			return;
		}

		sql.once << "DELETE p " 
			<< from<SwaptionSkewAccessor>::value << conditions::value
			, use(dbSnapshot);

		// insert data points
		::add(sql, dbSnapshot, *data, size);

		tr.commit();
	}

	SOCI_ODBC_CATCH;
}





}
