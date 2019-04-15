# EZG18 - Haunted Castle
This project was created for the course [Real Time Rendering](https://www.cg.tuwien.ac.at/courses/Realtime/) at the [TU Wien](tuwien.at).
## Story
The user finds herself/himself in a medieval dark room and can barely identify the objects around him/her. Suddenly torches catch fire as if by ghost hand. He/She takes a look around the room and sees different things moving: A chair that is moving forward, chess figure starting moving ([Fool's Mate](https://en.wikipedia.org/wiki/Fool%27s_mate)), torches move to an upright position and paintings falling down. All ends with the wardrobe falling on the user.

## Scene
Medieval room with torches on fire, knight armors, chairs with chess desk, stone walls, paintings and closet.
![overview2](Submission%202/doc/images/overview2.png)

![overview3](Submission%202/doc/images/overview3.png)

## CONTROLS

### ALWAYS AVAILABLE

Key(s) | control
--- | ---
F1 | Help / Show Controls
F2 | Frame Time on/off
+/- | Ambient Light up/down
Page up/Page down/Pos1 | Speed up/down/reset (The speed of the sound is not changed. Therefore it gets out of sync)
C| Debug Mode on/off
ESC | Quit Game

### JUST IN DEBUG MODE

Key(s) | control
--- | ---
F3 | Wire Frame on/off
F4 | Textur-Sampling-Quality: Nearest Neighbor/Bilinear
F5 | Mip Mapping-Quality: Off/Nearest Neighbor/Linear
F6 | Viewfrustum-Culling on/off
F7 | Normal Mapping on/off
F8 | Fire and Shadows 1 on/off
F9 | Fire and Shadows 2 on/off
F10 | Bloom on/off
F11 | Light Shafts on/off
W, ARROW UP | Accelerate
S, ARROW DOWN | Decelerate/Backwards
A, ARROW LEFT, MOUSE DRAG RIGHT | Pan left
D, ARROW RIGHT, MOUSE DRAG LEFT | Pan right
Q | Go down
E | Go up
MOUSE DRAG UP | Camera down
MOUSE DRAG DOWN | Camera up

## DEVELOPMENT STATUS
Implemented:
- [Bling-Phong illumination model](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
- Loading all [models](#Models) with textures
- We perform view frustum culling.
- Debug camera ([Controls](#CONTROLS) see above)
- Automatic Camera Movement: The Camera is moving along a predefined [Cubic Hermite Spline](https://en.wikipedia.org/wiki/Cubic_Hermite_spline)
- All [Effects](#EFFECTS) (See below)

## EFFECTS
- Direction shadows with PCF (from the sun)
  - The Direction shadows can be seen at the floor. They represent the light of the sunn falling in through the Window.
  - https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

![DirectionShadows](Submission%202/doc/images/DirectionShadows.png)

- Bloom (Bright Window)
  - In the scene the Bloom effect is used in the Windows to simulate that it is very bright outside. This effect extracts the brightest parts of the image, blurs them in an extra shader and in a final step adds them to the original image. 
  - http://learnopengl.com/#!Advanced-Lighting/Bloom

![Bloom](Submission%202/doc/images/Bloom.png)

- Fire (Particle Effect)
  - The Fire is realized using Particle Billboards for drawing and Compute Shaders for position updates.
  - Revision Course Slides: Compute Shaders (2018W)
  - Revision Course Slides: Particle Systems (2018W)

![Fire](Submission%202/doc/images/Fire.png)

- Omnidirectional Shadows
  - The Omnidirectional Shadows are the Shadows of Torches. They are “flickering” because the intensity of the fire is irregular.
  - Revision Course Slides: Omnidirectional Shadows (2018W)

![OmnidirectionalShadows](Submission%202/doc/images/OmnidirectionalShadows.png)

- Normal/Bump Mapping (Wall)
  - The Normal Mapping can be seen on the walls. It makes texture look more like 2D
  - Lecture Slides: Shading (2018W)
  - http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/

![BumpMapping](Submission%202/doc/images/BumpMapping.png)

- Volumetric Lighting (Light Rays from Sun)
  - Volumetric Lighting is visible as the light shafts falling in through the windows.
  - Revision Course Slides: Volumetric Lighting (2018W)

![VolumetricLighting](Submission%202/doc/images/VolumetricLighting.png)

## IMPLEMENTATION DETAILS
### Requirements
- Windows
- OpenGL 3.3 Core Profile
- NVIDIA GTX 1060

### Tools
- Visual Studios 2013
- Blender: https://www.blender.org/
- HitFilm Express: https://fxhome.com/express

### Libraries used
- Assimp: http://www.assimp.org/
- FreeImage: http://freeimage.sourceforge.net/
- Glew: http://glew.sourceforge.net/install.html
- PhysX: https://developer.nvidia.com/physx-sdk
- fmod: https://www.fmod.com/

### Models
- Chessboard with figures: https://free3d.com/3d-model/chess-table-18114.html
- Chest of drawers: https://www.turbosquid.com/3d-models/free-sideboard-3d-model/558818
- Frame: https://free3d.com/3d-model/frame-75994.html
- Knight1 and Knight2: https://free3d.com/3d-model/knight-84265.html
- Door: https://free3d.com/3d-model/medieval-door-16986.html
- Torches: https://www.yobi3d.com/q/3d-models-fire-torch
- Wardrobe: https://www.turbosquid.com/3d-models/classic-wardrobe-unwrap-model-1288182
- Chairs: https://www.turbosquid.com/3d-models/free-wooden-chair-3d-model/791045
- Table: https://free3d.com/3d-model/round-table-928375.html

### Additional Textures
- Walls of Room: Stone(1)01 from Total_Textures_V1
- Ceiling of Room: Wood(2)09 from Total_Textures_V1
- Floor of Room: Wood(2)10 from Total_Textures_V1
- Frame Image: https://dermeister.at/evilmaster01.jpg

### Music
- Spooky Music Instrumental - Ravensbrook: https://www.youtube.com/watch?v=H9D8fAC6CoU
- Sound Effects: https://freesound.org

## VIDEO
https://www.cg.tuwien.ac.at/courses/Realtime/HallOfFame/2018/video/video-Gruppe37.mp4

## Authors
* Michael Pointner
* Simon Reisinger
