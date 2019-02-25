GUIOBJECTPATH = ../bin/gui/
GUISRCPATH = ../src/

WIDGETOBJECTPATH = ../bin/gui/Widgets/
WIDGETSRCPATH = ../src/Widgets/


CC = g++




GUIOBJECTS = $(patsubst %.cpp, %.o, $(wildcard ../src/*.cpp))
GUIOBJECTSOURCES = $(GUIOBJECTS:$(GUISRCPATH)%=%)

WIDGETOBJECTS = $(patsubst %.cpp, %.o, $(wildcard ../src/Widgets/*.cpp))
WIDGETOBJECTSOURCES = $(WIDGETOBJECTS:$(WIDGETSRCPATH)%=%)


GUIFLAGS  = -I/usr/local/include/freetype2 -I/usr/include/libpng12 \
					-lglfw3 -lGLEW -lGL -lm -ldl -lXinerama -lXrandr -lXi \
					-lXcursor -lX11 -lXxf86vm -lpthread -std=c++17 \
					-L/usr/local/lib -lfreetype -lstdc++fs -lboost_system -lboost_filesystem \
					-O3 -g -rdynamic -ffast-math -lfreetype-gl -lfreetype-gl-cpp






$(GUIOBJECTPATH)%.o: $(GUISRCPATH)%.cpp
	@mkdir -p $(GUIOBJECTPATH)
	@mkdir -p $(WIDGETOBJECTPATH)
	@echo Compiling $<
	@$(CC) $(GUIFLAGS) -c $< -o $@


gui: $(addprefix $(GUIOBJECTPATH), $(GUIOBJECTSOURCES)) $(addprefix $(WIDGETOBJECTPATH), $(WIDGETOBJECTSOURCES))
	@echo Linking objects together, outputting: $@
	@$(CC) $(addprefix $(GUIOBJECTPATH), $(GUIOBJECTSOURCES))  $(addprefix $(WIDGETOBJECTPATH), $(WIDGETOBJECTSOURCES)) $(GUIFLAGS) -o $@

cleangui:
	cd ../bin/gui && find . -type f -name '*.o' -delete


test:
	@echo $(GUIOBJECTS)
	@echo $(GUISRCPATH)
	@echo $(GUIOBJECTSOURCES)
