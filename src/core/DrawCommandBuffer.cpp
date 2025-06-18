//
// Created by Tony Horrobin on 17/06/2025.
//

#include "config/config.h"

#include "core/DrawCommandBuffer.h"
#include "core/Rectangle.h"
#include "core/DrawCommand.h"
#include "util/Searchable.h"
#include "util/enums.h"
#include "gfx/Renderer.h"

namespace dagui
{
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

        virtual void makeItSo(Renderer& renderer) override;
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

    void DrawRectangle::makeItSo(Renderer &renderer)
    {
        renderer.drawRect(_rect);
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

    void DrawCommandBuffer::eachCommand(std::function<void(DrawCommand *)> f)
    {
        if (f)
        {
            std::for_each(_cmds.a.begin(), _cmds.a.end(), f);
        }
    }
}