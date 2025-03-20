//
// Created by Tony Horrobin on 20/03/2025.
//

#include "core/ValidatorNumber.h"

#include <gtest/gtest.h>

class ValidatorNumber_testFilter : public ::testing::TestWithParam<std::tuple<const char*, const char*>>
{

};

TEST_P(ValidatorNumber_testFilter, testExpectedOutput)
{
    std::string input = std::get<0>(GetParam());
    auto expected = std::get<1>(GetParam());

    dagui::ValidatorNumber sut;
    for (auto nextChar : input)
    {
        sut.filter(nextChar);
    }
    EXPECT_EQ(expected, sut.output());
}

INSTANTIATE_TEST_SUITE_P(ValidatorNumber, ValidatorNumber_testFilter, ::testing::Values(
    std::make_tuple("+1", "+1"),
    std::make_tuple("-1", "-1"),
    std::make_tuple("1", "1"),
    std::make_tuple("1.", "1."),
    std::make_tuple("1.0", "1.0"),
    std::make_tuple("10", "10"),
    std::make_tuple("+10", "+10"),
    std::make_tuple("-10", "-10"),
    std::make_tuple("10.5", "10.5"),
    std::make_tuple("+10.5", "+10.5"),
    std::make_tuple("-10.5", "-10.5"),
    std::make_tuple("10.56", "10.56"),
    std::make_tuple("+10.56", "+10.56"),
    std::make_tuple("-10.56", "-10.56"),
    std::make_tuple("++", "+"),
    std::make_tuple("--", "-"),
    std::make_tuple("+-", "+"),
    std::make_tuple("-+", "-"),
    std::make_tuple("-+1", "-1"),
    std::make_tuple("-+1..45", "-1.45")
    ));