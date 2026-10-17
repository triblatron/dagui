require "etc/commands"

root=
{
	numRequests=0,
	cmds=
	{
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
