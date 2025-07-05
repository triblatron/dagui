//
// Created by Tony Horrobin on 17/04/2025.
//

#include "config/config.h"

#include "core/Vertical.h"
#include "util/Searchable.h"

namespace dagui
{
    void Vertical::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory, ShapeFactory &shapeFactory)
    {
        Widget::configure(config, factory, shapeFactory);
        if (auto element=config.findElement("layoutProperties"); element)
        {
            _props.configure(*element);
        }
        dagbase::ConfigurationElement::readConfig(config, "minWidth", &_minWidth);
        computeConstraints();
    }

    dagbase::ConfigurationElement::ValueType Vertical::find(std::string_view path) const
    {
        dagbase::Variant retval =  Widget::find(path);

        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "layoutProperties", _props);
        if (retval.has_value())
            return retval;

        return {};
    }

    Vertical::Vertical(Widget *parent) : Widget(dagbase::Atom::intern("Vertical"), parent)
    {
        // Do nothing.
    }

    void Vertical::computeConstraints()
    {
        addConstraint(Constraint::width(id(), Constraint::Relation::GE, _minWidth));
    }
}
