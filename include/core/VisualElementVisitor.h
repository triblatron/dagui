//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"
#include "util/VectorMap.h"
#include "VisualElement.h"

#include <functional>
#include <string>

namespace dagui
{
    class DAGUI_API VisualElementVisitor
    {
    public:
        using Handler = std::function<bool(VisualElement*)>;
    public:
        virtual ~VisualElementVisitor() = default;

        void registerHandler(std::string name, Handler handler);

        Handler findHandler(std::string name);

        virtual void visit(VisualElement* visualElement) = 0;
    private:
        dagbase::VectorMap<std::string, Handler> _handlers;
    };

    inline void VisualElementVisitor::registerHandler(std::string name, Handler handler)
    {
        _handlers.insert(std::make_pair(name, handler));
    }

    inline VisualElementVisitor::Handler VisualElementVisitor::findHandler(std::string name)
    {
        if (auto it=_handlers.find(name); it!=_handlers.end())
            return it->second;
        else
            return nullptr;
    }
}
