
CC = g++
LD = g++

CFLAGS = -c -Wall -fno-elide-constructors
LFLAGS = -Wall -framework SDL -framework SDL_TTF -framework SDL_mixer -framework SDL_image -framework Cocoa

FRAMEWORK_FILES = gui/SDL.framework gui/SDL_ttf.framework gui/SDL_mixer.framework gui/SDL_image.framework

# Put any user application resources here. (ie images, sounds, etc.)
RESOURCES :=

PROG = app

PREXISTING_FRAMEWORKS =  $(wildcard /Library/Frameworks/SDL*.framework)

OBJS = main.o fake_lib1.o

default: setup gui_files $(PROG)
	@echo "Done!"
	@echo $(PROG)

gui_files:
	@echo "Making GUI files."
	cd gui;	make

setup: setup_resources setup_frameworks
	

setup_resources: 
	@echo "\nCopying Resource files."
	mkdir -p Resources
	cp -R gui/GUIImages Resources
	cp -R gui/GUIFonts Resources
ifneq ($(RESOURCES),)
	cp -R $(RESOURCES) Resources
endif


setup_frameworks: $(FRAMEWORK_FILES)
	@echo "\nChecking Frameworks.."
ifneq ($(PREXISTING_FRAMEWORKS),)
	@echo "Frameworks are already installed"
else
	@echo "Installing Frameworks to /Library/Frameworks"
	cp -R -n $(FRAMEWORK_FILES) /Library/Frameworks
endif
	@echo ""



$(PROG):  $(wildcard gui/*.o) $(OBJS)
	@echo "\nBuilding Project."
	$(LD) $(LFLAGS) $(wildcard gui/*.o) $(OBJS) -o $(PROG)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

fake_lib1.o: fake_lib1.cpp
	$(CC) $(CFLAGS) fake_lib1.cpp


clean:
	rm -f *.o
	rm -f gui/*.o
clean_full:
	rm -f *.o $(PROG)
	rm -f gui/*.o
	rm -rf Resources

