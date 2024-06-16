# VULKAN'S ESCAPE

Eugeni Soler (253566) eugeni.soler01@estudiant.upf.edu
David Obrero (251739) david.obrero01@estudiant.upf.edu

- Camera: first person, implemented together in class.
- Controls: WASD/Arrows to move, SPACE to jump and mouse to orbit and F to do actions (the game tells when). Implemented together in class.
- Parse and render scene: implemented together in class. We took an internet map from Wii Sports island because it had a volcano and matched well in our story. The scene also has different audio and skyboxes for day and night.
- Collisions: Are a little glitchy specially on the terrain zones with ramps, because the formula of slide with the dotproduct wasn't working. Sometimes the player doesn't detect the collision and starts falling, so by clicking TAB you can start floating and come back up.Implemented by Eugeni.
- Player's HUD/UI: we've tried to fill part of the screen with menus and bars indicating time of the game and stats, also a cross to know where the collisions act. Implemented by Eugeni.
- Loading stage: to not make it so empty, while everything is loading there's some music and a background image. There's also a stage for win and loose that would activate if the boat was repaired or 30 days spent on the sisland. Because this is slow to do, we have put shortcuts in keys M and N to skip to those stages. Implemented by David.
- Stages logic: the movement between stages. Implemented by David.
- Audio: Music, and audio effects for cutting trees and day and night music. Implemented by David.

**Disclaimers**
*Loading time is really high, as all assets and frame of the video is loaded at once in start. We are sorry for the time lost if you execute the code.
*Also, we debugged it using the default screen, and we realised we have not taken into account resizings and UI may get located badly if so.
*We diddn't reach the integration of AI, althought the class was created. We wanted to use a mesh and add some animations to it, like the sheeps in Minecraft, but ran out of time.