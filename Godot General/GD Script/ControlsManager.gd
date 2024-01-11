extends Node
class_name ControlsManager

#*************************************************************
# VARIABLES
#*************************************************************
const DEADZONE: float = 0.333
enum InputMode { Keyboard, Xbox, PSX, SNES }
enum InputType { KEY, MOUSEBUTTON, MOUSEAXIS, JOYBUTTON, JOYAXIS }

const DEFAULT_ACTION_MAP: Dictionary = {
	"move_forward":		[InputType.KEY, KEY_W,		InputType.JOYAXIS, JOY_AXIS_LEFT_Y],
	"move_backward":	[InputType.KEY, KEY_S,		InputType.JOYAXIS, JOY_AXIS_LEFT_Y],
	"move_right":		[InputType.KEY, KEY_D,		InputType.JOYAXIS, JOY_AXIS_LEFT_X],
	"move_left":		[InputType.KEY, KEY_A,		InputType.JOYAXIS, JOY_AXIS_LEFT_X],
	"jump":				[InputType.KEY, KEY_SPACE,	InputType.JOYBUTTON, JOY_BUTTON_A],
	"run":				[InputType.KEY, KEY_SHIFT,	InputType.JOYBUTTON, KEY_SHIFT],
	"crouch":			[InputType.KEY, KEY_CTRL,	InputType.JOYBUTTON, JOY_BUTTON_B],
	"attack":			[InputType.MOUSEBUTTON, MOUSE_BUTTON_LEFT,	InputType.JOYAXIS, JOY_AXIS_TRIGGER_RIGHT],
	"alt_attack":		[InputType.MOUSEBUTTON, MOUSE_BUTTON_RIGHT,	InputType.JOYAXIS, JOY_AXIS_TRIGGER_LEFT],
	"use":				[InputType.KEY, KEY_E,		InputType.JOYBUTTON, JOY_BUTTON_X],
	"torch":			[InputType.KEY, KEY_F,		InputType.JOYBUTTON, JOY_BUTTON_Y],
	"weapon_wheel":		[InputType.KEY, KEY_Q,		InputType.JOYBUTTON, JOY_BUTTON_RIGHT_SHOULDER],
	"menu":				[InputType.KEY, KEY_ESCAPE,	InputType.JOYBUTTON, JOY_BUTTON_BACK],
	"objective":		[InputType.KEY, KEY_TAB,	InputType.JOYBUTTON, JOY_BUTTON_START]
}

var action_map: Dictionary = {}
var action_list: Array[String] = []

var map: Dictionary = {}
var input_mode: InputMode = InputMode.Keyboard
var mouse_motion: Vector2 = Vector2.ZERO
var mouse_sensitivity: Vector2 = Vector2(3.0, 3.0):
	set(value):
		mouse_sensitivity = value
		mouse_sensitivity.x = clampf(value.x, 0.01, 20.0)
		mouse_sensitivity.y = clampf(value.y, 0.01, 20.0)
var mouse_invert: Vector2 = Vector2.ONE:
	set(value):
		if value.length() != 0:
			mouse_invert = value.sign()
var gamepad_sensitivity: Vector2 = Vector2(3.0, 3.0):
	set(value):
		gamepad_sensitivity = value
		gamepad_sensitivity.x = clampf(value.x, 0.01, 20.0)
		gamepad_sensitivity.y = clampf(value.y, 0.01, 20.0)
var gamepad_invert: Vector2 = Vector2.ONE:
	set(value):
		if value.length() != 0:
			gamepad_invert = value.sign()
var move_motion: Vector2 = Vector2.ZERO
var lockout: float = 0.0
var console_mode: bool = false
var remap_mode: bool = false
var remap_wait: float = 0.0
var remap_action: String
var held_time: Dictionary = {}

signal console_input
signal action_pressed(action: String)
signal action_released(action: String)
signal action_remapped(action: String, remap_successful: bool)

#*************************************************************
# PROPERTIES
#*************************************************************
func locked() -> bool:
	return lockout > 0.0

#*************************************************************
# INPUT MODE DETECTION
#*************************************************************
# Gamepad initialize
func _gamepad_connected(device_id: int, pad_connected: bool) -> void:
	if pad_connected:
		if Input.is_joy_known(device_id):
			# Determine what kind of controller it is
			if input_mode != InputMode.Keyboard:
				var joy_name: String = Input.get_joy_name(device_id)
				if joy_name.find("SNES") > -1:
					input_mode = InputMode.SNES
				elif joy_name.find("PS4") > -1:
					input_mode = InputMode.PSX
				else:
					input_mode = InputMode.Xbox

