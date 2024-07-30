//
// Created by tony on 30/07/24.
//


#include "gtest/gtest.h"
#include "core/Rectangle.h"

class Rectangle_testIsInside : public ::testing::TestWithParam<std::tuple<double, double, double, double, double, double, bool>>
{

};

TEST_P(Rectangle_testIsInside, testIsInside)
{
    auto x = std::get<0>(GetParam());
    auto y = std::get<1>(GetParam());
    auto width = std::get<2>(GetParam());
    auto height = std::get<3>(GetParam());
    auto hitX = std::get<4>(GetParam());
    auto hitY = std::get<5>(GetParam());
    auto inside = std::get<6>(GetParam());

    nfe::Rectangle sut;
    sut.setPos(x,y);
    sut.setSize(width,height);
    EXPECT_EQ(inside, sut.isInside(hitX,hitY));
}

INSTANTIATE_TEST_SUITE_P(Rectangle, Rectangle_testIsInside, ::testing::Values(
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 200.0, 200.0, false),
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 50.0, 50.0, true)
        ));