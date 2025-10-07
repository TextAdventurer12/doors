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
  arguments.erase(arguments.begin()); // erase the first argument
  type = getType(identifier); // identify the type from the alias
  this->arguments = arguments; // store arguments
}


int Command::getType(std::string name)
{
  int type = TYPE_NONE; // default to TYPE_NONE (0)
  // continue checking aliases until there are no types remaining
  for (type = 0; type < typeNames.size(); type++)
    if (typeNames[type] == name) // check if the provided name fits the alias
      return type; // if so, this is the correct type
  return TYPE_NONE; // if no aliases are found, it must be a NONE type
}


void Command::Process(Terminal &target)
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
