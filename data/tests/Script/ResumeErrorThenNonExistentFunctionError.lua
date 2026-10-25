require "etc/commands"

function test()
	return 1
end

function test2(self)
	assert(false)
end

root=
{
	numRequests=0,
	cmds=
	{
		{
			cmd="CMD_CONFIGURE",
			code="test2",
		},
		{
			cmd="CMD_STEP_UNTIL_FINISHED",
			assertions=
			{
				{
					path="state",
					value="STATE_THREAD_ERROR",
				},
			},
		},
        {
            cmd="CMD_RESET",
        },
        {
			cmd="CMD_CONFIGURE",
			code="nonExistentFunction",
        },
        {
            cmd="CMD_STEP_UNTIL_FINISHED",
            assertions=
            {
                {
                    path="state",
                    value="STATE_THREAD_ERROR",
                },
            },
        },
	},
}
