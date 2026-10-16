function addPort(name, direction, flags, initialValue)
	return coroutine.yield(luadag.NAMESPACE_SYSTEM, luadag.ACTION_ADD_PORT, name, direction, flags, initialValue)
end

function deletePort(id)
	return coroutine.yield(luadag.NAMESPACE_SYSTEM, luadag.ACTION_DELETE_PORT, id)
end

function look(direction)
	return coroutine.yield(luadag.NAMESPACE_USER, reptol.ACTION_LOOK, direction)
end
