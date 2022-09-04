The `gameplay::ParticleEmitter` class defines all the information needed to simulate and render a system of particles. The emitter can be defined in various ways to represent smoke, steam, fire and other atmospheric effects, such as rain and lightning. Once created, the emitter can be set on a `gameplay::Node` in order to follow an object, or it can be placed within a scene.
## Particles as sprites

A `gameplay::ParticleEmitter` always has a sprite/texture and a maximum number of particles that can be alive at any given time. After the emitter is created, these cannot be changed. Particles are rendered as camera-facing billboards using the emitter's sprite/texture. The emitter's sprite/texture properties determine whether the texture is treated as a single image, a texture atlas, or an animated sprite.

## Particle properties

A `gameplay::ParticleEmitter` also has a number of properties that determine values assigned to the individual particles it emits. Scalar properties, such as particle begin- and end-size, are assigned within a minimum and a maximum value; vector properties are assigned within the domain or space, and are defined by a base vector and a variance vector.

 

The variance vector is multiplied by a random scalar between 1 and -1, and the base vector is added to this result. This allows an emitter to be created, which emits particles with properties that are randomized, yet fit within a well-defined range. To make a property deterministic, simply set the minimum value to the same value as the maximum for that property, or set its variance to a zero vector. To learn more about different scalars, vector and rendering properties that can be set on a `gameplay::ParticleEmitter`, look at the C++ API.

## Creating particle emitters

Use the `ParticlEmitter::create()` method to create an emitter from a particle file. The .particle file format and semantics are very similar to the .material file format. This is because it also leverages the `gameplay::Properties` file definition and supports all the properties supported in the C++ API for the `gameplay::ParticleEmitter` class.

## Animated sprites for particles

It is very easy to make the particles animate through a list of images. Just make your images have a tile of sprite images and then modify the sprite's base properties in the emitter to control the animation behavior.

You can then even do things such as animate images of 3-D dice using only 2-D images.
