//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/RenderBin.h"

#include <map>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Batcher
    {
    public:
        using RenderBinMap = std::map<RenderBinKey, RenderBin*>;
    public:
        void configure(dagbase::ConfigurationElement& config);

        void addRenderBin(const RenderBinKey& key, RenderBin* bin)
        {
            if (bin)
                _renderBins.emplace(key, bin);
        }

        RenderBinMap::iterator findRenderBin(const RenderBinKey& key)
        {
            return _renderBins.find(key);
        }

        RenderBinMap::iterator end()
        {
            return _renderBins.end();
        }
//        RenderBin* operator[](std::size_t index)
//        {
//            return index<_renderBins.size()?_renderBins[index]:nullptr;
//        }
//
//        const RenderBin* operator[](std::size_t index) const
//        {
//            return index<_renderBins.size()?_renderBins[index]:nullptr;
//        }
    private:
        RenderBinMap _renderBins;
    };
}
