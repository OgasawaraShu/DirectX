#pragma once


//当たり判定属性

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0;//地形
const unsigned short COLLISION_ATTR_PLAYER = 0b1 << 1;//自機
const unsigned short COLLISION_ATTR_BULLET_BLUE = 0b1 << 2;//球
const unsigned short COLLISION_ATTR_BULLET_RED = 0b1 << 3;//球
const unsigned short COLLISION_ATTR_OBJ= 0b1 << 4;//球
const unsigned short COLLISION_ATTR_WALL = 0b1 << 5;//壁.j
const unsigned short COLLISION_ATTR_SPHEREOBJ = 0b1 << 6;//円形の壁
const unsigned short COLLISION_ATTR_OBJ2 = 0b1 << 7;//円形の壁
const unsigned short COLLISION_ATTR_EXIT = 0b1 << 8;//円形の壁




