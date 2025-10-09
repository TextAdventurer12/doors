#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

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

