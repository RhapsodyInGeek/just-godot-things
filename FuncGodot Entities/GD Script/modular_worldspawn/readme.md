For my func_godot games I like to make my worldspawn a WorldEnvironment.

Was working on a new project and decided to set up a GDScript worldspawn entity, but I got a bit carried away and added support for every single Environment resource feature. I do not need every single one for my game, and it gets a bit overstuffed with every single one, but instead of deleting my work I decided to make it modular. This makes the properties much easier to manage as well.

Sharing it here because someone else might find it useful for their own projects. Simply add any desired base classes to both the worldspawn definition's base classes and your FGD File's entity definitions array.

This worldspawn entity is licensed Creative Commons Zero. While unnecessary, if you want to give a shoutout in your game's credits, give it to Blue Key Games.