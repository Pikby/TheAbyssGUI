#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "widgets.h"
#include "../gui.h"
Label::Label(const std::string& text,const glm::vec2& Origin,double CharacterScale)
                         : label(text), Widget(Origin), characterScale(CharacterScale)
{
}

void Label::draw()
{
  GUI::renderText(label,origin,characterScale,glm::vec4(1));
}

EditBox::EditBox(const std::string& text,const glm::vec2& Origin,const glm::vec2& Dims,double CharacterScale)
                         : text(text), Widget(Origin,Dims), characterScale(CharacterScale)
{
}

void EditBox::handleCharInput(uint character)
{
  text.insert(cursorPosition,(char*)&character);
  cursorPosition++;
}

void EditBox::handleKeyInput(int key,int action)
{
  if(action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch(key)
    {
      case(GLFW_KEY_BACKSPACE):
        if(cursorPosition > 0)
        {
          text.erase(--cursorPosition,1);
        }
      break;

      case(GLFW_KEY_ENTER):
      break;

      case(GLFW_KEY_LEFT):
        if(cursorPosition>0) cursorPosition--;
      break;

      case(GLFW_KEY_RIGHT):
        if(cursorPosition<text.size()) cursorPosition++;
      break;

      case(GLFW_KEY_DELETE):
          if(cursorPosition<text.size()) text.erase(cursorPosition,1);
    }
  }
}

void EditBox::draw()
{
  bool hasCursor = ((int)round(glfwGetTime()*2)) % 2;
  if(hasCursor && focused) GUI::textRenderer.renderText(text+' ',origin,characterScale,glm::vec4(1),glm::mat3(1),TEXTALILEFT,cursorPosition);
  else GUI::textRenderer.renderText(text+' ',origin,characterScale,glm::vec4(1),glm::mat3(1),TEXTALILEFT,-1);

  GUI::setQuadDepth(0.5);
    GUI::drawQuad(origin-glm::vec2(padding),origin+dimensions+glm::vec2(padding),glm::vec4(glm::vec3(0.5),1));
  GUI::setQuadDepth(0.0);
}
