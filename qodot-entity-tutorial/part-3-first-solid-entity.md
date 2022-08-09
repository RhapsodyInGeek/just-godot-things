# THE MAP GEOMETRY IS AN ENTITY?!
Yes. And no. If we look at _Quake_, it defines the map geometry as a `worldspawn`, just another entity in the QC scripts. Pretty weird, right? Qodot itself even treats the brushwork similarly, combining all of the non-brush-entity brushwork you do in Trenchbroom into a single _StaticBody_ node, referring to it as the _worldspawn_. So we can just leave the map brushwork alone in Trenchbroom because Qodot takes care of that for us, right?

Nah, let's not do that. Truth is, we can have a whole lot more control if we treat our map brushwork as _their own entities_.

## DEFINING THE GOAL
When defining an entity, it's best to think of it as, _"what problem am I solving?"_ Oftentimes, the problem is _"I want to be able to do / have this"_. When you get far enough, you may start finding _"I already solved this problem with a previous entity."_ Why do you need a `func_plat` when you can just modify your `func_door` that can already translate, rotate, and scale? Maybe the real answer is you just rename your `func_door` into a `func_move`?

So what problem are we solving? We want full control over map geometry, and to be able to cull sections of our map with relative ease. Qodot's default _worldspawn_ feature doesn't give us this, so we'll define our own `func_geo` solid entity to solve for this. Doing so will give us greater control over UV unwrapping, shadowcasting, and allow us to expand on features later if our game asks us for more.

## FUNC_GEO.GD
The first thing we'll do is create a script class to handle our new `func_geo` entity. Since the entity in question will be created automatically by Godot, we won't bother creating a packed scene and just settle for making a script.

Create a script in `/entities/func/` called `func_geo.gd`. Copy the following into it:

```swift
#*******************************************************************************
# FUNC_GEO / FUNC_ILLUSIONARY
# Main map geometry class.
#*******************************************************************************
extends StaticBody
tool

#*******************************************************************************
# CLASS VARIABLES
#*******************************************************************************
export(Dictionary) var properties setget set_properties

#*******************************************************************************
# CLASS FUNCTIONS
#*******************************************************************************
func set_properties(new_properties : Dictionary) -> void:
	if(properties != new_properties):
		properties = new_properties
		update_properties()

func update_properties() -> void:
	for c in get_children():
		if c is MeshInstance:
			c.use_in_baked_light = true
			if "shadowcast" in properties:
				if properties.shadowcast == 0:
					c.cast_shadow = GeometryInstance.SHADOW_CASTING_SETTING_OFF
			var m = c.get_mesh()
			if m is ArrayMesh:
				m.lightmap_unwrap(Transform.IDENTITY, 0.3125)
		if c is CollisionShape:
			c.shape.margin = 0.001
```

Alright, so _what the Shub is going on here?_ On import, Qodot will copy our _Key / Value_ pairs to a dictionary called `properties`. In order to do this, the script needs to be a `tool` type so that it can run in the Godot Editor. The rest of it should make sense upon examination.

Our `update_properties()` function is where most of the work is done. You can see that we check to see if our `properties` Dictionary has a key called `shadowcast`, and if it does we check the value of that shadowcast to set our mesh's shadow casting appropriately.

_"But wait!"_ you cry out. _"What the hell is with this 0.3125 magic number?!"_ I'm glad I assumed you asked! In a QodotMap you can set the _Inverse Scale factor_, which determines how large your map is upon import. The default is 16, but I've found that Quake 1 textures seem to prefer an inverse scale factor of _32_, so that's what I use. 1/32 = 0.3125. If you use a different scaling factor, yours should be 1 divided by your Inverse Scale factor. That line itself unwraps the mesh's UV2 for lightmapping purposes.

You don't need those last 2 lines. I did it for more accurate collision for my game (the default is 0.04) but it can affect performance so act according to what works best for your game. I just figured I'd show it here for you as an example of the flexibility offered.

## THE MAKEUP OF A SOLID CLASS ENTITY
Our next step is creating a new resource in our `/maps/fgd/solid_entities/` folder. When selecting the resource type to add, choose `QodotFGDSolidClass`. Name the new resource `func_geo_solid_class.tres`. _"Why not just name it func_geo?"_ Well, we're going to establish a naming convention here that will become important in the future when we start adding in entities that use _Packed Scenes_.

