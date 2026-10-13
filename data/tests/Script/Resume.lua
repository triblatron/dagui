require "etc/commands"
require "luadag"

function test()
	return 1
end

function test2(self)
	local port = addPort("spoo", luadag.PortDirection_DIR_INTERNAL, luadag.Port_FLAGS_NONE, 1.0)
	assert(port~=nil)
end

root=
{
	existing=
	{
		{
			class="Node",
			properties=
			{
				totalPorts=1,
			},
		},
		{
			class="Port",
			properties=
			{
				value=1.0,
			},
		},
	},
}
