//
// Created by Tony Horrobin on 16/07/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"

#include <string_view>

namespace dagui
{
    class AttributeArray;
    class IndexArray;

    class DAGUI_API MeshBackend
    {
    public:
        virtual ~MeshBackend() = default;

        virtual void addVertexBuffer() = 0;

        virtual void allocate() = 0;

        virtual void uploadVertices(AttributeArray& a) = 0;

        virtual void uploadIndices(IndexArray& a) = 0;

        virtual void draw() = 0;

        virtual dagbase::Variant find(std::string_view path) const = 0;
    };
}