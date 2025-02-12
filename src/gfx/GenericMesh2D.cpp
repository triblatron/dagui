//
// Created by Tony Horrobin on 12/02/2025.
//

#include "config/config.h"

#include "gfx/GenericMesh2D.h"

namespace dagui
{
    void GenericMesh2D::addArray(const ArrayDescriptor& descriptor)
    {
        _arrays.push_back(descriptor);
    }
}
