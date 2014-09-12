Tales Of Theia Project Assets BRANCH
============

Games Development

Assets Branch

See README.txt for descriptions of branch folder contents
------------


Test Data (Specifically for Declan)
============
Test data for object loader is contained in Models/Biome_Cavern/OGRE Files/Test_Data

Order of size:

  * StartingRoom (mesh: 574 KB)

  * LushCavern_Big (mesh: 4599 KB)

  * Tunnels (mesh: 19,025 KB)

Animation (Specifically for Chris)
============

Do not start any animation until I have created the base character mesh and applied it to the AnimationSkeleton_Humanoid.blend file. I will update this readme document or inform you when I have done this. I think I need to do so because tacking on different armor assets onto the character mesh may affect animations if I don't.

Remember: README.txt file outlines each DIFFERENT animation. It does NOT outline each INSTANCE of each different animation.

Below are the animations that have been completed:

* NONE
* 

Level Models
============

Each level model will have 4 .blend files at 4 specific stages. Below they are listed in order of individual object size.

ICT207LevelName_walls
---------------------

   At this stage, the individual pieces that make up the level (lengths of walls, concave / convex segments / etc.) will NOT be connected together. Floor is included.

ICT207LevelName_walls_zoned
---------------------------

   At this stage, the individual rooms and corridoors are combined together into their respective 'zones'. Floor is included here.

ICT207LevelName_walls_joined
----------------------------

   At this stage, all wall segments are combined together into one massive object. Floor remains seperate.

ICT207LevelName_walls_joined_withFloor
--------------------------------------

   At this stage the floor is combined with the wall object so that the entire file is one object.
