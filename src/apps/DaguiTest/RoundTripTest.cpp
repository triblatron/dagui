#include "config/config.h"

#include "core/Window.h"
#include "gfx/FontImageSource.h"
#include "core/SpaceTree.h"
#include "gfx/TextureAtlas.h"
#include "gfx/Mesh2D.h"
#include "core/Shape.h"
#include "core/Event.h"
#include "core/EventSystem.h"

#include <gtest/gtest.h>

#include "core/ValidatorNumber.h"
#include "gfx/AttributeDescriptor.h"

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

class TextureAtlas_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::TextureAtlas::Error>>
{

};

TEST_P(TextureAtlas_testRoundTrip, testRoundtrip)
{
    auto str = std::get<0>(GetParam());
    auto error = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::TextureAtlas::errorToString(error));
    EXPECT_EQ(error, dagui::TextureAtlas::parseError(str));
}

INSTANTIATE_TEST_SUITE_P(TextureAtlas, TextureAtlas_testRoundTrip, ::testing::Values(
    std::make_tuple("ERR_OK", dagui::TextureAtlas::ERR_OK),
    std::make_tuple("ERR_NON_POWER_OF_TWO_DIMS", dagui::TextureAtlas::ERR_NON_POWER_OF_TWO_DIMS),
    std::make_tuple("ERR_FAILED_TO_ALLOCATE", dagui::TextureAtlas::ERR_FAILED_TO_ALLOCATE),
    std::make_tuple("ERR_UNKNOWN", dagui::TextureAtlas::ERR_UNKNOWN)
    ));

class Mesh2D_PrimitiveType_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Mesh2D::PrimitiveType>>
{

};

TEST_P(Mesh2D_PrimitiveType_testRoundTrip, testRoundtrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::Mesh2D::primitiveTypeToString(value));
    EXPECT_EQ(value, dagui::Mesh2D::parsePrimitiveType(str));
}

INSTANTIATE_TEST_SUITE_P(Mesh2D, Mesh2D_PrimitiveType_testRoundTrip, ::testing::Values(
    std::make_tuple("PRIMITIVE_UNKNOWN", dagui::Mesh2D::PRIMITIVE_UNKNOWN),
    std::make_tuple("PRIMITIVE_POINT", dagui::Mesh2D::PRIMITIVE_POINT),
    std::make_tuple("PRIMITIVE_LINE", dagui::Mesh2D::PRIMITIVE_LINE),
    std::make_tuple("PRIMITIVE_LINE_STRIP", dagui::Mesh2D::PRIMITIVE_LINE_STRIP),
    std::make_tuple("PRIMITIVE_LINE_LOOP", dagui::Mesh2D::PRIMITIVE_LINE_LOOP),
    std::make_tuple("PRIMITIVE_TRIANGLE", dagui::Mesh2D::PRIMITIVE_TRIANGLE),
    std::make_tuple("PRIMITIVE_TRIANGLE_LIST", dagui::Mesh2D::PRIMITIVE_TRIANGLE_LIST),
    std::make_tuple("PRIMITIVE_TRIANGLE_STRIP", dagui::Mesh2D::PRIMITIVE_TRIANGLE_STRIP),
    std::make_tuple("PRIMITIVE_TRIANGLE_FAN", dagui::Mesh2D::PRIMITIVE_TRIANGLE_FAN)
    ));

class AttributeDataType_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::AttributeDescriptor::DataType>>
{

};

TEST_P(AttributeDataType_testRoundTrip, testRoundtrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::AttributeDescriptor::dataTypeToString(value));
    EXPECT_EQ(value, dagui::AttributeDescriptor::parseDataType(str));
}

INSTANTIATE_TEST_SUITE_P(AttributeDataType, AttributeDataType_testRoundTrip, ::testing::Values(
    std::make_tuple("TYPE_UNKNOWN", dagui::AttributeDescriptor::TYPE_UNKNOWN),
    std::make_tuple("TYPE_BYTE", dagui::AttributeDescriptor::TYPE_BYTE),
    std::make_tuple("TYPE_UINT16", dagui::AttributeDescriptor::TYPE_UINT16),
    std::make_tuple("TYPE_UINT32", dagui::AttributeDescriptor::TYPE_UINT32),
    std::make_tuple("TYPE_INT32", dagui::AttributeDescriptor::TYPE_INT32),
    std::make_tuple("TYPE_FLOAT", dagui::AttributeDescriptor::TYPE_FLOAT),
    std::make_tuple("TYPE_DOUBLE", dagui::AttributeDescriptor::TYPE_DOUBLE)
    ));

class AttributeUsage_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::AttributeDescriptor::Usage>>
{
};

