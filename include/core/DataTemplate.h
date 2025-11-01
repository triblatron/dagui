//
// Created by Tony Horrobin on 28/10/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Atom.h"
#include "core/Widget.h"
#include "util/ParameterLookup.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class ShapeFactory;
    class WidgetFactory;

    class DAGUI_API DataTemplate
    {
    public:
        DataTemplate() = default;

        void setParameter(dagbase::Atom name, dagbase::Variant value)
        {
            if (auto it=_paramLookup.find(name); it!=_paramLookup.end())
            {
                it->second = value;
            }
            else
            {
                _paramLookup.emplace(name,value);
            }
        }

        void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory, ShapeFactory& shapeFactory);

        void resolve();

        dagbase::Variant find(std::string_view path) const;
    private:
        dagbase::Atom _name;
        dagbase::Atom _dataType;
        Widget* _tree{nullptr};
        dagbase::ParameterLookup _paramLookup;
    };
}
