#include "raylib.h"
#include <sstream>
#include <vector>
#include <string>
#include "terminal.h"
#include "command.h"
#include <stdio.h>
#include "ship.h"

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

// TODO: Create map display

int main(int argc, char **argv)
{
  const int screenWidth = 1920;
  const int screenHeight = 1080;

  InitWindow(screenWidth, screenHeight, "raylib window");

  SetTargetFPS(60);

  Terminal ter = Terminal((Vector2){100, 50}, 1, 1, 35);
  Ship sh = Ship((Vector2){100, 200}, 800, 600);
  sh.addRoom(sh.root, Ship::NORTH);
  sh.addRoom(sh.root->n, Ship::EAST);

  while (!WindowShouldClose())
  {
    int key;
    key = GetKeyPressed();
    ter.Update(key);

    if (ter.interrupt)
    {
      std::vector<std::string> components = split(ter.command, " ");
      Command cmd = Command(components);
      cmd.Process(ter);
    }
    
    BeginDrawing();
      ClearBackground(RAYWHITE);

      ter.Draw();
      sh.Draw();
      
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

