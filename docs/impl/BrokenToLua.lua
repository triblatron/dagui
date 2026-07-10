graph = 
{
	nodes = 
	{
		{ 
			id = 0, 
			class = "Boundary", 
			name = "bound1", 
			category = "Group", 
			flags = 0, 
			ports = 
			{ 
				{ 
					id = 0, 
					name = "foo", 
					type = "TYPE_BOOL", 
					direction = "DIR_OUT", 
					class = "TypedPort<bool>", 
					parent = "bound1", 
					sharedParent = nil, 
					value = true 
				}, 
				{ 
					id = 1, 
					name = "bar", 
					type = "TYPE_STRING", 
					direction = "DIR_OUT", 
					class = "TypedPort<string>", 
					parent = "bound1", 
					sharedParent = nil, 
					value = "wibble", 
				}, 
				{ 
					id = 2, 
					name = "baz", 
					type = "TYPE_INT64", 
					direction = "DIR_OUT", 
					class = "TypedPort<int64_t>", 
					parent = "bound1", 
					sharedParent = nil, 
					value = 1
				}, 
				{ 
					id = 3, 
					name = "qux", 
					type = "TYPE_DOUBLE", 
					direction = "DIR_OUT",
					class = "TypedPort<double>", 
					parent = "bound1", 
					sharedParent = nil, 
					value = 1 
				},  
			} 
		}, 
	},
	signalpaths = 
	{
	},
	children = 
	{
	}
}
