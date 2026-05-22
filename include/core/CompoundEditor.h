//
// Created by Tony Horrobin on 02/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Editor.h"
#include "core/TypeRegistry.h"

#include "util/SearchableArray.h"
#include <vector>

namespace dagui
{
    class DAGUI_API CompoundEditor : public Editor
    {
    public:
        CompoundEditor() = default;

        CompoundEditor(const CompoundEditor& other);

        CompoundEditor& operator=(const CompoundEditor& other);

        ~CompoundEditor();

        Editor* clone() override;

        void setName(dagbase::Atom name)
        {
            _name = name;
        }

        void setObject(void* obj) override
        {
            _object = obj;
            for (auto child : _children)
            {
                child->setObject(obj);
            }
        }

        void setProperty(const dagbase::Property &prop) override
        {
            _prop = prop;
        }

        void makeItSo() override;

        dagbase::Variant find(std::string_view path) const override;

        void addChild(Editor* child)
        {
            if (child)
            {
                _children.a.emplace_back(child);
            }
        }
    private:
        dagbase::Atom _name;
        void* _object{nullptr};
        dagbase::Property _prop;
        using ChildArray = dagbase::SearchableArray<std::vector<Editor*>>;
        ChildArray _children;
    };
}