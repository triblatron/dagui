//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"
#include "util/VectorMap.h"
#include "VisualElement.h"
#include "core/Atom.h"
#include "core/DynamicVisitor.h"

#include <functional>
#include <string>

namespace dagui
{
    class VisualElement;

    using VisualElementVisitor = dagbase::DynamicVisitor<VisualElement>;

//    class DAGUI_API VisualElementVisitor
//    {
//    public:
//        using Handler = std::function<bool(VisualElement*)>;
//    public:
//        virtual ~VisualElementVisitor() = default;
//
//        void registerHandler(dagbase::Atom name, Handler handler);
//
//        Handler findHandler(dagbase::Atom name);
//
//        virtual void visit(VisualElement* visualElement) = 0;
//    private:
//        dagbase::VectorMap<dagbase::Atom, Handler> _handlers;
//    };
//
//    inline void VisualElementVisitor::registerHandler(dagbase::Atom name, Handler handler)
//    {
//        _handlers.insert(std::make_pair(name, handler));
//    }
//
//    inline VisualElementVisitor::Handler VisualElementVisitor::findHandler(dagbase::Atom name)
//    {
//        if (auto it=_handlers.find(name); it!=_handlers.end())
//            return it->second;
//        else
//            return nullptr;
//    }
}
