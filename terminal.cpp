#include "terminal.h"

Terminal::Terminal()
{
}

Terminal::Terminal(Vector2 topLeft, int width, int rows, int fontSize)
{
  this->topLeft = topLeft;
  this->width = width;
  this->rows = rows;
  this->fontSize = fontSize;
  currentLine = "";
  interrupt = 0;
}

// process a keyboard input
void Terminal::Update(int key)
{
  // reset the state of the terminal so commands are not passed over multiple frames
  interrupt = 0;
  // if no key is input, return
  if (key == KEY_NULL)
    return;
  // if the enter key was pressed, provide the current line as a command and return
  if (key == KEY_ENTER)
  {
    pastLines.push(currentLine); // store this line in the terminal's history
    if (pastLines.size() >= rows)
      pastLines.pop(); // ensure that the terminal has the correct number of rows
    command = currentLine; // fill the command property for other parts of the program to fetch
    interrupt = 1; // notify that a command is available
    currentLine = ""; // clear the current line
    return;
  }
  // if an alphanumeric key is pressed, type it into the terminal
  if ((key >= KEY_APOSTROPHE && key <= KEY_GRAVE) || key == KEY_SPACE)
  {
    // only allow for inputs up to the maximum size of the terminal
    // Adds some extra padding to ensure the last character doesn't go past the edge of the window
    if (MeasureText(currentLine.c_str(), fontSize) >= width - fontSize)
      return;
    // add the pressed key to the current line
    currentLine += key;
    return;
  }
  // if the backspace key was pressed and the current line is not empty
  if (key == KEY_BACKSPACE && !currentLine.empty())
  {
    // remove the most recently pressed character
    currentLine.pop_back();
    return;
  }
}

// draw a terminal to the screen
void Terminal::Draw()
{
  // draw a black background
  DrawRectangleV(topLeft, (Vector2){width,(rows+0.1) * fontSize}, BLACK);
  // store the history of the terminal
  std::queue<std::string> tmp(pastLines);
  // add the current line alongside the history
  tmp.push(currentLine);
  for (int i = 0; !tmp.empty(); i++) // continue until all lines are displayed
  {
    // fetch the current line
    std::string activeLine = tmp.front();
    DrawText(tmp.front().c_str(), topLeft.x + fontSize * (10.f/32), topLeft.y + (i+0.1) * fontSize, fontSize, LIGHTGRAY);
    tmp.pop(); // remove the processed line
  }
  // draw a cursor
  DrawRectangle(topLeft.x + MeasureText(currentLine.c_str(), fontSize) + fontSize * (15.f/32), topLeft.y + pastLines.size() * fontSize, 20.0/32 * fontSize, fontSize, LIGHTGRAY);
}

// write a line of text from an external source into history
void Terminal::WriteLine(std::string line)
{
  pastLines.push(line);
  if (pastLines.size() >= rows) // ensure the terminal remains the correct size
    pastLines.pop();
  return;
}
