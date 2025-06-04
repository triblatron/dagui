//
// Created by Tony Horrobin on 17/05/2025.
//

#include "config/config.h"

#include "core/Constraint.h"

#include <utility>
#include "core/ConfigurationElement.h"

#include "util/Searchable.h"
#include "util/enums.h"
#include "core/Widget.h"

namespace dagui
{

    Constraint Constraint::create(dagbase::ConfigurationElement &config)
    {
        std::string type;

        dagbase::ConfigurationElement::readConfig(config, "type", &type);

        if (type == "growth")
        {
            float ratio{0.0f};
            dagbase::ConfigurationElement::readConfig(config, "ratio", &ratio);
            std::string parent;
            dagbase::ConfigurationElement::readConfig(config, "parent", &parent);
            Attribute attr{Attribute::ATTR_UNKNOWN};
            if (auto element=config.findElement("attribute"); element)
            {
                attr = parseAttribute(element->asString().c_str());
            }
            std::string child;
            dagbase::ConfigurationElement::readConfig(config, "child", &child);
            return proportional(dagbase::Atom::intern(child), attr, dagbase::Atom::intern(parent), ratio);
        }
        else if (type == "position")
        {
            AnchorType anchor{AnchorType::FIXED};
            dagbase::ConfigurationElement::readConfig<AnchorType>(config, "anchor", [](const char* str) {
                return Constraint::parseAnchorType(str);
            }, &anchor);
            float constant = 0.0f;
            dagbase::ConfigurationElement::readConfig(config, "constant", &constant);
            std::string ref;
            dagbase::ConfigurationElement::readConfig(config, "widget", &ref);
            Attribute attr{Attribute::ATTR_UNKNOWN};
            if (auto element=config.findElement("attribute"); element)
            {
                attr = parseAttribute(element->asString().c_str());
            }
            return position(WidgetRef(dagbase::Atom::intern(ref)), anchor, attr, constant);
        }
        return {};
    }

    Constraint
    Constraint::proportional(const dagbase::Atom& child, Constraint::Attribute attr, const dagbase::Atom& parent, float ratio)
    {
        Constraint constraint;

        constraint._firstItem.setId(child);
        constraint._firstAttr = attr;
        constraint._relation = Relation::EQ;
        constraint._secondItem.setId(parent);
        constraint._secondAttr = attr;
        constraint._constant = ratio;

        return constraint;
    }

