//
// Created by Tony Horrobin on 25/06/2025.
//

#include "config/config.h"

#include "core/RenderBin.h"
#include "core/ConfigurationElement.h"
#include "gfx/TextureAtlas.h"

namespace dagui
{
    void RenderBin::configure(dagbase::ConfigurationElement &config)
    {
        if (auto element = config.findElement("mesh"); element)
        {
            std::string meshClass;

            dagbase::ConfigurationElement::readConfig(*element, "class", &meshClass);
            delete _mesh;
            _mesh = nullptr;

            if (meshClass == "ShapeMesh")
            {
                _mesh = new ShapeMesh();
            }

            if (_mesh)
            {
                _mesh->configure(*element);
            }
        }
    }

    dagbase::Variant RenderBin::find(std::string_view path) const
    {
        dagbase::Variant retval;

        if (_mesh)
        {
            retval = dagbase::findInternal(path, "mesh", _mesh);
            if (retval.has_value())
                return retval;
        }

        return {};
    }

    void RenderBin::draw()
    {
        if (_atlas)
        {
            _atlas->makeItSo();
        }
        if (_mesh)
        {
            _mesh->draw();
        }
        if (_atlas)
        {
            _atlas->unMakeItSo();
        }
    }
}