# Game Progress UPDATE

Eugeni Soler (253566)
David Obrero (251739)

VULKAN’S ESCAPE
At the moment the game is not working, since during this week we are redoing our code, but it's possible to see an example island, the sky cube map and move around the map. 
We have implemented next classes (some of them need to be improved):
    - World (render, update, parse scene, load sky)
    - Stage (stage, stage manager, menu stage, play stage)
    - EntityMesh (render, update, set type, change visibility)
    - EntityPlayer (render, update, hunger, sleep)
    - EntityCollider (check player collisions)
    - EntityUI (render, update, set type)
    - EntityAI (render, update, change state)


- Stage (PlayStage and MenuStage) is partially done, with its StageManager. We have to still define if a "outro" or "intro" stage is going to be needed and the conditions for navigating across stages.
- We have partially implemented Player collisions and the camera gameplay that follows it. We also need to manage its stats (healt, points, stamina...).

- We look forward to adapt and test the start menu, and create an inventory and minimap. For that we still have to download the images to display.
- We also want to add entities that drop items (food and wood) when hit and its health goes to 0, but for that we still have to manage and work out collisions.