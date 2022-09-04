Animation is a key component to bringing your game to life. Within gameplay, there is support to create both property animations and character animations. The `gameplay::Animation` class provides factory methods for creating animations on properties of classes that extend `gameplay::AnimationTarget`. Character animations from within the scene file are imported and stored on the `gameplay::AnimationTarget` they target. All animations on a `gameplay::AnimationTarget` can be obtained by ID.

## AnimationTargets

`gameplay::Transform`, `gameplay::Node`, and `gameplay::MaterialParameter` are animation targets.

Animations can be created on the scale, rotation and translation properties of the `gameplay::Transform`. Animations can also target any `gameplay::Node`, which extends `gameplay::Transform`.

Also, animations can target instances of `gameplay::MaterialParameter`. Any parameters on a material of type float, integer, or 2-, 3-, and 4-dimensional vectors can be animated.


## Creating property animations

Animations are created from the `gameplay::AnimationTarget`. AnimationTarget provides methods to create simple two key frame animations using createAnimationFromTo(), and createAnimationFromBy(). Multiple key frame sequences can be created from createAnimation().

Here is an example of how to create a multiple key frame animation on a node’s translation properties:

```c++
unsigned int keyCount = 3;
unsigned long keyTimes[] = {0L, 500L, 1000L};
float keyValues[] = 
{
    0.0f, -4.0f, 0.0f,    
    0.0f, 0.0f, 0.0f,     
    0.0f, 4.0f, 0.0f   
};   
Animation* sampleAnim = enemyNode->createAnimation("sample", Transform::ANIMATE_TRANSLATE, 
                                                   keyCount, keyTimes, keyValues, Curve::LINEAR);  
```

Here is the same animation specified in a .animation file that can also be loaded by the `gameplay::AnimationTarget`:

```
animation sample
{
    property = ANIMATE_TRANSLATE
    keyCount = 3
    keyTimes = 0, 500, 1000
    keyValues = 0.0 -4.0 0.0 0.0 0.0 0.0 0.0 4.0 0.0
    curve = LINEAR
}
```

To create the animation from this file you would call the following code:

```c++
Animation* sampleAnim = enemyNode->createAnimation("sample", "sample.animation");
```

## Curves

There are many different interpolation types defined within the `gameplay::Curve` class that can be used to interpolate through the animation data.

## Character animations

Character animations are complex because they can be composed of multiple animations targeting numerous joints within a character model. For this reason, character animations are usually included within the scene file and are imported when the .gpb file is loaded. To simplify and optimize all animations under single animation. The gameplay-encoder supports grouping all the animation on joints leading up to a common root joint under a single animation. This is an option in the gameplay-encoder using the -groupAnimations or -g option. This groups them under a single animation called ‘animations’.

 

These animations can be obtained by calling AnimationTarget::getAnimation() specifying the animation’s ID.

## AnimationClips

A `gameplay::AnimationClip` is created from the `gameplay::Animation` class and is a snapshot of the animation that can be played back, manipulated with speed and repeated.

Here is an AnimationClip that has been created from a character animation of an elf:

 
```c++
AnimationClip* elfRun = elfAnimation->createClip("elf_run", 200L, 310L);
elfRun->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
elfRun->setSpeed(2.0f);
```
 

Animation clips can be specified within an `.animation` file that can be given to an animation to create clips. The total number of frames that make up the animation must be specified in the file. The begin and end parameters of each clip are specified in frames. An assumption that the animation runs at 60 frames per second has been made. Here is an example of an .animation file for an elf animation:

```
animation elf
{
    frameCount = 350
    clip idle
    {
        begin =  0
        end = 75
        repeatCount = INDEFINITE
    }
    clip walk
    {
        begin = 75
        end = 200
        repeatCount = INDEFINITE
    }
    clip run
    {
        begin = 200
        end = 310
         repeatCount = INDEFINITE
        speed = 2.0
    }
    clip jump
    {
        begin = 310
        end = 350
        repeatCount = 1
    }
}
```

Animations can be played back by calling `Animation::play()`, passing a clip ID, or by calling `AnimationClip::play()` directly on the clip. Animations can also be paused and stopped in the same fashion.


## Animation blending

The `gameplay::AnimationClip` class has a blend weight property that can be used to blend multiple animations. There is also a method called AnimationClip::crossFade() that conveniently provides the ability to fade the currently playing clip out and fade in the specified clip over a given period of time.


## AnimationClip listeners

Animation events can be triggered on a `gameplay::AnimationClip` by registering instances of `gameplay::AnimationClip::Listener` with the clip. The listeners can be registered to be called back at the beginning or end of the clip, or at any specific time throughout the playback of the clip. This can be useful for starting a particle emitter when a character’s foot hits the ground in an animation, or to play back a sound of a gun firing during an animation of an enemy shooting.
