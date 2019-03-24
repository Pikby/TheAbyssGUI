#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define FREETYPE_GL_USE_VAO
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/transform.hpp>
#include "gui.h"
#include "Widgets/widgets.h"
#include "menus.h"
void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  if(severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_NOTIFICATION)
  {
    return;
  }
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

int main()
{

	glfwInit();
  int winWidth = 1600;
  int winHeight = 900;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


  GLFWwindow* window = glfwCreateWindow(winWidth,winHeight,"Gui Testing",NULL,NULL);
 	glfwSetKeyCallback(window, GUI::GLFWKeyCallBack);
	glfwSetCharCallback(window, GUI::GLFWCharCallBack);
	glfwSetCursorPosCallback(window, GUI::GLFWCursorCallback);
	glfwSetMouseButtonCallback(window, GUI::GLFWMouseButtonCallback);
	glfwSetScrollCallback(window, GUI::GLFWScrollCallback);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
	glewExperimental = GL_TRUE;
 	glewInit();
  glEnable              ( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( MessageCallback, 0 );
	//glViewport(0, 0, winWidth, winHeight);

	GUI::initGUI(glm::vec2(winWidth,winHeight));
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  /*
	ChatBox* chatBox = new ChatBox(glm::vec2(0.1,0.1),glm::vec2(0.2,0.2),12.0/64.0);
	ChatBox* chatBox2 = new ChatBox(glm::vec2(0.6,0.1),glm::vec2(0.2,0.2),1);
	chatBox->addLineToHistory("Line1");
  chatBox->addLineToHistory("Line2");
	chatBox->addLineToHistory("Line3");
  chatBox->addLineToHistory("Line4");
	chatBox->addLineToHistory("Line5");
	chatBox->addLineToHistory("Line6");
  StaticLabel* label = new StaticLabel("The quick brown fox jumps over the lazy dog",glm::vec2(0.3,0.5),1);
	GUI::addWidget(chatBox);
	GUI::addToViewableList(chatBox);
	GUI::addWidget(chatBox2);

  auto testButton = [&](int a)
  {
    static bool visible = true;
    static std::list<Widget*>::iterator itr;
    if(visible)
    {
      itr = GUI::addToViewableList(chatBox2);
      visible = false;
    }
    else
    {
      GUI::removeFromViewableList(itr);
      visible = true;
    }
  };
  Button* button = new Button(glm::vec2(0.5,0.5),glm::vec2(0.1),"Test Button",testButton);

  GUI::addWidget(button);
  GUI::addToViewableList(button);




  ImageGrid* grid = new ImageGrid(glm::vec2(0,0.5),3,5,glm::vec2(0.05,0.05),glm::vec2(0.01,0.01));
  GUI::addWidget(grid);
	GUI::addToViewableList(grid);

  //for(int i=0;i<100;i++)
  {
    //labelArr[i] = StaticLabel("Static Drawing Test",glm::vec2((rand() %1000)/1000.f,(rand() %1000)/1000.f),1);
  }
  */
  Menu mainMenu;
  Menu subMenu;
  GUI::setMenu(&mainMenu);
  ChatBox* chatBox = new ChatBox(glm::vec2(0.1,0.1),glm::vec2(0.2,0.2),12.0/64.0);
  Label* label1 = new Label("MainMenu",glm::vec2(0.8,0.8),0.5);
  mainMenu.addWidget(chatBox);
  mainMenu.addWidget(label1);

  Button* button1 =  new Button(glm::vec2(0.5),glm::vec2(0.1),"Main Menu Button",[&](int a)
  {
    GUI::setMenu(&subMenu);
  });
  mainMenu.addWidget(button1);
  Label* label2 = new Label("SubMenu",glm::vec2(0.8,0.8),0.5);
  subMenu.addWidget(label2);
  Button* button2 =  new Button(glm::vec2(0.5),glm::vec2(0.1),"Sub Menu Button",[&](int a)
  {
    GUI::setMenu(&mainMenu);
  });
  subMenu.addWidget(button2);

	while(!glfwWindowShouldClose(window))
	{
    double start = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f,0.7f,0.7f,1.0f);
		glfwPollEvents();
    GUI::handleMouseHover();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		//GUI::drawCircle(glm::vec2(0.5,0.5),0.1,0.0);
		//GUI::drawCircle(glm::vec2(0.3,0.3),0.1,0.0);
    for(int i =0;i<1000;i++)
    {
      //GUI::renderText("Dynamic drawing test",glm::vec2((rand() %1000)/1000.f,(rand() %1000)/1000.f),1,glm::vec4(1));
      //labelArr[i].draw();
    }


    static float angle = 0;
    angle+= 0.0001;
    glm::mat3 rot = glm::rotate(glm::mat3(1.0f),angle);
    GUI::renderText("Rotating text",glm::vec2(0.5,0.3),12.0/64.0,glm::vec4(1),rot,TEXTALICENTER);
    const double vals[] = {0.1,0.1,0.2,0.3,0.4,0.5,1.0,2.0,3.0,4.0};
    for(int i=0;i<10;+i++)
    {
      // /GUI::renderText("The quick brown fox jumps over the lazy dog123456789"+std::to_string(vals[i]),glm::vec2(0.3,i*0.1),vals[i],glm::vec4(1));
    }
		GUI::drawGUI();
    GUI::textRenderer.drawAllText();

    double end = glfwGetTime();

    GUI::renderText("FPS: " + std::to_string(1.0f/(end-start)),glm::vec2(0.1,0.95),24.0/64.0,glm::vec4(glm::vec3(0),1));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,1);

    GUI::drawQuad(glm::vec2(0.1,0.8),glm::vec2(0.2,0.9),glm::vec4(1),DEFAULTQUAD);
    GUI::drawQuad(glm::vec2(0.3,0.8),glm::vec2(0.4,0.9),glm::vec4(1),ROUNDEDQUAD);
    GUI::drawQuad(glm::vec2(0.5,0.8),glm::vec2(0.6,0.9),glm::vec4(1),INVERTEDQUAD);
    GUI::drawQuad(glm::vec2(0.7,0.8),glm::vec2(0.8,0.9),glm::vec4(1),CIRCLEQUAD);
		glfwSwapBuffers(window);

    //std::cout << (end-start)*1000 << "ms\n";
	}
	GUI::freeGUI();
}
