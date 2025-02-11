//
// Created by Tony Horrobin on 11/02/2025.
//

#pragma once

#include "Mesh2D.h"
#include "OpenGL.h"
#include "config/Export.h"

namespace dagui
{
    class Mesh2D;
}

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui::gl
{
    class DAGUI_API DrawingCommand
    {
    public:
        virtual ~DrawingCommand() = default;

        virtual void configure(dagbase::ConfigurationElement& config) = 0;

        virtual void setup(const Mesh2D& mesh) = 0;

        virtual void draw(const Mesh2D& mesh) = 0;
    };

    class DAGUI_API DrawArrays : public DrawingCommand
    {
    public:
        void configure(dagbase::ConfigurationElement& config) override;

        void setup(const Mesh2D& mesh) override;

        void draw(const Mesh2D& mesh) override;
    private:
        VertexBuffer _vertexBuffer;
        VertexBuffer _colourBuffer;
    };
}
