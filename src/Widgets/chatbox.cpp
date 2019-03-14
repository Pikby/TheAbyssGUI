#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "widgets.h"
#include "../gui.h"


ChatBox::ChatBox(glm::vec2 newOrigin,glm::vec2 dimensions,double newSize) : Widget(newOrigin,dimensions),characterScale(newSize)
{
  padding = 0.005;
  focusable = true;
  focused = false;
  focusTarget = history.begin();
}

void ChatBox::draw()
{
  GUI::drawQuad(origin,origin+glm::vec2(0.001,0.001),glm::vec4(1));
  GUI::drawQuad(origin-glm::vec2(padding),origin+dimensions+glm::vec2(padding),glm::vec4(0.5));
  double curTime = glfwGetTime();
  bool extended = curTime < extendTime;


  glm::vec2 offset = glm::vec2(0.0f,(64.0f/GUI::dimensions.y)*characterScale);


  int numbOfMainLines = ceil((dimensions.y/offset.y));
  int linesToDraw;
  if(extended)
  {
    linesToDraw = ceil((1-origin.y)/offset.y);
  }
  else
  {
    linesToDraw = numbOfMainLines;
  }
  bool a = ((int)round(curTime*2)) % 2;
  char effect = a ? ' ' : '|';
  if(focused) GUI::renderText(currentLine+effect,origin,characterScale,glm::vec4(1,1,1,1));
  else GUI::renderText(currentLine,origin,characterScale,glm::vec4(1,1,1,1));



  auto target = focusTarget;
  int i = 1;
  while(target != history.end() && i < linesToDraw)
  {
    double alpha = 1;
    if(extended && extendTime - curTime < 1 && i >= numbOfMainLines)
    {
      alpha = extendTime-curTime;
    }
    GUI::renderText(*target,origin + (float)i*offset,characterScale,glm::vec4(1,1,1,alpha));
    i++;
    target++;
  }
}
void ChatBox::addLineToHistory(std::string line)
{
  history.push_front(line);
  focusTarget = history.begin();
  extendFor(5);
}

void ChatBox::handleCharInput(uint character)
{
  extendFor(5);
  double lineSize = GUI::calculateStringDimensions(currentLine+(char)character,characterScale).x/GUI::dimensions.x;
  if(lineSize > dimensions.x ) return;
  currentLine += char(character);


}
void ChatBox::extendFor(double seconds)
{
  extendTime = glfwGetTime()+seconds;
}
void ChatBox::handleScrollInput(double xoffset, double yoffset)
{
  extendFor(5);
  if(yoffset > 0 && focusTarget != history.end())
  {
    focusTarget++;
  }
  if(yoffset < 0 && focusTarget != history.begin())
  {
    focusTarget--;
  }
}

void ChatBox::handleKeyInput(int key, int action)
{

  if(action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch(key)
    {
      case(GLFW_KEY_BACKSPACE):
        if(currentLine.size() >0)
        {
          currentLine.pop_back();
        }
      break;
      case(GLFW_KEY_ENTER):
        addLineToHistory(currentLine);
        currentLine = "";
      break;
      case(GLFW_KEY_UP):
        extendFor(5);
        if(focusTarget!=history.end()) focusTarget++;
      break;
      case(GLFW_KEY_DOWN):
        extendFor(5);
        if(focusTarget!=history.begin()) focusTarget--;
      break;
    }
  }
}
