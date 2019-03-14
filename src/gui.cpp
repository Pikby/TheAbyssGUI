
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


#define GLSLSHADERSIMPLEMNTATION
#include "shaders.h"

#define GUILIBRARYIMPLEMENTATION
#include "gui.h"
#include "Widgets/widgets.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>




uint quadVAO,quadVBO;
void GUI::initGUI(const glm::ivec2 Dimensions)
{
  glm::mat4 projection = glm::ortho(0.0f,1.0f,0.0f,1.0f);
  dimensions = Dimensions;

  Shader::setShaderDirectory("../src/shaders/");
  GUIShader2D = Shader("guiShader2D.vs","guiShader2D.fs");
  GUIShaderCircle = Shader("guiCircleShader.vs","guiCircleShader.fs");
  GUIShaderImage = Shader("guiImageShader.vs","guiImageShader.fs");

  GUIShaderImage.use();
  GUIShaderImage.setMat4("projection",projection);
  GUIShaderImage.setInt("text",0);



  GUIShader2D.use();
  GUIShader2D.setMat4("projection",projection);




  GUIShaderCircle.use();
  GUIShaderCircle.setMat4("projection",projection);

  initQuadVAO();
  textRenderer.init();
  GUI::textRenderer.GUIShaderText.use();
}

void GUI::drawGUI()
{
  //textRenderer.GUIShaderText.use();
  for(auto itr = viewableList.begin(); itr!= viewableList.end();itr++)
  {
    (*itr)->draw();
  }
}

void GUI::freeGUI()
{
  for(auto itr = widgetList.begin();itr!=widgetList.end();itr++)
  {
    delete *itr;
  }
  widgetList.clear();
}


glm::mat3 GUI::calculateQuadModel(const glm::vec2& botLeft, const glm::vec2& topRight)
{
  float x2 = botLeft.x;
  float x1 = topRight.x;
  float y2 = botLeft.y;
  float y1 = topRight.y;

  float a = (x2-x1)/-2.0f;
  float b = x2+a;
  float c = (y2-y1)/-2.0f;
  float d = y2+c;

  glm::mat3 model =
  {
    a,0.0f,0.0f,
    0.0f,c,0.0f,
    b,d,1.0f,
  };
  //std::cout << a << ":" << b<< ":"<<c << ":" << d << "\n";

  return model;
}

void GUI::drawInstancedQuads(int count)
{
  glBindVertexArray(quadVAO);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4,count);
  glBindVertexArray(0);
}

void GUI::initQuadVAO()
{
  float quadVertices[] = {
      // positions        // texture Coords
      -1.0f,  1.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
       1.0f,  1.0f, 1.0f, 1.0f,
       1.0f, -1.0f, 1.0f, 0.0f,
  };
  // setup plane VAO
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}



void GUI::drawQuad(const glm::mat3& model,const glm::vec4 &color, QuadDrawType type, Shader* shader)
{
  shader->use();
  shader->setMat3("model",model);
  shader->setVec4("color",color);
  shader->setInt("quadStyle",type);
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}


void GUI::drawQuad(const glm::vec2 &botLeft,const glm::vec2 &topRight,const glm::vec4& color, QuadDrawType type, Shader* shader)
{
  glm::mat3 model = calculateQuadModel(botLeft,topRight);
  shader->use();
  shader->setMat3("model",model);
  shader->setVec4("color",color);
  shader->setInt("quadStyle",type);
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void GUI::drawQuad(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3 ,const glm::vec2 &p4, QuadDrawType type, Shader* shader)
{
  shader->setInt("quadStyle",type);
  drawTriangle(p1,p2,p3,shader);
  drawTriangle(p1,p3,p4,shader);
}



void GUI::drawTriangle(const glm::vec2& p1,const glm::vec2& p2, const glm::vec2& p3,Shader* shader)
{
  static uint triVAO,triVBO;
  const glm::vec2 src1(-1,-1);
  const glm::vec2 src2(0,1);
  const glm::vec2 src3(1.0f,-1.0f);
  if (triVAO == 0)
  {
      float triVertices[] = {
          // positions        // texture Coords
           src1.x, src1.y, 0.0f, 0.0f,
           src2.x, src2.y, 0.5f, 1.0f,
           src3.x, src3.y, 1.0f, 0.0f,
      };
      // setup plane VAO
      glGenVertexArrays(1, &triVAO);
      glGenBuffers(1, &triVBO);
      glBindVertexArray(triVAO);
      glBindBuffer(GL_ARRAY_BUFFER, triVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), &triVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  }
  glm::mat3 target =
  {
    p1.x,p1.y,1,
    p2.x,p2.y,1,
    p3.x,p3.y,1
  };
  const glm::mat3 src =
  {
    src1.x,src1.y,1,
    src2.x,src2.y,1,
    src3.x,src3.y,1
  };
  const glm::mat3 invSrc = glm::inverse(src);

  glm::mat3 model = target*invSrc;
  GUIShader2D.use();
  GUIShader2D.setMat3("model",model);

  glViewport(0,0,dimensions.x,dimensions.y);
  glBindVertexArray(triVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
  glBindVertexArray(0);
}
void GUI::drawImage(const glm::vec2& botLeft, const glm::vec2& topRight, const uint id)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);
  drawQuad(botLeft,topRight,glm::vec4(1),DEFAULTQUAD,&GUIShaderImage);
}

