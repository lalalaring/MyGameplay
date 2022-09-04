- <a href="#Legend">Legend</a>
- <a href="#File_Types">File Types</a>
- <a href="#Game_Config">Game Config</a>
- <a href="#Scene">Scene</a>
  - <a href="#Node">Node</a>
- <a href="#Materials">Materials</a>
- <a href="#Physics">Physics</a>
  - <a href="#Collision_Objects">Collision Objects</a>
  - <a href="#Constraints">Constraints</a>
- <a href="#Animation">Animation</a>
- <a href="#Audio">Audio</a>
- <a href="#Particles">Particles</a>
- <a href="#UI_Forms">UI Forms</a>
  - <a href="#Form">Form</a>
  - <a href="#Theme">Theme</a>

This is a work in progress. Please refer to the [HTMLFile Formats file](http://gameplay3d.github.io/GamePlay/docs/file-formats.html).
  

## <a name="Legend"></a>Legend

<table cellpadding="0">
 <tr>
  <th align="left"><b>Legend</b></th>
  <th>&nbsp;</th>
 </tr>
 <tr>
  <td>[0..1]</td>
  <td>Zero or one. Denotes an optional property.</td>
 </tr>
 <tr>
  <td>[0..*]</td>
  <td>Zero or many. The property may appear many times.</td>
 </tr>
 <tr>
  <td>[1..*]</td>
  <td>One or many. Required property and may appear many times.</td>
 </tr>
 <tr>
  <td>required</td>
  <td>Required property.</td>
 </tr>
 <tr>
  <td><span class=Non-literal><span style='font-family:"Courier New"'>Italics</span></span></td>
  <td>Non-literal</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'>// Comment</span></span></td>
  <td>Just like in C++</td>
 </tr>
 <tr>
  <td><span class=Non-literalCode>A | B | C</span></td>
  <td>A or B or C. (Shows the valid enumeration values)</td>
 </tr>
</table>



<table>
 <tr>
  <th align="left"><b>Types</b></th>
  <th>&nbsp;</th>
 </tr>
 <tr>
  <td>block</td>
  <td>A block property that contains other properties.</td>
 </tr>
 <tr>
  <td>string</td>
  <td>A string of characters with no quotations.</td>
 </tr>
 <tr>
  <td>bool</td>
  <td><span class=Non-literal><span style='font-family:"Courier New"'>true</span></span>
  or <span class=Non-literal><span style='font-family:"Courier New"'>false</span></span></td>
 </tr>
 <tr>
  <td>float</td>
  <td>Floating point number.<br>
  Decimal is optional, don’t include &quot;f&quot;.</td>
 </tr>
 <tr>
  <td>int</td>
  <td>Integer</td>
 </tr>
 <tr>
  <td>uint</td>
  <td>Unsigned integer</td>
 </tr>
 <tr>
  <td>axis angle</td>
  <td>Rotation axis angle (4 floats): <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, angle_in_degrees</span></span></td>
 </tr>
 <tr>
  <td>color</td>
  <td>Hex color in the format #RRGGBBAA. (<span class=Non-literal><span
  style='font-family:"Courier New"'>#FF6A00FF</span></span>)</td>
 </tr>
 <tr>
  <td>xref</td>
  <td>External reference.<br />
  This can be
  <ul>
  <li>a file path (<span class=Non-literal>res/common/game.scene</span>)</li>
  <li>a reference to a named object in a property file (<span
  class=Non-literal>res/common/game.scene#duck</span>)
  </li></ul>
  </td>
 </tr>
 <tr>
  <td>lua xref</td>
  <td>External reference to a lua function.</td>
 </tr>
 <tr>
  <td>file path</td>
  <td>A path to a file. This should be a relative path for the
  purpose of being cross platform. </td>
 </tr>
 <tr>
  <td>image path</td>
  <td>A path to an image.</p>
  <ul>
  <li>A file path (<span class=Non-literal>res/wood.png</span>)</li>
  <li> An alias for an image that will be defined in game config file for the purpose of using different compressed textures on different platforms. (<span class=Non-literal>@wood)</span>.</li>
  </ul>
  </td>
 </tr>
</table>

**Note:**

- The order of the properties does not matter.
- Blocks do not require an <span class=Non-literalCode>id</span>.

## <a name="File_Types"></a>File Types

<table>
 <tr>
  <th align="left" colspan="2"><b>Gameplay property files</b></th>
  <th><b>blocks</b></th>
 </tr>
 <tr>
  <td>.config</td>
  <td>Game config definitions.</td>
  <td><a href="#window">window</a>, <a href="#graphics">graphics</a>,
  <a href="#scripts">scripts</a>, <a href="#aliases">aliases</a>, <a
  href="#gamepads">gamepads</a></td>
 </tr>
 <tr>
  <td>.scene</td>
  <td>Scene definition</td>
  <td><a href="#scene">scene</a>, <a href="#node">node</a>, <a
  href="#physics">physics</a></td>
 </tr>
 <tr>
  <td>.material</td>
  <td>Material definitions</td>
  <td><a href="#material">material</a>, <a href="#technique">technique</a>,
  <a href="#pass">pass</a></td>
 </tr>
 <tr>
  <td>.physics</td>
  <td>Collision objects and physics constraints</td>
  <td><a href="#physics">physics</a>, <a href="#CollisionObjects">collisionObject</a>,
  <a href="#Constraints">constraint</a></td>
 </tr>
 <tr>
  <td>.animation</td>
  <td>Animation and animation clip definitions</td>
  <td><a href="#animation">animation</a>, <a href="#clip">clip</a></td>
 </tr>
 <tr>
  <td>.audio</td>
  <td>Audio source definitions</td>
  <td><a href="#Audio">audio</a></td>
 </tr>
 <tr>
  <td>.particle</td>
  <td>Particle emitter definitions</td>
  <td><a href="#Particles">particle</a></td>
 </tr>
 <tr>
  <td>.form</td>
  <td>UI form definitions</td>
  <td><a href="#Form">form</a> </td>
 </tr>
 <tr>
  <td>.theme</td>
  <td>UI theme definitions</td>
  <td><a href="#Theme">theme</a></td>
 </tr>
</table>

<table>
 <tr>
  <th align="left" colspan="3"><b>Gameplay files</b></th>
 </tr>
 <tr>
  <td>.gpb</td>
  <td>Gameplay Bundle</td>
  <td>Binary encoded scene that is created by the gameplay-encoder
  from a .dae or .fbx file.</td>
 </tr>
 <tr>
  <td>.vert</td>
  <td>Vertex shader</td>
  <td>GLSL vertex shader source.</td>
 </tr>
 <tr>
  <td>.frag</td>
  <td>Fragment shader</td>
  <td>GLSL fragment shader source.</td>
 </tr>
</table>



<table>
 <tr>
  <th align="left" colspan="2"><b>Other file extensions</b></th>
 </tr>
 <tr>
  <td>.dae</td>
  <td>COLLADA</td>
 </tr>
 <tr>
  <td>.fbx</td>
  <td>Autodesk </td>
 </tr>
 <tr>
  <td>.mb</td>
  <td>Maya Binary</td>
 </tr>
 <tr>
  <td>.ma</td>
  <td>Maya ASCII</td>
 </tr>
 <tr>
  <td>.ttf</td>
  <td>TrueType Font</td>
 </tr>
 <tr>
  <td>.lua</td>
  <td>Lua source code</td>
 </tr>
 <tr>
  <td>.dds, .dds, .pvr</td>
  <td>Compressed texture</td>
 </tr>
</table>



## <a name="Game_Config"></a>Game Config

<table>
 <tr>
  <td><a name=window>window</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  width = <span class=Non-literal><span style='font-family:
  "Courier New"'>pixels</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>pixels</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  x = <span class=Non-literal><span style='font-family:"Courier New"'>x_offset_from_top_left</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  y = <span class=Non-literal><span style='font-family:"Courier New"'>y_offset_from_top_left</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  fullscreen = <span class=Non-literal><span style='font-family:
  "Courier New"'>bool</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  title =<span class=Non-literal><span style='font-family:"Courier New"'>
  Hello World</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=graphics>graphics</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  samples = <span class=Non-literal><span style='font-family:
  "Courier New"'>multisampling_state</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  defaultMaterial = <span class=Non-literal><span
  style='font-family:"Courier New"'>none | res/foo.material#bar</span></span></td>
  <td>[0..1]</td>
  <td>none | file</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

Note: By default defaultMaterial uses a pink material.



<table>
 <tr>
  <td><a name=scripts>scripts</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  initialize = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#initialize</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  update = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#update</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  render = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#render</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  finalize = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#finalize</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  keyEvent = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#keyEvent</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  touchEvent = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#touchEvent</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  mouseEvent = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.lua#mouseEvent</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>  gamepadEvent = <span class=Non-literal><span
  style='font-family:"Courier New"'>res/game.lua#gamepadEvent</span></span></td>
  <td>[0..1]</td>
  <td>lua xref</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=aliases>aliases</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Non-literal><span style='font-family:"Courier New"'>alias_name
  = file_path</span></span></td>
  <td>[0..*]</td>
  <td>file path</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=gamepads>gamepads</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  form = <span class=Non-literal><span style='font-family:"Courier New"'>res/common/gamepad.form</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

## <a name="Scene"></a>Scene

```c++
// Load a scene from a .scene property file
Scene* scene = Scene::load("res/common/game.scene");
```

<table>
 <tr>
  <td><a name=scene>scene</a></td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  path = <span class=Non-literal><span style='font-family:"Courier New"'>res/game.scene</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>  activeCamera = <span class=Non-literal><span
  style='font-family:"Courier New"'>node_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  node <span class=Non-literal><span style='font-family:"Courier New"'>node_id</span></span>
  {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  physics {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

### <a name="Node"></a>Node

<table>
 <tr>
  <td><a name=node>node</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>node_id </span></span>: <span
  class=Non-literal><span style='font-family:"Courier New"'>parent_node_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  url = <span class=Non-literal><span style='font-family:"Courier New"'>res/common/stuff.gpb#duck</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>  material = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/scene.material#wood</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>  collisionObject = <span class=Non-literal><span
  style='font-family:"Courier New"'>res/obj.physics#box</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>  audio = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/game.audio#quack</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  translate = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  rotate = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  scale = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  tags</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  {</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>    <span class=Non-literal><span style='font-family:"Courier New"'>tag_name1</span></span></td>
  <td>[0..*]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>    <span class=Non-literal><span style='font-family:"Courier New"'>tag_name2</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  }</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



## <a name="Materials"></a>Materials

**Note:**

<ul>
 <li class=MsoNormal><span class=Non-literalCode>id</span> is optional for
     material, technique and pass.</li>
 <li class=MsoNormal>Materials can inherit values from another material by
     optionally setting a <span class=Non-literalCode>parent_material_id</span>.</li>
 <li class=MsoNormal>Vertex and fragment shader file extensions do not matter.
     The convention in gameplay is to use <span style='font-family:"Times New Roman","serif"'>“</span>.vert<span
     style='font-family:"Times New Roman","serif"'>”</span> and <span
     style='font-family:"Times New Roman","serif"'>“</span>.frag<span
     style='font-family:"Times New Roman","serif"'>”</span>.</li>
 <li class=MsoNormal><a name=scalar><span class=Non-literalCode>scalar</span> </a>is
     float, int or bool.</li>
 <li class=MsoNormal><a name=vector><span class=Non-literalCode>vector</span></a><span
     class=Non-literalCode> </span>is a comma separated list of floats.</li>
</ul>

```c++
// When the .material file contains one material
Material* material = model->setMaterial("res/common/box.material");
// When the .material file contains multiple materials
Material* m = model->setMaterial("res/common/stuff.material#wood");
```

<table>
 <tr>
  <td><a name=material>material</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>material_id</span></span> : <span
  class=Non-literal><span style='font-family:"Courier New"'>parent_material_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Non-literal><span style='font-family:"Courier New"'> 
  uniform_name </span></span>=<span class=Non-literal><span style='font-family:
  "Courier New"'> </span></span><a href="#scalar">scalar</a><span
  class=Non-literal><span style='font-family:"Courier New"'> | </span></span><a
  href="#vector">vector</a></td>
  <td>[0..*]</td>
  <td>uniform</td>
 </tr>
 <tr>
  <td>  <span class=Non-literal><span style='font-family:"Courier New"'>uniform_name
  </span></span>= <a href="#AUTO_BIND_ENUM">AUTO_BIND_ENUM</a></td>
  <td>[0..*]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  sampler <span class=Non-literal><span style='font-family:
  "Courier New"'>uniform_name </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  renderState {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  technique <span class=Non-literal><span style='font-family:
  "Courier New"'>id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=technique>technique</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>technique_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Non-literal><span style='font-family:"Courier New"'> 
  uniform_name </span></span>=<span class=Non-literal><span style='font-family:
  "Courier New"'> </span></span><a href="#scalar">scalar</a><span
  class=Non-literal><span style='font-family:"Courier New"'> | </span></span><a
  href="#vector">vector</a></td>
  <td>[0..*]</td>
  <td>uniform</td>
 </tr>
 <tr>
  <td>  <span class=Non-literal><span style='font-family:"Courier New"'>uniform_name
  </span></span>= <a href="#AUTO_BIND_ENUM">AUTO_BIND_ENUM</a></td>
  <td>[0..*]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  sampler <span class=Non-literal><span style='font-family:
  "Courier New"'>uniform_name </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  renderState {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  pass <span class=Non-literal><span style='font-family:"Courier New"'>id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=pass>pass </a><span class=Non-literal><span
  style='font-family:"Courier New"'>pass_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  vertexShader = <span class=Non-literal><span
  style='font-family:"Courier New"'>res/colored.vert</span></span></td>
  <td>[0..1]</td>
  <td>file path</td>
 </tr>
 <tr>
  <td>  fragmentShader = <span class=Non-literal><span
  style='font-family:"Courier New"'>res/colored.frag</span></span></td>
  <td>[0..1]</td>
  <td>file path</td>
 </tr>
 <tr>
  <td>  defines = <span class=Non-literal><span style='font-family:
  "Courier New"'>semicolon separated list</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td><span class=Non-literal><span style='font-family:"Courier New"'>&nbsp;</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Non-literal><span style='font-family:"Courier New"'> 
  uniform_name </span></span>=<span class=Non-literal><span style='font-family:
  "Courier New"'> </span></span><a href="#scalar">scalar</a><span
  class=Non-literal><span style='font-family:"Courier New"'> | </span></span><a
  href="#vector">vector</a></td>
  <td>[0..*]</td>
  <td>uniform</td>
 </tr>
 <tr>
  <td>  <span class=Non-literal><span style='font-family:"Courier New"'>uniform_name
  </span></span>= <a href="#AUTO_BIND_ENUM">AUTO_BIND_ENUM</a></td>
  <td>[0..*]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  sampler <span class=Non-literal><span style='font-family:
  "Courier New"'>uniform_name </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  renderState {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=sampler>sampler</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>uniform_name</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  path = <span class=Non-literal><span style='font-family:"Courier New"'>res/wood.png
  | @wood</span></span></td>
  <td>[0..1]</td>
  <td>image path</td>
 </tr>
 <tr>
  <td>  mipmap = <span class=Non-literal><span style='font-family:
  "Courier New"'>bool</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  wrapS = <span class=Non-literal><span style='font-family:
  "Courier New"'>REPEAT | CLAMP</span></span></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  wrapT = <span class=Non-literal><span style='font-family:
  "Courier New"'>REPEAT | CLAMP</span></span></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td> minFilter = <a href="#TEXTURE_MIN_FILTER_ENUM">TEXTURE_MIN_FILTER_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  magFilter = <a href="#TEXTURE_MAG_FILTER_ENUM">TEXTURE_MAG_FILTER_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=renderState>renderState</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  blend = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  blendSrc = <a href="#BLEND_ENUM">BLEND_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  blendDst = <a href="#BLEND_ENUM">BLEND_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  cullFace = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  depthTest = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  depthWrite = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Material Enums

<table>
 <tr>
  <th align="left"><a name="AUTO_BIND_ENUM"></a><b>AUTO_BIND_ENUM</b></th>
  <th>&nbsp;</th>
 </tr>
 <tr>
  <td>WORLD_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>VIEW_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>PROJECTION_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>WORLD_VIEW_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>VIEW_PROJECTION_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>WORLD_VIEW_PROJECTION_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>INVERSE_TRANSPOSE_WORLD_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>CAMERA_WORLD_POSITION</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>CAMERA_VIEW_POSITION</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>MATRIX_PALETTE</td>
  <td>Used for vertex skinning</td>
 </tr>
</table>



<table>
 <tr>
  <th align="left"><a name="TEXTURE_MIN_FILTER_ENUM"></a><b>TEXTURE_MIN_FILTER_ENUM</b></th>
  <th>&nbsp;</th>
 </tr>
 <tr>
  <td>NEAREST</td>
  <td>Lowest quality non-mipmapped</td>
 </tr>
 <tr>
  <td>LINEAR</td>
  <td>Better quality non-mipmapped</td>
 </tr>
 <tr>
  <td>NEAREST_MIPMAP_NEAREST</td>
  <td>Fast but low quality mipmapping</td>
 </tr>
 <tr>
  <td>LINEAR_MIPMAP_NEAREST</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>NEAREST_MIPMAP_LINEAR</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>LINEAR_MIPMAP_LINEAR</td>
  <td>Best quality mipmapping</td>
 </tr>
</table>

<table>
 <tr>
  <th align="left"><a name="TEXTURE_MAG_FILTER_ENUM"></a><b>TEXTURE_MAG_FILTER_ENUM</b></th>
  <th>&nbsp;</th>
 </tr>
 <tr>
  <td>NEAREST</td>
  <td>Lowest quality</td>
 </tr>
 <tr>
  <td>LINEAR</td>
  <td>Better quality</td>
 </tr>
</table>

<table>
 <tr>
  <th align="left"><a name="BLEND_ENUM"></a><b>BLEND_ENUM</b></th>
  <th>&nbsp;</th>
 </tr>
 <tr>
  <td>ZERO</td>
  <td>ONE_MINUS_DST_ALPHA</td>
 </tr>
 <tr>
  <td>ONE</td>
  <td>CONSTANT_ALPHA</td>
 </tr>
 <tr>
  <td>SRC_ALPHA</td>
  <td>ONE_MINUS_CONSTANT_ALPHA</td>
 </tr>
 <tr>
  <td>ONE_MINUS_SRC_ALPHA</td>
  <td>SRC_ALPHA_SATURATE</td>
 </tr>
 <tr>
  <td>DST_ALPHA</td>
  <td>&nbsp;</td>
 </tr>
</table>

## <a name="Physics"></a>Physics

<table>
 <tr>
  <td><a name=physics>physics</a></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  gravity = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  constraint {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

<p>Default gravity: <span class=Non-literalCode>0.0, -9.8, 0.0</span></p>

### <a name="Collision_Objects"></a>Collision Objects

#### Physics Rigid Body

<table>
 <tr>
  <td>collisionObject <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>RIGID_BODY</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  shape = <span class=Non-literal><span style='font-family:
  "Courier New"'>BOX | SPHERE | MESH | CAPSULE |</span></span><br>
            <span class=Non-literal><span style='font-family:"Courier New"'>HEIGHTFIELD</span></span></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  </td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  mass = <span class=Non-literal><span style='font-family:"Courier New"'>kilograms</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  friction = <span class=Non-literal><span style='font-family:
  "Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  restitution = <span class=Non-literal><span style='font-family:
  "Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearDamping = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularDamping = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  kinematic = <span class=Non-literal><span style='font-family:
  "Courier New"'>bool</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  anisotropicFriction = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  gravity = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // BOX properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  extents = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // SPHERE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  CAPSULE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  HEIGHTFIELD properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  image = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/common/image.png</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>





#### Physics Ghost Object

<table>
 <tr>
  <td>collisionObject <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>GHOST_OBJECT</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  shape = <span class=Non-literal><span style='font-family:
  "Courier New"'>BOX | SPHERE | MESH | CAPSULE |</span></span><br>
            <span class=Non-literal><span style='font-family:"Courier New"'>HEIGHTFIELD</span></span></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  </td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // BOX properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  extents = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // SPHERE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  CAPSULE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  HEIGHTFIELD properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  image = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/common/image.png</span></span></td>
  <td>[0..1]</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Physics Character

<table>
 <tr>
  <td>collisionObject <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>CHARACTER</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  shape = <span class=Non-literal><span style='font-family:
  "Courier New"'>BOX | SPHERE | MESH | CAPSULE</span></span></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  mass = <span class=Non-literal><span style='font-family:"Courier New"'>kilograms</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  </td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // BOX properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  extents = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // SPHERE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  CAPSULE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



#### Physics Vehicle

<table>
 <tr>
  <td>collisionObject <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>VEHICLE</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  shape = <span class=Non-literal><span style='font-family:
  "Courier New"'>BOX | SPHERE | MESH | CAPSULE</span></span></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  </td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  mass = <span class=Non-literal><span style='font-family:"Courier New"'>kilograms</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  friction = <span class=Non-literal><span style='font-family:
  "Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  restitution = <span class=Non-literal><span style='font-family:
  "Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearDamping = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularDamping = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  kinematic = <span class=Non-literal><span style='font-family:
  "Courier New"'>bool</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  anisotropicFriction = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  gravity = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // BOX properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  extents = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // SPHERE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>radius</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  CAPSULE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // VEHICLE only</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  steeringGain = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  brakingForce = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  drivingForce = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  steerdownSpeed = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  steerdownGain = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  brakedownStart = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  brakedownFull = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  drivedownStart = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  drivedownFull = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  boostSpeed = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  boostGain = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  downforce = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Physics Vehicle Wheel

<table>
 <tr>
  <td>collisionObject <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>VEHICLE_WHEEL</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  shape = <span class=Non-literal><span style='font-family:
  "Courier New"'>BOX | SPHERE | MESH | CAPSULE</span></span></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  </td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  mass = <span class=Non-literal><span style='font-family:"Courier New"'>kilograms</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  friction = <span class=Non-literal><span style='font-family:
  "Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  restitution = <span class=Non-literal><span style='font-family:
  "Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearDamping = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularDamping = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  kinematic = <span class=Non-literal><span style='font-family:
  "Courier New"'>bool</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  anisotropicFriction = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  gravity = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // BOX properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  extents = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // SPHERE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  <span class=Comment><span style='font-family:"Courier New"'>//
  CAPSULE properties</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  radius = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  center = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  centerAbsolute = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><span class=Comment><span style='font-family:"Courier New"'> 
  // VEHICLE_WHEEL only</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  steerable = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  wheelDirection = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  wheelAxle = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  strutConnectionOffset =<span class=Non-literal><span
  style='font-family:"Courier New"'> x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  strutRestLength = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  strutStiffness = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  strutDampingCompression = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  strutDampingRelaxation = <span class=Non-literal><span
  style='font-family:"Courier New"'>coefficient</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  frictionBreakout = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  wheelRadius = <span class=Non-literal><span style='font-family:
  "Courier New"'>radius</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  rollInfluence = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

### <a name="Constraints"></a>Constraints

#### Fixed Constraint

<table>
 <tr>
  <td>constraint <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>FIXED</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  breakingImpulse = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Generic Constraint

<table>
 <tr>
  <td>constraint <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>GENERIC</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  translationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  translationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  rotationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  rotationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  angularLowerLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  angularUpperLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  linearLowerLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  linearUpperLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  breakingImpulse = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Hinge Constraint

<table>
 <tr>
  <td>constraint <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>HINGE</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  translationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  translationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  rotationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  rotationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  limits = <span class=Non-literal><span style='font-family:
  "Courier New"'>lowerLimit, upperLimit,<br>
             bounciness</span></span></td>
  <td>[0..1]</td>
  <td>2 or 3 floats</td>
 </tr>
 <tr>
  <td>  breakingImpulse = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Socket Constraint

<table>
 <tr>
  <td>constraint <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>SOCKET</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  translationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  translationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  breakingImpulse = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Sprint Constraint

<table>
 <tr>
  <td>constraint <span class=Non-literal><span style='font-family:
  "Courier New"'>id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  type = <b>SPRING</b></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  translationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  translationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  rotationOffsetA = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  rotationOffsetB = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z, degrees</span></span></td>
  <td>[0..1]</td>
  <td>axis angle</td>
 </tr>
 <tr>
  <td>  angularLowerLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  angularUpperLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  linearLowerLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  linearUpperLimit = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  angularDampingX = <span class=Non-literal><span
  style='font-family:"Courier New"'>damping</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularDampingY = <span class=Non-literal><span
  style='font-family:"Courier New"'>damping</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularDampingZ = <span class=Non-literal><span
  style='font-family:"Courier New"'>damping</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularStrengthX = <span class=Non-literal><span
  style='font-family:"Courier New"'>strength</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularStrengthY = <span class=Non-literal><span
  style='font-family:"Courier New"'>strength</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  angularStrengthZ = <span class=Non-literal><span
  style='font-family:"Courier New"'>strength</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearDampingX = <span class=Non-literal><span
  style='font-family:"Courier New"'>damping</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearDampingY = <span class=Non-literal><span
  style='font-family:"Courier New"'>damping</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearDampingZ = <span class=Non-literal><span
  style='font-family:"Courier New"'>damping</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearStrengthX = <span class=Non-literal><span
  style='font-family:"Courier New"'>strength</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearStrengthY = <span class=Non-literal><span
  style='font-family:"Courier New"'>strength</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  linearStrengthZ = <span class=Non-literal><span
  style='font-family:"Courier New"'>strength</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  breakingImpulse = <span class=Non-literal><span
  style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



## <a name="Animation"></a>Animation

<table>
 <tr>
  <td><a name=animation>animation </a><span class=Non-literal><span
  style='font-family:"Courier New"'>animation_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  property = <a href="#ANIMATION_PROPERTY">ANIMATION_PROPERTY</a></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  keyCount = <span class=Non-literal><span style='font-family:
  "Courier New"'>number_of_key_frames</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  keyTimes = <span class=Non-literal><span style='font-family:
  "Courier New"'>uint uint uint uint ...</span></span></td>
  <td>[0..1]</td>
  <td>uint(s)</td>
 </tr>
 <tr>
  <td>  keyValues = <span class=Non-literal><span style='font-family:
  "Courier New"'>float float float float ...</span></span></td>
  <td>[0..1]</td>
  <td>float(s)</td>
 </tr>
 <tr>
  <td>  curve = <a href="#INTERPOLATION_TYPE">INTERPOLATION_TYPE</a></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

```c++
// Load the animation clips for sample03-character
Animation* animation = node->getAnimation("animations");
animation->createClips("res/common/boy.animation");
```

<table>
 <tr>
  <td>animation <span class=Non-literal><span style='font-family:
  "Courier New"'>animation_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  frameCount = <span class=Non-literal><span style='font-family:
  "Courier New"'>frame_count</span></span></td>
  <td>required</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  clip <span class=Non-literal><span style='font-family:"Courier New"'>clip_id</span></span>
  {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Animation Clip

<table>
 <tr>
  <td><a name=clip>clip</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>clip_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  begin = <span class=Non-literal><span style='font-family:
  "Courier New"'>frame_index</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  end = <span class=Non-literal><span style='font-family:"Courier New"'>frame_index</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  repeatCount = <span class=Non-literal><span style='font-family:
  "Courier New"'>float | REPEAT_INDEFINITE</span></span></td>
  <td>[0..1]</td>
  <td>float | enum</td>
 </tr>
 <tr>
  <td>  speed = <span class=Non-literal><span style='font-family:
  "Courier New"'>1.0</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Animation Enums

<table>
 <tr>
  <td><a name="ANIMATION_PROPERTY"><b>ANIMATION_PROPERTY</b></a></td>
 </tr>
 <tr>
  <td>ANIMATE_SCALE</td>
  <td>ANIMATE_TRANSLATE</td>
 </tr>
 <tr>
  <td>ANIMATE_SCALE_X</td>
  <td>ANIMATE_TRANSLATE_X</td>
 </tr>
 <tr>
  <td>ANIMATE_SCALE_Y</td>
  <td>ANIMATE_TRANSLATE_Y</td>
 </tr>
 <tr>
  <td>ANIMATE_SCALE_Z</td>
  <td>ANIMATE_TRANSLATE_Z</td>
 </tr>
 <tr>
  <td>ANIMATE_ROTATE</td>
  <td>ANIMATE_ROTATE_TRANSLATE</td>
 </tr>
 <tr>
  <td>ANIMATE_UNIFORM</td>
  <td>ANIMATE_SCALE_ROTATE_TRANSLATE</td>
 </tr>
</table>

<table>
 <tr>
  <td><a name="INTERPOLATION_TYPE"><b>INTERPOLATION_TYPE</b></a></td>
 </tr>
 <tr>
  <td>BEZIER</td>
  <td>QUARTIC_IN</td>
  <td>CIRCULAR_IN</td>
 </tr>
 <tr>
  <td>BSPLINE</td>
  <td>QUARTIC_OUT</td>
  <td>CIRCULAR_OUT</td>
 </tr>
 <tr>
  <td>FLAT</td>
  <td>QUARTIC_IN_OUT</td>
  <td>CIRCULAR_IN_OUT</td>
 </tr>
 <tr>
  <td>HERMITE</td>
  <td>QUARTIC_OUT_IN</td>
  <td>CIRCULAR_OUT_IN</td>
 </tr>
 <tr>
  <td>LINEAR</td>
  <td>QUINTIC_IN</td>
  <td>ELASTIC_IN</td>
 </tr>
 <tr>
  <td>SMOOTH</td>
  <td>QUINTIC_OUT</td>
  <td>ELASTIC_OUT</td>
 </tr>
 <tr>
  <td>STEP</td>
  <td>QUINTIC_IN_OUT</td>
  <td>ELASTIC_IN_OUT</td>
 </tr>
 <tr>
  <td>QUADRATIC_IN</td>
  <td>QUINTIC_OUT_IN</td>
  <td>ELASTIC_OUT_IN</td>
 </tr>
 <tr>
  <td>QUADRATIC_OUT</td>
  <td>SINE_IN</td>
  <td>OVERSHOOT_IN</td>
 </tr>
 <tr>
  <td>QUADRATIC_IN_OUT</td>
  <td>SINE_OUT</td>
  <td>OVERSHOOT_OUT</td>
 </tr>
 <tr>
  <td>QUADRATIC_OUT_IN</td>
  <td>SINE_IN_OUT</td>
  <td>OVERSHOOT_IN_OUT</td>
 </tr>
 <tr>
  <td>CUBIC_IN</td>
  <td>SINE_OUT_IN</td>
  <td>OVERSHOOT_OUT_IN</td>
 </tr>
 <tr>
  <td>CUBIC_OUT</td>
  <td>EXPONENTIAL_IN</td>
  <td>BOUNCE_IN</td>
 </tr>
 <tr>
  <td>CUBIC_IN_OUT</td>
  <td>EXPONENTIAL_OUT</td>
  <td>BOUNCE_OUT</td>
 </tr>
 <tr>
  <td>CUBIC_OUT_IN</td>
  <td>EXPONENTIAL_IN_OUT</td>
  <td>BOUNCE_IN_OUT</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>EXPONENTIAL_OUT_IN</td>
  <td>BOUNCE_OUT_IN</td>
 </tr>
</table>

## <a name="Audio"></a>Audio

```c++
// Load a sound from file
AudioSource* source = AudioSource::create("res/game.audio#explode");
```

#### Audio Source

<table>
 <tr>
  <td>audio <span class=Non-literal><span style='font-family:"Courier New"'>audio_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  path = <span class=Non-literal><span style='font-family:"Courier New"'>res/common/engine_loop.ogg</span></span></td>
  <td>[0..1]</td>
  <td>file path</td>
 </tr>
 <tr>
  <td>  looped = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  gain = <span class=Non-literal><span style='font-family:"Courier New"'>volume_amplification
  (1.0)</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  pitch = <span class=Non-literal><span style='font-family:
  "Courier New"'>pitch_value [0.5-2.0]</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  velocity = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

## <a name=Particles></a>Particles

```c++
// Load a particle emitter from a property file
ParticleEmitter* p = ParticleEmitter::create("res/fire.particle");
```

<table>
 <tr>
  <td>particle <span class=Non-literal><span style='font-family:"Courier New"'>particle_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  sprite</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  {</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>    path = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/common/smoke.png</span></span></td>
  <td>[0..1]</td>
  <td>file path</td>
 </tr>
 <tr>
  <td>    width = <span class=Non-literal><span style='font-family:
  "Courier New"'>int</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    height = <span class=Non-literal><span style='font-family:
  "Courier New"'>int</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    frameCount = <span class=Non-literal><span
  style='font-family:"Courier New"'>count</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    frameDuration = <span class=Non-literal><span
  style='font-family:"Courier New"'>seconds</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>    frameRandomOffset = <span class=Non-literal><span
  style='font-family:"Courier New"'>offset</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    looped = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>    animated = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>    blending = <span class=Non-literal><span style='font-family:
  "Courier New"'>OPAQUE   | TRANSPARENT |</span></span> <br>
                 <span class=Non-literal><span style='font-family:"Courier New"'>ADDITIVE
  | MULTIPLIED</span></span></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  }</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  particleCountMax = <span class=Non-literal><span
  style='font-family:"Courier New"'>100</span></span></td>
  <td>[0..1]</td>
  <td>uint</td>
 </tr>
 <tr>
  <td>  emissionRate = <span class=Non-literal><span
  style='font-family:"Courier New"'>particles_per_second</span></span></td>
  <td>[0..1]</td>
  <td>uint</td>
 </tr>
 <tr>
  <td>  orbitPosition = <span class=Non-literal><span
  style='font-family:"Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  orbitVelocity = <span class=Non-literal><span
  style='font-family:"Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  orbitAcceleration = <span class=Non-literal><span
  style='font-family:"Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  ellipsoid = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  sizeStartMin = <span class=Non-literal><span
  style='font-family:"Courier New"'>1.0</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  sizeStartMax = <span class=Non-literal><span
  style='font-family:"Courier New"'>1.0</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  sizeEndMin = <span class=Non-literal><span style='font-family:
  "Courier New"'>1.0</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  sizeEndMax = <span class=Non-literal><span style='font-family:
  "Courier New"'>1.0</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  energyMin = <span class=Non-literal><span style='font-family:
  "Courier New"'>milliseconds</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  energyMax = <span class=Non-literal><span style='font-family:
  "Courier New"'>milliseconds</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  rotationPerParticleSpeedMin = <span class=Non-literal><span
  style='font-family:"Courier New"'>radians<br>
                                  per second</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  rotationPerParticleSpeedMax = <span class=Non-literal><span
  style='font-family:"Courier New"'>radians<br>
                                  per second</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  </td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  colorStart = <span class=Non-literal><span style='font-family:
  "Courier New"'>red, blue, green, alpha</span></span></td>
  <td>[0..1]</td>
  <td>4 floats</td>
 </tr>
 <tr>
  <td>  colorEnd = <span class=Non-literal><span style='font-family:
  "Courier New"'>red, blue, green, alpha</span></span></td>
  <td>[0..1]</td>
  <td>4 floats</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  positionVar = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  velocity = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  velocityVar = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  acceleration = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>  accelerationVar = <span class=Non-literal><span
  style='font-family:"Courier New"'>x, y, z</span></span></td>
  <td>[0..1]</td>
  <td>3 floats</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

## <a name="UI_Forms"></a>UI Forms

```c++
// Load a form from a property file
Form* form = Form::create("res/editor.form");
```

### <a name="Form"></a>Form

<table>
 <tr>
  <td>form <span class=Non-literal><span style='font-family:"Courier New"'>form_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  theme = <span class=Non-literal><span style='font-family:
  "Courier New"'>res/editor.theme</span></span></td>
  <td>required</td>
  <td>xref</td>
 </tr>
 <tr>
  <td>  layout = <a href="#LAYOUT_ENUM">LAYOUT_ENUM</a></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  style = <span class=Non-literal><span style='font-family:
  "Courier New"'>style_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  alignment = <a href="#ALIGNMENT_ENUM">ALIGNMENT_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  size = <span class=Non-literal><span style='font-family:"Courier New"'>width,
  height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  autoWidth = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  autoHeight = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  width = <span class=Non-literal><span style='font-family:
  "Courier New"'>width</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>height</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  consumeInputEvents = <span class=Non-literal><span
  style='font-family:"Courier New"'>true</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  container <span class=Non-literal><span style='font-family:
  "Courier New"'>container_id</span></span> {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  label <span class=Non-literal><span style='font-family:"Courier New"'>label_id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  textBox <span class=Non-literal><span style='font-family:
  "Courier New"'>textBox_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  button <span class=Non-literal><span style='font-family:"Courier New"'>button_id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  checkBox <span class=Non-literal><span style='font-family:
  "Courier New"'>checkBox_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  radioButton <span class=Non-literal><span style='font-family:
  "Courier New"'>radioButton_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  slider <span class=Non-literal><span style='font-family:"Courier New"'>slider_id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Container

<table>
 <tr>
  <td>container <span class=Non-literal><span style='font-family:
  "Courier New"'>container_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  layout = <a href="#LAYOUT_ENUM">LAYOUT_ENUM</a></td>
  <td>required</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  style = <span class=Non-literal><span style='font-family:
  "Courier New"'>style_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  alignment = <a href="#ALIGNMENT_ENUM">ALIGNMENT_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  size = <span class=Non-literal><span style='font-family:"Courier New"'>width,
  height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  autoWidth = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  autoHeight = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  width = <span class=Non-literal><span style='font-family:
  "Courier New"'>width</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  height = <span class=Non-literal><span style='font-family:
  "Courier New"'>height</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  consumeInputEvents = <span class=Non-literal><span
  style='font-family:"Courier New"'>true</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  container <span class=Non-literal><span style='font-family:
  "Courier New"'>container_id</span></span> {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  label <span class=Non-literal><span style='font-family:"Courier New"'>label_id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  textBox <span class=Non-literal><span style='font-family:
  "Courier New"'>textBox_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  button <span class=Non-literal><span style='font-family:"Courier New"'>button_id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  checkBox <span class=Non-literal><span style='font-family:
  "Courier New"'>checkBox_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  radioButton <span class=Non-literal><span style='font-family:
  "Courier New"'>radioButton_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  slider <span class=Non-literal><span style='font-family:"Courier New"'>slider_id
  </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Label

<table>
 <tr>
  <td>label <span class=Non-literal><span style='font-family:"Courier New"'>label_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  text = <span class=Non-literal><span style='font-family:"Courier New"'>Hello
  World</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  style = <span class=Non-literal><span style='font-family:
  "Courier New"'>style_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  alignment = <a href="#ALIGNMENT_ENUM">ALIGNMENT_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  size = <span class=Non-literal><span style='font-family:"Courier New"'>width,
  height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  autoWidth = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  autoHeight = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  consumeInputEvents = <span class=Non-literal><span
  style='font-family:"Courier New"'>true</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### TextBox

<table>
 <tr>
  <td>textBox <span class=Non-literal><span style='font-family:"Courier New"'>textBox_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  ... same as label</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Button

<table>
 <tr>
  <td>button <span class=Non-literal><span style='font-family:"Courier New"'>button_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  ... same as label</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### CheckBox

<table>
 <tr>
  <td>checkBox <span class=Non-literal><span style='font-family:"Courier New"'>checkBox_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  checked = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  text = <span class=Non-literal><span style='font-family:"Courier New"'>Hello
  World</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  style = <span class=Non-literal><span style='font-family:
  "Courier New"'>style_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  alignment = <a href="#ALIGNMENT_ENUM">ALIGNMENT_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  size = <span class=Non-literal><span style='font-family:"Courier New"'>width,
  height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  autoWidth = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  autoHeight = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  consumeInputEvents = <span class=Non-literal><span
  style='font-family:"Courier New"'>true</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### RadioButton

<table>
 <tr>
  <td>radioButton <span class=Non-literal><span style='font-family:
  "Courier New"'>radioButton_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  group = <span class=Non-literal><span style='font-family:
  "Courier New"'>group_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  checked = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  text = <span class=Non-literal><span style='font-family:"Courier New"'>Hello
  World</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  style = <span class=Non-literal><span style='font-family:
  "Courier New"'>style_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  alignment = <a href="#ALIGNMENT_ENUM">ALIGNMENT_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  size = <span class=Non-literal><span style='font-family:"Courier New"'>width,
  height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  autoWidth = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  autoHeight = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  imageSize = <span class=Non-literal><span style='font-family:
  "Courier New"'>width, height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  consumeInputEvents = <span class=Non-literal><span
  style='font-family:"Courier New"'>true</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Slider

<table>
 <tr>
  <td>slider <span class=Non-literal><span style='font-family:"Courier New"'>slider_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  style = <span class=Non-literal><span style='font-family:
  "Courier New"'>style_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  position = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  size = <span class=Non-literal><span style='font-family:"Courier New"'>width,
  height</span></span></td>
  <td>[0..1]</td>
  <td>2 floats</td>
 </tr>
 <tr>
  <td>  min = <span class=Non-literal><span style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  max = <span class=Non-literal><span style='font-family:"Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  value = <span class=Non-literal><span style='font-family:
  "Courier New"'>default_value</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  step = <span class=Non-literal><span style='font-family:"Courier New"'>discrete_steps</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>  text = <span class=Non-literal><span style='font-family:"Courier New"'>Hello
  World</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  consumeInputEvents = <span class=Non-literal><span
  style='font-family:"Courier New"'>true</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

#### Form Enums

<table>
 <tr>
  <td><a name="LAYOUT_ENUM"><b>LAYOUT_ENUM</b></a></td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>LAYOUT_FLOW</td>
  <td>Controls are placed next to one another horizontally until the
  right-most edge of the container is reached, at which point a new row is
  started.</td>
 </tr>
 <tr>
  <td>LAYOUT_VERTICAL</td>
  <td>Controls are placed next to one another vertically until the
  bottom-most edge of the container is reached.</td>
 </tr>
 <tr>
  <td>LAYOUT_ABSOLUTE</td>
  <td>Controls are not modified at all by this layout. They must be
  positioned and sized manually.</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name="ALIGNMENT_ENUM"><b>ALIGNMENT_ENUM</b></a></td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>ALIGN_LEFT</td>
  <td>Left</td>
 </tr>
 <tr>
  <td>ALIGN_HCENTER</td>
  <td>Horizontal center</td>
 </tr>
 <tr>
  <td>ALIGN_RIGHT</td>
  <td>Right</td>
 </tr>
 <tr>
  <td>ALIGN_TOP</td>
  <td>Top</td>
 </tr>
 <tr>
  <td>ALIGN_VCENTER</td>
  <td>Vertical center</td>
 </tr>
 <tr>
  <td>ALIGN_BOTTOM</td>
  <td>Bottom</td>
 </tr>
 <tr>
  <td>ALIGN_TOP_LEFT</td>
  <td>ALIGN_TOP | ALIGN_LEFT</td>
 </tr>
 <tr>
  <td>ALIGN_VCENTER_LEFT</td>
  <td>ALIGN_VCENTER | ALIGN_LEFT</td>
 </tr>
 <tr>
  <td>ALIGN_BOTTOM_LEFT</td>
  <td>ALIGN_BOTTOM | ALIGN_LEFT</td>
 </tr>
 <tr>
  <td>ALIGN_TOP_HCENTER</td>
  <td>ALIGN_TOP | ALIGN_HCENTER</td>
 </tr>
 <tr>
  <td>ALIGN_VCENTER_HCENTER</td>
  <td>ALIGN_VCENTER | ALIGN_HCENTER</td>
 </tr>
 <tr>
  <td>ALIGN_BOTTOM_HCENTER</td>
  <td>ALIGN_BOTTOM | ALIGN_HCENTER</td>
 </tr>
 <tr>
  <td>ALIGN_TOP_RIGHT</td>
  <td>ALIGN_TOP | ALIGN_RIGHT</td>
 </tr>
 <tr>
  <td>ALIGN_VCENTER_RIGHT</td>
  <td>ALIGN_VCENTER | ALIGN_RIGHT</td>
 </tr>
 <tr>
  <td>ALIGN_BOTTOM_RIGHT</td>
  <td>ALIGN_BOTTOM | ALIGN_RIGHT</td>
 </tr>
</table>

### <a name=Theme></a>Theme

<table>
 <tr>
  <td>theme <span class=Non-literal><span style='font-family:"Courier New"'>theme_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  texture = <span class=Non-literal><span style='font-family:
  "Courier New"'>image_path</span></span></td>
  <td>required</td>
  <td>image path</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  cursor <span class=Non-literal><span style='font-family:"Courier New"'>cursor_id</span></span>
  {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  imageList <span class=Non-literal><span style='font-family:
  "Courier New"'>imageList_id </span></span>{}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  skin <span class=Non-literal><span style='font-family:"Courier New"'>skin_id</span></span>
  {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  style <span class=Non-literal><span style='font-family:"Courier New"'>style_id</span></span>
  {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=cursor>cursor</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>cursor_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  region = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, width, height</span></span></td>
  <td>[0..1]</td>
  <td>4 floats</td>
 </tr>
 <tr>
  <td>  color = <span class=Non-literal><span style='font-family:
  "Courier New"'>#RRGGBBAA</span></span></td>
  <td>[0..1]</td>
  <td>color</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=imageList>imageList</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>imageList_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  color = <span class=Non-literal><span style='font-family:
  "Courier New"'>#RRGGBBAA</span></span></td>
  <td>[0..1]</td>
  <td>color</td>
 </tr>
 <tr>
  <td>  image <span class=Non-literal><span style='font-family:"Courier New"'>image_id</span></span>
  {}</td>
  <td>[0..*]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=skin>skin</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>skin_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  region = <span class=Non-literal><span style='font-family:
  "Courier New"'>x, y, width, height</span></span></td>
  <td>[0..1]</td>
  <td>4 floats</td>
 </tr>
 <tr>
  <td>  color = <span class=Non-literal><span style='font-family:
  "Courier New"'>#RRGGBBAA</span></span></td>
  <td>[0..1]</td>
  <td>color</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  border</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  {</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>    top = <span class=Non-literal><span style='font-family:
  "Courier New"'>top</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    bottom = <span class=Non-literal><span style='font-family:
  "Courier New"'>bottom</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    left = <span class=Non-literal><span style='font-family:
  "Courier New"'>left</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    right = <span class=Non-literal><span style='font-family:
  "Courier New"'>right</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  }</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td><a name=style>style</a> <span class=Non-literal><span
  style='font-family:"Courier New"'>style_id</span></span></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  margin</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  {</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>    top = <span class=Non-literal><span style='font-family:
  "Courier New"'>top</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    bottom = <span class=Non-literal><span style='font-family:
  "Courier New"'>bottom</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    left = <span class=Non-literal><span style='font-family:
  "Courier New"'>left</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    right = <span class=Non-literal><span style='font-family:
  "Courier New"'>right</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  }</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  padding</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  {</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>    top = <span class=Non-literal><span style='font-family:
  "Courier New"'>top</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    bottom = <span class=Non-literal><span style='font-family:
  "Courier New"'>bottom</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    left = <span class=Non-literal><span style='font-family:
  "Courier New"'>left</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>    right = <span class=Non-literal><span style='font-family:
  "Courier New"'>right</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  }</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  stateNormal {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  stateFocus {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  stateActive {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>  stateDisabled {}</td>
  <td>[0..1]</td>
  <td>block</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

<table>
 <tr>
  <td>stateNormal</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  skin = <span class=Non-literal><span style='font-family:"Courier New"'>skin_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  imageList = <span class=Non-literal><span style='font-family:
  "Courier New"'>imageList_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  cursor = <span class=Non-literal><span style='font-family:
  "Courier New"'>cursor_id</span></span></td>
  <td>[0..1]</td>
  <td>string</td>
 </tr>
 <tr>
  <td>  font = <span class=Non-literal><span style='font-family:"Courier New"'>res/common/arial40.gpb</span></span></td>
  <td>[0..1]</td>
  <td>file path</td>
 </tr>
 <tr>
  <td>  fontSize = <span class=Non-literal><span style='font-family:
  "Courier New"'>fontSize</span></span></td>
  <td>[0..1]</td>
  <td>int</td>
 </tr>
 <tr>
  <td>  textColor = <span class=Non-literal><span style='font-family:
  "Courier New"'>#RRGGBBAA</span></span></td>
  <td>[0..1]</td>
  <td>color</td>
 </tr>
 <tr>
  <td>  textAlignment = <a href="#ALIGNMENT_ENUM">ALIGNMENT_ENUM</a></td>
  <td>[0..1]</td>
  <td>enum</td>
 </tr>
 <tr>
  <td>  rightToLeft = <span class=Non-literal><span style='font-family:
  "Courier New"'>false</span></span></td>
  <td>[0..1]</td>
  <td>bool</td>
 </tr>
 <tr>
  <td>  opacity = <span class=Non-literal><span style='font-family:
  "Courier New"'>float</span></span></td>
  <td>[0..1]</td>
  <td>float</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td>stateFocus</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  ... same as stateNormal</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td>stateActive</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  ... same as stateNormal</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>



<table>
 <tr>
  <td>stateDisabled</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>{</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>  ... same as stateNormal</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>}</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>

