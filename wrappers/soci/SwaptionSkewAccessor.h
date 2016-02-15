#pragma once

#ifndef SwaptionSkewAccessor___h
#define SwaptionSkewAccessor___h

#include "VolatilityAccessor.h"
#include "VolatilityData.h"

#include <boost/shared_ptr.hpp>
#include <ql/patterns/singleton.hpp>


namespace lib
{
class SwaptionSkewAccessor : 
	public VolatilityAccessor,
	public QuantLib::Singleton<SwaptionSkewAccessor> 
{        
	friend class QuantLib::Singleton<SwaptionSkewAccessor>;

public:

	boost::shared_ptr<std::vector<SwaptionSkewData>>
	load(VolatilitySnapshot const& snapshot) const;

	void
	add(VolatilitySnapshot const& snapshot,
		boost::shared_ptr<std::vector<SwaptionSkewData>> data) const;

	void 
	update(VolatilitySnapshot const& snapshot,
		   boost::shared_ptr<std::vector<SwaptionSkewData>> data) const;

private:
	SwaptionSkewAccessor();

};

}
#endif