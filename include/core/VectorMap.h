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
		std::pair<iterator, bool> insert(const value_type& value)
		{
			auto it = find(value.first);
			if (it == _map.end())
			{				
				_map.insert(it, value);

				return std::make_pair(_map.end() - 1, true);
			}
			else
			{
				auto equalKey = it->first == value.first;
				
				if (!equalKey)
				{
					auto d = std::distance(_map.begin(), it);
					_map.insert(it, value);
					return std::make_pair(_map.begin()+d, true);
				}
				else
				{
					return std::make_pair(it, false);
				}
			}
		}

		iterator find(const Key& key)
		{
			return std::lower_bound(_map.begin(), _map.end(), value_type(key,Value()), Compare());
		}

		iterator end()
		{
			return _map.end();
		}
	private:
		container _map;
	};
}
