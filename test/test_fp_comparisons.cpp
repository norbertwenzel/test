//  (C) Copyright Gennadiy Rozental 2001-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests floating point comparison algorithms
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

// Boost
#include <boost/mpl/list.hpp>
#include <boost/bind.hpp>

// STL
#include <functional>

using namespace boost;
using namespace boost::unit_test;
using namespace boost::test_tools;
namespace tt=boost::test_tools;

bool not_func( bool argb ) { return !argb; }

//____________________________________________________________________________//

typedef boost::mpl::list<float,double,long double> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( test_BOOST_CHECK_CLOSE, FPT, test_types )
{
#define CHECK_CLOSE( first, second, e )     \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_CHECK_CLOSE( fp1, fp2, epsilon ); \
/**/

#ifdef BOOST_TEST_NO_OLD_TOOLS

#define CHECK_NOT_CLOSE( first, second, e ) \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_TEST( fp1 != fp2, fpc::percent_tolerance( epsilon ) ); \

#else

#define CHECK_NOT_CLOSE( first, second, e ) \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_TEST( !check_is_close( fp1, fp2, ::fpc::percent_tolerance( epsilon ) ) ); \
/**/
#endif

    FPT fp1, fp2, epsilon;

    CHECK_CLOSE( 1, 1, 0 );

    CHECK_NOT_CLOSE( 0, 1e-20, 1e-5 );
    CHECK_NOT_CLOSE( 0, 1e-30, 1e-5 );
    CHECK_NOT_CLOSE( 0, -1e-10, 0.1 );
    CHECK_NOT_CLOSE( 0.123456, 0.123457, 1e-4 );

    CHECK_CLOSE( 0.123456, 0.123457, 1e-3 );

    CHECK_NOT_CLOSE( 0.123456, -0.123457, 1e-3 );

    CHECK_CLOSE( 1.23456e28, 1.23457e28, 1e-3 );

    CHECK_CLOSE( 1.23456e-10, 1.23457e-10, 1e-3 );
    CHECK_NOT_CLOSE( 1.111e-10, 1.112e-10, 0.08999 );
    CHECK_CLOSE( 1.112e-10, 1.111e-10, 0.1 );

    CHECK_CLOSE( 1, 1.0001, 1.1e-2 );
    CHECK_CLOSE( 1.0002, 1.0001, 1.1e-2 );

    CHECK_NOT_CLOSE( 1, 1.0002, 1.1e-2 );

#undef CHECK_CLOSE
#undef CHECK_NOT_CLOSE
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE_TEMPLATE( test_CHECK_CLOSE_FRACTION, FPT, test_types )
{
#define CHECK_CLOSE( first, second, e )     \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_CHECK_CLOSE( fp1, fp2, epsilon ); \
/**/

#ifdef BOOST_TEST_NO_OLD_TOOLS

#define CHECK_NOT_CLOSE( first, second, e ) \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_TEST( fp1 != fp2, epsilon );      \

#else

#define CHECK_NOT_CLOSE( first, second, e ) \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_TEST( !check_is_close( fp1, fp2, epsilon ) ); \
/**/
#endif


    FPT fp1, fp2, epsilon;

    CHECK_CLOSE( 1, 1, 0 );

    CHECK_NOT_CLOSE( 0, 1e-20, 1e-5 );
    CHECK_NOT_CLOSE( 0, 1e-30, 1e-5 );
    CHECK_NOT_CLOSE( 0, -1e-10, 0.1 );
    CHECK_NOT_CLOSE( 0.123456, 0.123457, 1e-6 );

    CHECK_CLOSE( 0.123456, 0.123457, 1e-3 );

    CHECK_NOT_CLOSE( 0.123456, -0.123457, 1e-3 );

    CHECK_CLOSE( 1.23456e28, 1.23457e28, 1e-3 );

    CHECK_CLOSE( 1.23456e-10, 1.23457e-10, 1e-3 );
    CHECK_NOT_CLOSE( 1.111e-10, 1.112e-10, 0.0008999 );
    CHECK_CLOSE( 1.112e-10, 1.111e-10, 0.1 );

    CHECK_CLOSE( 1, 1.0001, 1.1e-2 );
    CHECK_CLOSE( 1.0002, 1.0001, 1.1e-2 );

    CHECK_NOT_CLOSE( 1, 1.0002, 1.1e-4 );

#undef CHECK_CLOSE
#undef CHECK_NOT_CLOSE
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_type_mismatch )
{
    BOOST_CHECK_CLOSE_FRACTION( 2., 2.1, 0.06 );
    BOOST_CHECK_CLOSE( 2.1, 2., 6. );
    BOOST_CHECK_CLOSE( 2.1, 2.f, 6. );
}


#if !defined(BOOST_TEST_NO_VARIADIC)


BOOST_AUTO_TEST_CASE( test_strong_weak, * expected_failures( 4 ) )
{
    BOOST_TEST(1./3 == 1./2, tt::tolerance(1.));
    BOOST_TEST(1./3 == 1./2, tt::tolerance(0.4));  // will fail 1/2 > 0.4
    BOOST_TEST(1./3 == 1./2, tt::tolerance(1./3)); // will fail; both 1/3 and 1/2 > 1/3
    BOOST_TEST(1./3 != 1./2, tt::tolerance(1.));   // will fail; both 1/3 and 1/2 < 1
    BOOST_TEST(1./3 != 1./2, tt::tolerance(0.4));  // will fail 1/3 < 0.4
    BOOST_TEST(1./3 != 1./2, tt::tolerance(1./3));
}

BOOST_AUTO_TEST_CASE(test_strict_order_operations_ne, 
                     * boost::unit_test::tolerance(0.01)
                     * expected_failures( 1 ) )
{
    double x = 10.00;
    double y = 10.01; // diff within tolerance
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x <= y);
    BOOST_TEST(x >= y);
}

