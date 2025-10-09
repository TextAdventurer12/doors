#include "room.h"

// Create a new room with default parameters
Room::Room()
{
  n = NULL;
  e = NULL;
  s = NULL;
  w = NULL;
  nDoor = NULL;
  eDoor = NULL;
  sDoor = NULL;
  wDoor = NULL;
  x = 0;
  y = 0;
}

// Create a new room at a given location relative to the root
Room::Room(int x, int y)
{
  n = NULL;
  e = NULL;
  s = NULL;
  w = NULL;
  nDoor = NULL;
  eDoor = NULL;
  sDoor = NULL;
  wDoor = NULL;
  this->x = x;
  this->y = y;
}


std::vector<Rectangle> Room::doorSpace()
{
  Rectangle north = { x + 0.3f, y - 0.2f, nDoor ? 0.4f : 0, nDoor ? 0.4f : 0};
  Rectangle east  = { x + 0.8f, y + 0.2f, eDoor ? 0.4f : 0, eDoor ? 0.4f : 0};
  Rectangle south = { x + 0.3f, y + 0.8f, sDoor ? 0.4f : 0, sDoor ? 0.4f : 0};
  Rectangle west  = { x - 0.2f, y + 0.2f, wDoor ? 0.4f : 0, wDoor ? 0.4f : 0};
  return std::vector<Rectangle> ({north, east, south, west});
}


int Room::_floor(float x)
{
  if (x == (float)(int)x) return x;
  return x > 0 ? (int)x : (int)(x - 1);
}

Vector2 Room::roomLocation(Vector2 pos)
{
  return (Vector2){(float)_floor(pos.x), (float)_floor(pos.y)};
}

int Room::sameRoom(Vector2 a, Vector2 b)
{
  Vector2 r_a = roomLocation(a);
  Vector2 r_b = roomLocation(b);
  
  return (r_a.x == r_b.x) && (r_a.y == r_b.y);
}
