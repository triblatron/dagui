//
// Created by Tony Horrobin on 25/06/2025.
//

#include "config/config.h"

#include "core/RenderBin.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    void RenderBin::configure(dagbase::ConfigurationElement &config)
    {
        if (auto element = config.findElement("mesh"); element)
        {
            std::string meshClass;

            dagbase::ConfigurationElement::readConfig(*element, "class", &meshClass);
            delete _mesh;
            _mesh = nullptr;

            if (meshClass == "ShapeMesh")
            {
                _mesh = new ShapeMesh();
            }

            if (_mesh)
            {
                _mesh->configure(*element);
            }
        }
    }

    dagbase::Variant RenderBin::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "mesh", _mesh!=nullptr);
        if (retval.has_value())
            return retval;

        return {};
    }

    bool RenderBinKey::operator<(const RenderBinKey &other) const
    {
        return material<other.material ||
                (material==other.material && texture<other.texture) ||
                (texture==other.texture && layer < other.layer) ||
                (texture==other.texture && shader < other.shader );
    }

    void RenderBinKey::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "material", &material);
        dagbase::ConfigurationElement::readConfig(config, "texture", &texture);
        dagbase::ConfigurationElement::readConfig(config, "shader", &shader);
        dagbase::ConfigurationElement::readConfig(config, "layer", &layer);
    }
}