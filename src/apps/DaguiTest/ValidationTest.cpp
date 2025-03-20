//
// Created by Tony Horrobin on 20/03/2025.
//

#include "core/ValidatorNumber.h"

#include <gtest/gtest.h>

class ValidatorNumber_testFilter : public ::testing::TestWithParam<std::tuple<const char*, const char*, bool>>
{

};

TEST_P(ValidatorNumber_testFilter, testExpectedOutput)
{
    std::string input = std::get<0>(GetParam());
    auto expected = std::get<1>(GetParam());
    auto valid = std::get<2>(GetParam());

    dagui::ValidatorNumber sut;
    for (auto nextChar : input)
    {
        sut.filter(nextChar);
    }
    EXPECT_EQ(expected, sut.output());
    sut.submit();
    EXPECT_EQ(valid, sut.isValid());
}

INSTANTIATE_TEST_SUITE_P(ValidatorNumber, ValidatorNumber_testFilter, ::testing::Values(
    std::make_tuple("+1", "+1", true),
    std::make_tuple("-1", "-1", true),
    std::make_tuple("1", "1", true),
    std::make_tuple("1.", "1.", true),
    std::make_tuple("1.0", "1.0", true),
    std::make_tuple("10", "10", true),
    std::make_tuple("+10", "+10", true),
    std::make_tuple("-10", "-10", true),
    std::make_tuple("10.5", "10.5", true),
    std::make_tuple("+10.5", "+10.5", true),
    std::make_tuple("-10.5", "-10.5", true),
    std::make_tuple("10.56", "10.56", true),
    std::make_tuple("+10.56", "+10.56", true),
    std::make_tuple("-10.56", "-10.56", true),
    std::make_tuple("1e3", "1e3", true),
    std::make_tuple("1e+3", "1e+3", true),
    std::make_tuple("1e-3", "1e-3", true),
    std::make_tuple("1e-30", "1e-30", true),
    std::make_tuple("1E3", "1E3", true),
    std::make_tuple("1E+3", "1E+3", true),
    std::make_tuple("1E-3", "1E-3", true),
    std::make_tuple("1E-30", "1E-30", true),
    std::make_tuple("1.e3", "1.e3", true),
    std::make_tuple("1.0e3", "1.0e3", true),
    std::make_tuple("1.54e10", "1.54e10", true),
    std::make_tuple("-1.54e10", "-1.54e10", true),
    std::make_tuple("-1.54e-5", "-1.54e-5", true),
    std::make_tuple("++", "+", false),
    std::make_tuple("--", "-", false),
    std::make_tuple("+-", "+", false),
    std::make_tuple("-+", "-", false),
    std::make_tuple("-+1e", "-1e", false),
    std::make_tuple("-+1e+", "-1e+", false),
    std::make_tuple("-+1e-", "-1e-", false),
    std::make_tuple("-+1.0eE--2", "-1.0e-2", true),
    std::make_tuple("-+1", "-1", true),
    std::make_tuple("-+1..45", "-1.45", true),
    std::make_tuple("-+1..45ee1", "-1.45e1", true)
    ));