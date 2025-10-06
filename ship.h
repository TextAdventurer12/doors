#pragma once

#include <stdlib.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "raylib.h"
#include <stdio.h>
#include <iomanip>

typedef struct displayRoom
{
  int x;
  int y;
} DisplayRoom;

class Door
{
  public:
    std::string name;
    int state;

    Door();
    Door(std::vector<Door*> otherDoors);
    Door(std::string name, int state);
    
    std::string generateId();

  private:
    int isUnique(std::vector<Door*> otherDoors, std::string id);
};

class Room
{
  public:
    Room *n, *e, *s, *w;
    Door *nDoor, *eDoor, *sDoor, *wDoor;
    int x, y;

    Room();
    Room(int x, int y);
};

class Ship
{
  public:
    enum Directions
    {
      NORTH,
      EAST,
      SOUTH,
      WEST
    };
    Room *root;
    std::vector<Room*> roomArena;
    std::vector<Door*> doorsArena;
    Vector2 topLeft;
    int width;
    int height;
    Ship();
    Ship(Vector2 topLeft, int width, int height);
    ~Ship();

    void addRoom(Room *stem, int direction);
    // This might be improved eventually - but right now, it relies on rooms being placed correctly
    void Draw();
};
