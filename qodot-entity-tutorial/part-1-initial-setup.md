## BEFORE WE BEGIN…
This tutorial applies to Windows users, though should be able to be easily adapted for Mac or Linux users. This tutorial assumes you have an intermediate amount of knowledge and skill with Godot and Trenchbroom, and that you have already installed the Qodot addon. If not, please familiarize yourself with the applications and follow the [Qodot installation instructions](https://qodotplugin.github.io/docs/beginner's-guide-to-qodot/).

## FOLDER STRUCTURE
A well-organized project setup is integral to straightforward flow and rapid iteration. That's why we'll be tossing all of our FGD stuff into the same folder, like a horribly mismanaged salad.

Create 2 subfolders in your Godot project. Call one `entities` and the other `maps`.

In your `entities` subfolder, add these folders:
- _func_
- _trigger_

After that, you'll want to add this list of folders in your `maps` subfolder:
- _autosave_
- _fgd_
- _textures_

You should also set up a folder structure you expect will help maintain organization for your entity definitions. For _They Came From Dimension X_, our Qodot entity definitions are located within the `/maps/fgd` folder as follows:
- _base_entities_
- _solid_entities_
- _point_entities_

You don't have to organize them this way, but I've found it has helped me as my entity list has grown immensely in size and it will make following the rest of these tutorials far less confusing if you do.

## IGNORING AUTOSAVES
After you've created these folders, navigate to the `/maps/autosave` folder in File Explorer. Look at the _View_ settings and ensure the _File name extensions_ option is checked off. Once this is done, create a new text file in the folder and rename it `.gdignore`.

### WHY ARE WE DOING THIS?
Trenchbroom has a handy autosave feature while you're working on your maps. Godot conveniently has an autoscan feature to detect new files or updates automatically. We don't actually want all those autosaved map backups in our Godot project though. Adding a `.gdignore` file to a subfolder in your Godot project tells Godot to ignore this folder entirely. It's optional, but you'll be glad you did.

## FGD AND GAME CONFIG: GODOT
Qodot provides a default example FGD with default entities. We're not going to use them for our maps. Instead we'll just use the FGD and Trenchbroom config resources as bases for our own.

In Godot, navigate to `/addons/qodot/game definitions/fgd/`. Make a duplicate of `qodot_fgd.tres` and move the duplicate to the `/maps/fgd/` folder we made earlier. Then, navigate to `/addons/qodot/game definitions/trenchbroom/` and duplicate `qodot_trenchbroom_config_file.tres` and `qodot_trenchbroom_config_folder.tres`. Move both duplicates to our `/maps/fgd` folder.

What's your game's name? Probably best to get the acronym or abbreviation for it, because we're going to replace the `qodot` in the files we just duplicated with our game's identifier! For example: my game is called _They Came From Dimension X_. That's a bit long, so for files I just shorten it to _tcfdx_, which means I will be renaming my new FGD resources `tcfdx_fgd.tres`, `tcfdx_trenchbroom_config_file.tres`, and `tcfdx_trenchbroom_config_folder.tres`.

Now that you have your FGD config resources, let's modify them so they're appropriate for our game! We'll start with our `game_fgd.tres`. Ignore the first option, _Export File_, since we need to make a few changes first.
- _Target Folder_: This will be the location your exported _.fgd_ file gets created at. It will overwrite itself in this folder without remorse whenever you click _Export File_. Should probably read `Trenchbroom Path/games/Game Name`.
- _Fgd Name_: What it gets called when you click _Export File_. Probably best to use your game's abbreviation or acronym.
- _Base Fgd Files_: Think of this like a set of includes or inherits. For the purposes of this tutorial, leave this empty. We want to be as flexible as possible: our game decides on the way that entities are defined, not the other way around.
- _Entity Definitions_: This array will be the thing we modify the most. Any time we create a new entity, we'll be tossing the definition here. This list gets compiled into the FGD file we create when we click _Export File_, which then tells Trenchbroom what kinds of entities we can create in our maps and what key value pairs they contain. Make sure to set an appropriate size, but remember that you can always increase it later if needed.

That takes care of the FGD for now. Don't click _Export File_ just yet though. Next up is our `game_trenchbroom_config_file.tres`. Let's open that up and take a look:
- _Target File_: This should be `Trenchbroom Path/games/Game Name/GameConfig.cfg`. Trenchbroom configurations get split up into at least 3 files: the `GameConfig.cfg` file, the icon image, and the FGD files.
- _Game Name_: The full beautiful capitalized and punctuated name of your game. This is what shows up when you look at the game configuration names when making a new map or messing about in preferences.
- _Brush Tags_: I emptied this because I use a different `trigger` and `detail` system than the ones built into Qodot.
- _Face Tags_: By default this should have `clip_tag.tres` and `skip_tag.tres`. We'll actually leave this one intact, because we don't need to reinvent the wheel in this instance. Any brush faces using a `skip` texture will have that face removed from the mesh on import without affecting the collision shapes. Cool.
- _Fgd Filenames_: This is where we tell Trenchbroom what FGDs we're using. Replace the Qodot default with the FGD name you set earlier, with the _.fgd_ extension, ex: `TCFDX.fgd`

Finally we have our `game_trenchbroom_config_folder.tres`. This is pretty straightforward:
- _Trenchbroom Games Folder_: Where Trenchbroom keeps all of its game definitions. This is NOT your game's specific folder. Should read `Trenchbroom Path/games/`.
- _Icon_: The icon Trenchbroom uses for your game.
- _Game Config File_: Drag and drop your `game_trenchbroom_config_file.tres` onto this.
- _Fgd Files_: An array of your FGD.tres files. Drag and drop your `game_fgd.tres` over the Qodot default.

Make sure to save the changes you've made to all of your resource files. Once you've finished, look at your `game_trenchbroom_config_folder.tres` and finally you can click _Export File_. This SHOULD automatically create all of the required files, but just to be safe you should use the _Export File_ buttons for both `game_trenchbroom_config_folder.tres` and `game_fgd.tres`.

## FGD AND GAME CONFIG: TRENCHBROOM
In _File Explorer_, navigate to your `Trenchbroom/games/` folder. Do you see your game folder? If so, congratulations! We're almost done with the setup, but we have just one last thing to do. Open Trenchbroom, click _New Map_, then choose _Open Preferences_. Navigate to your game in the list. You should see an option for _Game Path_. This should be `Godot Project Path/maps`. Apply the changes and you should be all set!

All of your Trenchbroom _.map_ files should be saved in the `maps` folder. Your _.tscn_ QodotMaps can be saved anywhere else in your project directory, but the Trenchbroom _.map_ files should ALL be saved in the `maps` folder.

## TEXTURES
All your textures should be saved to `Godot Project Path/maps/textures/`. You can organize them into subfolders for better organization, just so long as those folders are all located within the `/maps/textures` folder we made oh so long ago. Review the [Qodot Materials docs](https://qodotplugin.github.io/docs/materials.html) for more information.

## AND THAT'S IT!
With the preliminary setup complete we can now start [defining our entities](part-2-what-is-an-entity.md)!
