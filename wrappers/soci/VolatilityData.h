#pragma once

#ifndef VolatilityData___h
#define VolatilityData___h

#include <ql/time/date.hpp>
#include <ql/math/matrix.hpp>
#include <vector>

namespace lib
{

struct VolatilitySnapshot
{
	VolatilitySnapshot() {}
	VolatilitySnapshot(VolatilitySnapshot const& s) {
		Date = s.Date;
		QuoteType = s.QuoteType;
		SourceId = s.SourceId;
		Product = s.Product;
		Factor = s.Factor;
		IsSkew = s.IsSkew;
	}

	QuantLib::Date Date;
	std::string QuoteType;
	short SourceId;
	std::string Product;
	double Factor;
	bool IsSkew;
};

struct SwaptionSurfaceData
{
	// Option tenors in months
	std::vector<int> OptionTenors;
	// Swap tenors in months
	std::vector<int> SwapTenors;
	// change to Disposable
	QuantLib::Matrix Volatility;

	bool valid() const {
		return !OptionTenors.empty() && !SwapTenors.empty() &&
			OptionTenors.size() == Volatility.rows() &&
			SwapTenors.size() == Volatility.columns();
	}
};

struct SwaptionSkewData
{
	SwaptionSkewData() {}

	SwaptionSkewData(int optionTenor, int swapTenor,
		const std::vector<int>& moneyness,
		const std::vector<double>& skews) {
		OptionTenor = optionTenor;
		SwapTenor = swapTenor;
		Moneyness = moneyness;
		Skews = skews;
	}

	// Option tenor in months
	int OptionTenor;
	// Swap tenor in months
	int SwapTenor;
	// Moneyness in basis points
	std::vector<int> Moneyness;
	// Differences between the OTM vols and the ATM vol
	std::vector<double> Skews;
};

}

#endif

