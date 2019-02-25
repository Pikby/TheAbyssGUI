
#include "../shaders.h"


struct Character {
    glm::vec2 size;    // Size of glyph
    glm::vec2 bearing;  // Offset from baseline to left/top of glyph
    uint advance;    // Horizontal offset to advance to next glyph
    float xstart;
};




struct CharacterVertex
{
  glm::vec4 vertices[6];
};


class TextRenderer
{
private:
  uint VAO=0, VBO=0;
  uint textAtlas;
  uint atlasWidth,atlastHeight;
  glm::vec2 atlasDimensions;
  std::map<char, Character> characters;
  std::vector<CharacterVertex> characterVertices;
public:
  Shader GUIShaderText;
  void init();
  void drawAllText();
  void renderText(std::string text, glm::vec2 screenPos, GLfloat scale, glm::vec4 color);
  glm::vec3 calculateStringDimensions(const std::string& line,double scale);
};
/*
std::vector<std::string> GUI::splitString(const std::string& line, double scale, double width)
{
  uint horizontalLength = 0;
  uint stringStart = 0;
  uint stringEnd = 0;
  std::vector<std::string> stringList;
  for(auto itr = line.begin();itr!=line.end();itr++)
  {
    Character c = characters[*itr];
    std::cout << (*itr) << "is: " << (c.advance>>6) << "\n";
    horizontalLength += (c.advance>>6) * scale;
    if(horizontalLength/dimensions.x > width)
    {
      stringList.push_back(line.substr(stringStart,stringEnd))
      stringStart = stringEnd;
    }
    stringEnd++;
    return stringList;
  }

}
*/
