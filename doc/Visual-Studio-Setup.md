## Install Windows (64-bit)
- Windows 7
- Windows 8.1
- Windows 10

## Install External Dependencies

Run the following command-line from the top-level directory of your GamePlay repository:

    install.bat

## Install Visual Studio 2015
- [Install Visual Studio 2015 Community](https://www.visualstudio.com/downloads/download-visual-studio-vs)

## Install DirectX SDK
- [Install Direct SDK] (http://www.microsoft.com/en-ca/download/details.aspx?id=6812)
- Use the default installation path which the project are setup for:

C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
 
## Building
- Open `gameplay.sln`
- `Build > Build Solution` (F7)

## Running a sample
- Right click the sample you want to run
- `Set as StartUp Project`
- `Debug > Start Debugging` (F5)

***

## Troubleshooting

<img align="right" src="https://raw.github.com/wiki/gameplay3d/GamePlay/img/setup/StartUpProject.png" alt="Set as StartUp Project" />

### Unable to start program

`Unable to start program 'C:\Git\gaming\GamePlay\windows\x64\Release\gameplay.lib'`

This error occurs when the gameplay library is set as the StartUp Project.

To fix this, right click on one of the application projects like `sample-browser` and choose `Set as StartUp Project`.

### Cannot open include file: 'btBulletDynamicsCommon.h'

Make sure you install the external dependencies by running `install.bat`.

```bash
base.h(123): fatal error C1083: Cannot open include file: 'btBulletDynamicsCommon.h': No such file or directory
```

### Assertion failure when I run an exe directly but it works in Visual Studio

If you are running the exe from the Debug or Release directory then your game probably can't find your resource files. Each Visual Studio project has a "Working Directory" property (`Project -> Properties -> Debugging`), which is usually set to the base directory of your project `$(ProjectDir)`. An easy solution would be to copy your exe to your base directory. 

For example: Copy `GamePlay\samples\mesh\Debug\sample-mesh.exe` to `GamePlay\samples\mesh\`