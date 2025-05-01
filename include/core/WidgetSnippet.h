//
// Created by Tony Horrobin on 01/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Snippet.h"

namespace dagui
{
    class Widget;

    class DAGUI_API WidgetSnippet : public dagbase::Snippet
    {
    public:
        WidgetSnippet();

        void setWidget(Widget* widget)
        {
            _widget = widget;
        }

        std::string resolve(const std::string& name) override;
    private:
        Widget* _widget{nullptr};
    };
}
