#pragma once

#include "door.h"
#include "raylib.h"

// A node of a four-way linked list, with references to Doors for each path
// a reference is stored for each cardinal direction, as well as an (x, y) position relative to the root node
class Room
{
  public:
    // Rooms to the north, east, south, and west
    Room *n, *e, *s, *w;
    // Doors associated with each room that may be stored
    Door *nDoor, *eDoor, *sDoor, *wDoor;
    // position relative to the root node
    int x, y;

    Room();
    Room(int x, int y);

    // returns four rectangles to represent the collision boxes of doors in each direction
    // the direction is mapped to doorSpace[Directions]
    std::vector<Rectangle> doorSpace();

    // returns the door that leads to the given room
    Door *doorTo(Room *room);

    // returns the direction to given room
    int directionTo(Room *room);

    // Get the (x, y) index of the room which contains the position
    static Vector2 roomLocation(Vector2 pos);

    // returns if the two positions are in the same room
    static int sameRoom(Vector2 a, Vector2 b);
  private:
    static int _floor(float x);
};