    dagbase::Variant Constraint::find(std::string_view path) const
    {
        dagbase::Variant retval;

        if (_firstItem.ref())
        {
            retval = dagbase::findInternal(path, "firstItem", *_firstItem.ref());
        }
        else
        {
            retval = dagbase::findEndpoint(path, "firstItem", _firstItem.toString());
        }
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "firstAttribute", attributeToString(_firstAttr));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "relation", relationToString(_relation));
        if (retval.has_value())
            return retval;

        if (_secondItem.ref())
        {
            retval = dagbase::findInternal(path, "secondItem", *_secondItem.ref());
        }
        else
        {
            retval = dagbase::findEndpoint(path, "secondItem", _secondItem.toString());
        }
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "secondAttribute", attributeToString(_secondAttr));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "multiplier", double(_multiplier));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "constant", double(_constant));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "strength", strengthToString(_strength));
        if (retval.has_value())
            return retval;

        return {};
    }

    const char *Constraint::attributeToString(Constraint::Attribute value)
    {
        switch (value)
        {
            ENUM_NAME(Attribute::ATTR_UNKNOWN)
            ENUM_NAME(Attribute::LEFT)
            ENUM_NAME(Attribute::RIGHT)
            ENUM_NAME(Attribute::TOP)
            ENUM_NAME(Attribute::BOTTOM)
            ENUM_NAME(Attribute::WIDTH)
            ENUM_NAME(Attribute::HEIGHT)
            ENUM_NAME(Attribute::CENTRE_X)
            ENUM_NAME(Attribute::CENTRE_Y)
            ENUM_NAME(Attribute::BASELINE)
            ENUM_NAME(Attribute::X)
            ENUM_NAME(Attribute::Y)
        }

        return "<error>";
    }

    Constraint::Attribute Constraint::parseAttribute(const char *str)
    {
        TEST_ENUM(Attribute::LEFT, str)
        TEST_ENUM(Attribute::RIGHT, str)
        TEST_ENUM(Attribute::TOP, str)
        TEST_ENUM(Attribute::BOTTOM, str)
        TEST_ENUM(Attribute::WIDTH, str)
        TEST_ENUM(Attribute::HEIGHT, str)
        TEST_ENUM(Attribute::CENTRE_X, str)
        TEST_ENUM(Attribute::CENTRE_Y, str)
        TEST_ENUM(Attribute::BASELINE, str)
        TEST_ENUM(Attribute::X, str)
        TEST_ENUM(Attribute::Y, str)

        return Attribute::ATTR_UNKNOWN;
    }

    const char *Constraint::relationToString(Constraint::Relation value)
    {
        switch (value)
        {
            ENUM_NAME(Relation::EQ)
            ENUM_NAME(Relation::GE)
            ENUM_NAME(Relation::LE)
        }
        return "<error>";
    }

    Constraint::Relation Constraint::parseRelation(const char *str)
    {
        TEST_ENUM(Relation::EQ, str)
        TEST_ENUM(Relation::GE, str)
        TEST_ENUM(Relation::LE, str)

        return Constraint::Relation::EQ;
    }

    const char *Constraint::strengthToString(Constraint::Strength value)
    {
        switch (value)
        {
            ENUM_NAME(Strength::REQUIRED)
            ENUM_NAME(Strength::STRONG)
            ENUM_NAME(Strength::MEDIUM)
            ENUM_NAME(Strength::WEAK)
        }

        return "<error>";
    }

    Constraint::Strength Constraint::parseStrength(const char *str)
    {
        TEST_ENUM(Strength::REQUIRED, str)
        TEST_ENUM(Strength::STRONG, str)
        TEST_ENUM(Strength::MEDIUM, str)
        TEST_ENUM(Strength::WEAK, str)

        return Constraint::Strength::REQUIRED;
    }

    void Constraint::resolveRefs(Widget &widget)
    {
        _firstItem.resolve(&widget);
        _secondItem.resolve(&widget);
    }

    Constraint Constraint::width(dagbase::Atom ref, Relation relation, float value)
    {
        Constraint constraint;

        constraint._firstItem.setId(ref);
        constraint._firstAttr = Attribute::WIDTH;
        constraint._relation = relation;
        constraint._constant = value;

        return constraint;
    }

    const char *Constraint::anchorTypeToString(Constraint::AnchorType value)
    {
        switch (value)
        {
            ENUM_NAME(AnchorType::FIXED)
            ENUM_NAME(AnchorType::RELATIVE)
            ENUM_NAME(AnchorType::PARENT_RELATIVE)
            ENUM_NAME(AnchorType::SIBLING_RELATIVE)
        }
        return "<error>";
    }

    Constraint::AnchorType Constraint::parseAnchorType(const char *str)
    {
        TEST_ENUM(AnchorType::FIXED, str);
        TEST_ENUM(AnchorType::RELATIVE, str);
        TEST_ENUM(AnchorType::PARENT_RELATIVE, str);
        TEST_ENUM(AnchorType::SIBLING_RELATIVE, str);

        return Constraint::AnchorType::FIXED;
    }

    Constraint Constraint::position(WidgetRef ref, Constraint::AnchorType anchorType, Attribute attr, float value)
    {
        Constraint constraint;

        constraint._firstAttr = attr;
        constraint._constant = value;
        constraint._firstItem = std::move(ref);
        constraint._relation = Relation::EQ;

        return constraint;
    }

    void Constraint::makeItSo()
    {
        if (_firstAttr == Constraint::Attribute::X)
        {
            _firstItem.ref()->setX(int(_constant));
        }
        else if (_firstAttr == Constraint::Attribute::Y)
        {
            _firstItem.ref()->setY(int(_constant));
        }
    }

}