TEST_P(AttributeUsage_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto usage = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::AttributeDescriptor::usageToString(usage));
    EXPECT_EQ(usage, dagui::AttributeDescriptor::parseUsage(str));
}

INSTANTIATE_TEST_SUITE_P(AttributeDescriptor, AttributeUsage_testRoundTrip, ::testing::Values(
    std::make_tuple("USAGE_UNKNOWN", dagui::AttributeDescriptor::USAGE_UNKNOWN),
    std::make_tuple("USAGE_POSITION", dagui::AttributeDescriptor::USAGE_POSITION),
    std::make_tuple("USAGE_NORMAL", dagui::AttributeDescriptor::USAGE_NORMAL),
    std::make_tuple("USAGE_TEXCOORD", dagui::AttributeDescriptor::USAGE_TEXCOORD),
    std::make_tuple("USAGE_TANGENT", dagui::AttributeDescriptor::USAGE_TANGENT),
    std::make_tuple("USAGE_BINORMAL", dagui::AttributeDescriptor::USAGE_BINORMAL),
    std::make_tuple("USAGE_COLOUR", dagui::AttributeDescriptor::USAGE_COLOUR)
    ));

class ValidatorNumberStatus_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::ValidatorNumber::Status>>
{

};

TEST_P(ValidatorNumberStatus_testRoundTrip, testRoundtrip)
{
    auto str = std::get<0>(GetParam());
    auto status = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::ValidatorNumber::statusString(status));
    EXPECT_EQ(status, dagui::ValidatorNumber::parseStatus(str));
}

INSTANTIATE_TEST_SUITE_P(ValidatorNumberStatus, ValidatorNumberStatus_testRoundTrip, ::testing::Values(
    std::make_tuple("STATUS_OK", dagui::Validator::STATUS_OK),
    std::make_tuple("STATUS_ERR_EMPTY", dagui::Validator::STATUS_ERR_EMPTY),
    std::make_tuple("STATUS_ERR_SIGN", dagui::Validator::STATUS_ERR_SIGN),
    std::make_tuple("STATUS_ERR_INTEGER", dagui::Validator::STATUS_ERR_INTEGER),
    std::make_tuple("STATUS_ERR_POINT", dagui::Validator::STATUS_ERR_POINT),
    std::make_tuple("STATUS_ERR_EXPONENT_SIGN", dagui::Validator::STATUS_ERR_EXPONENT_SIGN),
    std::make_tuple("STATUS_ERR_EXPONENT", dagui::Validator::STATUS_ERR_EXPONENT),
    std::make_tuple("STATUS_ERR_TOO_LOW", dagui::Validator::STATUS_ERR_TOO_LOW),
    std::make_tuple("STATUS_ERR_TOO_HIGH", dagui::Validator::STATUS_ERR_TOO_HIGH),
    std::make_tuple("STATUS_ERR_MINMAX", dagui::Validator::STATUS_ERR_MINMAX),
    std::make_tuple("STATUS_UNKNOWN", dagui::Validator::STATUS_UNKNOWN)
    ));

class ConstraintAttribute_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Constraint::Attribute>>
{

};

TEST_P(ConstraintAttribute_testRoundTrip, testExpectedValue)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());
    EXPECT_STREQ(str, dagui::Constraint::attributeToString(value));
    EXPECT_EQ(value, dagui::Constraint::parseAttribute(str));
}
//"            RIGHT,\n"
//"            TOP,\n"
//"            BOTTOM,\n"
//"            WIDTH,\n"
//"            HEIGHT,\n"
//"            CENTRE_X,\n"
//"            CENTRE_Y,\n"
//"            BASELINE")

INSTANTIATE_TEST_SUITE_P(ConstraintAttribute, ConstraintAttribute_testRoundTrip, ::testing::Values(
        std::make_tuple("Attribute::ATTR_UNKNOWN", dagui::Constraint::Attribute::ATTR_UNKNOWN),
        std::make_tuple("Attribute::LEFT", dagui::Constraint::Attribute::LEFT),
        std::make_tuple("Attribute::RIGHT", dagui::Constraint::Attribute::RIGHT),
        std::make_tuple("Attribute::TOP", dagui::Constraint::Attribute::TOP),
        std::make_tuple("Attribute::BOTTOM", dagui::Constraint::Attribute::BOTTOM),
        std::make_tuple("Attribute::WIDTH", dagui::Constraint::Attribute::WIDTH),
        std::make_tuple("Attribute::HEIGHT", dagui::Constraint::Attribute::HEIGHT),
        std::make_tuple("Attribute::CENTRE_X", dagui::Constraint::Attribute::CENTRE_X),
        std::make_tuple("Attribute::CENTRE_Y", dagui::Constraint::Attribute::CENTRE_Y),
        std::make_tuple("Attribute::BASELINE", dagui::Constraint::Attribute::BASELINE),
        std::make_tuple("Attribute::X", dagui::Constraint::Attribute::X),
        std::make_tuple("Attribute::Y", dagui::Constraint::Attribute::Y)
        ));

