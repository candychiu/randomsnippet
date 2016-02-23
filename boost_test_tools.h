
#ifndef BOOST_TEST_TOOLS_UTIL_H_
#define BOOST_TEST_TOOLS_UTIL_H_

#include <boost/test/test_tools.hpp>
#include <iterator>

template <typename Left, typename Right, typename ToleranceType>
boost::test_tools::predicate_result
check_is_close_collection(Left left_begin, Left left_end, Right right_begin, Right right_end, ToleranceType tolerance)
{
	namespace fpc = boost::math::fpc;
	namespace tt_detail = boost::test_tools::tt_detail;

	typedef std::iterator_traits<Left>::value_type LeftValue;
	typedef std::iterator_traits<Right>::value_type RightValue;

	boost::test_tools::predicate_result pr(true);
	std::size_t pos = 0;

	fpc::close_at_tolerance<typename tt_detail::comp_supertype<LeftValue, RightValue>::type> pred(tolerance, fpc::FPC_STRONG);

	for (; left_begin != left_end && right_begin != right_end; ++left_begin, ++right_begin, ++pos) {
		if (!pred(*left_begin, *right_begin)) {
			pr = false;
			pr.message() << "\nMismatch at position " << pos << ": "
				<< ::boost::test_tools::tt_detail::print_helper(*left_begin)
				<< " != "
				<< ::boost::test_tools::tt_detail::print_helper(*right_begin)
				<< " by "
				<< pred.tested_rel_diff()
				<< " exceeded tolerance "
				<< tolerance;
		}
	}

	if (left_begin != left_end) {
		std::size_t r_size = pos;
		while (left_begin != left_end) {
			++pos;
			++left_begin;
		}

		pr = false;
		pr.message() << "\nCollections size mismatch: " << pos << " != " << r_size;
	}

	if (right_begin != right_end) {
		std::size_t l_size = pos;
		while (right_begin != right_end) {
			++pos;
			++right_begin;
		}

		pr = false;
		pr.message() << "\nCollections size mismatch: " << l_size << " != " << pos;
	}

	return pr;
}

#endif

