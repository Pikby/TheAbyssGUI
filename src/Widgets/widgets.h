#include <glm/glm.hpp>
#include <string>
#include <list>
#include <iostream>
#include <vector>

class Widget
{
protected:
  bool focusable;
  bool focused;
  glm::vec2 origin;
  glm::vec2 dimensions;
  double padding;
  double size;
public:
  Widget(){}
  Widget(glm::vec2 Origin,glm::vec2 Dimensions = glm::vec2(0)): origin(Origin),dimensions(Dimensions){}
  virtual bool isIn(const glm::vec2 pos)
  {
    glm::vec2 bottomLeft = origin-glm::vec2(padding);
    glm::vec2 topRight = origin+dimensions+glm::vec2(padding);
    //std::cout << topRight.x << ',' <<  topRight.y << ':' << bottomLeft.x << ',' << bottomLeft.y << "\n";
    bool horzIn = pos.x > bottomLeft.x && pos.x < topRight.x;
    bool vertIn = pos.y > bottomLeft.y && pos.y < topRight.y;
    //std::cout << horzIn << ":" << vertIn << "\n";
    return horzIn && vertIn;
  }
  virtual bool isFocusable(){return focusable;}
  virtual void setFocused(bool b){focused = b;}
  virtual void draw(){};
  virtual void handleKeyInput(int key,int action){}
  virtual void handleCharInput(uint character){}
  virtual void handleScrollInput(double xoffset,double yoffset){}
  virtual void handleMouseHover(const glm::vec2& mousePos){};
};


class StaticLabel : public Widget
{
private:
  std::string label;
  uint FBO=0,textureID=0;
  double characterScale;
  glm::mat4 model;
public:
  StaticLabel(){};
  ~StaticLabel();
  StaticLabel(const std::string& text,const glm::vec2& Origin,double CharacterScale);
  void draw();
};


class ChatBox : public Widget
{
private:
  unsigned char lineLength=32;
  double characterScale=0.5f;
  double extendTime;
  std::string currentLine;
  std::list<std::string> history;
  std::list<std::string>::iterator focusTarget;
public:
  ChatBox(glm::vec2 origin,glm::vec2 dimensions,double size);
  void draw();
  std::string getInputLine(){return currentLine;}
  void extendFor(double seconds);
  void updateInputLine(std::string line){currentLine = line;}
  void addLineToHistory(std::string line);
  void handleCharInput(uint character);
  void handleKeyInput(int key,int action);
  void handleScrollInput(double xoffset,double yoffset);

};

enum CellType{SQUAREIMAGECELL,ROUNDIMAGECELL,TEXTIMAGECELL};


class ImageGrid : public Widget
{
private:
  int rows;
  int columns;
  glm::vec2 cellDimensions;
  glm::vec2 cellOffset;
  uint* textureArray;
  uint getCellTexture(int x,int y){return textureArray[x+y*columns];}
public:
 ImageGrid(glm::vec2 Origin,int Rows, int Cols,glm::vec2 CellDimensions, glm::vec2 CellOffset);
  void draw();
  void setImage(int id,uint texture){textureArray[id] = texture;}
  void handleMouseHover(const glm::vec2& mousePos);
  bool isIn(const glm::vec2 pos);
};
