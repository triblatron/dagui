//
// Created by Tony Horrobin on 17/05/2026.
//

#pragma once

#include "imgui_internal.h"
#include "core/TypeRegistry.h"
#include "core/MetaClassRegistration.h"
#include "core/EditorRegistry.h"
#include "core/Editable.h"

namespace dagui
{
    class TestEditable : public Editable
    {
    public:
        DAGBASE_DEFINE_PROPERTY(TestEditable, asBool, foo, setFoo);

        void setFoo(bool on)
        {
            _foo = on;
        }

        bool foo() const
        {
            return _foo;
        }

        void edit(ImGuiContext *context) override;

        static dagbase::Type& getType();

        static dagbase::MetaClassRegistration<TestEditable> registration;
    private:
        bool _foo{false};
        double _bar{0.0};
        std::string _baz;
        std::int32_t _qux{0};
    };

    inline dagbase::Type& TestEditable::getType()
    {
        DAGBASE_BEGIN_COMPOUND(TestEditable)
        DAGBASE_ADD_PROPERTY(TestEditable, foo, dagbase::Boolean)
        DAGBASE_END_COMPOUND()

        return type;
    }

    inline dagbase::MetaClassRegistration<TestEditable> registration(dagbase::Atom::intern("TestEditable"));
}