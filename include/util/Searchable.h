//
// Created by tony on 05/10/24.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"

namespace nfe
{
    template<typename T>
	nfe::ConfigurationElement::ValueType findEndpoint(std::string path, const char* key, T value)
	{
		if (path == key)
		{
			return value;
		}

		return {};
	}

	//! The inline specifier is necessary to avoid multiple definition errors.
	template<> inline
		nfe::ConfigurationElement::ValueType findEndpoint(std::string path, const char* key, std::string value)
	{
		if (path == key)
		{
			return (value);
		}

		return {};
	}

	template<typename T, typename U>
	auto findInternal(const std::string& path, const char* key, U obj,
	                  ConfigurationElement::ValueType (T::*func)(const std::string&) const) ->
		nfe::ConfigurationElement::ValueType
	{
		auto pos = path.find(key);

		if (pos == 0)
		{
			auto dotPos = path.find('.');
			auto subPos = path.find('[');
			if (subPos != std::string::npos && subPos < dotPos)
			{
				return std::invoke(func, obj, path.substr(subPos));
			}
			if (dotPos < path.length() - 1)
			{
				return std::invoke(func, obj, path.substr(dotPos + 1));
			}
		}

		return {};
	}

	template<typename T, typename U>
	nfe::ConfigurationElement::ValueType findArray(std::string path, U obj, ConfigurationElement::ValueType (T::*func)(const std::string&) const)
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
					return std::invoke(func, (obj)[index], path.substr(dotPos + 1));
				}
			}
		}

		return {};
	}

	template<typename T>
	nfe::ConfigurationElement::ValueType findVector(std::string path, const T& a)
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
			if (index < (a).size())
			{
				auto dotPos = path.find('.');
				if (dotPos < path.length() - 1)
				{
					return std::to_string(a[index]);
				}
			}
		}

		return {};
	}
    
}