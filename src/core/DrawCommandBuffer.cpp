//
// Created by Tony Horrobin on 17/06/2025.
//

#include "config/config.h"

#include "core/DrawCommandBuffer.h"
#include "core/Rectangle.h"
#include "util/Searchable.h"
#include "util/enums.h"

namespace dagui
{
    class DrawCommand
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

        static const char* opCodeToString(OpCode op);
    private:
        OpCode _op{OP_UNKNOWN};
    };

    dagbase::Variant DrawCommand::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "op", std::string(opCodeToString(_op)));
        if (retval.has_value())
            return retval;

        return {};
    }

    const char *DrawCommand::opCodeToString(DrawCommand::OpCode op)
    {
        switch (op)
        {
            ENUM_NAME(OP_UNKNOWN)
            ENUM_NAME(OP_RECTANGLE)
        }
        return "<error>";
    }

    class DrawRectangle : public DrawCommand
    {
    public:
        DrawRectangle(const Rectangle& rect)
        :
                DrawCommand(OP_RECTANGLE),
                _rect(rect)
        {
            // Do nothing.
        }

        virtual dagbase::Variant find(std::string_view path) const;
    private:
        Rectangle _rect;

    };

    dagbase::Variant DrawRectangle::find(std::string_view path) const
    {
        dagbase::Variant retval =  DrawCommand::find(path);
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "rect", _rect);
        if (retval.has_value())
            return retval;

        return {};
    }

    void DrawCommandBuffer::drawRect(const Rectangle &rect)
    {
        addCommand(new DrawRectangle(rect));
    }

    dagbase::Variant DrawCommandBuffer::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "numCommands", std::int64_t(_cmds.a.size()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "commands", _cmds);
        if (retval.has_value())
            return retval;

        return {};
    }
}