

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
#include "../textureloading.h"

#include "../shaders.h"
#include "../gui.h"


const Point inside = { 0, 0 };
const Point empty = { 9999, 9999 };


Point TextRenderer::Get( Grid &g, int x, int y )
{
	// OPTIMIZATION: you can skip the edge check code if you make your grid
	// have a 1-pixel gutter.
	if ( x >= 0 && y >= 0 && x < atlasDimensions.x && y < atlasDimensions.y )
		return g.grid[y][x];
	else
		return empty;
}

void TextRenderer::Put( Grid &g, int x, int y, const Point &p )
{
	//std::cout << x << ":" << y << "\n";
	g.grid[y][x] = p;
}

void TextRenderer::Compare( Grid &g, Point &p, int x, int y, int offsetx, int offsety )
{
	Point other = Get( g, x+offsetx, y+offsety );
	other.dx += offsetx;
	other.dy += offsety;

	if (other.DistSq() < p.DistSq())
		p = other;
}

void TextRenderer::GenerateSDF( Grid &g )
{
	// Pass 0
	for (int y=0;y<atlasDimensions.y;y++)
	{
		for (int x=0;x<atlasDimensions.x;x++)
		{
			Point p = Get( g, x, y );
			Compare( g, p, x, y, -1,  0 );
			Compare( g, p, x, y,  0, -1 );
			Compare( g, p, x, y, -1, -1 );
			Compare( g, p, x, y,  1, -1 );
			Put( g, x, y, p );
		}

		for (int x=atlasDimensions.x-1;x>=0;x--)
		{
			Point p = Get( g, x, y );
			Compare( g, p, x, y, 1, 0 );
			Put( g, x, y, p );
		}
	}

	// Pass 1
	for (int y=atlasDimensions.y-1;y>=0;y--)
	{
		for (int x=atlasDimensions.x-1;x>=0;x--)
		{
			Point p = Get( g, x, y );
			Compare( g, p, x, y,  1,  0 );
			Compare( g, p, x, y,  0,  1 );
			Compare( g, p, x, y, -1,  1 );
			Compare( g, p, x, y,  1,  1 );
			Put( g, x, y, p );
		}

		for (int x=0;x<atlasDimensions.x;x++)
		{
			Point p = Get( g, x, y );
			Compare( g, p, x, y, -1, 0 );
			Put( g, x, y, p );
		}
	}
}