# Detect current input method; used for UI display mostly
func input_mode_swap(event: InputEvent) -> void:
	if input_mode == InputMode.Keyboard:
		var pad: bool = false
		if event.get_class() == "InputEventJoypadButton":
			pad = true
		# If we don't check the deadzone, it will always be putting out the JoypadMotion event
		if event.get_class() == "InputEventJoypadMotion" and absf(event.call("get_axis_value")) > DEADZONE:
			pad = true
		if pad:
			var joy_name: String = Input.get_joy_name(0)
			if joy_name.find("SNES") > -1:
				input_mode = InputMode.SNES
			elif joy_name.find("PS4") > -1:
				input_mode = InputMode.PSX
			else:
				input_mode = InputMode.Xbox
	elif event.get_class() == "InputEventKey" or event.get_class() == "InputEventMouseButton":
		input_mode = InputMode.Keyboard

#*************************************************************
# MAPPING
#*************************************************************
func set_action_map() -> void:
	var map_input: Array[int] = [0,0,0,0]
	for action in action_map.keys():
		InputMap.action_erase_events(action)
		
		for j in 2:
			if j == InputMode.Keyboard:
				map_input[0] = action_map[action][0] # input event type
				map_input[1] = action_map[action][1] # input global constant
				match map_input[0]:
					InputType.KEY:
						var event:= InputEventKey.new()
						event.keycode = map_input[1] as Key
						InputMap.action_add_event(action, event)
					InputType.MOUSEBUTTON:
						var event:= InputEventMouseButton.new()
						event.button_index = map_input[1] as MouseButton
						InputMap.action_add_event(action, event)
			else: # Gamepad
				map_input[2] = action_map[action][2] # input event type
				map_input[3] = action_map[action][3] # input global constant
				match map_input[0]:
					InputType.JOYBUTTON:
						var event:= InputEventJoypadButton.new()
						event.button_index = map_input[1] as JoyButton
						InputMap.action_add_event(action, event)
					InputType.JOYAXIS:
						var event:= InputEventJoypadMotion.new()
						event.axis = map_input[1] as JoyAxis
						InputMap.action_add_event(action, event)
						InputMap.action_set_deadzone(action, DEADZONE)

func reset_to_defaults() -> void:
	action_map = DEFAULT_ACTION_MAP.duplicate(true)
	mouse_sensitivity = Vector2(0.5, 0.5)
	mouse_invert = Vector2.ONE
	gamepad_invert = Vector2.ONE
	set_action_map()

func set_remap_mode(new_remap_action: String, new_remap_wait: float = 0.1, new_remap_mode: bool = true) -> void:
	remap_action = new_remap_action
	remap_wait = new_remap_wait
	remap_mode = new_remap_mode

func action_remap(event: InputEvent, action: String) -> bool:
	var _class: String = event.get_class()
	
	# We don't accept mouse motion because it's more work than I feel like putting in right now, maybe later
	if _class == "InputEventMouseMotion":
		return false
	
	var new_input: Array[int] = [-1, -1]
	var input_check: Array[int] = [-1, -1]
	
	if _class == "InputEventKey":
		input_check[1] = (event as InputEventKey).keycode
		if input_check[1] != KEY_QUOTELEFT:
			if input_check[1] < KEY_F1 or input_check[1] > KEY_F12:
				new_input[0] = InputType.KEY
				new_input[1] = input_check[1]
	elif _class == "InputEventMouseButton":
		new_input[0] = InputType.MOUSEBUTTON
		new_input[1] = (event as InputEventMouseButton).button_index
	elif _class == "InputEventJoypadMotion":
		input_check[1] = (event as InputEventJoypadMotion).axis
		# Joysticks are reserved for movement and aim, but the triggers are fair game
		if input_check[1] > JOY_AXIS_RIGHT_Y:
			new_input[0] = InputType.JOYAXIS
			new_input[1] = input_check[1]
	elif _class == "InputEventJoypadButton":
		input_check[1] = (event as InputEventJoypadButton).button_index
		if input_check[1] != JOY_BUTTON_GUIDE and input_check[1] != JOY_BUTTON_MISC1:
			new_input[0] = InputType.JOYBUTTON
			new_input[1] = input_check[1]
	
	if new_input[1] >= 0:
		# Grab the right map
		var in_ofs: int = 0
		if input_mode != InputMode.Keyboard:
			in_ofs = 2
		
		# We need to make sure we don't have 2 actions assigned to the same input
		input_check[0] = action_map[action][in_ofs]
		input_check[1] = action_map[action][in_ofs + 1]
		for k in action_map.keys():
			if action_map[k][in_ofs + 1] == new_input[1] and action_map[k][in_ofs] == new_input[0]:
				action_map[k] = input_check
				break
		
		# Setting up the new control
		action_map[action] = new_input
		
		# Remap Controls
		set_action_map()
		release_all()
		emit_signal("action_remapped", remap_action, true)
		return true
	
	# We couldn't remap it?
	release_all()
	emit_signal("action_remapped", remap_action, false)
	return false