class ConstraintRelation_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Constraint::Relation>>
{

};

TEST_P(ConstraintRelation_testRoundTrip, testExpectedValue)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::Constraint::relationToString(value));
    EXPECT_EQ(value, dagui::Constraint::parseRelation(str));
}

INSTANTIATE_TEST_SUITE_P(Constraint, ConstraintRelation_testRoundTrip, ::testing::Values(
        std::make_tuple("Relation::EQ", dagui::Constraint::Relation::EQ),
        std::make_tuple("Relation::LE", dagui::Constraint::Relation::LE),
        std::make_tuple("Relation::GE", dagui::Constraint::Relation::GE)
        ));

class ConstraintStrength_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Constraint::Strength>>
{

};

TEST_P(ConstraintStrength_testRoundTrip, testExpectedValue)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::Constraint::strengthToString(value));
    EXPECT_EQ(value, dagui::Constraint::parseStrength(str));
}

INSTANTIATE_TEST_SUITE_P(Constraint, ConstraintStrength_testRoundTrip, ::testing::Values(
        std::make_tuple("Strength::REQUIRED", dagui::Constraint::Strength::REQUIRED),
        std::make_tuple("Strength::STRONG", dagui::Constraint::Strength::STRONG),
        std::make_tuple("Strength::MEDIUM", dagui::Constraint::Strength::MEDIUM),
        std::make_tuple("Strength::WEAK", dagui::Constraint::Strength::WEAK)
        ));

class ConstraintAnchorType_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Constraint::AnchorType>>
{

};

TEST_P(ConstraintAnchorType_testRoundTrip, testExpectedValue)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::Constraint::anchorTypeToString(value));
    EXPECT_EQ(value, dagui::Constraint::parseAnchorType(str));
}

INSTANTIATE_TEST_SUITE_P(ConstraintAnchorType, ConstraintAnchorType_testRoundTrip, ::testing::Values(
        std::make_tuple("AnchorType::FIXED", dagui::Constraint::AnchorType::FIXED),
        std::make_tuple("AnchorType::RELATIVE", dagui::Constraint::AnchorType::RELATIVE),
        std::make_tuple("AnchorType::PARENT_RELATIVE", dagui::Constraint::AnchorType::PARENT_RELATIVE),
        std::make_tuple("AnchorType::SIBLING_RELATIVE", dagui::Constraint::AnchorType::SIBLING_RELATIVE)
        ));

class ShapeFlags_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Shape::Flags>>
{

};

TEST_P(ShapeFlags_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::Shape::flagsToString(value));
    EXPECT_EQ(value, dagui::Shape::parseFlags(str));
}

INSTANTIATE_TEST_SUITE_P(ShapeFlags, ShapeFlags_testRoundTrip, ::testing::Values(
        std::make_tuple("FLAGS_NONE", dagui::Shape::FLAGS_NONE),
        std::make_tuple("FLAGS_DIRTY_TESSELLATION_BIT", dagui::Shape::FLAGS_DIRTY_TESSELLATION_BIT)
        ));

class EventType_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Event::Type>>
{

};

TEST_P(EventType_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::Event::typeToString(value));
    EXPECT_EQ(value, dagui::Event::parseType(str));
}

INSTANTIATE_TEST_SUITE_P(Event, EventType_testRoundTrip, ::testing::Values(
    std::make_tuple("TYPE_POINTER_MOVE", dagui::Event::TYPE_POINTER_MOVE),
    std::make_tuple("TYPE_POINTER_HOVER", dagui::Event::TYPE_POINTER_HOVER),
    std::make_tuple("TYPE_BUTTON_PRESS", dagui::Event::TYPE_BUTTON_PRESS),
    std::make_tuple("TYPE_BUTTON_RELEASE", dagui::Event::TYPE_BUTTON_RELEASE),
    std::make_tuple("TYPE_BUTTON_CLICK", dagui::Event::TYPE_BUTTON_CLICK),
    std::make_tuple("TYPE_BUTTON_DOUBLE_CLICK", dagui::Event::TYPE_BUTTON_DOUBLE_CLICK),
    std::make_tuple("TYPE_KEY_PRESS", dagui::Event::TYPE_KEY_PRESS),
    std::make_tuple("TYPE_KEY_RELEASE", dagui::Event::TYPE_KEY_RELEASE),
    std::make_tuple("TYPE_KEY_CHORD", dagui::Event::TYPE_KEY_CHORD),
    std::make_tuple("TYPE_ENTER_WIDGET", dagui::Event::TYPE_ENTER_WIDGET),
    std::make_tuple("TYPE_LEAVE_WIDGET", dagui::Event::TYPE_LEAVE_WIDGET),
    std::make_tuple("TYPE_DRAG_START", dagui::Event::TYPE_DRAG_START),
    std::make_tuple("TYPE_DRAGGING", dagui::Event::TYPE_DRAGGING),
    std::make_tuple("TYPE_DRAG_STOP", dagui::Event::TYPE_DRAG_STOP)
));

