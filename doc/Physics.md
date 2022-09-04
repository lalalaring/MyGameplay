The gameplay framework supports 3-D physics using the game service/controller `gameplay::PhysicsController`. The `gameplay::PhysicsController` class maintains a physics world that has gravity, and will simulate the objects you add to it.

The gameplay physics system supports 3-D rigid body dynamics, including collision shapes, constraints, and a physics character class. 
To simulate objects within the physics world, you need to create a `gameplay::PhysicsCollisionObject` object representing the geometry, or `gameplay::Model`. 
By attaching a collision object to a `gameplay::Node`, the rigid body will be added to the physics world and the simulation will automatically update the node's transformation.


## PhysicsCollisionObject

`PhysicsCollisionObject` is the base class that provides an interface for receiving collision events. 

You can add collision listeners to a `PhysicsCollisionObject` or test if the collision object currently collides with another collision object.

There are 3 types of collision objects:        
- PhysicsRigidBody
- PhysicsGhostObject
- PhysicsCharacter
- PhyiscsVehicle

## PhysicsRigidBody

A rigid body is an idealized, infinitely hard, non-deformable solid object. Rigid bodies have mass, shape and other properties that affect forces within the simulation.

A PhysicsRigidBody can be set to be a kinematic rigid body. A kinematic rigid body is an object that is not simulated by the physics system, and instead has its transform driven manually.

A note regarding the **mass** of a rigid body: In GamePlay, a mass value of zero causes the object to be immovable. This value is reserved by the underlying physics engine for making objects static, (albeit admittedly counter-intuitive).

### Create a PhysicsRigidBody

To create a rigid body, first you need to know what kind of shape you want to simulate. The physics system supports boxes, spheres, meshes, capsules, and terrain height fields. For basic shapes, such as boxes and spheres, you can programmatically create the rigid bodies by calling `Node::setCollisionObject()` and passing in the desired shape type.

 
```c++
PhysicsRigidBody::Parameters params;
params.mass = 10.0f;
node->setCollisionObject(PhysicsCollisionObject::RIGID_BODY,
                         PhysicsCollisionShape::box(), &params);
```

All other types of rigid bodies must be created using the .scene and .physics property definition files. The .scene file allows you to bind various attachments or properties to nodes, including a rigid body.

For example, to create a mesh rigid body for the node within the scene with ID equal to tree_1:

**game.scene:**

```
scene
{
    ...
    node tree_1
    {
        ...
        collisionObject = game.physics#tree_mesh
    }
    ...
}
```

**game.physics:**

```
collisionObject tree_mesh
{
    type = RIGID_BODY
    shape = MESH
    mass = 15.0
    ...
}
```

### RigidBody schema

All properties have default values if not defined. See `PhysicsRigidBody::Parameters` for more information.

```
collisionObject <string>
{
    type                = <RIGID_BODY | GHOST_OBJECT | CHARACTER>
    shape               = <BOX | SPHERE | MESH | HEIGHTFIELD | CAPSULE>
    image               = <string> // only for HEIGHTFIELD
    radius              = <float>    
    height              = <float>
    extents             = <float, float, float>
    center              = <float, float, float>
    centerAbsolute      = <float, float, float>
 
    mass                = <float>
    friction            = <float>
    restitution         = <float>
    linearDamping       = <float>
    angularDamping      = <float>
    kinematic           = <bool>   
    anisotropicFriction = <float, float, float>
    gravity             = <float, float, float>
}
```

<table>
<tr>
  <th align=left>Shapes</th>
  <th align=left>Properties</th>
</tr>
<tr>
  <td>BOX</td>
  <td>extents, center, center-absolute</td>
</tr>
<tr>
  <td>SPHERE</td>
  <td>radius, center, center-absolute</td>
</tr>
<tr>
  <td>MESH</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>HEIGHTFIELD</td>
  <td>image</td>
</tr>
<tr>
  <td>CAPSULE</td>
  <td>radius, height, center, center-absolute</td>
</tr>
</table>

## PhysicsGhostObject

A ghost object is like a rigid body except that it does not have an effect the simulation. It will not cause forces or react to the other rigid bodies. Ghost objects have a shape but they do not have mass, or any of the properties that affect forces.

