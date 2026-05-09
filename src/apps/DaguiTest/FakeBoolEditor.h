//
// Created by Tony Horrobin on 09/05/2026.
//

#pragma once

#include "core/Editor.h"
#include "core/TypeRegistry.h"
#include "core/Variant.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class FakeBoolEditor : public dagui::Editor
{
public:
    FakeBoolEditor(const dagui::EditorParameters& params)
    {
        setupCalls();
    }

    void setupCalls()
    {
        ON_CALL(*this, clone()).WillByDefault([this]() {
            return new FakeBoolEditor(*this);
        });
        ON_CALL(*this, makeItSo()).WillByDefault([this]() {
            if (_object && _prop.type)
            {
                auto initialValue = (*_prop.getter)(_object);
                if (initialValue.has_value() && initialValue.index() == dagbase::Variant::TYPE_BOOL)
                {
                    (*_prop.setter)(_object, dagbase::Variant(!initialValue.asBool()));
                }
            }
        });
        ON_CALL(*this, setObject(::testing::_)).WillByDefault([this](void* obj) {
            _object = obj;
        });
        ON_CALL(*this, setProperty(::testing::_)).WillByDefault([this](const dagbase::Property& prop) {
            _prop = prop;
        });
        EXPECT_CALL(*this, setObject(::testing::_)).Times(::testing::AnyNumber());
        EXPECT_CALL(*this, setProperty(::testing::_)).Times(::testing::AnyNumber());
        EXPECT_CALL(*this, clone()).Times(::testing::AnyNumber());
        EXPECT_CALL(*this, makeItSo()).Times(::testing::AnyNumber());
    }

    FakeBoolEditor(const FakeBoolEditor& other)
    {
        _object = other._object;
        _prop = other._prop;
        setupCalls();
    }

    MOCK_METHOD(Editor*, clone, (), (override));
    MOCK_METHOD(void, setObject, (void*), (override));
    MOCK_METHOD(void, setProperty, (const dagbase::Property&), (override));
    MOCK_METHOD(void, makeItSo, (), (override));
    MOCK_METHOD(dagbase::Variant, find, (std::string_view), (const,override));
private:
    void* _object{nullptr};
    dagbase::Property _prop;
};
