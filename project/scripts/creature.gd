extends Node2D

var body: CreatureBody

func _ready() -> void:
	self.body = CreatureBody.new()
	add_child(body)
	#var lengths = [52, 58, 40, 60, 68, 71, 65, 50, 28, 15, 11, 9, 7, 7]
	self.body.initialize_const_radius(25, 20)
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	body.move_towards(get_global_mouse_position(), delta * 200)
