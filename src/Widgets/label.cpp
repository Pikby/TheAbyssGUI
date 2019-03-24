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
