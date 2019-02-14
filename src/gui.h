#include <glm/glm.hpp>
class Shader;


class GUI
{
private:
  static glm::ivec2 dimensions;
  static Shader GUIShader2D;


public:
  static void initGUI(const glm::ivec2 Dimensions);
  static void drawQuad(const glm::vec2 botLeft,const glm::vec2 topRight);
  static void drawTriangle(const glm::vec2 p1,const glm::vec2 p2,const glm::vec2 p3);
};
