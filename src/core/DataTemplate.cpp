//
// Created by Tony Horrobin on 28/10/2025.
//

#include "config/config.h"

#include "core/DataTemplate.h"
#include "core/ConfigurationElement.h"
#include "core/WidgetFactory.h"
#include "core/DynamicVisitor.h"

namespace dagui
{
    class InstantiateParametersVisitor : public dagbase::DynamicVisitor<Widget>
    {
    public:
        InstantiateParametersVisitor();

         void setLookup(dagbase::ParameterLookup * lookup)
         {
             _lookup = lookup;
         }
    private:
        dagbase::ParameterLookup* _lookup{nullptr};
    };

    InstantiateParametersVisitor::InstantiateParametersVisitor()
    {
        registerHandler(dagbase::Atom::intern("Label"), [this](Widget& widget) {
            widget.interpolate(*_lookup);
        });

    }

    void DataTemplate::configure(dagbase::ConfigurationElement &config, WidgetFactory& factory, ShapeFactory& shapeFactory)
    {
        dagbase::ConfigurationElement::readConfig(config, "name", &_name);
        dagbase::ConfigurationElement::readConfig(config, "dataType", &_dataType);
        if (auto element = config.findElement("tree"); element)
        {
            _tree = factory.create(*element, shapeFactory);
        }
    }

    void DataTemplate::resolve()
    {
        if (_tree)
        {
            InstantiateParametersVisitor visitor;

            visitor.setLookup(&_paramLookup);
            _tree->accept(visitor);
        }
    }

    dagbase::Variant DataTemplate::find(std::string_view path) const
    {
        dagbase::Variant retval;

        if (_tree)
        {
            retval = dagbase::findInternal(path, "tree", *_tree);
            if (retval.has_value())
                return retval;
        }

        return {};
    }
}