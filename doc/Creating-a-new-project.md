To create a new cross-platform game project, run the script `newproject` from the repo root:
- [newproject.bat](https://github.com/gameplay3d/GamePlay/blob/master/newproject.bat) (on Windows) 
- [newproject.sh](https://github.com/gameplay3d/GamePlay/blob/master/newproject.sh) (on Mac or Linux). 

The following is an example of running the `newproject.bat` script:

```
1. Enter a name for the new project.

   This name will be given to the project
   executable and a folder with this name
   will be created to store all project files.
   Ex. foobar

Project Name: test


2. Enter a game title.

   On some platforms, this title is used to
   identify the game during installation and
   on shortcuts/icons.
   Ex. Foo Bar

Title: Test


3. Enter a unique identifier for your project.

   This should be a human readable package name,
   containing at least two words separated by a
   period.
   Ex. com.example.foobar

Unique ID: org.gameplay3d.test


4. Enter your game's main class name.

   Your initial game header and source file
   will be given this name and a class with
   this name will be created in these files.
   Ex. FooBarGame

Class name: TestGame


5. Enter the project path.

   This can be a relative path, absolute path,
   or empty for the current folder. Note that
   a project folder named test will also
   be created inside this folder.
   Ex. ./samples

Path: ./samples

```

## Adding and running the new project

### Windows (Visual Studio)

The simplest way to run the project on Windows is to add the Visual Studio project to the existing `gameplay.sln` solution, set the "gameplay" project as a dependency (right-click on the new project, click "Project Dependencies…", and select the "gameplay" project), and then build and run.

### MacOSX or iOS (Xcode)

To run the project on MacOSX desktop, simply open the Xcode project and build and run for either the macosx or ios configuration.

### Android/BlackBerry (Eclipse)

Similarly, the easiest way to run the project on Android/Blackberry is import the new project into eclipse workspace that already contains the gameplay project, set the "gameplay" project as a dependency (right-click the new project, click "Properties", go to "Project References", and select the "gameplay" project).
Ensure you select the android folder that is inside the new project folder that is created. This will import the android eclipse project instead of the linux version. Then build and run. (Note: make sure that both projects are set to the same Build Configuration.)  For BlackBerry you must also ensure the BlackBerry ADP proxy is running.
