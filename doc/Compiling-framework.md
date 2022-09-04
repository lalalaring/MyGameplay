To build the gameplay library, tools, and samples as well as write your own games, you will have to install and use the platform specific tools. 

## Install the Cross-platform Dependencies
Before you get started you will have to download the dependencies separately:
* [[Installing External Dependencies]]

## Development Environments

These are the supported development environments for each specific operating system as a target platform.

<table width="90%">
<tr>
  <th align="left" width="33%">Desktop OS</th>
  <th align="left" width="33%">Tool</th>
  <th align="left">Development environment</th>
</tr>
<tr>
  <td>Microsoft Windows</td>
  <td>Visual Studio</td>
  <td>Windows</td>
</tr>
<tr>
  <td>Apple MacOS X</td>
  <td>Xcode</td>
  <td>MacOS X</td>
</tr>
<tr>
  <td>Linux Ubuntu</td>
  <td>CMake</td>
  <td>Linux</td>
</tr>
</table>

<table width="90%">
<tr>
  <th align="left" width="33%">Mobile OS</th>
  <th align="left" width="33%">Tool</th>
  <th align="left">Development environment</th>
</tr>
<tr>
  <td>iOS</td>
  <td>Xcode</td>
  <td>MacOS X</td>
</tr>
<tr>
  <td>Android</td>
  <td>Android NDK</td>
  <td>Linux, Windows, MacOS X</td>
</tr>
</table>

## Setup instructions

Listed below are the basic setup instructions for downloading and installing the supported  platform and development environments for gameplay.
* [[Visual Studio|Visual-Studio-Setup]] (Windows)
* [[Apple Xcode|Apple-Xcode-Setup]] (MacOS X, iOS)
* [[Linux|CMake-Linux-Setup]] (Ubuntu)
* [[Android NDK|Android-NDK-Setup]] (Android)

## Project repository

Included in the project repository are the following notable folder and files: 

<table>
<tr>
  <th align="left">Folder/Files</th>
  <th align="left">Description</th>
</tr>
<tr>
  <td>/api</td>
  <td>Doxygen API Reference</td>
</tr>
<tr>
  <td>/bin</td>
  <td>Precompiled binary tools</td>
</tr>
<tr>
  <td>/external-deps</td>
  <td>External dependency libraries</td>
</tr>
<tr>
  <td>/gameplay</td>
  <td>The gameplay library</td>
</tr>
<tr>
  <td>/samples</td>
  <td>Game samples</td>
</tr>
<tr>
  <td>/template</td>
  <td>Game samples template project files</td>
</tr>
<tr>
  <td>/tools</td>
  <td>content authoring encoder and lua binding generator</td>
</tr>
<tr>
  <td>install.bat/.sh</td>
  <td>Needed to download external-deps and bin files for developing.</td>
</tr>
<tr>
  <td>newproject.bat/.sh</td>
  <td>New cross-platform project script</td>
</tr>
</table>

## Getting started on desktop

The quickest way to get started using the gameplay framework and tools is to simply start working in one of the desktop environments. 

You could use:

- Microsoft Visual Studio Express or Professional on a Windows
- Xcode on Apple MacOS X

Just open either `gameplay.sln` or `gameplay.xcworkspace`.These solutions/workspaces are set up by default to build all the projects needed and to run the samples you have selected as active.

## Getting started on mobile

Now that you are up and running on one of the desktop environments, we recommend you take this seriously and go mobile! In today's mobile game market, cross-platform development is a reality. It is quite simple and easy to use the gameplay framework to target a wider device audience. Start by downloading the native development kit for one of the various supported mobile operating system targets or set them all up.

## Mobile platform considerations

Ensure that you test early on the physical devices. Depending on the type of game you want to write and on your design ideas, you'll want to get some idea of what type of performance you'll get with the game plan and prototypes you are working towards. Be careful and do not to rely on desktop and mobile simulators as an indicator of performance or mobile device capabilities.

## API reference

- [API Reference](http://gameplay3d.github.com/GamePlay/api/index.html)

We firmly believe in making a very intuitive set of APIs that are as straight-forward and as consistent as possible. However, all of the classes in the framework have been well documented with Doxygen formatting. The classes will be updated and improved iteratively throughout each release. This is to help you learn about what classes or sets of functions can be used and the recommended practices in using them.

You should consider reading the latest versions of the API reference from the pre-generated HTML Doxygen API documentation in the gameplay-api folder. This will give you a deeper understanding of the C++ gameplay framework.

## Creating new projects

- [Tutorial: Creating a new project](Creating-a-new-project)