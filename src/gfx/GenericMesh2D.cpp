//
// Created by Tony Horrobin on 12/02/2025.
//

#include "config/config.h"

#include "gfx/GenericMesh2D.h"

namespace dagui
{
    GenericMesh2D::~GenericMesh2D()
    {
        for (auto a : _data)
        {
            delete a;
        }
    }
}
