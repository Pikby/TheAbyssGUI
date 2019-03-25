#include <glm/glm.hpp>
#include <string>
#include <list>
#include <iostream>
#include <vector>
#include <functional>

class Widget
{
protected:
  bool focusable=true;
  bool focused=false;
  glm::vec2 origin=glm::vec2(0);
  glm::vec2 dimensions=glm::vec2(0);
  double padding = 0.01;
  double size;
public:
  Widget(){}
  Widget(glm::vec2 Origin,glm::vec2 Dimensions = glm::vec2(0)): origin(Origin),dimensions(Dimensions){}
  virtual bool isIn(const glm::vec2 pos)
  {
    glm::vec2 bottomLeft = origin-glm::vec2(padding);
    glm::vec2 topRight = origin+dimensions+glm::vec2(padding);
    bool horzIn = pos.x > bottomLeft.x && pos.x < topRight.x;
    bool vertIn = pos.y > bottomLeft.y && pos.y < topRight.y;
    return horzIn && vertIn;
  }
  virtual bool isFocusable(){return focusable;}
  virtual void setFocused(bool b){focused = b;}
  virtual void draw(){};
  virtual void handleClick(){};
  virtual void handleKeyInput(int key,int action){}
  virtual void handleCharInput(uint character){}
  virtual void handleScrollInput(double xoffset,double yoffset){}
  virtual void handleMouseHover(const glm::vec2& mousePos){};
  virtual void handleMouseInput(int button,int action){};
};

class EditBox : public Widget
{
private:
  std::string text;
  double characterScale;
  int cursorPosition=0;
public:
  EditBox(){};
  EditBox(const std::string& text,const glm::vec2& Origin,const glm::vec2& dims,double CharacterScale);

  void draw() override;
  void handleCharInput(uint character) override;
  void handleKeyInput(int key,int action) override;
};

class Label : public Widget
{
private:
  std::string label;
  double characterScale;
public:
  Label(){};
  Label(const std::string& text,const glm::vec2& Origin,double CharacterScale);

  void draw() override;
};

class Button : public Widget
{
private:
  glm::vec2 textOrigin;
  double textScale;
  double pressedTime;
  std::string text;
  std::function<void(int)> clickHandler;
public:
  Button(const glm::vec2& Origin, const glm::vec2& Dimensions,const std::string& Text,std::function<void(int)> ClickHandler = std::function<void(int)>(NULL));
  ~Button(){};

  void draw() override;
  void handleMouseInput(int button, int action) override;

};

class ChatBox : public Widget
{
private:
  unsigned char lineLength=32;
  double characterScale=0.5f;
  double extendTime;
  int cursorPosition=0;
  std::string currentLine;
  std::list<std::string> history;
  std::list<std::string>::iterator focusTarget;
public:
  ChatBox(const glm::vec2& origin,const glm::vec2& dimensions,double size);
  std::string getInputLine(){return currentLine;}
  void extendFor(double seconds);
  void updateInputLine(const std::string &line){currentLine = line;}
  void addLineToHistory(const std::string &line);

  void handleCharInput(uint character) override;
  void handleKeyInput(int key,int action) override;
  void handleScrollInput(double xoffset,double yoffset) override;
  void draw() override;
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

  void setImage(int id,uint texture){textureArray[id] = texture;}
  void handleMouseHover(const glm::vec2& mousePos);

  bool isIn(const glm::vec2 pos) override;
  void draw() override;
};
