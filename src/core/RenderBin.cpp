//
// Created by Tony Horrobin on 25/06/2025.
//

#include "config/config.h"

#include "core/RenderBin.h"
#include "core/ConfigurationElement.h"

namespace dagui
{

    bool RenderBin::operator<(const RenderBin &other) const
    {
        return _material<other._material;
    }

    void RenderBin::configure(dagbase::ConfigurationElement &config)
    {
        // TODO:Specify primitives
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