BOOST_AUTO_TEST_CASE(test_strict_order_operations_lt, 
                     * boost::unit_test::tolerance(0.01)
                     * expected_failures( 3 ) )
{
    double x = 10.00;
    double y = 10.01; // diff within tolerance
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x <= y);
    BOOST_TEST(y <= x);
    BOOST_TEST(x < y);  // fails
    BOOST_TEST(y < x);  // fails
}

BOOST_AUTO_TEST_CASE(test_strict_order_operations_lt_0, 
                     * boost::unit_test::tolerance(0.02)
                     * expected_failures( 3 ) )
{
    double x = 0.00;
    double y = 0.01;
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x <= y); 
    BOOST_TEST(y <= x);     
    BOOST_TEST(x < y);  // fails, too close to 0
    BOOST_TEST(y < x);  // fails, too close to 0
}

BOOST_AUTO_TEST_CASE(test_strict_order_operations_le, 
                     * boost::unit_test::tolerance(0.01)
                     * expected_failures( 1 ) )
{
    double x = 10.01;
    double y = 10.00; // diff within tolerance
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x <= y);
}


BOOST_AUTO_TEST_CASE(test_strict_order_operations_gt, 
                     * boost::unit_test::tolerance(0.01)
                     * expected_failures( 3 ) )
{
    double x = 10.00;
    double y = 10.01; // diff within tolerance
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x > y);  // fails
    BOOST_TEST(y > x);  // fails    
}

BOOST_AUTO_TEST_CASE(test_strict_order_operations_ge, 
                     * boost::unit_test::tolerance(0.01)
                     * expected_failures( 1 ) )
{
    double x = 10.00;
    double y = 10.01; // diff within tolerance
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x >= y);
    BOOST_TEST(y >= x);
}

BOOST_AUTO_TEST_CASE(test_strict_order_operations_gt_0, 
                     * boost::unit_test::tolerance(0.02)
                     * expected_failures( 3 ) )
{
    double x = 0.00;
    double y = 0.01;
    BOOST_TEST(x == y); 
    BOOST_TEST(x != y); // fails
    BOOST_TEST(x >= y); 
    BOOST_TEST(y >= x);     
    BOOST_TEST(x <= y); 
    BOOST_TEST(y <= x);      
    BOOST_TEST(x > y);  // fails, too close to 0
    BOOST_TEST(y > x);  // fails, too close to 0
}

#endif

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_CHECK_SMALL )
{
    BOOST_CHECK_SMALL( 1e-6, 1e-5 );
    BOOST_CHECK_SMALL( -1e-6, 1e-5 );

#ifndef BOOST_TEST_NO_NEW_TOOLS
    BOOST_TEST( 1e-6 != 0., 1e-7 );
#endif
}

//____________________________________________________________________________//

namespace fpc = boost::math::fpc;

BOOST_AUTO_TEST_CASE( test_close_at_tolerance )
{
    double fp1     = 1.00000001;
    double fp2     = 1.00000002;
    double epsilon = 1e-6;

    ::fpc::close_at_tolerance<double> pred( ::fpc::percent_tolerance( epsilon ), ::fpc::FPC_WEAK );
    BOOST_CHECK_PREDICATE( pred, (fp1)(fp2) );

    BOOST_TEST( !check_is_close( fp1, fp2, ::fpc::percent_tolerance( epsilon ) ) );

    fp1     = 1.23456e-10;
    fp2     = 1.23457e-10;
    epsilon = 8.1e-4;

    BOOST_CHECK_PREDICATE( ::fpc::close_at_tolerance<double>( ::fpc::percent_tolerance( epsilon ), ::fpc::FPC_WEAK ), (fp1)(fp2) );

    BOOST_TEST( !::fpc::close_at_tolerance<double>( ::fpc::percent_tolerance( epsilon ) )(fp1, fp2) );
}

//____________________________________________________________________________//

// EOF
