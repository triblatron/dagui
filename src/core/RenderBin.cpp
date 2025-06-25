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
        dagbase::ConfigurationElement::readConfig(config, "material", &_material);
        dagbase::ConfigurationElement::readConfig(config, "texture", &_texture);
    }
}