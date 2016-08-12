
#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "../../src/driver/format.hpp"

using namespace YAMML::Driver;

BOOST_AUTO_TEST_SUITE(DriverFormatTest)

BOOST_AUTO_TEST_CASE(NoArgs)
{
    BOOST_CHECK_EQUAL(FormatText("Foo", {}), "Foo");
    BOOST_CHECK_EQUAL(FormatText("Foo{{}{}}", {}), "Foo{}");
}

BOOST_AUTO_TEST_CASE(WithArgs)
{
    BOOST_CHECK_EQUAL(FormatText("Foo{0}", {"123"}), "Foo123");
    BOOST_CHECK_EQUAL(FormatText("Foo{{}{0}{}}", {"123"}), "Foo{123}");

    BOOST_CHECK_EQUAL(FormatText("{0}{0}{0}", {"123"}), "123123123");
    BOOST_CHECK_EQUAL(FormatText("{%%%}{1}{1}{A}", {"123", "456", "789"}), "%%%456456A");

    BOOST_CHECK_EQUAL(FormatText("{1}Foo{{}{0}{}}", {"123", "Bar"}), "BarFoo{123}");

    BOOST_CHECK_EQUAL(FormatText("{}}{2}{1}{{}Foo{{}{0}{2}{}}", {"123", "Bar", "Baz"}), "}BazBar{Foo{123Baz}");
}

BOOST_AUTO_TEST_CASE(Exceptions)
{
    BOOST_CHECK_THROW(FormatText("{0}", {}), std::out_of_range);
    BOOST_CHECK_THROW(FormatText("Foo{1}Bar", {"123"}), std::out_of_range);
    BOOST_CHECK_THROW(FormatText("{0}Foo{1}Bar", {"123"}), std::out_of_range);
    BOOST_CHECK_THROW(FormatText("{2}Foo{1}Bar", {"123", "456"}), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(NotClosed)
{
    BOOST_CHECK_EQUAL(FormatText("{1}Foo{{}{0}{ABC", {"123", "Bar"}), "BarFoo{123{ABC");
    BOOST_CHECK_EQUAL(FormatText("Foo{{}{0}{1", {"123", "Bar"}), "Foo{123{1");
}

BOOST_AUTO_TEST_SUITE_END()
