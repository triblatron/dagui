//
// Created by Tony Horrobin on 22/08/2025.
//

#include "config/config.h"

#include "core/RenderBinKey.h"
#include "core/ConfigurationElement.h"

namespace dagui
{

    bool RenderBinKey::operator<(const RenderBinKey &other) const
    {
        return (texture==other.texture && layer < other.layer) ||
            (material<other.material) ||
            (material==other.material && texture<other.texture) ||
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