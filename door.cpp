#include "door.h"



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