class ButtonMask_testRoundTrip : public ::testing::TestWithParam<std::tuple<std::string, dagui::ButtonMask>>
{

};

TEST_P(ButtonMask_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::buttonMaskToString(value));
    EXPECT_EQ(value, dagui::parseButtonMask(str));
}

INSTANTIATE_TEST_SUITE_P(ButtonMask, ButtonMask_testRoundTrip, ::testing::Values(
    std::make_tuple("BUTTON_PRIMARY_BIT", dagui::BUTTON_PRIMARY_BIT),
    std::make_tuple("BUTTON_SECONDARY_BIT", dagui::BUTTON_SECONDARY_BIT),
    std::make_tuple("BUTTON_MIDDLE_BIT", dagui::BUTTON_MIDDLE_BIT),
    std::make_tuple("BUTTON_PRIMARY_BIT BUTTON_SECONDARY_BIT", static_cast<dagui::ButtonMask>(dagui::BUTTON_PRIMARY_BIT|dagui::BUTTON_SECONDARY_BIT))
));

class EventTypeMask_testRoundTrip : public ::testing::TestWithParam<std::tuple<std::string, dagui::Event::TypeMask>>
{

};

TEST_P(EventTypeMask_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::Event::typeMaskToString(value));
    EXPECT_EQ(value, dagui::Event::parseTypeMask(str));
}

INSTANTIATE_TEST_SUITE_P(Event, EventTypeMask_testRoundTrip, ::testing::Values(
            std::make_tuple("EVENT_NONE", dagui::Event::EVENT_NONE),
            std::make_tuple("POINTER_MOVE_BIT", dagui::Event::POINTER_MOVE_BIT),
            std::make_tuple("POINTER_HOVER_BIT", dagui::Event::POINTER_HOVER_BIT),
            std::make_tuple("BUTTON_PRESS_BIT", dagui::Event::BUTTON_PRESS_BIT),
            std::make_tuple("BUTTON_RELEASE_BIT", dagui::Event::BUTTON_RELEASE_BIT),
            std::make_tuple("BUTTON_CLICK_BIT", dagui::Event::BUTTON_CLICK_BIT),
            std::make_tuple("BUTTON_DOUBLE_CLICK_BIT", dagui::Event::BUTTON_DOUBLE_CLICK_BIT),
            std::make_tuple("KEY_PRESS_BIT", dagui::Event::KEY_PRESS_BIT),
            std::make_tuple("KEY_RELEASE_BIT", dagui::Event::KEY_RELEASE_BIT),
            std::make_tuple("KEY_CHORD_BIT", dagui::Event::KEY_CHORD_BIT),
            std::make_tuple("ENTER_WIDGET_BIT", dagui::Event::ENTER_WIDGET_BIT),
            std::make_tuple("LEAVE_WIDGET_BIT", dagui::Event::LEAVE_WIDGET_BIT),
            std::make_tuple("DRAG_START_BIT", dagui::Event::DRAG_START_BIT),
            std::make_tuple("DRAGGING_BIT", dagui::Event::DRAGGING_BIT),
            std::make_tuple("DRAG_STOP_BIT", dagui::Event::DRAG_STOP_BIT),
            std::make_tuple("BUTTON_PRESS_BIT BUTTON_RELEASE_BIT", static_cast<dagui::Event::TypeMask>(dagui::Event::BUTTON_PRESS_BIT|dagui::Event::BUTTON_RELEASE_BIT))
        ));

class EventSource_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::EventSource>>
{

};

TEST_P(EventSource_testRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto value = std::get<1>(GetParam());

    EXPECT_STREQ(str, dagui::eventSourceToString(value));
    EXPECT_EQ(value, dagui::parseEventSource(str));
}

INSTANTIATE_TEST_SUITE_P(EventSource, EventSource_testRoundTrip, ::testing::Values(
        std::make_tuple("EVENT_SOURCE_UNKNOWN", dagui::EVENT_SOURCE_UNKNOWN),
        std::make_tuple("EVENT_SOURCE_POINTING", dagui::EVENT_SOURCE_POINTING),
        std::make_tuple("EVENT_SOURCE_KEYBOARD", dagui::EVENT_SOURCE_KEYBOARD)
        ));