Ghost objects are useful for querying the simulation, or detecting collisions without having rigid bodies react to the ghost object. A ghost object could be used to detect if an object entered a volume, such as a soccer ball going into a goal. This use of a ghost object is often called a volumetric trigger. Ghost objects can also detect if they collide with other ghost objects.

Collision objects do not require a model so you could use a ghost object to check if a camera collides with a wall.

### Creating a PhysicsGhostObject

Programmatically:

```c++
// Create a ghost object with radius 5
node->setCollisionObject(PhysicsCollisionObject::GHOST_OBJECT,
                         PhysicsCollisionShape::sphere(5.0f));
```

In a .physics file:

```
collisionObject powerup
{
    type = GHOST_OBJECT
    shape = SPHERE
    radius = 5.0
}
```

### PhysicsGhostObject schema

Ghost objects only have a shape and support the same shapes as rigid bodies. 

```
collisionObject <string>
{
    type                = GHOST_OBJECT
    shape               = <BOX | SPHERE | MESH | HEIGHTFIELD | CAPSULE>
    radius              = <float>
    height              = <float>
    extents             = <float, float, float>
    center              = <float, float, float>
    centerAbsolute      = <float, float, float>
    image               = <string>  // HEIGHTFIELD shapes only.
}
```

## PhysicsCharacter

The `PhysicsCharacter` class can be used to control the movements and collisions of a character in a game. 
It interacts with the physics system to apply gravity and handle collisions, however dynamics are not applied to the character directly by the physics system. 
Instead, the character's movement is controlled directly by the `PhysicsCharacter` class. 
This results in a more responsive and typical game character than would be possible if trying to move a character by applying physical simulation with forces.

### Creating a PhysicsCharacter

To programmatically create a `PhysicsCharacter` with mass 20 and capsule shape:

```c++
PhysicsRigidBody::Parameters params(20.0f);
node->setCollisionObject(PhysicsCollisionObject::CHARACTER,
                         PhysicsCollisionShape::capsule(1.2f, 5.0f, Vector3(0, 2.5, 0), true), &params);
PhysicsCharacter* character = static_cast<PhysicsCharacter*>(
                                                     node->getCollisionObject());
```

### PhysicsCharacter schema

Physics characters have a mass and shape. A capsule is a typical shape for a typical biped character.

```
collisionObject <string>
{
    type                = CHARACTER
    shape               = <BOX | SPHERE | MESH | CAPSULE>
    radius              = <float>
    height              = <float>
    extents             = <float, float, float>
    center              = <float, float, float>
    centerAbsolute      = <float, float, float>
    mass                = <float>
}
```

## Creating a PhysicsContraint

The gameplay framework supports various types of constraints between two rigid bodies (or one rigid body and the physics world), including hinge, fixed, generic (six-degree-of-freedom), socket, and spring. 
Constraints can be created programmatically using one of the create functions on `gameplay::PhysicsController`, or they can be specified within the physics section of the .scene file. 
For example, to create a hinge constraint from within a .scene file between the rigid body attached to the node with id door and the physics world:

**game.scene:**

```
scene
{
    ...
    physics
    {
        ...
        constraint
         {
            type = HINGE
            rigidBodyA = door
            rotationOffsetA = 0.0, 1.0, 0.0, 90.0
            translationOffsetA = 0.0, 0.0, 2.0
            limits = 0.0, 90.0, 0.5
       }
    }
}
```

**PhysicsConstraint schema**

```
constraint <string>
{
    type                = <FIXED | GENERIC | HINGE |  SOCKET | SPRING>
    rigidBodyA         = <string>
    rigidBodyB         = <string>
    translationOffsetA = <float, float, float>
    translationOffsetB = <float, float, float>
    rotationOffsetA    = <float>
    rotationOffsetB    = <float>
    angularLowerLimit  = <float, float, float>
    angularUpperLimit  = <float, float, float>
    linearLowerLimit   = <float, float, float>
    linearUpperLimit   = <float, float, float>
    limits             = <float, float, float>
    angularDampingX    = <float>
    angularDampingY    = <float>
    angularDampingZ    = <float>
    angularStrengthX   = <float>
    angularStrengthY   = <float>
    angularStrengthZ   = <float>
    linearDampingX     = <float>
    linearDampingY     = <float>
    linearDampingZ     = <float>
    linearStrengthX    = <float>
    linearStrengthY    = <float>
    linearStrengthZ    = <float>
    breakingImpulse    = <float>
}
```

