//
//  main.cpp
//  GUI_Showcase
//
//  Created by Nathan Daly on 1/16/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "NewGUIApp.h"
#include "Compatibility.h"
#include "NewGUIWindow.h"
#include "NewGUIButton.h"
#include "NewGUITextBox.h"

#include <iostream>
#include <tr1/functional>
using namespace std;
using tr1::bind;

int main(int argc, char **argv) {
    
    initGUI();

    NewGUIWindow window(800,600);
    
    SDL_Color bg = {0xcc, 0xcc, 0xcc};
    NewGUIView *view = new NewGUIView(800,600);
    view->draw_onto_self(GUIImage::create_filled(800, 600, bg), DispPoint());
    window.attach_subview(view, DispPoint(0,0));

// Add a Text Prompt asking for the user's name.
    NewGUITextView *prompt = new NewGUITextView(30, 300);
//    prompt->set_text("Please enter your name:");
    
    view->attach_subview(prompt, DispPoint(50,200));
    
// Add a Text Box to enter a name
    NewGUIView *text_box_view = new NewGUIView(320,50);
    SDL_Color tb_bg = {0xee, 0xee, 0xee};
    text_box_view->draw_onto_self(GUIImage::create_filled(800,600,tb_bg), DispPoint());
    NewGUITextBox *text_box = new NewGUITextBox(300,30);
    text_box_view->attach_subview(text_box, DispPoint(10,10));

    view->attach_subview(text_box_view, DispPoint(50,250));
        
// Add a Text View that display's the user's name.
    NewGUITextView *name_display = new NewGUITextView(30, 300);
//    name_display->set_text("Your name is: ");
    
    view->attach_subview(name_display, DispPoint(150,300));

// Add a Button to accept the name
    NewGUIButton *accept_name = NewGUI_create_button(bind(&NewGUITextView::set_text, name_display, bind(&NewGUITextView::get_text, text_box)));

    view->attach_subview(accept_name, DispPoint(400, 
                                                250 + text_box_view->get_h()/2 - accept_name->get_h()/2));
    
// Run!
    
    NewGUIApp::get()->run(&window);
    
    return 0;
};