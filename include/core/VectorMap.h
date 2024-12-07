#pragma once

#include <vector>
#include <utility>

namespace dagui
{
	template <typename Key, typename Value>
	class VectorMap
	{
	public:
		typedef std::pair<Key, Value> value_type;
		typedef std::vector<value_type> container;
		typedef typename container::iterator iterator;
		struct Compare
		{
			bool operator()(const value_type& op1, const value_type& op2) const
			{
				return op1.first < op2.first;
			}
		};
	public:
		void insert(const value_type& value)
		{
			_map.push_back(value);
			std::sort<typename container::iterator>(_map.begin(), _map.end(), Compare());
		}

		iterator find(const Key& key)
		{
			return std::lower_bound(_map.begin(), _map.end(), value_type(key,0), Compare());
		}

		iterator end()
		{
			return _map.end();
		}
	private:
		container _map;
	};
}
