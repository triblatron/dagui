//
// Created by Tony Horrobin on 02/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Atom.h"
#include "core/Editor.h"
#include "core/TypeRegistry.h"

namespace dagui
{
    struct EditorParameters;

    class DAGUI_API BoolEditor : public Editor
    {
    public:
        BoolEditor(const BoolEditor& other);

        BoolEditor& operator= (const BoolEditor& other);

        BoolEditor(const EditorParameters& params);

        void setName(dagbase::Atom name)
        {
            _name = name;
        }

        dagbase::Atom name() const
        {
            return _name;
        }

        void setContext(void* context) override
        {
            _context = context;
        }

        void* context()
        {
            return _context;
        }

        void setObject(void* obj) override
        {
            _object = obj;
        }

        void setProperty(const dagbase::Property& prop) override
        {
            _prop = prop;
        }

        const dagbase::Property& property() const
        {
            return _prop;
        }

        void get(bool* value);

        void set(bool value);

        void makeItSo() override;

        BoolEditor* clone() override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        dagbase::Atom _name;
        dagbase::Atom _typeName;
        void * _context{nullptr};
        void* _object{nullptr};
        dagbase::Property _prop;
    };
}
