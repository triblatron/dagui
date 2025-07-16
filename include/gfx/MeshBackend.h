//
// Created by Tony Horrobin on 16/07/2025.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    class AttributeArray;
    class IndexArray;

    class DAGUI_API MeshBackend
    {
    public:
        virtual ~MeshBackend() = default;

        virtual void allocate() = 0;

        virtual void uploadVertices(AttributeArray& a) = 0;

        virtual void uploadIndices(IndexArray& a) = 0;

        virtual void draw() = 0;
    };
}