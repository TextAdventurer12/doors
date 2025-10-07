#include "raylib.h"
#include <sstream>
#include <vector>
#include <string>
#include "terminal.h"
#include "command.h"
#include <stdio.h>
#include "ship.h"

// split a string into a vector of strings based on a delimeter
std::vector<std::string> split(std::string str, std::string delimeter)
{
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;
  while ((pos = str.find(delimeter)) != std::string::npos)
  {
    token = str.substr(0, pos);
    tokens.push_back(token);
    str.erase(0, pos + delimeter.length());
  }
  tokens.push_back(str);

  return tokens;
}

// TODO: implement enemies & pathfinding

int main(int argc, char **argv)
{
  const int screenWidth = 1920;
  const int screenHeight = 1080;

  InitWindow(screenWidth, screenHeight, "raylib window");

  SetTargetFPS(60);

  // Create a terminal
  Terminal ter = Terminal((Vector2){100, 50}, 1, 1, 35);
  // Create a ship and construct rooms
  Ship sh = Ship((Vector2){100, 200}, 800, 600);
  sh.addRoom(sh.root, Ship::NORTH);
  sh.addRoom(sh.root->n, Ship::EAST);

  while (!WindowShouldClose())
  {
    // fetch pressed key and pass it into the terminal
    int key;
    key = GetKeyPressed();
    ter.Update(key);

    // if a command was input this frame
    if (ter.interrupt)
    {
      // identify the arguments and type of this command
      std::vector<std::string> components = split(ter.command, " ");
      // pass off and process the Command
      Command cmd = Command(components);
      cmd.Process(ter);
    }
    
    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Display the terminal and ship
      ter.Draw();
      sh.Draw();
      
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

