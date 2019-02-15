#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "shaders.h"
#include "gui.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


glm::ivec2 GUI::dimensions;
Shader GUI::GUIShader2D,GUI::GUIShaderCircle;
void GUI::initGUI(const glm::ivec2 Dimensions)
{
  dimensions = Dimensions;
  GUIShader2D = Shader("../src/guiShader2D.vs","../src/guiShader2D.fs");
  GUIShaderCircle = Shader("../src/guiCircleShader.vs","../src/guiCircleShader.fs");
  glm::mat4 projection = glm::ortho(0.0f,1.0f,0.0f,1.0f);
  GUIShader2D.use();
  GUIShader2D.setMat4("projection",projection);

  GUIShaderCircle.use();
  GUIShaderCircle.setMat4("projection",projection);
}



void GUI::drawQuad(glm::vec2 botLeft,glm::vec2 topRight,Shader* shader)
{
  static uint quadVAO,quadVBO;
  if (quadVAO == 0)
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

  shader->use();
  shader->setMat3("model",model);

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void GUI::drawQuad(const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3 ,const glm::vec2 p4,Shader* shader)
{
  drawTriangle(p1,p2,p3,shader);
  drawTriangle(p1,p3,p4,shader);
}



void GUI::drawTriangle(const glm::vec2 p1,const glm::vec2 p2, const glm::vec2 p3,Shader* shader)
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

  glBindVertexArray(triVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
  glBindVertexArray(0);
}

void GUI::drawCircle(const glm::vec2 origin, const double radius, const double border)
{
  Shader* shader = &GUIShaderCircle;
  shader->setVec2("origin",origin);
  shader->setFloat("radius",radius);
  shader->setFloat("fall_off_distance",border);
  //shader = &GUIShader2D;
  glm::vec2 upper = origin + glm::vec2(radius+border);
  glm::vec2 lower = origin - glm::vec2(radius+border);
  drawQuad(lower,upper,shader);

}
