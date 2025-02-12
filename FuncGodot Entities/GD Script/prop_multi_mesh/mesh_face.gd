class_name MeshFace
extends RefCounted

enum Side {
	SIDE_OVER,
	SIDE_UNDER,
	SIDE_SPAWNING,
	SIDE_COPLANAR
}

var vertex: Array[Vector3] = [Vector3.ZERO,Vector3.ZERO,Vector3.ZERO]

enum ClockDirection { CLOCKWISE, COUNTER_CLOCKWISE}

func get_plane() -> Plane:
	return Plane(vertex[0], vertex[1], vertex[2])

func get_random_point_inside() -> Vector3:
	var a: float = randf_range(0, 1)
	var b: float = randf_range(0, 1)
	if a > b:
		var c: float = a
		a = b
		b = c
	return vertex[0] * a + vertex[1] * (b - a) + vertex[2] * (1 - b)

func get_area() -> float:
	return (vertex[0] - vertex[1]).cross(vertex[0] - vertex[2]).length() * 0.5

func print_face_info(face_index: int = -1) -> void:
	var s: String = "Face"
	if face_index > -1:
		s += " " + str(face_index)
	s += ":\n"
	for v in vertex:
		s += str(v) + "\n"
	s += str(get_area())
	s += "\n"
	print(s)

func _init(v1: Vector3, v2: Vector3, v3: Vector3) -> void:
	vertex[0] = v1
	vertex[1] = v2
	vertex[2] = v3
