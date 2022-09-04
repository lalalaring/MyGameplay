The gameplay framework has a full set of Lua script bindings allowing users to write the majority of their game completely from scripts. Also included is an open-source tool that can be used to generate bindings for user-defined classes (including classes that depend on features and technology of the gameplay framework itself).

## <a name="Writing_Lua_scripts"/>Writing Lua scripts

To write Lua scripts, you can use any text editor or IDE. If Visual Studio is your editor of choice, it is recommended that you install the [Lua Language Support extension](http://vslua.codeplex.com/), which adds syntax highlighting to the editor.

To generate your own bindings, you will need to download and install [Doxygen](http://www.doxygen.org).

## <a name="Lua_Basics"/>Lua Basics

For an introductory look at scripting with Lua, including basic language features, visit http://www.lua.org/pil/. Note that the documentation available at this link is aimed at Lua 5.0 and will contain some outdated APIs. However, the basic usage of the language remains the same.

There are two conventions that gameplay uses within its Lua script bindings. First, in order to create a new object, the user calls the **new** function on the class and passes the correct arguments for the corresponding C++ constructor. e.g.

```lua
-- Create a new Vector2 object.
local v = Vector2.new(1.0, 3.7)
```

## Game – Script Event Callbacks

In order to write a game primarily using Lua scripts, one must register for the main game events (initialize, update, render, and finalize), along with the desired input event handlers. To do this, you must edit the ‘scripts' section of the game's `game.config` file. If the user has a Lua script with functions for the four major events and functions that handle the key and touch input, the ‘scripts' section would look something like this:

```
scripts
{
    initialize = res/script.lua#initialize
    update = res/script.lua#update
    render = res/script.lua#render
    finalize = res/script.lua#finalize
    keyEvent = res/script.lua#keyEvent
    touchEvent = res/script.lua#touchEvent
}
```

Then on the C++ side, as usual, you must derive a class from Game and create a static instance on the stack. However, in the case of a script-based game, the .h file can simply contain the class definition with all empty implementations while the .cpp file would contain the static instance. For example,

MyScriptGame.h

```c++
class MyScriptGame: public Game
{
protected:
    void initialize() {};
    void finalize() {};
    void update(float elapsedTime) {};
    void render(float elapsedTime) {};
};
```

MyScriptGame.cpp

```c++
#include "MyScriptGame.h"
// Declare our game instance
MyScriptGame game;
```

The sample does not require any other C++ implementation. For a complete sample game using the techniques described above, see [sample05-lua](https://github.com/blackberry/GamePlay/tree/master/gameplay-samples/sample05-lua) as one-to-one mapping of sample00-mesh with a box instead of a duck. It also has some additional scripting code showcasing AIAgent and AIStates.

## Extending ScriptTarget 

To add scriptable events to your own class, you simply derive from the class `gameplay::ScriptTarget` and add the required function calls. First, to define the scriptable events that are supported for the class, we call `addScriptEvent` with the name of the event and, optionally (depending on if the callback takes arguments or not), the parameter string for a valid script callback function for that event (the parameter string follows the format of the parameter string argument to `ScriptController::executeFunction`). For example, to add a ‘notify' event that passes an integer to the callback, we would do the following:

```lua
addScriptEvent("notify", "i");
```

The supported format identifiers for passed parameters are as follows:

<table cellspacing=0 cellpadding=0>
 <tr>
  <th align=left>Format</th>
  <th align=left>Parameter Type (result)</th>
 </tr>
 <tr>
  <td>c, h, i, l</td>
  <td>Signed Integer (int)</td>
 </tr>
 <tr>
  <td>u</td>
  <td>Unsigned Integer (unsigned int)</td>
 </tr>
 <tr>
  <td>b</td>
  <td>Boolean (bool) </td>
 </tr>
 <tr>
  <td>f, d</td>
  <td>Floating Point (double)</td>
 </tr>
 <tr>
  <td>s</td>
  <td>String (char*)</td>
 </tr>
 <tr>
  <td>p</td>
  <td>Pointer (void*)</td>
 </tr>
 <tr>
  <td>[x]</td>
  <td>Enum Value [x]</td>
 </tr>
 <tr>
  <td>&lt;x&gt;</td>
  <td>Object References/Pointers &lt;x&gt;</td>
 </tr>
</table>

This step is usually done either in the constructor of the class or in a class initialization function. Next, the class will want to fire the event so that the script callback functions are actually called. This code is placed wherever it makes sense (depending on what the actual event is) and looks like so:

```c++
fireScriptEvent<void>("notify", 14);
```

The template argument corresponds to the return type of the callback function, the first parameter is the name of the event to fire, and the remaining arguments are the parameters to the actual callback function: in this case, the integer that is passed to the notify callback. To see a full example of a class that derives from `gameplay::ScriptTarget` within gameplay, take a look at the `gameplay::Control` class.

## Tips using Lua with gameplay

<ul>
<li>To do integer like comparisons or casts on a number variable x in Lua, use math.floor(x).</li>
<li>Make sure all your member function calls use ':' instead of '.'</li>
<li>Remember to access all gameplay variables, including static and global variables with '()' on the end of the name.</li>
<li>Primitive data type arrays and object arrays are both inefficient when created in Lua and passed to C++, so try to minimize this.</li>
<li>There is no reasonable way to unload a Lua script; thus, the recommended usage pattern is to put each script's variables and functions inside a table (see Lua technical note 7 at <a href="http://www.lua.org/notes/ltn007.html">http://www.lua.org/notes/ltn007.html</a>). i.e.

```lua
-- If you want to load the module at most once, add a line like this.
if Module then return end
-- Declare the module Module.
Module = {}

-- Declare a variable within the module.
Module.a = 47

-- Declare a function within the module.
function Module.myFunc()
    return Module.a + 17
end

-- Cleanup function; call when done with this module.
function cleanupModule()
    Module = nil
end
```

<b>Note:</b> you can't pass an enumeration value to a function that doesn't explicitly take an enumeration type (i.e. Control::setTextColor, which takes an unsigned char). In these cases, you need to look up the enumeration values and pass them directly.</li>
<li>On any function that returns a pointer that is owned by the user calling that function (i.e. a create() function), add @script{create} as the last line of its Doxygen comments.</li>
<li>On any function, variable, class, struct, enum, etc. that should not be accessible from Lua (except for things that are static to a .cpp file, which are already ignored), add @script{ignore} to its Doxygen comments.</li>
<li>On any functions, variables, classes, structs, enums, etc. that are local to a .cpp file, declare them as static 1) because it is good practice and 2) so that Lua does not generate bindings for them.</li>
</ul>

## Generating user defined script bindings

The following instructions detail how to generate Lua script bindings for your own project. Note: this requires doxygen to be installed.

1. Copy the gameplay-luagen.doxyfile file to your project's root directory (and rename it). Then, either manually using a text editor or using the Doxywizard tool, go to the INPUT section and ensure both that the path to gameplay's 'src' folder is valid (relative to where the doxyfile is) and that your own source folder is added.
2. Run doxygen using the above doxyfile from your project's root directory. For example, run `doxygen my-project.doxyfile` from the command line or run Doxygen using the Doxywizard application.
3. Create a 'lua' folder inside your source folder.
4. Run gameplay-luagen using the following command (make sure you have a trailing '/' for the output directory (second) parameter):
```
path-to-gameplay/bin/your-platform/gameplay-luagen.exe ./xml path-to-your-source/lua/ <your-project-name-here>
```
Note: the parameter <your-project-name-here> is used as the namespace that the bindings are generated within. This can be anything you want except for "gameplay".
5. Ensure that your project has `path-to-gameplay/gameplay/src/lua` in its include path.
6. Add the generated Lua script bindings (.h/.cpp) files from path-to-your-source/lua to your project.
7. Compile and run - now you can use your own classes from Lua scripts.