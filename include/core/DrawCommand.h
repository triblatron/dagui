//
// Created by Tony Horrobin on 18/06/2025.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    class Renderer;

    class DAGUI_API DrawCommand
    {
    public:
        enum OpCode : std::uint32_t
        {
            OP_UNKNOWN,
            OP_RECTANGLE,
        };
    public:
        DrawCommand(OpCode op)
                :
                _op(op)
        {
            // Do nothing.
        }

        virtual ~DrawCommand() = default;

        virtual dagbase::Variant find(std::string_view path) const;

        virtual void makeItSo(Renderer& renderer) = 0;

        static const char* opCodeToString(OpCode op);
    private:
        OpCode _op{OP_UNKNOWN};
    };
}