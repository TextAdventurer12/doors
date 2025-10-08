#include "ship.h"

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

  double x_count = x_max - x_min + 3;
  double y_count = y_max - y_min + 3;

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

Door::Door()
{
  // generate a random 2 digit hex code
  name = generateId();
  state = 0; // open
}

Door::Door(std::vector<Door*> otherDoors)
{
  std::string id = generateId(); // random 2 digit hex code
  while (!isUnique(otherDoors, id)) // continue generating new random 2 digit hex codes until it is unique
    id = generateId();
  name = id; // store the name
  state = 0;
}

int Door::isUnique(std::vector<Door*> otherDoors, std::string id)
{
  for (Door *door : otherDoors) // check every provided door
    if (door->name == id) // if the given id is found - it is not unique
      return 0;
  return 1; // if it is not found, it is unique
}

Door::Door(std::string name, int state)
{
  this->name = name;
  this->state = state;
}


std::string Door::generateId()
{
  int id = rand() % 256; // random number from 0x00 to 0xff
  std::stringstream stream;
  stream << std::hex << id; // store as a hex string
  std::string str = stream.str();
  for (char &ch : str) ch = ch + (ch >= 'a' && ch <= 'z' ? 'A' - 'a' : 0);
  return str;
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
    Vector2 pos = worldSpace((Vector2){room->x, room->y});
    int x = pos.x;
    int y = pos.y;

    // draw the walls of the room
    DrawRectangleLines(pos.x, pos.y, r_width, r_height, WHITE);

    double d_width = r_width * 0.4;
    double d_height = r_height * 0.4;
    
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
  DrawRectangleLines(x, y, width, height, door->state ? RED : GREEN);
  int t_width = MeasureText(door->name.c_str(), fontSize);
  int t_x = x + width/2 - t_width / 2;
  int t_y = y + (height - fontSize)/2;
  DrawText(door->name.c_str(), t_x, t_y, fontSize, WHITE);
}


std::vector<Rectangle> Room::doorSpace()
{
  Rectangle north = { x + 0.3, y, nDoor ? 0.4 : 0, nDoor ? 0.4 : 0};
  Rectangle east  = { x + 0.6, y + 0.3, eDoor ? 0.4 : 0, eDoor ? 0.4 : 0};
  Rectangle south = { x + 0.3, y + 0.6, sDoor ? 0.4 : 0, sDoor ? 0.4 : 0};
  Rectangle west  = { x, y + 0.3,  wDoor ? 0.4 : 0, wDoor ? 0.4 : 0};
  return std::vector<Rectangle> ({north, east, south, west});
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

Vector2 Ship::roomSpace(Vector2 worldSpace)
{
  float x = (worldSpace.x - topLeft.x) / roomLength + x_min - 1;
  float y = (worldSpace.y - topLeft.y) / roomLength + y_min - 1;

  return (Vector2) {x, y};
}

double Ship::roomScale(double px)
{
  return px / roomLength;
}
