#include "terminal.h"

Terminal::Terminal()
{
  currentLine = "";
  cols = 80;
  rows = 20;
  fontSize = 12;
  interrupt = 0;
}

Terminal::Terminal(Vector2 topLeft, int cols, int rows, int fontSize)
{
  this->topLeft = topLeft;
  this->cols = cols;
  this->rows = rows;
  this->fontSize = fontSize;
  currentLine = "";
  interrupt = 0;
}

void Terminal::Update(int key)
{
  interrupt = 0;
  if (key == KEY_NULL)
    return;
  if (key == KEY_ENTER)
  {
    pastLines.push(currentLine);
    if (pastLines.size() >= rows)
      pastLines.pop();
    command = currentLine;
    interrupt = 1;
    currentLine = "";
    return;
  }
  if ((key >= KEY_APOSTROPHE && key <= KEY_GRAVE) || key == KEY_SPACE)
  {
    if (currentLine.size() >= cols)
      return;
    currentLine += key;
    return;
  }
  if (key == KEY_BACKSPACE && !currentLine.empty())
  {
    currentLine.pop_back();
    return;
  }
}

void Terminal::Draw()
{
  DrawRectangleV(topLeft, (Vector2){(cols + 1) * fontSize * 0.8,(rows+0.1) * fontSize}, BLACK);
  std::queue<std::string> tmp(pastLines);
  tmp.push(currentLine);
  for (int i = 0; !tmp.empty(); i++)
  {
    std::string activeLine = tmp.front();
    DrawText(tmp.front().c_str(), topLeft.x + fontSize * (10.f/32), topLeft.y + (i+0.1) * fontSize, fontSize, LIGHTGRAY);
    tmp.pop();
  }
  DrawRectangle(topLeft.x + MeasureText(currentLine.c_str(), fontSize) + fontSize * (15.f/32), topLeft.y + pastLines.size() * fontSize, 20.0/32 * fontSize, fontSize, LIGHTGRAY);
}


void Terminal::WriteLine(std::string line)
{
  pastLines.push(line);
  if (pastLines.size() >= rows)
    pastLines.pop();
  return;
}
