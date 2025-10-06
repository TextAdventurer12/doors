#pragma once

#include "raylib.h"
#include <queue>
#include <string>

class Terminal
{
  public:
    Terminal();
    Terminal(Vector2 topLeft, int cols, int rows, int fontSize);
    // update the terminal each frame. Involves a check for newly inputted keys (if two keys are pressed on the same frame one is lost)
    void Update(int key);
    // draw the terminal to the screen, displaying at the specified position with the contents of pastLines and currentLine
    void Draw();
    Vector2 topLeft;
    // the most recently input command. Null if interrupt is zero
    std::string command;
    // was a command entered last frame.
    // 0 for false (no command entered last frame)
    // 1 for true (command entered last frame)
    int interrupt;
    // write a line of text to the terminal
    void WriteLine(std::string line);
  protected:
    std::queue<std::string> pastLines;
    std::string currentLine;
    int cols;
    int rows;
    int fontSize;
};
