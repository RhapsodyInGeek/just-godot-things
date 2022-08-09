# YOU'RE AN ENTITY! YOU'RE AN ENTITY! YOU'RE AN ENTITY!
Before we jump into defining our game's entities, we should probably define what an entity _is_. The best way I found is to think of entities in _Quake_ design terms. That is to say, _everything_ is an entity: the player, the enemies, the ambient sounds, the doors, the lifts, the stage geometry itself… all of these should be thought of as _entities_.

Trenchbroom and Qodot support 2 different kinds of entities: _brush entities_ (a.k.a. solid) and _point entities_. Both these kingdoms of entity types have their own subtypes built into Trenchbroom that we can take advantage of.

## SOLID / BRUSH ENTITIES
Qodot refers to any entity constructed from brushwork as a _solid entity_. It isn't entirely accurate to call them "solid", since we actually don't need to provide these entities with any collision. We also don't need to provide these entities with meshes from the brushwork either. This is cool, because it means we have a lot of flexibility in how to construct our maps.

Trenchbroom refers to these entity types as _brush entities_. Trenchbroom renders these brush entities in specific ways based upon keyword prefixes. What does that mean for us? Well, let's say we want a brush entity to show up translucent in the editor: we can do this by prefixing our entity classname with `trigger_`. If we want it to appear opaque then we can omit the prefix, though I prefer to prefix with `func_` for organization and consistency purposes. These visual effects do not transfer over to Godot when imported, and are strictly to aid in Trenchbroom editor visibility.

Note that `skip` and `clip` textures will always appear translucent in Trenchbroom, and when imported to Godot these faces are removed from the mesh entirely. If collision is enabled, the collision shape will match the brushwork prior to face removal. This is useful for more than just reducing polycount and optimization: this has applications from smaller lightmaps to portal and occlusion culling, and possibly more.

The origin of solid entities will always be the center of the mesh bounds, or `AABB`. _Worldspawn_ is the exception, but by the end of all this you probably won't even use worldspawn ever again.

## POINT ENTITIES
Trenchbroom _point entities_ are all the entities not constructed via a brush. Things like players, monsters, weapons, items, lights, ambient sounds… all of these are _point entities_. Trenchbroom has a number of keyword built-ins for point entities, but the most important ones to remember are `light_` and `info_intermission`. If your entity has the `light_` prefix or is named `info_intermission`, Trenchbroom changes the way the `mangle` key value is interpreted, and thus the rotation interpretation into Godot rotations will be different. We'll look at that later.

Qodot imports point entities by placing them at positions defined by their Trenchbroom `origin` key value. _It does not copy rotations_, although setting an `angle` or `mangle` in Trenchbroom is not useless. We just have to look at how to apply our data differently.

Because Qodot imports point entities at a specific 3D position, our entity's _NodeClass_ is required to inherit from _Spatial_, otherwise Qodot will throw an error and the map build will break, and that's no good!

## KEY / VALUE PAIRS
This is where the magic happens. _Key / Value_ pairs are what gives us everything we need to build and populate our maps entirely in Trenchbroom, enabling a more consistent and smoother workflow. All entities, whether Solid or Point, have access to Key Value pairs in Trenchbroom.

In Godot, Qodot expects all of your entities to have an exported Dictionary variant called `properties`. On import, Qodot will populate this `properties` Dictionary with the _Key Value_ pairs you defined in Trenchbroom. We can then use our entity scripts to define how our entities interpret this data, in any way that we want. That `mangle` you set in Trenchbroom? We can tell Godot how we want that value interpreted.

## SETGET PROPERTIES / UPDATE_PROPERTIES()
So every time Qodot builds, it sets the entity's `properties` var with the _Key Value_ pairs from Trenchbroom. If you remember your GDScript training, variants have a feature called `setget` that calls the function we define in it. That way, every time the `properties` var is set, our script calls that function. Qodot's example entities have a pretty handy setup for taking advantage of this:

```swift
export(Dictionary) var properties setget set_properties

func set_properties(new_properties : Dictionary) -> void:
	if(properties != new_properties):
		properties = new_properties
		update_properties()

func update_properties() -> void:
	Pass
```

If you're extending from _QodotEntity_, then this will be built in and all you will need to do is define the `update_properties()` function. However, if you're choosing to extend from another Node type, you'll need to define these to all of your entity scripts.

Don't forget! Your entity script MUST be a `tool` script, or else it will not be able to run the functions that will set up your entities!

With all those concepts out of the way, I guess it's time to dig into a real example and actually learn something.
