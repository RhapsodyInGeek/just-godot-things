QODOT ROOM + PORTAL BRUSH ENTITIES v1.0
2022, Tim Maccabe (RhapsodyInGeek), Blue Key Games
twitter.com/BlueKeyGames

#*************************************************************************************************
# INSTRUCTIONS
#*************************************************************************************************
GODOT SETUP:------------------------------------------------------------------------------------------
Extract to your game's project folder. Then, make sure the .tres files are added to the 
entity definitions of your game's FGD resource. NOTE: You may have to update .tres files to correctly
point to the trigger_room and trigger_portal .gd scripts. Don't forget to update the FGD!

Before hopping into Trenchbroom, you'll want to make sure you have a decent grasp of how 
Rooms + Portals work in Godot.
https://docs.godotengine.org/en/stable/tutorials/3d/portals/index.html

I'm not going to cover best practices with Rooms + Portals in this readme. The instructions here 
are solely on how to set them up between Trechbroom and Godot via Qodot.

First step, you'll need to make sure you have an entity set up specifically for splitting up 
your worldspawn geometry. I have a "func_geo" entity that builds both visual and collision data, 
along with a special script that automatically unwraps a UV2 for lightmap baking and sets up 
the mesh's Portal Mode to "Static", as they will not be leaving their locations. 
I have a "func_illusionary" entity set up the same way, except it does not create a CollisionShape.

I also have a "func_move" entity for doors, lifts, and other moving brushes. Their meshes' Portal Modes 
are set to "Global", since they may have the ability to not only travel from room to room but be in 
several rooms at once.

Your game may have different needs for the type of game you're creating. Set up your own brush entities 
accordingly. The important thing is to try to make sure there are zero meshes in the Worldspawn entity,
or else they may be culled into VISUAL NON-EXISTENCE!!! And that would be sad.

Once you have all of your brush entities set up and your FGD updated, you're ready
to hop into...

TRENCHBROOM:------------------------------------------------------------------------------------------
I'd advise creating special "room" and "portal" textures for clarity's sake. They're not required,
but you'll thank yourself later. You will also need to have specified a SKIP texture.

To set up a Room, create a SINGLE convex brush that will completely (or at least nearly completely)
envelop the geometry you wish to have visible within it. Convert it into a TRIGGER_ROOM brush entity
and voila! You have a room! The "targetname" field is only required if you wish to have any Portals
linked to a Room. If no Portals will be linked, no "targetname" is required. If you do assign a
"targetname" it MUST be unique. Rooms must also be made up of only a single brush. Otherwise bad things happen.

To set up a Portal, create a single convex brush with the SKIP texture. Then, assign one face any
other visible texture (that "portal" texture I recommended earlier is a good one to use!). This face
is going to be your Portal. Align your Portal face so that it is flush between the two Rooms you wish
to connect. Convert the brush into a TRIGGER_PORTAL, then assign the "target" to the Room "targetname"
you wish to link it to.

BACK TO GODOT:------------------------------------------------------------------------------------------
Make sure you have a RoomManager node in your SceneTree. Choose the QodotMap node as your RoomList.
After performing a Full Build of your map with Qodot, convert your rooms and make sure they're working
appropriately. You may need to convert them at runtime via script, depending upon your game's setup.
Consult the Godot Rooms + Portals guide linked above for more details and tips.

Let the culling begin!   (:

#*************************************************************************************************
# MIT LICENSE
#*************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.