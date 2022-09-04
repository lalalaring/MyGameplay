Game assets are extremely important for the quality of a good game. Not only do the game assets need to be fitted for the game design, but they also need to load as quickly as possible and at the highest quality within the platform hardware limitations.

## Binary game assets

A very practical way to ensure that you're being efficient is to always bundle and load all your game assets as binary formats. Common assets include images, fonts, audio, and 3-D scenes. Most game engines will always include some sort of authoring tool to allow developers to encode and process their content to be game-ready. The gameplay framework also includes an executable tool for this called the gameplay-encoder.

## Using fonts and scenes

For fonts and scenes, you will want to support industry-standard file formats, such as [TrueType](http://en.wikipedia.org/wiki/TrueType) for fonts and popular 3D formats such as [FBX](http://usa.autodesk.com/fbx/) formats. 

Although these formats are popular and have the widest support in tooling options, they are not considered efficient runtime formats. The gameplay library requires that you convert these formats to its documented [gameplay bundle format](https://github.com/gameplay3d/GamePlay/blob/master/tools/encoder/gameplay-bundle.txt) (.gpb) using the [gameplay-encoder](https://github.com/gameplay3d/GamePlay/tree/master/tools/encoder) executable.

## Pre-built gameplay-encoder tool

The gameplay-encoder executable tool comes pre-built for Windows, MacOS X and Linux. They can be found in the `<gameplay-root>/bin folders`. The general usage is:

```
Usage: gameplay-encoder [options] <file(s)>
```

## Building gameplay-encoder

Even though the gameplay-encoder tool comes pre-built, you make want to customize it and built it again yourself. To build the gameplay-encoder project, open the gameplay-encoder project in Visual Studio or XCode and build the executable. 

## Content pipeline

The content pipeline for fonts and scenes works like this:

1. Take any TrueType fonts or FBX scene files.            
2. Run the gameplay-encoder executable passing in the font or scene file path and optional parameters to produce a gameplay binary version for the file (.gpb).          
3. Bundle your game and include the gameplay binary file as a binary game asset.
4. Load any binary game assets using the `gameplay::Bundle` class.                    

## Using binary bundles

Use the `gameplay::Bundle` class from your C++ game source code to load your encoded binary files as bundles. The class offers methods to load both fonts and scenes. Scenes are loaded as a hierarchical structure of nodes, with various entities attached to them. These entities include things like mesh geometry or groups of meshes, and cameras and lights. The `gameplay::Bundle` class also has methods to filter only the parts of a scene that you want to load.

## Release mode assets

When releasing your game title, all of the images should be optimized and converted to the compressed texture format that is supported by OpenGL and OpenGL ES. Audio should be encoded to compressed OGG format to save space on storage to.