#ifndef GUILIBRARY
#define GUILIBRARY

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <list>
#include <GLFW/glfw3.h>
#include "Widgets/textrenderer.h"
class Shader;


class Widget;



class GUI
{
private:
  static glm::vec2 mousePos;
  static Widget* focusTarget;
  static std::vector<Widget*> widgetList;
  static std::list<Widget*> viewableList;
  static Shader GUIShader2D,GUIShaderCircle,GUIShaderImage;

public:
  static TextRenderer textRenderer;
  static void addToViewableList(Widget* widget){viewableList.push_back(widget);}
  static glm::ivec2 dimensions;
  static void renderText(std::string text, glm::vec2 pos, float scale=1, glm::vec4 color=glm::vec4(1)){textRenderer.renderText(text,pos,scale,color);}
  static glm::vec3 calculateStringDimensions(const std::string &line, double scale){return textRenderer.calculateStringDimensions(line,scale);}
  static void initGUI(const glm::ivec2 Dimensions);
  static void freeGUI();
  static void initQuadVAO();

  static glm::mat3 calculateQuadModel(const glm::vec2 botLeft, const glm::vec2 topRight);
  static void drawImage(const glm::vec2 botLeft, const glm::vec2 topRight, const uint id);
  static void drawImage(const glm::mat3& model, const uint id);

  static void drawInstancedQuads(int count);
  static void drawQuad(const glm::mat3&  model,const glm::vec4 color, Shader* shader = &GUIShader2D);
  static void drawQuad(const glm::vec2 botLeft,const glm::vec2 topRight,const glm::vec4 color, Shader* shader = &GUIShader2D);
  static void drawQuad(const glm::vec2 p1,const glm::vec2 p2, const glm::vec2 p3, const glm::vec2 p4,Shader* shader = &GUIShader2D);
  static void drawTriangle(const glm::vec2 p1,const glm::vec2 p2,const glm::vec2 p3, Shader* shader = &GUIShader2D);
  static void drawCircle(const glm::vec2 origin, const double radius, const double border);
  static void drawEllipse(const glm::vec2 origin, const double radiusx, const double radiusy,const double border);
  static void drawGUI();
  static uint addWidget(Widget* widget){widgetList.push_back(widget);return widgetList.size()-1;}
  static Widget* getWidget(uint id){return widgetList[id];}
  static void GLFWKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
  static void GLFWCharCallBack(GLFWwindow* window, uint character);
  static void GLFWCursorCallback(GLFWwindow* window, double xpos, double ypos);
  static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
  static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  static void handleMouseHover();
  static Widget* findWidgetAtMouse();
};
#ifdef GUILIBRARYIMPLEMENTATION
glm::vec2 GUI::mousePos;
glm::ivec2 GUI::dimensions;
Shader GUI::GUIShader2D,GUI::GUIShaderCircle,GUI::GUIShaderImage;

std::list<Widget*> GUI::viewableList;
Widget* GUI::focusTarget;
std::vector<Widget*> GUI::widgetList;
TextRenderer GUI::textRenderer;
#endif



#endif
