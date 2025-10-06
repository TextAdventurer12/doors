#include "command.h"

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
  std::string identifier = arguments[0];
  arguments.erase(arguments.begin());
  type = getType(identifier);
  this->arguments = arguments;
}


int Command::getType(std::string name)
{
  int type = 0;
  for (type = 0; type < typeNames.size(); type++)
    if (typeNames[type] == name)
      return type;
  return TYPE_NONE;
}


void Command::Process(Terminal &target)
{
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
}

void Command::ProcessJoke(Terminal &target)
{
  target.WriteLine("Why did the chicken cross the road? to cross it");
}

void Command::ProcessEcho(Terminal &target)
{
  for (std::string str : arguments)
    target.WriteLine(str);
}
