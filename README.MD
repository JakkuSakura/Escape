# Escape
This is a simple shooting game where AI control the hero to escape the underground world, as a platform for convenience to learn AI stuff.

It's still in very early stage.

## Requirements
Ogre 1.12
Box2D
Lua

## TODO

- [ ] Replace 

- [ ] Replace Ogre with Cocos2dx. I think Ogre is for 3d graphics, while for pure and simple 2D game like this is too heavy. Due to the separated implementation, it's very easy to change to another graphic library.

- [ ] Use lua to manipulate Agent behaviors.
 
 ## Folders
 assets holds all the runtime resources and will be copied into bin when calling cmake. Sometimes there's a cache issue, use copy assets into bin manually.
  