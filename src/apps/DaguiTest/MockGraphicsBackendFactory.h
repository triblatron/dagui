//
// Created by Tony Horrobin on 26/08/2025.
//

#pragma once

#include "core/GraphicsBackendFactory.h"
#include "gfx/MeshBackend.h"
#include "gfx/TextureAtlasBackend.h"
#include "core/PositionStack.h"
#include "core/Blending.h"

#include <gmock/gmock.h>
#include <stack>

class MockMeshBackend : public dagui::MeshBackend
{
public:
    MOCK_METHOD(void, addVertexBuffer, (), (override));

    MOCK_METHOD(void, allocate, (), (override));

    MOCK_METHOD(void, uploadVertices, (std::size_t bufferIndex, dagui::AttributeArray& a), (override));

    MOCK_METHOD(void, uploadIndices, (dagui::IndexArray& a), (override));

    MOCK_METHOD(void, draw, (), (override));

    MOCK_METHOD(dagbase::Variant, find, (std::string_view path), (const, override));
};

class MockTextureAtlasBackend : public dagui::TextureAtlasBackend
{
public:
    MOCK_METHOD(void, allocate, (), (override));
    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, setParameters, (), (override));
    MOCK_METHOD(void, upload, (dagui::Image&), (override));
    MOCK_METHOD(void, unbind, (), (override));
};

class MockPositionStack : public dagui::PositionStack
{
public:
    MockPositionStack()
    {
        ON_CALL(*this, push()).WillByDefault([this]() {
            _stack.push(_position);
        });

        ON_CALL(*this, pop()).WillByDefault([this]() {
            if (!_stack.empty())
            {
                _position = _stack.top();
                _stack.pop();
            }
        });

        ON_CALL(*this, top()).WillByDefault([this]() {
            return _position;
        });

        ON_CALL(*this, translate(::testing::_,::testing::_)).WillByDefault([this](float x, float y) {
            _position.x += x;
            _position.y += y;
        });

        ON_CALL(*this, empty()).WillByDefault([this]() {
            return _stack.empty();
        });
    }

    MOCK_METHOD(void, push, (), (override));
    MOCK_METHOD(glm::vec2, top, (), (const,override));
    MOCK_METHOD(bool, empty, (), (const,override));
    MOCK_METHOD(void, pop, (), (override));
    MOCK_METHOD(void, translate, (float,float), (override));

private:
    using Stack = std::stack<glm::vec2>;
    Stack _stack;
    glm::vec2 _position{};
};

class MockBlending : public dagui::Blending
{
public:
    MOCK_METHOD(void, enable, (), (override));
    MOCK_METHOD(void, makeItSo, (), (override));
    MOCK_METHOD(void, disable, (), (override));
};

class MockGraphicsBackendFactory : public dagui::GraphicsBackendFactory
{
public:
    MockGraphicsBackendFactory()
    {
        ON_CALL(*this, createMesh).WillByDefault([this](dagui::Mesh* mesh) {
            auto backend = new MockMeshBackend();
            EXPECT_CALL(*backend, addVertexBuffer()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, allocate()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, uploadVertices(::testing::_, ::testing::_)).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, uploadIndices(::testing::_)).Times(::testing::AnyNumber());

            _meshes.emplace_back(backend);

            return backend;
        });

        ON_CALL(*this, createTextureAtlas).WillByDefault([this](dagui::TextureAtlas* atlas) {
            auto backend = new MockTextureAtlasBackend();
            EXPECT_CALL(*backend, allocate()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, bind()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, setParameters()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, upload(::testing::_)).Times(::testing::AnyNumber());

            _atlases.emplace_back(backend);

            return backend;
        });

        ON_CALL(*this, createPositionStack).WillByDefault([this]() {
            auto backend = new MockPositionStack();
            EXPECT_CALL(*backend, push()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, empty()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, top()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, pop()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, translate(::testing::_,::testing::_)).Times(::testing::AnyNumber());

            _positionStacks.emplace_back(backend);

            return backend;
        });

        ON_CALL(*this, createBlending).WillByDefault([this]() {
            auto backend = new MockBlending();

            EXPECT_CALL(*backend, enable()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, makeItSo()).Times(::testing::AnyNumber());
            EXPECT_CALL(*backend, disable()).Times(::testing::AnyNumber());

            _blendings.emplace_back(backend);

            return backend;
        });
    }

    ~MockGraphicsBackendFactory() override
    {
        for (auto obj : _meshes)
        {
            delete obj;
        }

        for (auto obj : _atlases)
        {
            delete obj;
        }

        for (auto obj : _positionStacks)
        {
            delete obj;
        }

        for (auto obj : _blendings)
        {
            delete obj;
        }
    }
    MOCK_METHOD(dagui::MeshBackend*, createMesh, (dagui::Mesh*), (override));
    MOCK_METHOD(dagui::TextureAtlasBackend*, createTextureAtlas, (dagui::TextureAtlas *atlas), (override));
    MOCK_METHOD(dagui::PositionStack*, createPositionStack, (), (override));
    MOCK_METHOD(dagui::Blending*, createBlending, (), (override));
private:
    std::vector<MockMeshBackend*> _meshes;
    std::vector<MockTextureAtlasBackend*> _atlases;
    std::vector<MockPositionStack*> _positionStacks;
    std::vector<MockBlending*> _blendings;
};
