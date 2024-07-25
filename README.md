# Ball-Box
This program simulates a bunch of balls trapped in a box. Collisions between balls are modeled as perfectly elastic collisions, and collisions with the wall reduce the ball's velocity by a constant factor. Video of it in action: https://youtu.be/W4434Vk-xig

## Input
input_handler.cpp takes keyboard/mouse input and writes the appropriate events to the event queue so that other components can react to user input. Here are the controls

|Key|Action|
|---|------|
|Escape|Close the simulation|
|C|Remove all balls|
|G|Toggle Gravity|
|T|Toggle Collisions|
|P|Pause the simulation|
|S|Save|
|L|Load|
|Left Click|Use current tool|
|Right Click|Select next tool|

## Physics
world.cpp is responsible for the physics. Below are the major physics components that are simulated

### Gravity
Gravity applies a constant downward acceleration to each ball independent of its size. The strength of this acceleration can be modified in the WorldSettings object passed to World's constructor.

### Velocity
Each frame, every ball's position is updated based on the time since the last frame and the ball's velocity

### Collisions
When a ball touches a wall horizontal wall, the vertical component of its velocity is negated and scaled by WorldSettings.wall_damping_factor. When a ball touches a vertical wall, the horizontal component of its velocity is negated and scaled by the same factor.
When a ball touches another ball, the velocity of both balls after the collision is calculated based on conservation of momentum & energy. Some special handling is done in World.are_colliding to prevent multiple collisions from being detected since in this 
simulation the change in velocity occurs instantly, but in the real world this is not the case. 

### Explosion/Implosion
The user is able to simulate an explosion which applies a force to each ball whose strength is inversely proportional to the ball's distance from the explosion. The user can also simulate an "implosion" which attracts all balls to a point with a force that is also
inversely proportional to the ball's distance from the explosion.

## Rendering
renderer.cpp is responsible for rendering objects in the simulation. It can only render circles. It does so by drawing RenderSettings.segments_per_object triangles around the origin of the circle. 

## Event Queue
event_queue.cpp implements a simple messaging system that components can use to broadcast that an important event happened. Possible events are defined in event.hpp. Classes that use the messaging system can subscribe to specific events,
and they can also publish events without having to subscribe. 

## Inventory
inventory.cpp implements a simple inventory system. The user can cycle through tools in their inventory to perform different actions in the simulation. There are 3 different-sized circles that will spawn a new circle into the simulation when used. There is 
also a small magenta circle used to create an explosion, and there's a small black circle used to create an implosion. 

## Sound Engine
sound_engine.cpp synthesizes sound samples for different events that happen in the simulation. When there is a collision, a sound whose frequency is dependent on the size of the balls involved is produced. When the user switches tools in their inventory,
a different sound is produced. There can be up to 16 sounds playing at a time. 

## Game
game.cpp orchestrates all the different components and also handles saving/loading game state to/from a text file. On start, it will try to load the last save file, if any.

## Initialization
BallBox.cpp creates the SDL window and renderer that are needed to produce graphics on the screen. It also contains the main loop of the program.

## Vectors
vec2d.cpp has the 2D vectors used to implement physics and mouse operations. These vectors are implemented as immutable objects, and operations are designed to be easy to chain. 

