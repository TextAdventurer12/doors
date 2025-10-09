#include "ship.h"

// Create and initialise the memory for a new ship
Ship::Ship()
{
  root = new Room(0, 0); // create a default root at (0, 0)
  roomArena.push_back(root); // store reference to the root
}

// Clear all memory associated with this ship
Ship::~Ship()
{
  for (Room *room : roomArena) // iterate through the vector of references
    delete room; // delete each stored room
}

// Add a new room in a given direction from the provided stem
void Ship::addRoom(Room *stem, int direction)
{
  int x = stem->x; // fetch the (x, y) position of the stem
  int y = stem->y;
  // find the (x, y) of this new room
  switch (direction)
  {
    case NORTH:
      y--;
      break;
    case EAST:
      x++;
      break;
    case SOUTH:
      y++;
      break;
    case WEST:
      x--;
      break;
  }
  // update bounding boxes of this ship
  x_min = std::min(x, x_min);
  x_max = std::max(x, x_max);
  y_min = std::min(y, y_min);
  y_max = std::max(y, y_max);

  float x_count = x_max - x_min + 3;
  float y_count = y_max - y_min + 3;

  roomLength = std::min(width / x_count, height / y_count);
  
  // Allocate and store references to memory
  Room *newRoom = new Room(x, y);
  roomArena.push_back(newRoom);
  Door *door = new Door(doorsArena);
  doorsArena.push_back(door);
  // Update references in the linked list depending on the direction of the new room
  // Sets the references both in the stem Room and the newRoom
  switch (direction)
  {
    case NORTH:
      stem->n = newRoom;
      stem->nDoor = door;
      newRoom->s = stem;
      newRoom->sDoor = door;
      break;

    case EAST:
      stem->e = newRoom;
      stem->eDoor = door;
      newRoom->w = stem;
      newRoom->wDoor = door;
      break;

    case SOUTH:
      stem->s = newRoom;
      stem->sDoor = door;
      newRoom->n = stem;
      newRoom->nDoor = door;
      break;

    case WEST: 
      stem->w = newRoom;
      stem->wDoor = door;
      newRoom->e = stem;
      newRoom->eDoor = door;
      break;
  }
}

Ship::Ship(Vector2 topLeft, int width, int height)
{
  this->topLeft = topLeft;
  this->width = width;
  this->height = height;
  root = new Room(0, 0); // allocate memory
  roomArena.push_back(root); // store reference
}

void Ship::Draw()
{
  // these could be calculated from world pos (and probably should be) but this functions fine and requires less changes
  int r_width = width / (x_max - x_min + 3);
  int r_height = height / (y_max - y_min + 3);

  // this is why calculated from world pos is probably better - if this behaviour changes it has to be changed here too
  r_width = std::min(r_width, r_height);
  r_height = std::min(r_width, r_height);
  
  // draw a black background
  DrawRectangle(topLeft.x, topLeft.y, width, height, BLACK);

  // draw each room individually
  for (Room *room : roomArena)
  {
    Vector2 pos = worldSpace((Vector2){(float)room->x, (float)room->y});
    int x = pos.x;
    int y = pos.y;

    // draw the walls of the room
    DrawRectangleLines(pos.x, pos.y, r_width, r_height, WHITE);

    float d_width = r_width * 0.4f;
    float d_height = r_height * 0.4f;
    
    // Draw doors on this room
    if (room->nDoor)
    {
      int d_x = x + (r_width - d_width) / 2;
      int d_y = y - d_height / 2;
      DrawDoor(room->nDoor, d_x, d_y, d_height, d_width);
    }

    if (room->sDoor)
    {
      int d_x = x + (r_width - d_width) / 2;
      int d_y = y + r_height - d_height / 2;
      DrawDoor(room->sDoor, d_x, d_y, d_height, d_width);
    }
    
    if (room->eDoor)
    {
      int d_x = x + r_width - d_width / 2;
      int d_y = y + d_height / 2;
      DrawDoor(room->eDoor, d_x, d_y, d_height, d_width);
    }

    if (room->wDoor)
    {
      int d_x = x - d_width / 2;
      int d_y = y + d_height / 2;
      DrawDoor(room->wDoor, d_x, d_y, d_height, d_width);
    }
  }
}

void Ship::DrawDoor(Door *door, int x, int y, int height, int width)
{
  int fontSize = height * 0.7;
  DrawRectangle(x, y, width, height, BLACK);
  DrawRectangleLines(x, y, width, height, door->state == Door::CLOSED ? RED : GREEN);
  int t_width = MeasureText(door->name.c_str(), fontSize);
  int t_x = x + width/2 - t_width / 2;
  int t_y = y + (height - fontSize)/2;
  DrawText(door->name.c_str(), t_x, t_y, fontSize, WHITE);
}


Vector2 Ship::worldSpace(Vector2 roomSpace)
{
  float x = (roomSpace.x - x_min + 1) * roomLength + topLeft.x;
  float y = (roomSpace.y - y_min + 1) * roomLength + topLeft.y;

  return (Vector2) {x, y};
}

Room *Ship::getRoom(int x, int y)
{
  for (Room *room : roomArena)
    if (room->x == x && room->y == y)
      return room;
  return NULL;
}

Room *Ship::getRoom(Vector2 pos)
{
  Vector2 rPos = Room::roomLocation(pos);
  return getRoom(rPos.x, rPos.y);
}

Vector2 Ship::roomSpace(Vector2 worldSpace)
{
  float x = (worldSpace.x - topLeft.x) / roomLength + x_min - 1;
  float y = (worldSpace.y - topLeft.y) / roomLength + y_min - 1;

  return (Vector2) {x, y};
}

float Ship::roomScale(float px)
{
  return px / roomLength;
}

void Ship::linkRooms(Room *a, Room *b, Direction aDir, Direction bDir)
{
  Door *connector = new Door(doorsArena);
  doorsArena.push_back(connector);
  switch (aDir)
  {
    case NORTH:
      a->nDoor = connector;
      a->n = b;
      break;
    case EAST:
      a->eDoor = connector;
      a->e = b;
      break;
    case SOUTH:
      a->sDoor = connector;
      a->s = b;
      break;
    case WEST:
      a->wDoor = connector;
      a->w = b;
      break;
  }
  switch (bDir)
  {
    case NORTH:
      b->nDoor = connector;
      b->n = a;
      break;
    case EAST:
      b->eDoor = connector;
      b->e = a;
      break;
    case SOUTH:
      b->sDoor = connector;
      b->s = a;
      break;
    case WEST:
      b->wDoor = connector;
      b->w = a;
      break;
  }
}

