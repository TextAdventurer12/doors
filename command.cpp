#include "command.h"

// Initialise aliases for command types
std::vector<std::string> Command::typeNames =
  {
  "NONE",
  "JOKE",
  "ECHO"
};

Command::Command()
{
  
}

Command::Command(std::vector<std::string> arguments)
{
  // take the first argument to be the type of command
  std::string identifier = arguments[0];
  type = getType(identifier); // identify the type from the alias
  if (type != TYPE_DOOR)
    arguments.erase(arguments.begin()); // erase the first argument
  this->arguments = arguments; // store arguments
}


int Command::getType(std::string name)
{
  int type = TYPE_NONE; // default to TYPE_NONE (0)
  if (isCode(name))
    return TYPE_DOOR;
  // continue checking aliases until there are no types remaining
  for (type = 0; type < typeNames.size(); type++)
    if (typeNames[type] == name) // check if the provided name fits the alias
      return type; // if so, this is the correct type
  return TYPE_NONE; // if no aliases are found, it must be a NONE type
}

int isHexChar(char ch)
{
  return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z');
}

int Command::isCode(std::string name)
{
  if (name.size() != 2)
    return 0;
  return isHexChar(name[0]) && isHexChar(name[1]);
}


void Command::Process(Terminal &target, Ship &ship, Terminal &log)
{
  // Logic performed depends on the type of the command
  // Check for type and pass off to other Process functions for each type
  switch(type)
  {
    case TYPE_NONE:
      ProcessNone(target);
      break;

    case TYPE_JOKE:
      ProcessJoke(target);
      break;

     case TYPE_ECHO:
       ProcessEcho(target);
       break;

      case TYPE_DOOR:
        ProcessDoor(ship, log);
        break;
  }
  return;
}

void Command::ProcessNone(Terminal &target)
{
  // Do nothing
}

void Command::ProcessJoke(Terminal &target)
{
  // Write predefined line to the provided terminal
  target.WriteLine("Why did the chicken cross the road? to cross it");
}

void Command::ProcessEcho(Terminal &target)
{
  for (std::string str : arguments) // check each string in the stored arguments
    target.WriteLine(str); // repeat on a new line
}

void Command::ProcessDoor(Ship &target, Terminal &log)
{
  for (Door *door : target.doorsArena)
  {
    if (door->name == arguments[0])
    {
      if (door->state == Door::CLOSED)
      {
        char buf[16];
        snprintf(buf, 16, "OPENED: %s", door->name.c_str());
        log.WriteLine(buf);
      }
      if (door->state == Door::OPEN)
      {
        char buf[16];
        snprintf(buf, 16, "CLOSED: %s", door->name.c_str());
        log.WriteLine(buf);
      }
      door->state = !door->state;
      return;
    }
  }
}
