#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "widgets.h"
#include "../gui.h"
StaticLabel::StaticLabel(const std::string& text,const glm::vec2& Origin,double CharacterScale)
                         : label(text), Widget(Origin), characterScale(CharacterScale)
{
  glm::vec3 a = GUI::calculateStringDimensions(label,1);
  int stringWidth = a.x;
  int startY = a.y;
  int maxY = a.z;
  //std::cout << a.x << ":" << a.y << ":" << a.z <<"\n";


  //std::cout << GUI::dimensions.x << ":" << GUI::dimensions.y << "\n";
  glm::vec2 stringDimensions = glm::vec2(stringWidth/(double)GUI::dimensions.x,((maxY)/(double)GUI::dimensions.y));
  //std::cout << stringDimensions.x << ":" << stringDimensions.y << "\n";
  dimensions = stringDimensions*glm::vec2(1);

  int width = stringDimensions.x*GUI::dimensions.x;
  int height = stringDimensions.y*GUI::dimensions.y;
  //std::cout << width <<":" << height << "\n";
  //std::cout << (a.y/height)*(height/(double)GUI::dimensions.y) << "Asdsa\n";
  glGenFramebuffers(1,&FBO);
  glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,textureID,0);
    glBindTexture(GL_TEXTURE_2D,0);
  glBindFramebuffer(GL_FRAMEBUFFER,0);


  glViewport(0,0,width,height);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      GUI::renderText(text,glm::vec2(0,startY/GUI::dimensions.y),characterScale,glm::vec4(1));
    glBindFramebuffer(GL_FRAMEBUFFER,0);


  model = GUI::calculateQuadModel(origin,origin+dimensions);


}

void StaticLabel::draw()
{
  //GUI::drawQuad(model,glm::vec4(0,0,0,1));
  GUI::drawImage(model,textureID);
}

StaticLabel::~StaticLabel()
{
  glDeleteFramebuffers(1, &FBO);
  glDeleteTextures(1,&textureID);
}
