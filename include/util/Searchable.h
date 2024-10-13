//
// Created by tony on 05/10/24.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"

namespace nfe
{
    template<typename T>
	nfe::ConfigurationElement::ValueType findEndpoint(const std::string& path, const char* key, T value)
	{
		if (path == key)
		{
			return value;
		}

		return {};
	}

	template<typename Ref>
	auto findInternal(const std::string& path, const char* key, const Ref& obj) ->
		nfe::ConfigurationElement::ValueType
	{
		auto pos = path.find(key);

		if (pos == 0)
		{
			auto dotPos = path.find('.');
			auto subPos = path.find('[');
			if (subPos != std::string::npos && subPos < dotPos)
			{
				return std::invoke(&std::remove_pointer_t<Ref>::find, obj, path.substr(subPos));
			}
			if (dotPos < path.length() - 1)
			{
				return std::invoke(&std::remove_pointer_t<Ref>::find, obj, path.substr(dotPos + 1));
			}
		}

		return {};
	}

	template<typename Array>
	nfe::ConfigurationElement::ValueType findArray(const std::string& path, const Array& obj)
	{
		auto subPos = path.find('[');

		if (subPos == 0)
		{
			std::size_t firstIndex = subPos + 1;
			std::size_t index = 0;
			char* endPtr = nullptr;
			if (firstIndex < path.length())
			{
				index = strtoull(&path[firstIndex], &endPtr, 10);
			}
			if (index < (obj).size())
			{
				auto dotPos = path.find('.');
				if (dotPos < path.length() - 1)
				{
					return std::invoke(&std::remove_pointer_t<typename Array::value_type>::find, (obj)[index], path.substr(dotPos + 1));
				}
			}
		}

		return {};
	}
}