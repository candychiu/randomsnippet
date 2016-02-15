
#ifndef Data_MappingObjects___h
#define Data_MappingObjects___h

#include <soci.h>

#include "TypeConverters.h"
#include "VolatilityData.h"
#include <boost/optional.hpp>
#include <boost-optional.h>
#include <ql/time/date.hpp>
#include <string>
#include <limits>

#pragma warning(push)
#pragma warning(disable:4800)	// disable short/bool conversion warning

namespace lib { namespace Data_details
{
struct _DB_VolatilitySnapshot : VolatilitySnapshot
{
	long long Id;
	boost::optional<long long> ConvertedFromId;

	_DB_VolatilitySnapshot() 
		:Id(std::numeric_limits<long long>::min()) {}
	_DB_VolatilitySnapshot(VolatilitySnapshot const& snapshot)
		:VolatilitySnapshot(snapshot),
		 Id(std::numeric_limits<long long>::min()) {}
	bool validId() const {
		return Id != std::numeric_limits<long long>::min(); 
	}
};

struct _DB_DiscountCurveSnapshot : DiscountCurveSnapshot
{
	long long Id;

	_DB_DiscountCurveSnapshot() 
		:Id(std::numeric_limits<long long>::min()) {}
	_DB_DiscountCurveSnapshot(DiscountCurveSnapshot const& snapshot)
		:DiscountCurveSnapshot(snapshot),
		 Id(std::numeric_limits<long long>::min()) {}
	bool validId() const {
		return Id != std::numeric_limits<long long>::min(); 
	}
};

}}

template <>
struct soci::type_conversion<lib::Data_details::_DB_VolatilitySnapshot>
{
	typedef values base_type;

	static void from_base(values const & v, indicator /* ind */, lib::Data_details::_DB_VolatilitySnapshot & p) {

		p.Id = v.get<long long>("Id");
		p.Date = v.get<QuantLib::Date>("Date");
		p.QuoteType = v.get<std::string>("QuoteType");
		p.SourceId = v.get<short>("SourceId");
		p.Product = v.get<std::string>("Product");
		p.Factor = v.get<double>("Factor");
		p.IsSkew = static_cast<bool>(v.get<short>("IsSkew"));
		p.ConvertedFromId = v.get<boost::optional<long long> >("ConvertedFromId");
    }

    static void to_base(const lib::Data_details::_DB_VolatilitySnapshot & p, values & v, indicator & ind) {

		v.set("Id", p.Id);		
		v.set("Date", p.Date);
		v.set("QuoteType", p.QuoteType);
		v.set("SourceId", p.SourceId);
		v.set("Product", p.Product);
		v.set("Factor", p.Factor);
		v.set("IsSkew", static_cast<short>(p.IsSkew));
		v.set("ConvertedFromId", p.ConvertedFromId);
	}
};

template <>
struct soci::type_conversion<lib::Data_details::_DB_DiscountCurveSnapshot>
{
	typedef values base_type;

	static void from_base(values const & v, indicator /* ind */, lib::Data_details::_DB_DiscountCurveSnapshot & p) {

		p.Id = v.get<long long>("Id");
		p.Date = v.get<QuantLib::Date>("Date");
		p.Type = v.get<std::string>("Type");
		p.SourceId = v.get<short>("SourceId");
		p.Environment = v.get<std::string>("Environment");
    }

    static void to_base(const lib::Data_details::_DB_DiscountCurveSnapshot & p, values & v, indicator & ind) {

		v.set("Id", p.Id);		
		v.set("Date", p.Date);
		v.set("Type", p.Type);
		v.set("SourceId", p.SourceId);
		v.set("Environment", p.Environment);
	}
};

template <>
struct soci::type_conversion<lib::GenericTradeValuation>
{
	typedef values base_type;

	static void from_base(values const & v, indicator /* ind */, lib::GenericTradeValuation & p) {

		p.Id = v.get<std::string>("Id");
		p.ValuationDate = v.get<QuantLib::Date>("ValuationDate");
		p.ScenarioId = v.get<int>("ScenarioId");
		p.HorizonId = v.get<short>("HorizonId");
		p.Price = v.get<double>("Price");
		p.Source = v.get<std::string>("Source");
    }

    static void to_base(const lib::GenericTradeValuation & p, values & v, indicator & ind) {

		v.set("Id", p.Id);		
		v.set("ValuationDate", p.ValuationDate);
		v.set("ScenarioId", p.ScenarioId);
		v.set("HorizonId", p.HorizonId);
		v.set("Price", p.Price);
		v.set("Source", p.Source);
	}
};

#pragma warning(pop)

#endif
