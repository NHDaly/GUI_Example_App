
CC = g++
LD = g++

CFLAGS = -c -Wall -fno-elide-constructors
LFLAGS = -Wall -framework SDL -framework SDL_TTF -framework SDL_mixer -framework SDL_image -framework Cocoa

FRAMEWORK_FILES = gui/SDL.framework gui/SDL_ttf.framework gui/SDL_mixer.framework gui/SDL_image.framework

# Put any application resources here. (ie images, sounds, etc.)
RESOURCES :=

PROG = app

PREXISTING_FRAMEWORKS =  $(wildcard /Library/Frameworks/SDL*.framework)

GUI_LIB_O = gui/Compatibility.o gui/GUIApp.o gui/GUIAudio.o gui/GUIController.o gui/GUIImage.o gui/GUIImageCache.o gui/GUIImageView.o gui/GUILetter.o gui/GUIMsg.o gui/GUIScrollView.o gui/GUISpreadSheet.o gui/GUITextViews.o gui/GUITimer.o gui/GUIUtility.o gui/GUIValBox.o gui/GUIView.o gui/GUIWindow.o gui/GameDisplay.o gui/Letter.o gui/NewNewGUITextView.o 

OBJS = main.o fake_lib1.o SDLMain.o

default: settup $(PROG)

settup: settup_resources settup_frameworks
	

settup_resources:
	mkdir -p Resources
	cp -R gui/GUIImages Resources
	cp -R gui/GUIFonts Resources
ifneq ($(RESOURCES),)
	cp -R $(RESOURCES) Resources
endif

settup_frameworks: $(FRAMEWORK_FILES)
ifneq ($(PREXISTING_FRAMEWORKS),)
	echo "Already has Frameworks installed"
else
	echo "Installing Frameworks to /Library/Frameworks"
	cp -R -n $(FRAMEWORK_FILES) /Library/Frameworks/
endif


$(PROG): $(GUI_LIB_O) $(OBJS)
	$(LD) $(LFLAGS) $(GUI_LIB_O) $(OBJS) -o $(PROG)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

fake_lib1.o: fake_lib1.cpp
	$(CC) $(CFLAGS) fake_lib1.cpp

SDLMain.o: gui/SDLMain.m
	gcc $(CFLAGS) gui/SDLMain.m

clean:
	rm -f *.o
clean_full:
	rm -f *.o $(PROG)
	rm -f gui/*.o
	rm -rf Resources

