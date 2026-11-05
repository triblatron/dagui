require "etc/commands"

function test()
	return 1
end

function test2(self, ...)
	local port = addPort("spoo", "DIR_INTERNAL", "FLAGS_NONE", 1.0)
	assert(port~=nil)
	port = addPort("wibble", "DIR_INTERNAL", "FLAGS_NONE", 2.0)
	setPortValue(0, 3.0, 1, 4.0)
	return 0
end

root=
{
	numRequests=4,
	cmds=
	{
		{
			cmd="CMD_CONFIGURE",
			code="test2",
		},
		{
			cmd="CMD_STEP_UNTIL_FINISHED",
			existing=
			{
				{
					class="Node",
					properties=
					{
						totalPorts=2,
					},
				},
				{
					class="Port",
					properties=
					{
						direction=3,
						value=3.0,
					},
				},
				{
					class="Port",
					properties=
					{
						direction=3,
						value=4.0,
					},
				},
			},
		},
		{
			cmd="CMD_RESET",
		},
		{
			cmd="CMD_CONFIGURE",
			code=function(self)
				return test2(self)
			end,
		},
		{
			cmd="CMD_STEP_UNTIL_FINISHED",
			existing=
			{
				{
					class="Node",
					properties=
					{
						totalPorts=4,
					},
					count=1,
				},
				{
					class="Port",
					properties=
					{
						value=1.0,
					},
					count=1,
				},
				{
					class="Port",
					properties=
					{
						value=2.0,
					},
					count=1,
				},
				{
					class="Port",
					properties=
					{
						value=3.0
					},
					count=1,
				},
				{
					class="Port",
					properties=
					{
						value=4.0
					},
					count=1,
				},
			},
		},
	},
}
