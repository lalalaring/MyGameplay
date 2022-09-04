You can integrate 3-D audio into your game using the audio services supported by gameplay. The framework uses a very traditional way of representing audio. The `gameplay::AudioController` manages all of the playing audio sources.

## Creating an AudioSource

An AudioSource can be created from audio files or from a `.audio` property file. You can either use`.ogg` audio files that are compressed and that use less memory. You can also use larger uncompressed PCM `.wav` files.

```c++
AudioSource* wheelsSound = AudioSource::create("res/longboard.wav");
AudioSource* backgroundMusic = AudioSource::create("res/music.ogg");
```

## Playing the AudioSource

To play an audio source:
```c++
wheelsSound->play();
```

## Updating the AudioListener

By default, the `AudioListener` is bound to the active camera of the scene. You can manually bind the camera to the `gameplay::AudioListener` using `gameplay::AudioListener::setCamera()`.

## Audio Properties

The `gameplay::AudioSource` class has methods for modifying the properties of the AudioSource, such as pitch, gain, and velocity.

Audio sources can be loaded from `.audio` property files to make it easier to set these properties.
```
audio fireball
{
    path = res/audio/fireball.wav
    looped = false
    gain = 0.7
    pitch = 0.5
    velocity = 0.5 0.0 1.0
}
```

To load an AudioSource from a property file in C++.

```c++
AudioSource* source = AudioSource::create("res/game.audio#fireball");
```

## Binding an AudioSource to a node

An `AudioSource` can be bound to a `Node` in your scene using `Node::setAudioSource()`. The position of the audio source is automatically updated when the node is transformed.