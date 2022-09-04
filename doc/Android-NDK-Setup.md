Android NDK development works on Windows, MacOS and Linux development environments.

## Install External Dependencies
Before you get started you will have to download the dependencies separately:
* [Installing External Dependencies](Installing-External-Dependencies)

## Install Tools

### Install the Android SDK
* [Download and install the Android SDK](http://developer.android.com/sdk/index.html).
* Make sure `<android-sdk-path>/tools` and `<android-sdk-path>/platform-tools` are added to [PATH](http://en.wikipedia.org/wiki/PATH_%28variable%29).

### Install the Android NDK
* [Download and install the Android NDK](http://developer.android.com/tools/sdk/ndk/index.html).
* Make sure `<android-ndk-path>` is added to [PATH](http://en.wikipedia.org/wiki/PATH_%28variable%29).

### Install Apache Ant
* [Download and install Apache Ant](http://ant.apache.org/manual/install.html).
* Make sure `<ant-path>/bin` is added the [PATH](http://en.wikipedia.org/wiki/PATH_%28variable%29).

### Install Android SDK Package (API 16)
* [Run the SDK Manager](http://developer.android.com/tools/help/sdk-manager.html) to install SDK Platform for Android 4.1.2 (API 16)

## Configure Build Target
* Run the following command to list the available build targets and note the target id for 'android-16'
```
android list targets
```
* This will produce a list like this:
```
Available Android targets:
----------
id: 1 or "android-16"
     Name: Android 4.1.2
     API level: 16
     Revision: 1
----------
id: 2 or "android-21"
     Name: Android 5.0
     API level: 21
     Revision: 1
...
```

* Run the following command to generate the property files for ant for the selected target id: 1 (android-16):
```
android update project -t 1 -p . -s
```

## Build Library and Samples

* Change to the android directory for the sample you want to build: 
```
cd samples/browser/android
```
* Run the following command: 
```
ant debug
```

## Package and Install

Make sure developer mode is enabled and USB debugging is turned on.

* Change directory to the sample you want to build, package and install:
``` 
cd samples/browser/android
```
* Install the app: 
```
ant debug install
```

## Troubleshooting

### Device not detected
If `ant debug install` does not work, ensure your device is being detected properly by running `adb devices`. If no devices are reported, try downloading the correct USB drivers from you phone manufacturer's website. ([OEM Drivers](http://developer.android.com/tools/extras/oem-usb.html#Drivers))

<a name="Debugging"></a>
### Debugging
Use `logcat` to capture the log file from the device.

```
adb logcat -d > logcat.txt
```

If your program crashed then the log file will contain the stack trace without symbols. Use `ndk-stack` to make the stack trace readable.

`-sym` points to the directory that contains the symbols while `-dump` points to the log file.

```html
ndk-stack -sym C:\Git\gaming\GamePlay\samples\browser\android\obj\local\armeabi -dump d:\logcat.txt
```