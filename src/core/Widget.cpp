//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Widget.h"

#include "core/ConfigurationElement.h"
#include "core/WidgetFactory.h"
#include "gfx/ArrayDescriptor.h"
#include "util/Searchable.h"

#include <utility>

namespace dagui
{
    Widget::Widget(dagbase::Atom typeName, Widget *parent)
    :
    _typeName(typeName),
    _parent(parent)
    {
        // Do nothing.
    }

    void Widget::configure(dagbase::ConfigurationElement& config, WidgetFactory& factory)
    {
        if (auto element=config.findElement("id"); element)
        {
            _id = element->asString();
            if (!_id.empty())
                root()->addIdentified(this);
        }

        if (auto element=config.findElement("children"); element)
        {
            element->eachChild([this, &factory](dagbase::ConfigurationElement& child)
            {
                Widget* childWidget = factory.create(child);

                if (childWidget)
                {
                    addChild(childWidget);
                }
                return true;
            });
        }
    }

    Widget* Widget::root()
    {
        Widget* root = this;

        while (root && root->_parent)
        {
            root = root->_parent;
        }

        return root;
    }

    void Widget::addChild(Widget* child)
    {
        if (child)
            _children.push_back(child);
    }

    Widget* Widget::lookupWidget(std::string name)
    {
        Widget* root=this;
        while (root && root->_parent)
        {
            root=root->_parent;
        }
        if (root && !root->_parent)
        {
            return root->lookupWidget(std::move(name));
        }

        return nullptr;
    }

    dagbase::ConfigurationElement::ValueType Widget::find(std::string_view path) const
    {
        dagbase::ConfigurationElement::ValueType retval;

        retval = dagbase::findEndpoint(path, "id", _id);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "numChildren", std::int64_t(_children.size()));
        if (retval.has_value())
            return retval;

        return {};
    }

    void Widget::accept(WidgetVisitor &visitor)
    {
        visitor.visit(*this);
    }
}