func action_to_ui(action: String, mode: int):
	var ui_input: Array[int] = [-1, -1]
	if mode == InputMode.Keyboard:
		ui_input[0] = action_map[action][0]
		ui_input[1] = action_map[action][1]
	else:
		ui_input[0] = action_map[action][2]
		ui_input[1] = action_map[action][3]
	
	if ui_input[0] == InputType.KEY:
		return OS.get_keycode_string(ui_input[1])
	
	if ui_input[0] == InputType.MOUSEBUTTON:
		match ui_input[1]:
			MOUSE_BUTTON_LEFT:
				return "Mouse Left"
			MOUSE_BUTTON_RIGHT:
				return "Mouse Right"
			MOUSE_BUTTON_MIDDLE:
				return "Mouse Middle"
			MOUSE_BUTTON_WHEEL_UP:
				return "Mouse Wheel Up"
			MOUSE_BUTTON_WHEEL_DOWN:
				return "Mouse Wheel Down"
			MOUSE_BUTTON_WHEEL_RIGHT:
				return "Mouse Wheel Right"
			MOUSE_BUTTON_WHEEL_LEFT:
				return "Mouse Wheel Left"
			MOUSE_BUTTON_XBUTTON1:
				return "Mouse X Button 1"
			MOUSE_BUTTON_XBUTTON2:
				return "Mouse X Button 2"
	
	if ui_input[0] == InputType.JOYBUTTON:
		return ""
	
	if ui_input[0] == InputType.JOYAXIS:
		return ""

#*************************************************************
# INPUT STATES
#*************************************************************
func pressed(action: String) -> bool:
	return lockout <= 0.0 and Input.is_action_just_pressed(action)

func released(action: String) -> bool:
	return lockout <= 0.0 and Input.is_action_just_released(action)

func held(action: String) -> bool:
	return lockout <= 0.0 and Input.is_action_pressed(action)

func update_held_time(delta: float) -> void:
	for action in action_list:
		if held(action):
			held_time[action] = held_time[action] + delta
		else:
			held_time[action] = 0.0

func release_all() -> void:
	for action in action_list:
		Input.action_release(action)

func mouse_lock(is_locked: bool) -> void:
	if is_locked:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	else:
		Input.set_mouse_mode(Input.MOUSE_MODE_HIDDEN)

#*************************************************************
# BASE PROCESSING
#*************************************************************
func _init():
	set_process_mode(Node.PROCESS_MODE_ALWAYS)
	action_map = DEFAULT_ACTION_MAP.duplicate(true)
	for action in action_map.keys():
		action_list.push_back(action)
		held_time[action] = 0.0

func _ready():
	Input.connect("joy_connection_changed", Callable(self, "_gamepad_connected"))
	for action in action_list:
		InputMap.add_action(action)
	set_action_map()

func _process(delta):
	if input_mode == InputMode.Keyboard:
		set_deferred("mouse_motion", Vector2.ZERO)
	update_held_time(delta)
	if lockout > 0:
		release_all()
		lockout -= delta

func _input(event):
	# Dev console eats inputs
	if console_mode:
		if event.get_class() == "InputEventKey":
			emit_signal("console_input", event)
		get_viewport().set_input_as_handled()
		return
	
	# Input remapping eats inputs
	if remap_mode:
		if remap_wait > 0.0:
			remap_wait -= get_process_delta_time()
		elif action_remap(event, remap_action):
			remap_mode = false
		get_viewport().set_input_as_handled()
		return
	
	input_mode_swap(event)
	
	# Mouselook
	if event.get_class() == "InputEventMouseMotion" and input_mode == InputMode.Keyboard:
		mouse_motion = Vector2((event as InputEventMouseMotion).relative) * mouse_sensitivity
	
	# Gamepad movement
	elif event.get_class() == "InputEventJoypadMotion" and input_mode != InputMode.Keyboard:
		var axis: int = (event as InputEventJoypadMotion).axis
		var av: float = (event as InputEventJoypadMotion).axis_value
		
		match axis:
			# Movement
			JOY_AXIS_LEFT_X:
				if abs(av) > 0.0:
					move_motion.x = av
				else:
					move_motion.x = 0.0
			JOY_AXIS_LEFT_Y:
				if abs(av) > 0.0:
					move_motion.y = av
				else:
					move_motion.y = 0.0
			# Aiming
			JOY_AXIS_RIGHT_X:
				if abs(av) > 0.0:
					mouse_motion.x = av * mouse_sensitivity.x
				else:
					mouse_motion.x = 0.0
			JOY_AXIS_RIGHT_Y:
				if abs(av) > 0.0:
					mouse_motion.y = av * mouse_sensitivity.y
				else:
					mouse_motion.y = 0.0
	
	# Input states
	for action in action_list:
		if event.is_action_pressed(action):
			emit_signal("action_pressed", action)
			break
		if event.is_action_released(action):
			emit_signal("action_released", action)
			break
