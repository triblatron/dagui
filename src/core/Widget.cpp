//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Widget.h"

#include "core/ConfigurationElement.h"
#include "core/WidgetFactory.h"
#include "gfx/ArrayDescriptor.h"
#include "util/Searchable.h"
#include "core/Shape.h"
#include "core/ShapeFactory.h"
#include "core/Batcher.h"

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

    void Widget::configure(dagbase::ConfigurationElement& config, WidgetFactory& factory, ShapeFactory& shapeFactory)
    {
        if (auto element=config.findElement("id"); element)
        {
            _id = dagbase::Atom::intern(element->asString());
            if (!_id.empty())
                root()->addIdentified(this);
        }

        if (auto element=config.findElement("children"); element)
        {
            element->eachChild([this, &factory, &shapeFactory](dagbase::ConfigurationElement& child)
            {
                Widget* childWidget = factory.create(child, shapeFactory);

                if (childWidget)
                {
                    addChild(childWidget);
                }
                return true;
            });
        }

        if (auto element = config.findElement("constraints"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                auto constraint = Constraint::create(child);

                _constraints.a.emplace_back(constraint);
                return true;
            });
        }

        std::string styleClass;
        dagbase::ConfigurationElement::readConfig(config, "styleClass", &styleClass);

        _styleClass = dagbase::Atom::intern(styleClass);
        _style.setId(_styleClass);

        if (auto element = config.findElement("shape"); element)
        {
            addShape(shapeFactory.createShape(*element));
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
            _children.a.emplace_back(child);
    }

    Widget* Widget::lookup(dagbase::Atom name)
    {
        Widget* root=this;
        while (root && root->_parent)
        {
            root=root->_parent;
        }
        if (root && !root->_parent)
        {
            return root->lookup(std::move(name));
        }

        return nullptr;
    }

    dagbase::ConfigurationElement::ValueType Widget::find(std::string_view path) const
    {
        dagbase::ConfigurationElement::ValueType retval;

        if (!_id.empty())
        {
            retval = dagbase::findEndpoint(path, "id", std::string(_id.value()));
            if (retval.has_value())
                return retval;
        }

        retval = dagbase::findEndpoint(path, "class", std::string(_typeName.value()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "numChildren", std::int64_t(_children.size()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "constraints", _constraints);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "x", std::int64_t(_pos.x));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "y", std::int64_t(_pos.y));
        if (retval.has_value())
            return retval;

        if (!_styleClass.empty())
        {
            retval = dagbase::findEndpoint(path, "styleClass", std::string(_styleClass.value()));
            if (retval.has_value())
                return retval;
        }

        retval = dagbase::findEndpoint(path, "parent", _parent!=nullptr);
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "children", _children);
        if (retval.has_value())
            return retval;

        if (_style.ref())
        {
            retval = dagbase::findInternal(path, "style", *_style.ref());
            if (retval.has_value())
                return retval;
        }

        if (!_shapes.a.empty())
        {
            retval = dagbase::findInternal(path, "shape", _shapes);
            if (retval.has_value())
                return retval;
        }

        return {};
    }

    void Widget::accept(WidgetVisitor &visitor)
    {
        visitor.visit(*this);
        eachChild([this, &visitor](Widget& child) {
            child.accept(visitor);
            return true;
        });
    }

    void Widget::eachChild(std::function<bool(Widget &)> f)
    {
        if (f)
            for (auto child : _children.a)
            {
                if (!f(*child))
                {
                    break;
                }
            }
    }

    void Widget::eachConstraint(std::function<bool(Constraint &)> f)
    {
        if (f)
            for (auto& constraint : _constraints.a)
            {
                if (!f(constraint))
                    break;
            }
    }

    void Widget::resolveRefs()
    {
        auto lookup = root()->styleLookup();
        if (lookup)
            _style.resolve(lookup);
    }

    void Widget::draw(Batcher &batcher)
    {
        auto it = batcher.findRenderBin({-1,-1,-1,0});
        if (it != batcher.end())
        {
            for (auto shape : _shapes.a)
            {
                shape->tessellate(*it->second->mesh());
            }
        }
    }
}