<table>
<tr>
  <th align=left>Constraint&nbsp;Types</th>
  <th align=left>Properties</th>
</tr>
<tr>
  <td>FIXED</td>
  <td>&nbsp;</td>
</tr>
<tr>
  <td>GENERIC</td>
  <td>translationOffsetA, translationOffsetB, rotationOffsetA, rotationOffsetB, angularLowerLimit, angularUpperLimit, linearLowerLimit, linearUpperLimit</td>
</tr>
<tr>
  <td>HINGE</td>
  <td>translationOffsetA, translationOffsetB, rotationOffsetA, rotationOffsetB, limits</td>
</tr>
<tr>
  <td>SOCKET</td>
  <td>translationOffsetA, translationOffsetB</td>
</tr>
<tr>
  <td>SPRING</td>
  <td>translationOffsetA, translationOffsetB, rotationOffsetA, rotationOffsetB, angularLowerLimit, angularUpperLimit, linearLowerLimit, linearUpperLimit, angularDampingX, angularDampingY, angularDampingZ, angularStrengthX, angularStrengthY, angularStrengthZ, linearDampingX, linearDampingY, linearDampingZ, linearStrengthX, linearStrengthY, linearStrengthZ </td>
</tr>
</table>

## Handling collisions

The gameplay framework allows you to register to be notified whenever a collision occurs between two rigid bodies (and also when two rigid bodies stop colliding). 
In order to do this, you must define a class that derives from `gameplay::PhysicsRigidBody::Listener` and implements the function `collisionEvent()`. 
Then, you must add an instance of the class as a listener on a given rigid body using the `PhysicsRigidBody::addCollisionListener` function. 
For example, to print all information for all collisions with the door and for collisions between the character and the wall:

**MyGame.h**

```c++
class MyGame: public gameplay::PhysicsRigidBody::Listener
{
public:
    /// ...
    /**
     * Collision event handler.
     */
    void collisionEvent(PhysicsRigidBody::Listener::EventType type,
                        const PhysicsRigidBody::CollisionPair& pair, 
    const Vector3& pointA, const Vector3& pointB);
    // ...
};
```

**MyGame.cpp:**

```c++
MyGame* mygame;
Node* door;
Node* character;
Node* wall;
// ...
door->getRigidBody()->addCollisionListener(mygame);
character->getRigidBody()->addCollisionListener(mygame, wall);
// ...
void MyGame::collisionEvent(PhysicsRigidBody::Listener::EventType type, 
                            const PhysicsRigidBody::CollisionPair& pair, 
                            const Vector3& pointA, const Vector3& pointB)
{
    GP_WARN("Collision between rigid bodies %s (at point (%f, %f, %f)) "
            "and %s (at point (%f, %f, %f)).",
            pair._rbA-getNode()-getId(), pointA.x, pointA.y, pointA.z, 
            pair._rbB-getNode()-getId(), pointB.x, pointB.y, pointB.z);
}
```

## PhysicsVehicle

The PhysicsVehicle and PhysicsVehicleWheel classes give you access to vehicle physics for racing games. 
You designate a VEHICLE type collision object for the node in your scene representing the vehicle body or chassis, and VEHICLE_WHEEL for the nodes representing the wheels. 
When gameplay loads the scene, it attempts to automatically bind the wheels to the associated vehicle chassis. 
In the hierarchy of your scene be sure to locate the wheel nodes and the vehicle chassis under a common group node. 
This is how gameplay determines which vehicle body the wheels belong to – by searching for a common ancestor in the hierarchy. 
The nodes do not need to be direct descendents of the group node; they just need to appear somewhere below it in the hierarchy. 
The presence of other mesh nodes under the common node has no effect. All that matters is that the node of collision type VEHICLE shares a common ancestor with the nodes of collision type VEHICLE_WHEEL (details below):

