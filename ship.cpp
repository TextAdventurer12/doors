#include "ship.h"

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


Ship::Ship()
{
  root = new Room(0, 0);
  roomArena.push_back(root);
}

Ship::~Ship()
{
  for (Room *room : roomArena)
    delete room;
}

void Ship::addRoom(Room *stem, int direction)
{
  int x = stem->x;
  int y = stem->y;
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
  Room *newRoom = new Room(x, y);
  roomArena.push_back(newRoom);
  Door *door = new Door(doorsArena);
  doorsArena.push_back(door);
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
  name = generateId();
  state = 0;
}

Door::Door(std::vector<Door*> otherDoors)
{
  std::string id = generateId();
  while (!isUnique(otherDoors, id))
    id = generateId();
  name = id;
  state = 0;
}

int Door::isUnique(std::vector<Door*> otherDoors, std::string id)
{
  for (Door *door : otherDoors)
    if (door->name == id)
      return 0;
  return 1;
}

Door::Door(std::string name, int state)
{
  this->name = name;
  this->state = state;
}


std::string Door::generateId()
{
  int id = rand() % 256;
  std::stringstream stream;
  stream << std::hex << id;
  return stream.str();
}

Ship::Ship(Vector2 topLeft, int width, int height)
{
  this->topLeft = topLeft;
  this->width = width;
  this->height = height;
  root = new Room(0, 0);
  roomArena.push_back(root);
}

void Ship::Draw()
{
  int x_min = 1000;
  int x_max = -1000;
  int y_min = 1000;
  int y_max = -1000;
  for (Room *room : roomArena)
  {
    x_min = std::min(x_min, room->x);
    y_min = std::min(y_min, room->y);
    x_max = std::max(x_max, room->x);
    y_max = std::max(y_max, room->y);
  }
  double x_count = x_max - x_min + 3;
  double y_count = y_max - y_min + 3;
 
  double r_width = width / x_count;
  double r_height = height / y_count;

  DrawRectangle(topLeft.x, topLeft.y, width, height, BLACK);
 
  for (Room *room : roomArena)
  {
    // generate a grid of cells, and draw the cells with rooms on top of them
    int x = (room->x - x_min + 1) * r_width + topLeft.x;
    int y = (room->y - y_min + 1) * r_height + topLeft.y;

    DrawRectangleLines(x, y, r_width, r_height, WHITE);

    
    

    if (room->nDoor)
    {
      double d_width = r_width * 0.66;
      double d_height = r_height * 0.2;
      int x_door = x + (r_width - d_width) / 2;
      int y_door = y - d_height / 2;
      DrawRectangle(x_door, y_door, d_width, d_height, BLACK);
      DrawRectangleLines(x_door, y_door, d_width, d_height, WHITE);
      int t_x = x + r_width/2 - d_height * 0.6;
      int t_y = y_door + d_height * 0.1;
      DrawText(room->nDoor->name.c_str(), t_x, t_y, d_height * 0.8, WHITE);
    }

    if (room->sDoor)
    {
      double d_width = r_width * 0.66;
      double d_height = r_height * 0.2;
      int x_door = x + (r_width - d_width) / 2;
      int y_door = y - d_height / 2 + r_height;
      DrawRectangle(x_door, y_door, d_width, d_height, BLACK);
      DrawRectangleLines(x_door, y_door, d_width, d_height, WHITE);
      int t_x = x + r_width/2 - d_height * 0.6;
      int t_y = y_door + d_height * 0.1;
      DrawText(room->sDoor->name.c_str(), t_x, t_y, d_height * 0.8, WHITE);
    }
    

    if (room->eDoor)
    {
      double d_width = r_width * 0.2;
      double d_height = r_height * 0.66;
      int x_door = x + r_width - d_width/2;
      int y_door = y + (r_height - d_height)/2;
      DrawRectangle(x_door, y_door, d_width, d_height, BLACK);
      DrawRectangleLines(x_door, y_door, d_width, d_height, WHITE);
      Font ft = GetFontDefault();
      int t_x = x_door + d_height * 0.3;
      int t_y = y_door + d_height * 0.1;
      Vector2 pos = (Vector2){t_x, t_y};
      float spacing = d_width * 0.8 / 10;
      spacing = spacing < 1 ? 1 : spacing;
      DrawTextPro(ft, room->eDoor->name.c_str(), pos, (Vector2){0, 0}, 90, d_width * 0.8, spacing, WHITE);
    }

    if (room->wDoor)
    {
      double d_width = r_width * 0.2;
      double d_height = r_height * 0.66;
      int x_door = x - d_width/2;
      int y_door = y + (r_height - d_height)/2;
      DrawRectangle(x_door, y_door, d_width, d_height, BLACK);
      DrawRectangleLines(x_door, y_door, d_width, d_height, WHITE);
      Font ft = GetFontDefault();
      int t_x = x_door + d_height * 0.35;
      int t_y = y_door + d_height * 0.3;
      Vector2 pos = (Vector2){t_x, t_y};
      float spacing = d_width * 0.8 / 10;
      spacing = spacing < 1 ? 1 : spacing;
      DrawTextPro(ft, room->wDoor->name.c_str(), pos, (Vector2){0, 0}, 90, d_width * 0.8, spacing, WHITE);
    }

  }
}
