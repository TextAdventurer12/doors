#pragma once

#include <vector>
#include <string>
#include "terminal.h"

class Command
{
  public:
    enum CMD_TYPE
    {
      TYPE_NONE,
      TYPE_JOKE,
      TYPE_ECHO
    };
    static std::vector<std::string> typeNames;
    int type;
    std::vector<std::string> arguments;
    static int getType(std::string name);
    Command();
    Command(std::vector<std::string> arguments);
    void Process(Terminal &target);

    void ProcessNone(Terminal &target);
    void ProcessJoke(Terminal &target);
    void ProcessEcho(Terminal &target);
};
