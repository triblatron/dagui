//
// Created by Tony Horrobin on 15/02/2025.
//

#pragma once

#include "config/Export.h"

#include "ArrayDescriptor.h"

namespace dagui
{
    class DAGUI_API AttributeArray
    {
    public:
        AttributeArray() = default;

        virtual ~AttributeArray() = default;

        void setDescriptor(const ArrayDescriptor& descriptor)
        {
            _descriptor = descriptor;
        }
    protected:
        ArrayDescriptor _descriptor;
    };
}