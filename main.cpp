#include "raylib.h"
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

int _floor(double x)
{
  if (x == (double)(int)x) return x;
  return x > 0 ? (int)x : (int)(x - 1);
}

// TODO: implement enemies & pathfinding

int main(int argc, char **argv)
{
  const int screenWidth = 1920;
  const int screenHeight = 1080;

  InitWindow(screenWidth, screenHeight, "raylib window");

  SetTargetFPS(60);

  // Create a terminal
  Terminal ter = Terminal((Vector2){460, 675}, 1000, 10, 35);
  // Create a ship and construct rooms
  Ship sh = Ship((Vector2){660, 50}, 600, 600);
  sh.addRoom(sh.root, Ship::NORTH);
  sh.addRoom(sh.getRoom(0, -1), Ship::EAST);
  sh.addRoom(sh.getRoom(1, -1), Ship::EAST);
  sh.addRoom(sh.getRoom(2, -1), Ship::SOUTH);


  Vector2 guyPos = (Vector2) { 0.5, 0.5};
  float guyRad = 20;
  float guySpeed = 0.1;

  while (!WindowShouldClose())
  {
    // fetch pressed key and pass it into the terminal
    int key;
    key = GetKeyPressed();
    ter.Update(key);

    Vector2 lastPos = guyPos;
    if (IsKeyDown(KEY_LEFT))
      guyPos.x -= guySpeed;
    if (IsKeyDown(KEY_RIGHT))
      guyPos.x += guySpeed;
    if (IsKeyDown(KEY_UP))
      guyPos.y -= guySpeed;
    if (IsKeyDown(KEY_DOWN))
      guyPos.y += guySpeed;

    if (_floor(lastPos.x) != _floor(guyPos.x)) guyPos.x = lastPos.x;
    if (_floor(lastPos.y) != _floor(guyPos.y)) guyPos.y = lastPos.y;

    Room *currRoom = sh.getRoom(_floor(guyPos.x), _floor(guyPos.y));
    std::vector<Rectangle> worldDoors;
    std::vector<Ship::Direction> dir;
    if (currRoom)
    {
      std::vector<Rectangle> doors = currRoom->doorSpace();
      for (int i = 0; i < doors.size(); i++)
      {
        Rectangle &rect = doors[i];
        Vector2 pos = (Vector2){rect.x, rect.y};
        pos = sh.worldSpace(pos);
        Rectangle r = (Rectangle){pos.x, pos.y, sh.roomLength * rect.width, sh.roomLength * rect.height};
        //worldDoors.push_back(r);
      }

      if      (currRoom->nDoor && currRoom->nDoor->state == Door::OPEN && CheckCollisionCircleRec((guyPos), sh.roomScale(guyRad), doors[Ship::NORTH]))
      {
        guyPos.x = currRoom->n->x + 0.5;
        guyPos.y = currRoom->n->y + 0.5;
      }
      else if (currRoom->eDoor && currRoom->eDoor->state == Door::OPEN && CheckCollisionCircleRec((guyPos), sh.roomScale(guyRad), doors[Ship::EAST]))
      {
        guyPos.x = currRoom->e->x + 0.5;
        guyPos.y = currRoom->e->y + 0.5;
      }
      else if (currRoom->sDoor && currRoom->sDoor->state == Door::OPEN && CheckCollisionCircleRec((guyPos), sh.roomScale(guyRad), doors[Ship::SOUTH]))
      {
        guyPos.x = currRoom->s->x + 0.5;
        guyPos.y = currRoom->s->y + 0.5;
      }
      else if (currRoom->wDoor && currRoom->wDoor->state == Door::OPEN && CheckCollisionCircleRec(guyPos, sh.roomScale(guyRad), doors[Ship::WEST]))
      {
        guyPos.x = currRoom->w->x + 0.5;
        guyPos.y = currRoom->w->y + 0.5;
      }
    }
   
    // if a command was input this frame
    if (ter.interrupt)
    {
      // identify the arguments and type of this command
      std::vector<std::string> components = split(ter.command, " ");
      // pass off and process the Command
      Command cmd = Command(components);
      cmd.Process(ter, sh);
    }
    
    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Display the terminal and ship
      ter.Draw();
      sh.Draw();
      DrawCircleV(sh.worldSpace(guyPos), guyRad, BLUE);
      
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

