//
// Created by Tony Horrobin on 20/03/2025.
//

#include "core/ValidatorNumber.h"

#include <gtest/gtest.h>
#include <regex>

#include "core/ValidatorRegex.h"

class ValidatorNumber_testFilter : public ::testing::TestWithParam<std::tuple<const char*, const char*, bool, double, dagui::ValidatorNumber::Status>>
{

};

TEST_P(ValidatorNumber_testFilter, testExpectedOutput)
{
    std::string input = std::get<0>(GetParam());
    auto expected = std::get<1>(GetParam());
    auto valid = std::get<2>(GetParam());
    auto value = std::get<3>(GetParam());
    auto status = std::get<4>(GetParam());

    dagui::ValidatorNumber sut;
    for (auto nextChar : input)
    {
        sut.filter(nextChar);
    }
    EXPECT_EQ(expected, sut.output());
    sut.submit();
    EXPECT_EQ(valid, sut.isValid());
    if (!std::isnan(value))
    {
        EXPECT_EQ(value, sut.asDouble());
    }
    else
    {
        EXPECT_TRUE(std::isnan(sut.asDouble()));
    }

    EXPECT_EQ(status, sut.status());
}

INSTANTIATE_TEST_SUITE_P(ValidatorNumber, ValidatorNumber_testFilter, ::testing::Values(
    std::make_tuple("", "", false, std::numeric_limits<double>::quiet_NaN(), dagui::ValidatorNumber::STATUS_ERR_EMPTY),
    std::make_tuple("+1", "+1", true, 1, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("-1", "-1", true, -1, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1", "1", true, 1, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1.", "1.", true, 1, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1.0", "1.0", true, 1, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("10", "10", true, 10, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("+10", "+10", true, 10, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("-10", "-10", true, -10, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("10.5", "10.5", true, 10.5, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("+10.5", "+10.5", true, 10.5, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("-10.5", "-10.5", true, -10.5, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("10.56", "10.56", true, 10.56, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("+10.56", "+10.56", true, 10.56, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("-10.56", "-10.56", true, -10.56, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1e3", "1e3", true, 1e3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1e+3", "1e+3", true, 1e3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1e-3", "1e-3", true, 1e-3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1e-30", "1e-30", true, 1e-30, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1E3", "1E3", true, 1e3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1E+3", "1E+3", true, 1e3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1E-3", "1E-3", true, 1e-3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1E-30", "1E-30", true, 1e-30, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1.e3", "1.e3", true, 1e3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1.0e3", "1.0e3", true, 1e3, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("1.54e10", "1.54e10", true, 1.54e10, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("-1.54e10", "-1.54e10", true, -1.54e10, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("-1.54e-5", "-1.54e-5", true, -1.54e-5, dagui::ValidatorNumber::STATUS_OK),
    std::make_tuple("++", "+", false, std::numeric_limits<double>::quiet_NaN(), dagui::ValidatorNumber::STATUS_ERR_SIGN),
    std::make_tuple("--", "-", false, std::numeric_limits<double>::quiet_NaN(), dagui::ValidatorNumber::STATUS_ERR_SIGN),
    std::make_tuple("+-", "+", false, std::numeric_limits<double>::quiet_NaN(), dagui::ValidatorNumber::STATUS_ERR_SIGN),
    std::make_tuple("-+", "-", false, std::numeric_limits<double>::quiet_NaN(), dagui::ValidatorNumber::STATUS_ERR_SIGN),
    std::make_tuple("-+1e", "-1e", false, -1, dagui::ValidatorNumber::STATUS_ERR_SIGN),
    std::make_tuple("-+1e+", "-1e+", false, -1, dagui::ValidatorNumber::STATUS_ERR_EXPONENT),
    std::make_tuple("-+1e-", "-1e-", false, -1, dagui::ValidatorNumber::STATUS_ERR_EXPONENT ),
    std::make_tuple("-+1.0eE--2", "-1.0e-2", true, -1e-2, dagui::ValidatorNumber::STATUS_ERR_EXPONENT_SIGN),
    std::make_tuple("-+1", "-1", true, -1, dagui::ValidatorNumber::STATUS_ERR_SIGN),
    std::make_tuple("-+1..45", "-1.45", true, -1.45, dagui::ValidatorNumber::STATUS_ERR_POINT),
    std::make_tuple("-+1..45ee1", "-1.45e1", true, -1.45e1, dagui::ValidatorNumber::STATUS_ERR_EXPONENT_SIGN),
    std::make_tuple("-+1..", "-1.", true, -1, dagui::ValidatorNumber::STATUS_ERR_POINT)
    ));

class ValidatorRegex_testFilter : public ::testing::TestWithParam<std::tuple<std::regex, const char*, const char*, bool>>
{

};

TEST_P(ValidatorRegex_testFilter, testExpectedOutput)
{
    auto regex = std::get<0>(GetParam());
    std::string input = std::get<1>(GetParam());
    auto output = std::get<2>(GetParam());
    auto valid = std::get<3>(GetParam());

    dagui::ValidatorRegex sut;
    sut.setRegex(regex);
    for (auto nextChar : input)
    {
        sut.filter(nextChar);
    }
    EXPECT_EQ(output, sut.output());
    sut.submit();
    EXPECT_EQ(valid, sut.isValid());
}

INSTANTIATE_TEST_SUITE_P(ValidatorRegex, ValidatorRegex_testFilter, ::testing::Values(
    std::make_tuple("[0-9]+", "1", "1", true),
    std::make_tuple("[0-9]+", "", "", false),
    std::make_tuple("[0-9]+", "10", "10", true),
    std::make_tuple("\\w+", "wibble", "wibble", true),
    std::make_tuple("\\w+@\\w+", "wibble@wobble", "wibble@wobble", true),
    std::make_tuple("\\w+@\\w+(\\.\\w+)*", "triblatron@github.com", "triblatron@github.com", true),
    std::make_tuple("[0-9]*", "1", "1", true)
    ));