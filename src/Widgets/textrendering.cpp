

// Std. Includes
#include <iostream>
#include <map>
#include <string>
// GLEW
#include <algorithm>
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
// GL includes

#include "../shaders.h"
#include "../gui.h"



//void TextRenderer::loadFont()


void TextRenderer::init()
{
   GUIShaderText = Shader("guiTextShader.vs","guiTextShader.fs");
   std::cout << GUI::dimensions.x << ":" << GUI::dimensions.y << "\n";
   glm::mat4 projection = glm::ortho(0, GUI::dimensions.x, 0,  GUI::dimensions.y);
   GUIShaderText.use();
   GUIShaderText.setMat4("projection",projection);

   std::cout << "stuff\n";
   FT_Library ft;
   if (FT_Init_FreeType(&ft))
       std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
   FT_Face face;
   if (FT_New_Face(ft, "../arial.ttf", 0, &face))
          std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

   FT_Set_Pixel_Sizes(face,0,12);


   uint w = 0;
   uint h = 0;
   for(int i = 32; i < 128; i++)
   {
     if(FT_Load_Char(face, i, FT_LOAD_RENDER))
     {
       fprintf(stderr, "Loading character %c failed!\n", i);
       continue;
     }


     w += face->glyph->bitmap.width;
     h = std::max(h, face->glyph->bitmap.rows);
   }
   atlasDimensions =  glm::vec2(w,h);

   std::cout << atlasWidth << ":" <<atlastHeight << "\n";
   glActiveTexture(GL_TEXTURE0);
   glGenTextures(1,&textAtlas);
   glBindTexture(GL_TEXTURE_2D, textAtlas);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   unsigned char arr[w*h];
   for(int i=0;i<w*h;i++)
   {
     arr[i] = rand()%255;
   }
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


   int x = 0;

   for(int i = 32; i < 128; i++)
   {
     if(FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;
     glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

     Character character =
     {
       glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
       glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
       (unsigned int)face->glyph->advance.x,x/atlasDimensions.x,

     };
     x += face->glyph->bitmap.width;
     characters.insert(std::pair<char, Character>(i, character));
     GUIShaderText.setVec2("characters[" + std::to_string(i) + "].size",character.size/atlasDimensions);
     GUIShaderText.setVec2("characters[" + std::to_string(i) + "].bearing",character.bearing/atlasDimensions);
     GUIShaderText.setUInt("characters[" + std::to_string(i) + "].advance",character.advance);
     GUIShaderText.setFloat("characters[" + std::to_string(i) + "].xstart",character.xstart);

   }

   std::cout << textAtlas;
   glBindTexture(GL_TEXTURE_2D,0);
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER,VBO);
   glBufferData(GL_ARRAY_BUFFER,characterVertices.size()*sizeof(float)*6*4,&(characterVertices.front()), GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE,4*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   FT_Done_Face(face);
   FT_Done_FreeType(ft);

}

void TextRenderer::renderText(std::string text, glm::vec2 screenPos, GLfloat scale, glm::vec4 color)
{
  glm:: vec2 pos = screenPos*glm::vec2(GUI::dimensions);
  std::string::const_iterator c;

  unsigned char r = (int)(color.r*255);
  unsigned char g = (int)(color.g*255);
  unsigned char b = (int)(color.b*255);
  unsigned char a = (int)(color.a*255);


  int packedColor = r | (g << 8) | (b << 16) | (a << 24);


  for (c = text.begin(); c != text.end(); c++)
  {
      //GUIShaderText.setInt("currentCharacter",*c);
      Character ch = characters[*c];

      float xpos = pos.x + ch.bearing.x * scale;
      float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

      float w = ch.size.x * scale;
      float h = ch.size.y * scale;

      auto toFloat = [](int a)
      {
        return *(float*)&a;
      };
      CharacterVertex vert =
      {
        glm::vec4(xpos, ypos+h,   toFloat(00 | (*c << 8)),toFloat(packedColor)),
        glm::vec4(xpos, ypos,     toFloat(01 | (*c << 8)),toFloat(packedColor)),
        glm::vec4(xpos+w,ypos,    toFloat(11 | (*c << 8)),toFloat(packedColor)),

        glm::vec4(xpos, ypos+h,   toFloat(00 | (*c << 8)),toFloat(packedColor)),
        glm::vec4(xpos+w,ypos,    toFloat(11 | (*c << 8)),toFloat(packedColor)),
        glm::vec4(xpos+w,ypos+h , toFloat(10 | (*c << 8)),toFloat(packedColor)),
      };
      characterVertices.push_back(vert);
      pos.x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
  }
}

void TextRenderer::drawAllText()
{

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,textAtlas);
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER,VBO);
   glBufferData(GL_ARRAY_BUFFER,characterVertices.size()*6*4*sizeof(float),&(characterVertices.front()), GL_DYNAMIC_DRAW);
   glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE,4*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);





   glViewport(0,0,GUI::dimensions.x,GUI::dimensions.y);
   GUIShaderText.use();
   glm::mat4 projection = glm::ortho(0.0f, (float)GUI::dimensions.x, 0.0f,  (float)GUI::dimensions.y);
   GUIShaderText.setMat4("projection",projection);

   glDrawArrays(GL_TRIANGLES, 0, characterVertices.size()*6);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   characterVertices.clear();

   static int lastSize = 10;
   characterVertices.reserve(lastSize);
   lastSize = characterVertices.size();

}

glm::vec3 TextRenderer::calculateStringDimensions(const std::string& line,double scale)
{
  double horizontalLength = 0;
  double offY = -std::numeric_limits<double>::max();
  double maxY = -std::numeric_limits<double>::max();
  for(auto itr = line.begin();itr!=line.end();itr++)
  {
    Character c = characters[*itr];
    horizontalLength += (c.advance>>6) * scale;
    double ypos = (c.size.y - c.bearing.y) * scale;
    //std::cout << ypos << "\n";
    offY = std::max(offY,ypos);
    maxY = std::max(maxY,ypos+c.size.y * scale);
    //std::cout << (char)(*itr) << "is: " <<  (c.advance>>6) * scale << "\n";
  }
  std::cout << horizontalLength << ":" << offY << ":" << maxY << "\n";
  // /std::cout << "minmax"<<maxY << ":" << minY << "\n";
  return glm::vec3(horizontalLength,offY,maxY);
}

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
