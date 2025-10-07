#pragma once

#include <vector>
#include <string>
#include "terminal.h"

class Command
{
  public:
    enum CMD_TYPE
    {
      TYPE_NONE, // Default type (should not have attached logic)
      TYPE_JOKE, // Repeat a predefined line of text from code (debug command)
      TYPE_ECHO  // Echo the provided parameters (debug command)
    };
    // string aliases for each command. The position of each alias corresponds to the integer value of the command's type
    static std::vector<std::string> typeNames;
    // the type of this command
    int type;
    // the arguments that were passed alongside this command
    std::vector<std::string> arguments;
    // fetch the corresponding type to param name from member typeNames
    static int getType(std::string name);
    Command();
    // Construct a new command from the given arguments, with the first argument being used to identify the type of command
    Command(std::vector<std::string> arguments);
    // Process a command and pass its output into target
    void Process(Terminal &target);

    // Process a command of type TYPE_NONE
    void ProcessNone(Terminal &target);
    // Process a command of type TYPE_JOKE
    void ProcessJoke(Terminal &target);
    // Process a command of type TYPE_ECHO
    void ProcessEcho(Terminal &target);
};
