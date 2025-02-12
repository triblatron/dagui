//
// Created by Tony Horrobin on 12/02/2025.
//

#include "config/Export.h"

#include <vector>

namespace dagui
{
    struct ArrayDescriptor;
    struct AttributeLayout;
}

namespace dagui
{
    class DAGUI_API GenericMesh2D
    {
    public:
        enum Flags : std::uint32_t
        {
            FLAG_NONE = 0,
            FLAG_OWN = 1<<0
        };
    public:
        GenericMesh2D() = default;

        void addArray(const ArrayDescriptor& descriptor);

        template<typename T>
        void addData(std::size_t arrayIndex, const T* data, Flags flags)
        {
            if (arrayIndex >= _data.size())
                _data.resize(arrayIndex + 1);

            _data[arrayIndex] = data;
        }
    private:
        using ArrayDescriptorList = std::vector<ArrayDescriptor>;
        ArrayDescriptorList _arrays;
        //! We cannot delete these because we don't know the real type that was passed to addData().
        using DataArray = std::vector<const void*>;
        DataArray _data;
    };
}
