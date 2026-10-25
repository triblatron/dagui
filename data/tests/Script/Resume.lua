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
	numRequests=2,
	cmds=
	{
		{
			cmd="CMD_STEP_UNTIL_FINISHED",
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
		{
			cmd="CMD_RESET",
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
						value=2.0,
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
			},
		},
	},
}
