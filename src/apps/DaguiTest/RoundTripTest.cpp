#include "config/config.h"

#include "core/Window.h"
#include "gfx/FontImageSource.h"
#include "core/SpaceTree.h"

#include <gtest/gtest.h>

class Window_testFeaturesRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Window::Features>>
{

};

TEST_P(Window_testFeaturesRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto features = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::Window::featuresName(features));
    EXPECT_EQ(features, dagui::Window::parseFeatures(str));
}

INSTANTIATE_TEST_SUITE_P(Window, Window_testFeaturesRoundTrip, ::testing::Values(
    std::make_tuple("TITLE_BIT", dagui::Window::TITLE_BIT),
    std::make_tuple("MINIMISE_BIT", dagui::Window::MINIMISE_BIT),
    std::make_tuple("MAXIMISE_BIT", dagui::Window::MAXIMISE_BIT),
    std::make_tuple("CLOSE_BIT", dagui::Window::CLOSE_BIT),
    std::make_tuple("BORDER_BIT", dagui::Window::BORDER_BIT),
    std::make_tuple("RESIZEABLE_BIT", dagui::Window::RESIZEABLE_BIT),
    std::make_tuple("MOVEABLE_BIT", dagui::Window::MOVEABLE_BIT),
    std::make_tuple("TITLE_BIT MINIMISE_BIT MAXIMISE_BIT CLOSE_BIT BORDER_BIT RESIZEABLE_BIT MOVEABLE_BIT", static_cast<dagui::Window::Features>(dagui::Window::DEFAULT_FEATURES))
));

class Window_testStatusRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Window::Status>>
{

};

TEST_P(Window_testStatusRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto status = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::Window::statusName(status));
    EXPECT_EQ(status, dagui::Window::parseStatus(str));
}

INSTANTIATE_TEST_SUITE_P(Window, Window_testStatusRoundTrip, ::testing::Values(
    std::make_tuple("VISIBLE_BIT", dagui::Window::VISIBLE_BIT),
    std::make_tuple("MINIMISED_BIT", dagui::Window::MINIMISED_BIT),
    std::make_tuple("MAXIMISED_BIT", dagui::Window::MAXIMISED_BIT)
));

class FontImageSource_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::FontImageSource::Error>>
{
};

TEST_P(FontImageSource_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto err = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::FontImageSource::errorToString(err));
    EXPECT_EQ(err, dagui::FontImageSource::parseError(str));
}

INSTANTIATE_TEST_SUITE_P(FontImageSource, FontImageSource_testRoundTrip, ::testing::Values(
    std::make_tuple("ERR_OK", dagui::FontImageSource::ERR_OK),
    std::make_tuple("ERR_UNSUPPORTED_FORMAT", dagui::FontImageSource::ERR_UNSUPPORTED_FORMAT),
    std::make_tuple("ERR_FAILED_TO_OPEN_FONT", dagui::FontImageSource::ERR_FAILED_TO_OPEN_FONT),
    std::make_tuple("ERR_LOADING_GLYPH", dagui::FontImageSource::ERR_LOADING_GLYPH),
    std::make_tuple("ERR_FAILED_TO_RENDER_GLYPH", dagui::FontImageSource::ERR_FAILED_TO_RENDER_GLYPH)
));

class SpaceTreeType_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::SpaceTree::Type>>
{

};

TEST_P(SpaceTreeType_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto type = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::SpaceTree::typeToString(type));
    EXPECT_EQ(type, dagui::SpaceTree::parseType(str));
}

INSTANTIATE_TEST_SUITE_P(SpaceTree, SpaceTreeType_testRoundTrip, ::testing::Values(
    std::make_tuple("TYPE_UNKNOWN", dagui::SpaceTree::TYPE_UNKNOWN),
    std::make_tuple("TYPE_INTERNAL", dagui::SpaceTree::TYPE_INTERNAL),
    std::make_tuple("TYPE_FREE", dagui::SpaceTree::TYPE_FREE),
    std::make_tuple("TYPE_FULL", dagui::SpaceTree::TYPE_FULL)
));

class SpaceTreeSplit_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::SpaceTree::Split>>
{

};

TEST_P(SpaceTreeSplit_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto split = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::SpaceTree::splitToString(split));
    EXPECT_EQ(split, dagui::SpaceTree::parseSplit(str));
}

INSTANTIATE_TEST_SUITE_P(SpaceTree, SpaceTreeSplit_testRoundTrip, ::testing::Values(
    std::make_tuple("SPLIT_UNKNOWN", dagui::SpaceTree::SPLIT_UNKNOWN),
    std::make_tuple("SPLIT_HORIZONTAL", dagui::SpaceTree::SPLIT_HORIZONTAL),
    std::make_tuple("SPLIT_VERTICAL", dagui::SpaceTree::SPLIT_VERTICAL)
));

class SpaceTreeResult_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::SpaceTree::Result>>
{

};

TEST_P(SpaceTreeResult_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto result = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::SpaceTree::resultToString(result));
    EXPECT_EQ(result, dagui::SpaceTree::parseResult(str));
}

INSTANTIATE_TEST_SUITE_P(SpaceTree, SpaceTreeResult_testRoundTrip, ::testing::Values(
    std::make_tuple("RESULT_OK", dagui::SpaceTree::RESULT_OK),
    std::make_tuple("RESULT_UNKNOWN", dagui::SpaceTree::RESULT_UNKNOWN),
    std::make_tuple("RESULT_FAILED_TO_INSERT", dagui::SpaceTree::RESULT_FAILED_TO_INSERT),
    std::make_tuple("RESULT_FAILED_TO_SPLIT", dagui::SpaceTree::RESULT_FAILED_TO_SPLIT)
));

class Heuristic_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::SpaceTree::Heuristic>>
{

};

TEST_P(Heuristic_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::SpaceTree::heuristicToString(value));
    EXPECT_EQ(value, dagui::SpaceTree::parseHeuristic(str));
}

INSTANTIATE_TEST_SUITE_P(Heuristic, Heuristic_testRoundTrip, ::testing::Values(
    std::make_tuple("FIT_UNKNOWN", dagui::SpaceTree::FIT_UNKNOWN),
    std::make_tuple("FIT_NEXT", dagui::SpaceTree::FIT_NEXT),
    std::make_tuple("FIT_BEST_SHORT_SIDE", dagui::SpaceTree::FIT_BEST_SHORT_SIDE)
));