void GUI::drawImage(const glm::mat3& model, const uint id)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);
  drawQuad(model,glm::vec4(1),DEFAULTQUAD,&GUIShaderImage);
}

void GUI::drawCircle(const glm::vec2& origin, const double radius, const double border)
{
  float radius_squared = pow(radius,4);
  float max_distance = pow(border+radius,2);
  float fall_off_distance = border*border;
  float borderFloat = sqrt(pow(radius+border,2)/radius_squared);

  Shader* shader = &GUIShaderCircle;
  shader->setVec2("origin",origin);
  shader->setVec2("radius_squared",glm::vec2(radius_squared));
  shader->setVec2("max_distance",glm::vec2(max_distance));
  shader->setFloat("border_float",borderFloat);
  glm::vec2 upper = origin + glm::vec2(radius+border);
  glm::vec2 lower = origin - glm::vec2(radius+border);
  drawQuad(lower,upper,glm::vec4(1),DEFAULTQUAD,shader);

}

void GUI::GLFWKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(focusTarget == NULL) return;
  focusTarget->handleKeyInput(key,action);
}

void GUI::GLFWCharCallBack(GLFWwindow* window, uint character)
{
  if(focusTarget == NULL) return;
  focusTarget->handleCharInput(character);
}

void GUI::GLFWCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
  mousePos = glm::vec2(xpos/dimensions.x,1-ypos/dimensions.y);


}
void GUI::handleMouseHover()
{
  for(auto itr = viewableList.begin();itr != viewableList.end();itr++)
  {
    Widget* widget = (*itr);
    if(widget->isIn(mousePos))
    {
      widget->handleMouseHover(mousePos);
    }
  }
}


void GUI::GLFWMouseButtonCallback(GLFWwindow* window, int button,int action, int mods)
{
  for(auto itr = viewableList.begin();itr != viewableList.end();itr++)
  {
    Widget* widget = (*itr);
    if(widget->isIn(mousePos))
    {

      widget->handleMouseInput(button,action);
      if(widget->isFocusable())
      {
        if(focusTarget != NULL)
        {
          focusTarget->setFocused(false);
        }
        focusTarget = widget;
        widget->setFocused(true);
      }
    }
  }
}

void GUI::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(focusTarget == NULL) return;
  focusTarget->handleScrollInput(xoffset,yoffset);
}


/*
void GUI::drawEllipse(const glm::vec2 origin, const double radiusx, const double radiusy)
{
  float radius_squared = radius*radius;
  float max_distance = pow(border+radius,2);
  float fall_off_distance = border*border;
  float borderFloat = sqrt(pow(radius+border,2)/radius_squared);
  //std::cout << "borderFloat:  " << borderFloat <<"\n";

  Shader* shader = &GUIShaderCircle;
  shader->setVec2("origin",origin);
  shader->setVec2("radius_squared",glm::vec2(radius_squared));
  shader->setFloat("border_float",borderFloat);
  //shader->setFloat("max_distance",max_distance);
  glm::vec2 upper = origin + glm::vec2(radius+border);
  glm::vec2 lower = origin - glm::vec2(radius+border);
  drawQuad(lower,upper,shader);
}
*/
