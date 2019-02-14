GUIOBJECTPATH = ../bin/gui/
GUISRCPATH = ../src/


CC = g++




GUIOBJECTS = $(patsubst %.cpp, %.o, $(wildcard ../src/*.cpp))
GUIOBJECTSOURCES = $(GUIOBJECTS:$(GUISRCPATH)%=%)




GUIFLAGS  = -I/usr/local/include/freetype2 -I/usr/include/libpng12 \
					-lglfw3 -lGLEW -lGL -lm -ldl -lXinerama -lXrandr -lXi \
					-lXcursor -lX11 -lXxf86vm -lpthread -std=c++17 \
					-L/usr/local/lib -lfreetype -lstdc++fs -lboost_system -lboost_filesystem \
					-O3 -g -rdynamic -ffast-math






$(GUIOBJECTPATH)%.o: $(GUISRCPATH)%.cpp
	mkdir -p $(GUIOBJECTPATH)
	@echo Compiling $<
	@$(CC) $(GUIFLAGS) -c $< -o $@


gui: $(addprefix $(GUIOBJECTPATH), $(GUIOBJECTSOURCES))
	@echo Linking objects together, outputting: $@
	@$(CC) $(addprefix $(GUIOBJECTPATH), $(GUIOBJECTSOURCES)) $(GUIFLAGS) -o $@

cleangui:
	cd ../bin/gui && find . -type f -name '*.o' -delete


test:
	@echo $(GUIOBJECTS)
	@echo $(GUISRCPATH)
	@echo $(GUIOBJECTSOURCES)