void TextRenderer::loadTextAtlas(const FT_Face &face, int fontSize,int level)
{
   FT_Set_Pixel_Sizes(face,0,fontSize);

	 int glyphSize = fontSize + fontSize/4;


   uint w = 0;
   uint h = 0;
   for(int i = 32; i < 128; i++)
   {
     if(FT_Load_Char(face, i, FT_LOAD_RENDER))
     {
       fprintf(stderr, "Loading character %c failed!\n", i);
       continue;
     }


     w += face->glyph->bitmap.width+5;
     h = std::max(h, face->glyph->bitmap.rows);
   }
   if(level == 0 )
   {
     atlasDimensions =  glm::vec2(w+3,h);
   }

	 atlasDimensions =  13.0f*glm::vec2(glyphSize);


 	char zeroImage[int(atlasDimensions.x*atlasDimensions.y)];
  memset(zeroImage,0,atlasDimensions.x*atlasDimensions.y);

  glTexImage2D(GL_TEXTURE_2D, level, GL_RED, atlasDimensions.x, atlasDimensions.y, 0, GL_RED, GL_UNSIGNED_BYTE, zeroImage);
  int x = fontSize;
	int y = fontSize;
  for(int i = 32; i < 128; i++)
  {
    if(FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;
    glTexSubImage2D(GL_TEXTURE_2D, level, x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		int padding = glyphSize/3;
    Character character =
    {
      glm::vec2(face->glyph->bitmap.width+padding, face->glyph->bitmap.rows+padding),
      glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      (unsigned int)face->glyph->advance.x,(x-padding/2)/atlasDimensions.x,(y-padding/2)/atlasDimensions.y

    };
    x += glyphSize;
		if(x>= atlasDimensions.x)
		{
			x = glyphSize;
			y += glyphSize;
		}

    if(level == 0)
    {
      characters.insert(std::pair<char, Character>(i, character));
      GUIShaderText.setVec2("characters[" + std::to_string(i) + "].size",character.size/atlasDimensions);
      GUIShaderText.setVec2("characters[" + std::to_string(i) + "].bearing",character.bearing/atlasDimensions);
      GUIShaderText.setUInt("characters[" + std::to_string(i) + "].advance",character.advance);
      GUIShaderText.setFloat("characters[" + std::to_string(i) + "].xstart",character.xstart);
			GUIShaderText.setFloat("characters[" + std::to_string(i) + "].ystart",character.ystart);

    }


  }
}


void TextRenderer::init()
{
   GUIShaderText = Shader("guiTextShader.vs","guiTextShader.fs");
   std::cout << GUI::dimensions.x << ":" << GUI::dimensions.y << "\n";
   glm::mat4 projection = glm::ortho(0, GUI::dimensions.x, 0,  GUI::dimensions.y);
   GUIShaderText.use();
   GUIShaderText.setMat4("projection",projection);

   FT_Library ft;
   if (FT_Init_FreeType(&ft))
       std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
   FT_Face face;
   if (FT_New_Face(ft, "../arial.ttf", 0, &face))
          std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;


   std::cout << atlasWidth << ":" <<atlastHeight << "\n";
   glActiveTexture(GL_TEXTURE0);
   glGenTextures(1,&textAtlas);
   glBindTexture(GL_TEXTURE_2D, textAtlas);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



   glGenerateMipmap(GL_TEXTURE_2D);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);


   loadTextAtlas(face,64,0);
   //loadTextAtlas(face,32,1);
   //loadTextAtlas(face,16,2);
   //loadTextAtlas(face,8,3);

	 std::cout <<"dims:" << atlasDimensions.x << ":" << atlasDimensions.y << "\n";

   unsigned char image[int((atlasDimensions.x)*atlasDimensions.y)];


   glGetTexImage(GL_TEXTURE_2D,0,GL_RED,GL_UNSIGNED_BYTE,image);
   saveTexture("atlas.bmp",atlasDimensions.x,atlasDimensions.y,image);
   Grid Grid1(atlasDimensions.y,atlasDimensions.x);
   Grid Grid2(atlasDimensions.y,atlasDimensions.x);



   for( int y=0;y<atlasDimensions.y;y++ )
   {
     for ( int x=0;x<atlasDimensions.x;x++ )
     {
       unsigned char val = image[int(x+y*atlasDimensions.x)];
       //std::cout << (int)val << "\n";
       if(val < 128)
       {
 				Put( Grid1, x, y, inside );
 				Put( Grid2, x, y, empty );
 			} else {
 				Put( Grid2, x, y, inside );
 				Put( Grid1, x, y, empty );
 			}
      //std::cout << Get(Grid1,x,y).dx << "," <<Get(Grid1,x,y).dy << ":" << Get(Grid2,x,y).dx << "," << Get(Grid1,x,y).dy << "\n";;
    }
  }
   GenerateSDF(Grid1);
   GenerateSDF(Grid2);
   int maxDistance = -99;
   int minDistance = 99;

   for( int y=0;y<atlasDimensions.y;y++ )
   {
     for ( int x=0;x<atlasDimensions.x;x++ )
     {
     int dist1 = (int)( sqrt( (double)Get( Grid1, x, y ).DistSq() ) );
     int dist2 = (int)( sqrt( (double)Get( Grid2, x, y ).DistSq() ) );
     int dist = dist1 - dist2;

          int c = (dist+11)*21;
          maxDistance = std::max(dist,maxDistance);
          minDistance =std::min(dist,minDistance);
          if ( c < 0 ) c = 0;
          if ( c > 255 ) c = 255;

    image[int(x+y*atlasDimensions.x)]=c;
    }
  }



	int factor = 4;

	unsigned char newImage[int(atlasDimensions.x/factor*atlasDimensions.y/factor)];
	for(int x = 0;x<atlasDimensions.x/factor;x++ )
	{
		for(int y = 0;y<atlasDimensions.y/factor;y++)
		{
			int total = image[int((x)*factor+(y)*factor*atlasDimensions.x)] +image[int((x+1)*factor+(y)*factor*atlasDimensions.x)]
								+ image[int((x)*factor+(y+1)*factor*atlasDimensions.x)] +image[int((x+1)*factor+(y+1)*factor*atlasDimensions.x)];

			newImage[int(x+y*atlasDimensions.x/factor)] = total/4;
		}
	}


    //std::cout << maxDistance<< ":" << minDistance <<"\n";
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasDimensions.x/factor,atlasDimensions.y/factor, 0, GL_RED, GL_UNSIGNED_BYTE, newImage);

   glGenerateMipmap(GL_TEXTURE_2D);
   saveTexture("signedDistanceFieldsmall.bmp",atlasDimensions.x/factor,atlasDimensions.y/factor,newImage);
   for(int y = 0;y<atlasDimensions.y;y++)
   {
     for(int x=0; x<atlasDimensions.x/4;x++)
     {
       //std::cout << signedDistanceField[int(x+y*(atlasDimensions.x+1))]<< ',';
     }
     //std::cout << "\n";
   }

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

void TextRenderer::updateTextBuffer(float newBuf)
{
  if(newBuf > 1.0f) textBuffer = 1.0f;
  else if(newBuf < 0.0f) textBuffer = 0.0f;
  else textBuffer = newBuf;

  GUIShaderText.setFloat("buf",textBuffer);
}
void TextRenderer::updateTextGamma(float newGamma)
{
  if(newGamma > 1.0f) textGamma = 1.0f;
  else if(newGamma < 0.0f) textGamma = 0.0f;
  else textGamma = newGamma;
  GUIShaderText.setFloat("gamma",textGamma);
}
