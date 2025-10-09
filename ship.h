#pragma once

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include <stdio.h>
#include "door.h"
#include "room.h"

// Holds any handles allocation for a root node Room, alongside the ability to add new rooms and acces data
class Ship
{
  public:
    // Rooms may be placed at cardinal directions to each other
    enum Direction
    {
      NORTH,
      EAST,
      SOUTH,
      WEST
    };
    // The root node of the rooms list
    Room *root;
    // Stores references to all allocated Rooms and Doors, to be deleted alongside this ship
    std::vector<Room*> roomArena;
    std::vector<Door*> doorsArena;
    //  Where should this be rendered
    Vector2 topLeft;
    // Proportions for display
    int width;
    int height;
    // the side length of a room in pixels
    float roomLength;
    Ship();
    Ship(Vector2 topLeft, int width, int height);
    ~Ship();

    // add a new room from the given reference in the given direction (specified from the Directions enum)
    void addRoom(Room *stem, int direction);
    // Create a door at the specified direction to link the two provided rooms
    void linkRooms(Room *a, Room *b, Direction aDir, Direction bDir);
    // Display this ship. Automatically works out the size for square rooms to fit within the specified window
    // Relies on the generated room layout being correct and 2-D (ie stacking rooms is undefined behaviour)
    void Draw();

    // maps a point relative to a room to the point on the canvas
    // (0, 0) is the top left of the root, with the top lefts of other rooms placed at integer (x, y)
    Vector2 worldSpace(Vector2 roomSpace);

    // inverse of worldSpace(roomSpace)
    Vector2 roomSpace(Vector2 worldSpace);

    // convert a length in pixels to a length in room widths
    float roomScale(float px);

    Room *getRoom(int x, int y);
    Room *getRoom(Vector2 pos);

  private:
    void DrawDoor(Door *door, int x, int y, int height, int width);
    int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
};
