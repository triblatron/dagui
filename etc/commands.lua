function addPort(name, direction, flags, typeName, initialValue)
	return coroutine.yield(luadag.NAMESPACE_SYSTEM, luadag.ACTION_ADD_PORT, name, direction, flags, typeName, initialValue)
end

function deletePort(id)
	return coroutine.yield(luadag.NAMESPACE_SYSTEM, luadag.ACTION_DELETE_PORT, id)
end

function look(direction)
	return coroutine.yield(luadag.NAMESPACE_USER, reptol.ACTION_LOOK, direction)
end

root=
{
	update=function(self)
	
		addPort("spoo", luadag.PortDirection_DIR_INTERNAL, luadag.Port_FLAGS_NONE, "TestEnumWrapper", "TEST_FOO")
	end,
}
