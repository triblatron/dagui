//
// Created by Tony Horrobin on 25/06/2025.
//

#include "config/config.h"

#include "core/Batcher.h"
#include "core/ConfigurationElement.h"
#include "core/RenderBin.h"
#include "util/Searchable.h"
#include "core/PositionStack.h"
#include "core/GraphicsBackendFactory.h"

namespace dagui
{

    void Batcher::configure(dagbase::ConfigurationElement &config, GraphicsBackendFactory& factory)
    {
        if (auto element = config.findElement("mesh"); element)
        {
            _meshProto = new ShapeMesh();
            _meshProto->configure(*element);
        }
        _positionStack = factory.createPositionStack();

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
                    bin->configure(* valueElement);
                }
                addRenderBin(key, bin);

                return true;
            });
        }
    }

    dagbase::Variant Batcher::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findInternal(path, "renderBins", _renderBinArray);
        if (retval.has_value())
            return retval;

        if (_positionStack)
        {
            retval = dagbase::findEndpoint(path, "x", _positionStack->top().x);
            if (retval.has_value())
                return retval;

            retval = dagbase::findEndpoint(path, "y", _positionStack->top().y);
            if (retval.has_value())
                return retval;
        }

        return {};
    }

    void Batcher::draw()
    {
        for (auto bin : _renderBinArray.a)
        {
            bin->draw();
        }
    }

    void Batcher::translate(float x, float y)
    {
        if (_positionStack)
        {
            _positionStack->translate(x,y);
        }
    }

    void Batcher::pushTranslate()
    {
        if (_positionStack)
            _positionStack->push();
    }

    void Batcher::popTranslate()
    {
        if (_positionStack && !_positionStack->empty())
        {
            _positionStack->pop();
        }
    }
}