Selecting your new resource, it's time to go through it in the _Inspector_ tab:
- _Classname_: this is the `classname` key value in Trenchbroom. We'll name this solid entity class `func_geo`.
- _Description_: helper text in Trenchbroom that tells us what this entity does. We'll put `Geo mesh. Used to split up the map geometry for better culling.`
- _Base Classes_: This is an array of inherited classes. We won't worry about this for now, but it's useful when we want entities to share property types / key values, metadata, etc...
- _Class Properties_: This is where the magic begins. We define our _Key / Value_ pairs with this array. We'll attack this in a moment.
- _Class Property Descriptions_: This is where _informing_ what the magic is happens. In Trenchbroom, these will be the description text for each entity key.
- _Meta Properties_: Mostly used to define how Trenchbroom displays the entity. This is used mostly for _Point Entities_, so we'll go over it more when we get to those.
- _Node Class_: The Godot class this entity will create on import. For our func_geo, make sure it says `StaticBody`, since we'll be using this entity as our main map geometry class.
- _Spawn Type_: We've got a few options here. _Worldspawn_ will merge this with all the other _worldspawn_ entities we make, which is what we don't actually want. Choose `Entity` from the drop down list, so that it creates the brush entity as its own object for culling purposes.
- _Build Visuals_: This tells Qodot that we want to create a mesh from the combined brushwork of our func_geo. Unchecking this will skip building the mesh, rendering the entity effectively invisible. Leave it checked for this entity.
- _Collision Shape Type_: This tells Qodot how to or even whether to build collision shapes from the brushwork. We'll go over the options in more detail below. For now, leave this on `Convex`.
- _Script Class_: The script you put here gets applied to the entity on build. You know that `func_geo.gd` script we made earlier? Drag and drop that script onto this part.

## CLASS PROPERTIES
Alright, so in our func_geo script, we checked to see if we had a property key called `shadowcast`, and whether or not it equaled the integer zero. Go back to `func_geo_solid_class.tres`'s _Class Properties_ dictionary and click on it. Set the _New Key_ to a _String_ and name it `shadowcast`. Set the _New Value_ to _int_ and set it to `1`. Make sure to click _Add Key/Value Pair_. Our func_geo now has a the _shadowcast_ key with a default value of _1_, meaning it will default to casting shadows from light sources that have shadows enabled, according to our _func_geo.gd_.

Next, we'll update our _Class Property Descriptions_ dictionary. Set the _New Key_ to a _String_ and name it `shadowcast`. The key name has to match the one from _Class Properties_ exactly, so that Trenchbroom can associate them properly. Set the _New Value_ to _String_ as well, and input `Set to 0 if you want light to pass through. Useful for glass or forcefields.` Remember to click _Add Key/Value Pair_ after. When we click on the key in Trenchbroom we'll now see this description and help us know its use cases.

## COLLISION SHAPE TYPE
So the setting you choose here is going to depend on your game. `None` should be obvious: the collision shape nodes are not created, effectively removing any collision from the entity. `Convex` will create a collision shape for each individual brush that makes up the entity. `Concave` will create a single trimesh collision shape based upon the mesh prior to _Skip/Clip_ face removal. You can read more on the differences in the [Godot Docs CollisionShapes chapter](https://docs.godotengine.org/en/stable/tutorials/physics/collision_shapes_3d.html).
My recommendation is to leave it Convex if you're unsure but you should ultimately choose the option that is best for you game.

## UPDATING THE FGD
Navigate to your `/maps/fgd/GAMENAME_fgd.tres` and in the Inspector find _Entity Definitions_. Make sure the size is greater than 0. Then, drag and drop your new `func_geo_solid_class.tres` into any open slot in the Entity Definitions array. I'd recommend leaving some spaces open at the start of the array. I'm not too sure if Trenchbroom requires _base class_ dependencies to appear before the inheriting classes or not, so best to play it safe.

Once _func_geo_solid_class_ is in the Entity Definitions, _Export_ the FGD. It should replace only the FGD file in your Trenchbroom games folder. Congratulations! You should now be able to create _func_geo brush entities_!

## FUNC_ILLUSIONARY
But hey, what if we don't want all of our geometry to have collision? Well, we can easily remedy that with minimal effort!

Duplicate `/maps/fgd/solid_entities/func_geo_solid_class.tres` and rename it `func_illusionary_solid_class.tres`. Open the resource. In its _Inspector_ change the _Classname_ to `func_illusionary`. In its _Description_, change it to `Non-collidable geo mesh. Used to split up the map geometry for better culling.` And finally, change the _Collision_Shape_Type_ to `None`. Once you've done that, add it to the FGD Entity Definitions like the _func_geo_solid_entity.tres_ before it and _Export_ the FGD. Congratulations, you've made a SECOND entity!

Next, we'll be taking a look at implementing Point Entities.
