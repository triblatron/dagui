//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include <vector>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class RenderBin;

    class DAGUI_API Batcher
    {
    public:
        void configure(dagbase::ConfigurationElement& config);

        void addRenderBin(RenderBin* bin)
        {
            if (bin)
                _renderBins.emplace_back(bin);
        }

        RenderBin* operator[](std::size_t index)
        {
            return index<_renderBins.size()?_renderBins[index]:nullptr;
        }

        const RenderBin* operator[](std::size_t index) const
        {
            return index<_renderBins.size()?_renderBins[index]:nullptr;
        }
    private:
        using RenderBinArray = std::vector<RenderBin*>;
        RenderBinArray _renderBins;
    };
}
