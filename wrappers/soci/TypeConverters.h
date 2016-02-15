
#ifndef TypeConverters___h
#define TypeConverters___h

#include <soci.h>

#include <ql/time/date.hpp>
#include <ctime>

template <>
struct soci::type_conversion<QuantLib::Date>
{
	typedef std::tm base_type;

	static void from_base(
		base_type const& in, indicator ind, QuantLib::Date & out)
	{
		out = QuantLib::Date(
			in.tm_mday,
			(QuantLib::Month)(in.tm_mon + 1),
			in.tm_year + 1900);
	}

    static void to_base(
        QuantLib::Date const& in, base_type & out, indicator & ind)
    {
		out.tm_year = in.year() - 1900;
		out.tm_mon = in.month() - 1;
		out.tm_mday = in.dayOfMonth();
		out.tm_hour = 0;
		out.tm_min = 0;
		out.tm_sec = 0;

		ind = i_ok;
    }
};


#endif
