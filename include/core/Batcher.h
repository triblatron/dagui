//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/RenderBin.h"
#include "util/SearchableMap.h"

#include <map>
#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Batcher
    {
    public:
        using RenderBinMap = dagbase::SearchableMap<std::map<RenderBinKey, RenderBin*>>;
        using RenderBinArray = dagbase::SearchableArray<std::vector<RenderBin*>>;
    public:
        void configure(dagbase::ConfigurationElement& config);

        void addRenderBin(const RenderBinKey& key, RenderBin* bin)
        {
            if (bin)
            {
                _renderBins.m.emplace(key, bin);
                _renderBinArray.a.emplace_back(bin);
            }
        }

        RenderBinMap::iterator findRenderBin(const RenderBinKey& key)
        {
            return _renderBins.m.find(key);
        }

        RenderBinMap::iterator end()
        {
            return _renderBins.m.end();
        }

        dagbase::Variant find(std::string_view path) const;
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
        RenderBinArray _renderBinArray;
    };
}