![Car model in Maya](https://raw.github.com/wiki/gameplay3d/GamePlay/img/maya_car.jpg)

Then in your .scene file, designate collision objects for the car and its wheels, like this:

```
scene main
{
    path = res/common/game.gpb
    activeCamera = camera1
    node carbody
    {
        url = car_top
        material = res/common/game.material#car
        collisionObject = res/common/game.physics#car
    }
    node wheelFrontLeft
    {
        url = Left_top
        material = res/common/game.material#car
        collisionObject = res/common/game.physics#carWheelFrontLeft
    }
    node wheelFrontRight
    {
        url = Right_top
        material = res/common/game.material#car
        collisionObject = res/common/game.physics#carWheelFrontRight
    }
    node wheelBackLeft
    {
        url = Left_bottom
        material = res/common/game.material#car
        collisionObject = res/common/game.physics#carWheelBackLeft
    }
    node wheelBackRight
    {
        url = Right_bottom
        material = res/common/game.material#car
        collisionObject = res/common/game.physics#carWheelBackRight
    }
//...
    physics
    {
        gravity = 0.0, -9.8, 0.0
    }
}
```

In the .physics file be sure to specify type `VEHICLE` for the chassis and type `VEHICLE_WHEEL` for the wheels:

```
collisionObject car
{
    type = VEHICLE
 
    shape = BOX
    mass = 800.0
    friction = 0.5
    restitution = 0.01
    linearDamping = 0.025
    angularDamping = 0.6
 
    steeringGain = 0.4
    brakingForce = 350.0
    drivingForce = 2000.0
 
    steerdownSpeed = 87
    steerdownGain = 0.22
    brakedownStart = 100
    brakedownFull = 170
    drivedownStart = 105
    drivedownFull = 180
    boostSpeed = 74
    boostGain = 2.6
    downforce = 4.5
}
 
collisionObject carWheel
{
    type = VEHICLE_WHEEL
 
    shape = MESH
    mass = 1.0
    friction = 0.5
    restitution = 0.01
    linearDamping = 0.025
    angularDamping = 0.16
 
    wheelDirection = 0, -1, 0
    wheelAxle = -1, 0, 0
 
    strutRestLength = 0.6
    strutStiffness = 25.0
    strutDampingCompression = 5.1
    strutDampingRelaxation = 2.3
    frictionBreakout = 1000.0
    wheelRadius = 0.5
    rollInfluence = 0.1
    strutConnectionOffset = 0.0, 0.0, 1.4
}
 
collisionObject carWheelFrontLeft : carWheel
{
    steerable = true
}
 
collisionObject carWheelFrontRight : carWheel
{
    steerable = true
}
 
collisionObject carWheelBackLeft : carWheel
{
    steerable = false
}
 
collisionObject carWheelBackRight : carWheel
{
    steerable = false
}
```

Practically speaking the only collision shape that makes sense right now for the vehicle chassis is BOX because currently there is a known issue with MESH collision shapes.

In the initialize() method of your Game class, you can set a member variable for accessing the vehicle via PhysicsVehicle:

```c++
Node* carNode = _scene->findNode("carbody");
if (carNode && carNode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE)
{
    _carVehicle = static_cast<PhysicsVehicle*>(carNode->getCollisionObject());
}
```

Then, in the `update()` method of your Game class you need to call `PhysicsVehcicle::update()` with the various control inputs described below:

```c++
_carVehicle->update(elapsedTime, steering, braking, driving);
```

The `steering` parameter controls vehicle steering and has an expected range of -1 to +1. 
The `braking` parameter applies wheel brakes and has an expected range of 0 to 1. 
The `driving` parameter lumps together engine output and overall drivetrain, with an expected range of 0 to 1. 
Optional properties in the .physics definition for your vehicle give you greater control of the handling characteristics, and are described below in turn.

### Overall Vehicle Controls

The following properties specify the vehicle's overall response to control inputs:

```
// Vehicle steering, braking, and powertrain
steeringGain   = <float>    // steering at full deflection
brakingForce   = <float>    // braking force at full braking
drivingForce   = <float>    // driving force at full throttle
```

This is an over-simplification of vehicle handling, and therefore gameplay provides further refinement as follows.

### Steering Reduction at High Speed

Turning the steering wheel of a real car by 1 degree has a much different effect at 100 km/h than it does at 10 km/h. (Please do not attempt this). 
In a real vehicle, the "feel" of a steering wheel tends to stiffen as speed increases. In a racing game, we can approximate this effect by reducing the amount of authority at higher speeds. 
gameplay provides the following properties to control this effect:

```
// Steering gain reduction with speed (optional)
steerdownSpeed = <float>    // steering gain fades to this point
steerdownGain  = <float>    // gain value at that point (less than 1)
```

The gain at zero speed is always 1. The properties steerdownSpeed and steerdownGain specify a point of reduced gain, above which the gain remains constant. A steerdownGain of 1 effectively disables this feature.

### Brake Reduction at High Speed

Due to imperfections in the simulated physics, full braking at high speeds can cause unexpected behavior. gameplay provides the following properties to reduce braking above a certain threshold speed:

```
// Brake force reduction at high speeds (optional)
brakedownStart = <float>    // braking fades above this speed
brakedownFull  = <float>    // braking is fully faded at this speed
```

Braking remains unaffected up to the speed specified by brakedownStart. 
Above that speed, braking fades and reaches zero at the speed specified by brakedownFull. An unreachably-large speed value for brakedownStart will effectively disable this feature.

### Vehicle Acceleration, All-out Speed, and Aerodynamic Downforce

Driving force is currently simplified down to a single value that lumps together the engine and drivetrain. In the absence of a proper gearbox simulation gameplay provides the following properties for affecting all-out speed and bottom-end acceleration:

```
// Driving force reduction at high speeds (optional)
drivedownStart = <float>   // driving force fades above this speed
drivedownFull  = <float>   // driving force is fully faded at this speed
 
// Driving force boost at low speeds (optional)
boostSpeed     = <float>    // Boost fades to 1 at this point
boostGain      = <float>    // Boost at zero speed (greater than 1)
```

The first 2 properties allow you to reduce driving force at high speeds which limits the top speed of the vehicle. Above the speed specified by drivedownStart driving force begins to fade, and eventually reaches zero at the speed specified by drivedownFull, more or less. An unreachably-large value for drivedownStart will effectively disable this feature.

The last 2 properties allow you to increase acceleration at low speeds. boostGain specifies the gain at zero speed, so a value greater than 1 will increase vehicle acceleration from a standing start. This supplemental gain then fades to 1 at the speed specified by boostSpeed. A boostGain of 1 effectively disables this feature.

Racing cars typically make use of airfoils to produce a downward force at high speeds. This improves handling and performance. gameplay provides the following property to simulate this effect:

```
// Aerodynamic downforce effect (optional)
downforce      = <float>    // proportional control of downforce
```

The value of downforce controls the amount of downward force at a given speed. In particular, the value of this property represents the product of a reference area and an aerodynamic coefficient. However what's important is that this property acts as a constant of proportionality in computing the downward force as a function of speed. A value of 0 effectively disables this feature.

### PhysicsVehicleWheel

Tire and suspension characteristics can be specified at each individual wheel as follows:

```
collisionObject <wheelID>
{
    type                     = VEHICLE_WHEEL
 
    steerable                = <bool>      // indicates whether wheel is steerable
    wheelDirection           = <float, float, float> // direction of strut extension, in chassis space
    wheelAxle                = <float, float, float> // direction of axle (spin axis), in chassis space
    strutConnectionOffset    = <float, float, float> // offset from default strut connection point
    strutRestLength          = <float>     // strut rest length
    strutTravelMax           = <float>     // maximum strut travel
    strutStiffness           = <float>     // strut stiffness, normalized to chassis mass
    strutDampingCompression  = <float>     // strut damping under compression, normalized to chassis mass
    strutDampingRelaxation   = <float>     // strut damping under relaxation, normalized to chassis mass
    strutForceMax            = <float>     // maximum strut force
    frictionBreakout         = <float>     // breakout friction
    wheelRadius              = <float>     // wheel radius
    rollInfluence            = <float>     // how side friction affects chassis roll, normalized
}
```

gameplay automatically determines a default location on the chassis for the strut connection point based on the position of the wheel nodes relative to the car body. The strutConnectionOffset property allows you to specify an offset from the default. This is useful, for example, if the origin of the car body is not located at the center of the mesh:

```
strutConnectionOffset = 0.0, 0.0, 1.4
```

