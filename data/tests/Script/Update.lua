require "etc/commands"

function test()
	return 1
end

function test2(self, ...)
	local port = addPort("spoo", "DIR_INTERNAL", "FLAGS_NONE", 1.0)
	assert(port~=nil)
	return 2.0
end

root=
{
	numRequests=1,
	cmds=
	{
		{
			cmd="CMD_CONFIGURE",
			code="test2",
		},
		{
			cmd="CMD_UPDATE",
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
						direction=3,
						value=2.0,
					},
				},
			},
		},
	},
}
