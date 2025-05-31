//
// Created by Tony Horrobin on 17/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Atom.h"
#include "core/Variant.h"
#include "core/WidgetRef.h"

#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Constraint
    {
    public:
        enum class Attribute
        {
            ATTR_UNKNOWN,
            LEFT,
            RIGHT,
            TOP,
            BOTTOM,
            WIDTH,
            HEIGHT,
            CENTRE_X,
            CENTRE_Y,
            BASELINE,
            X,
            Y
        };

        enum class Relation
        {
            EQ,
            LE,
            GE
        };

        enum class Strength
        {
            REQUIRED=1000,
            STRONG=750,
            MEDIUM=500,
            WEAK=250
        };

        enum class AnchorType
        {
            FIXED,
            RELATIVE,
            PARENT_RELATIVE,
            SIBLING_RELATIVE
        };

        struct Position
        {
            AnchorType anchor{AnchorType::FIXED};
            float value{0.0f};
            WidgetRef ref{};
        };
    public:
        dagbase::Variant find(std::string_view path) const;

        void resolveRefs(Widget& widget);

        void makeItSo();

        static Constraint create(dagbase::ConfigurationElement& config);

        static Constraint width();

        static Constraint proportional(const dagbase::Atom& child, Attribute attr, const dagbase::Atom& parent, float ratio);

        static Constraint position(WidgetRef ref, AnchorType anchorType, Attribute, float value);

        static const char* attributeToString(Attribute value);

        static Attribute parseAttribute(const char *str);

        static const char* relationToString(Relation value);

        static Relation parseRelation(const char* str);

        static const char* strengthToString(Strength value);

        static Strength parseStrength(const char* str);

        static const char* anchorTypeToString(AnchorType value);

        static AnchorType parseAnchorType(const char* str);
    private:
        WidgetRef _firstItem{};
        Attribute _firstAttr{Attribute::LEFT};
        Relation _relation{Relation::EQ};
        WidgetRef _secondItem{};
        Attribute _secondAttr{Attribute::LEFT};
        Strength _strength{Strength::REQUIRED};
        float _multiplier{1.0f};
        float _constant{0.0f};
    };
}
