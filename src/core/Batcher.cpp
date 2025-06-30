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
                auto bin = new RenderBin();
                RenderBinKey key;
                if (auto keyElement=child.findElement("key"); keyElement)
                {
                    key.configure(*keyElement);
                }
                if (auto valueElement = child.findElement("value"); valueElement)
                {
                    bin->configure(child);
                }
                addRenderBin(key, bin);

                return true;
            });
        }
    }
}