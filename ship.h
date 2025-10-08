#pragma once

#include <stdlib.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "raylib.h"
#include <stdio.h>
#include <iomanip>
#include <sstream>

// Store a state and identifier for doors between rooms.
// Does not hold any identifying information for the rooms and position that this door is associated with
class Door
{
  public:
    enum State
    {
      CLOSED,
      OPEN,
      OPENING
    };
    // Intended to be a 2 digit hexadecimal number, unique to this door
    // However it may be any arbitrary string
    std::string name;
    // The current state of this door
    // TODO: May be open, closed, or changing state
    int state;

    Door();
    // Construct a new, open door with a unique name
    Door(std::vector<Door*> otherDoors);
    // Construct a door with the given parameters
    Door(std::string name, int state);

    // Generate a random 2 digit hexadecimal number
    static std::string generateId();

  private:
    // Check if the given vector of Doors contains another door with the given id
    static int isUnique(std::vector<Door*> otherDoors, std::string id);
};

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
};

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
    double roomLength;
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
    double roomScale(double px);

    Room *getRoom(int x, int y);

  private:
    void DrawDoor(Door *door, int x, int y, int height, int width);
    int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
};
