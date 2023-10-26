#*****************************************************
# FUNC COMMON
#
# Common Func object enums and static methods.
#*****************************************************
class_name Func
extends RefCounted

enum {
	FUNC_FLAG_NO_CULL = (1 << 0),
	FUNC_FLAG_NO_JUMP = (1 << 1),
	FUNC_FLAG_PLAYER_ONLY = (1 << 2),	# FuncMove only
	FUNC_FLAG_START_OPEN = (1 << 3),	# FuncMove only
	FUNC_FLAG_TOGGLE = (1 << 4)			# FuncMove only
}

enum MoveStates {
	READY,
	MOVE,
	WAIT
}
