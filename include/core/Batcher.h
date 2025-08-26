//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/RenderBin.h"
#include "util/SearchableMap.h"

#include <glm/glm.hpp>

#include <map>
#include <string_view>
#include <cstdint>
#include <stack>

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

        RenderBinMap::iterator addRenderBin(const RenderBinKey& key, RenderBin* bin)
        {
            if (bin)
            {
                auto p = _renderBins.m.emplace(key, bin);
                _renderBinArray.a.emplace_back(bin);
                return p.first;
            }
            else
            {
                return end();
            }
        }

        std::int32_t allocateTexture()
        {
            return _nextTextureIndex++;
        }

        RenderBinMap::iterator findOrCreateRenderBin(const RenderBinKey& key)
        {
            if (auto it = _renderBins.m.find(key); it!=_renderBins.m.end())
            {
                return it;
            }
            else
            {
                if (_meshProto)
                {
                    auto *bin = new RenderBin();
                    bin->setMesh(new ShapeMesh(*_meshProto));
                    return addRenderBin(key, bin);
                }
                else
                {
                    return end();
                }
            }
        }

        RenderBinMap::iterator end()
        {
            return _renderBins.m.end();
        }

        dagbase::Variant find(std::string_view path) const;

        RenderBin* operator[](std::size_t index)
        {
            return index<_renderBinArray.size()?_renderBinArray.a[index]:nullptr;
        }

        const RenderBin* operator[](std::size_t index) const
        {
            return index<_renderBinArray.size()?_renderBinArray.a[index]:nullptr;
        }

        void translate(float x, float y);

        void pushTranslate();

        void popTranslate();

        void draw();
    private:
        RenderBinMap _renderBins;
        RenderBinArray _renderBinArray;
        ShapeMesh* _meshProto{nullptr};
        glm::vec2 _translation{};
        using PositionStack = std::stack<glm::vec2>;
        PositionStack _positionStack;
        std::int32_t _nextTextureIndex{0};
    };
}
