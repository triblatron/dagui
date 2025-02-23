//
// Created by Tony Horrobin on 12/02/2025.
//

#include "config/Export.h"

#include <vector>

#include "GenericAttributeArray.h"
#include "OpaqueAttributeArray.h"
#include "core/ConfigurationElement.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    struct ArrayDescriptor;
    class AttributeArray;
    struct AttributeLayout;

    template<typename Vertex>
    class GenericMesh
    {
    public:
        enum Flags : std::uint32_t
        {
            FLAG_NONE = 0,
            FLAG_OWN = 1<<0
        };
    public:
        GenericMesh() = default;

        ~GenericMesh()
        {
            for (auto a : _data)
                delete a;
        }

        void addAttribute(const AttributeLayout& layout)
        {
            _attrs.push_back(layout);
        }

        void addVertex(const Vertex& v)
        {
            // TODO:Copy data into attribute arrays
            for (auto a : _data)
            {
                a->addVertex(&v, sizeof(Vertex));
            }
        }

        void getVertex(std::size_t i, Vertex* v)
        {
            // TOOD:Copy from all arrays into v
            if (v)
            {
                for (auto a : _data)
                {
                    a->getVertex(i, v, sizeof(Vertex));
                }
            }
        }

        void configure(dagbase::ConfigurationElement& config)
        {
            if (auto element = config.findElement("arrays"); element)
            {
                std::size_t offsetSoFar = 0;
                element->eachChild([this,&offsetSoFar](dagbase::ConfigurationElement& child)
                {
                    ArrayDescriptor descriptor;
                    descriptor.offsetSoFar = offsetSoFar;
                    descriptor.configure(child);
                    offsetSoFar += descriptor.size();
                    auto array = new OpaqueAttributeArray();
                    array->setDescriptor(descriptor);
                    _data.push_back(array);
                    return true;
                });


            }
            // for each array do
            //   create a VertexArray
        }
        // void addData(std::size_t arrayIndex, AttributeArray* data)
        // {
        //     if (arrayIndex >= _data.size())
        //         _data.resize(arrayIndex + 1);
        //
        //     _data[arrayIndex] = data;
        // }
    private:
        using AttributeLayoutArray = std::vector<dagui::AttributeLayout>;
        AttributeLayoutArray _attrs;
        //! We cannot delete these because we don't know the real type that was passed to addData().
        using AttributeArrays = std::vector<AttributeArray*>;
        AttributeArrays _data;
    };
}
