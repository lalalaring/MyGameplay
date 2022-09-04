## Install the External Dependencies
Before you get started you will have to download the dependencies separately:
* [Installing External Dependencies](Installing-External-Dependencies)

## Install Qt 5.8 (includes QtCreator)
Download and install the latest Qt 5.8 64-bit for Linux, MacOS or Windows that comes with QtCreator:
* [Qt 5.8 + QtCreator](https://www.qt.io/download-open-source/)
* Install to the default directory or ~/Qt

## Open and Configure (gameplay)

* Open gameplay/gameplay.pro
* Under "Configure Project" tab select "Desktop Qt 5.8.0 xxx 64bit" and expand "Details" section.
* Change the enormous default user based build paths for Debug and Release to:
```
Debug = "Debug"
Release ="Release"
```

## Open and Configure (sample-browser)

* Open samples/browser/sample-browser.pro
* Under "Configure Project" tab select "Desktop Qt 5.8.0 xxx 64bit" and expand "Details" section.
* Change the enormous default user based build paths for Debug and Release to:
```
Debug = "Debug"
Release ="Release"
```
## Build and Run

* Build gameplay and sample-browser projects
* To run the sample-browser, select the sample-browser project
* Select the "Projects" section on the left to edit the project properties.
* Chagne the run settings under the Desktop Qt 5.8.0 xxx 64bit kit by changing to the Run sub-section tab.
* Change the working directory to:
```
%{buildDir}/..
```

## Debugging

* Linux and MacOS should be setup to Debug without any additional software.
* Windows needs to install the additional "Standalone Debugging Tools for Windows (WinDbg)"
https://msdn.microsoft.com/en-us/windows/hardware/hh852365






