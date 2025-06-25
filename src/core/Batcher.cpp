//
// Created by Tony Horrobin on 25/06/2025.
//

#include "config/config.h"

#include "core/Batcher.h"
#include "core/ConfigurationElement.h"
#include "core/RenderBin.h"

namespace dagui
{

    void Batcher::configure(dagbase::ConfigurationElement &config)
    {
        if (auto element = config.findElement("bins"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                RenderBin* bin = new RenderBin();
                bin->configure(child);
                addRenderBin(bin);

                return true;
            });
        }
    }
}