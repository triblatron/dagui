//
// Created by Tony Horrobin on 12/02/2025.
//

#include "config/Export.h"

#include <vector>

#include "GenericAttributeArray.h"

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

        }

        void getVertex(std::size_t i, Vertex* v)
        {

        }

        void configure(dagbase::ConfigurationElement& config)
        {

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
