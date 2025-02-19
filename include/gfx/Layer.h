//
// Created by Tony Horrobin on 19/02/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    //! A data structure
    struct DAGUI_API LayerAttributes
    {
        //! Texture handle, used to optimise switching between textures.
        //! \note A value of zero means no texture.
        std::uint32_t texture{0};
        //! Shader handle, used to optimise switching between shaders.
        //! \note A value of zero means no shader.
        std::uint32_t shader{0};
        //! Type of layer, used to optimise drawing of all components of a given type in one batch
        enum Type
        {
            //! Initial value, meaningless to the renderer.
            TYPE_UNKNOWN,
            //! A drop-shadow or similar
            TYPE_SHADOW,
            //! A border, typically around a control such as a label or button.
            TYPE_BORDER,
            //! A piece of text in a font, uses a texture
            TYPE_TEXT,
            //! An icon, may be vectors
            TYPE_ICON,
            //! A 2D array of pixels
            TYPE_IMAGE,
            //! A shape such as a rounded rectangle or circle.
            TYPE_SHAPE
        };
        //! Layer type, useful to draw all at once.
        Type type{TYPE_UNKNOWN};
        //! z-order
        std::uint32_t depth{0};

        void configure(dagbase::ConfigurationElement& config);

        bool operator<(const LayerAttributes& rhs) const;
    };

    class DAGUI_API Layer
    {
    public:
    private:
        LayerAttributes _attrs;
    };
}
