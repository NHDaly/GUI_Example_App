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

// Libraries!
#include "fake_lib1.h"

#include <iostream>
#include <cctype>
#include <sstream>
#include <tr1/functional>
using namespace std;
using tr1::bind;
using namespace std::tr1;
using namespace std::tr1::placeholders;


// Forward Declarations
template <typename T>
string my_itoa(T i);

string get_age(string birthdate);
string get_birthdate(string age);

struct MyError;
string get_msg(const MyError& e);
string get_msg(const FakeLib1Error& e);

string no_action(const string& str);

// Action should expect a string and return a string
template <typename Action>
NewGUIView* create_entry_form(const string &prompt_text, const string &answer_text,
                              Action action);
NewGUIView* create_entry_form(const string &prompt_text, const string &answer_text);



// User-Defined Error Struct
struct MyError {
    std::string msg;
    MyError(const std::string &msg_) : msg(msg_) { }
};

#define CreateMyError(x) MyError(std::string(x) + \
                         "                  File: " + my_itoa(__FILE__) + \
                         "  Line: " + my_itoa(__LINE__))


// A View Class to display Error Messages and self-destruct on click.
class ErrorMsg : public NewGUIImageView {
public:
    template <typename Error_t>
    ErrorMsg(const Error_t& e)
    :NewGUIImageView(GUIImage::create_filled(400, 150, bg))
    { 
        NewGUITextView *msg = new NewGUITextView(350, 130);
        attach_subview(msg, DispPoint(25,10));
        msg->set_text("Error: " + get_msg(e));
        msg->set_text_size(18);
    }
private:
    static SDL_Color bg;
    
    virtual bool handle_mouse_down(DispPoint coord) {
        get_parent()->remove_subview(this);
        delete this;    // Self deletion! :D
        return true;
    }
};
SDL_Color ErrorMsg::bg = {0xbb, 0xbb, 0xbb};


// Used to make the above struct generic.
string get_msg(const MyError& e) {
    return e.msg;
}
string get_msg(const FakeLib1Error& e) {
    return e.error_msg;
}
string get_msg(const std::exception& e) {
    return e.what();
}

// A functor to display an error message onto a view
struct Display_Error {
    Display_Error(NewGUIView *view_) : view(view_) { }
    
    template <typename Error_t>
    void operator()(const Error_t& e) {

        NewGUIView *error_msg = new ErrorMsg(e);
        
        DispPoint center(view->get_w()/2 - error_msg->get_w()/2,
                         view->get_h()/2 - error_msg->get_h()/2);
                         
        view->attach_subview(error_msg, center);
    }
private:
    NewGUIView *view;
};


// Main Program!
// Create a few question forms, register error handlers, and run
// The main exciting thing here is register_erorr_handlers, because
// it allows you to use errors from a library whose code you cannot change.
int main(int argc, char **argv) {

    initGUI();

    NewGUIWindow window(800,600);
    
// **** Create the Main View ***********
    SDL_Color bg = {0xcc, 0xcc, 0xcc};
    NewGUIView *view = new NewGUIView(800,600);
    view->draw_onto_self(GUIImage::create_filled(800, 600, bg), DispPoint());
    
    window.attach_subview(view, DispPoint(0,0));

    
// Create the Questions
    view->attach_subview(create_entry_form("Please enter your name: ", "Your name is: "),
                         DispPoint(15, 50));

    view->attach_subview(create_entry_form("Date of birth (\"Mmm dd yyyy\"): ",
                                           "Your age is: ", get_age),
                         DispPoint(15, 200));

    view->attach_subview(create_entry_form("Please enter your age: ",
                                           "Your date of birth is: ", get_birthdate),
                         DispPoint(15, 350));

// Register Error Handlers
    NewGUIApp::get()->register_error_handler<FakeLib1Error>(Display_Error(view));
    NewGUIApp::get()->register_error_handler<MyError>(Display_Error(view));

// Run!
    NewGUIApp::get()->run(&window);
    
    return 0;
};


string get_age(string birthdate) {
    return my_itoa(age_from_dob(birthdate));
}
string get_birthdate(string age) {
    
    for (int i = 0; i < age.length(); i++) {
        if (!(isdigit(age[i]) || age[i] == '.')) {
            throw CreateMyError("Age must be a number!");
        }
    }
    
    return date_of_birth(atof(age.c_str()));
}

NewGUIView* create_entry_form(const string &prompt_text, const string &answer_text) {
    return create_entry_form<string(*)(const string&)>(prompt_text, answer_text, no_action);
}
string no_action(const string& str) { 
    return str; 
}


template <typename Action>
NewGUIView* create_entry_form(const string &prompt_text, const string &answer_text,
                              Action action) {
    
    NewGUIView *view = new NewGUITextView(600, 200);
    
    const int text_size = 20;
    
// Add a Text Prompt asking for the user's name.
    NewGUITextView *prompt = new NewGUITextView(300, 30);
    prompt->set_text(prompt_text);
    prompt->set_text_size(text_size);
    
    view->attach_subview(prompt, DispPoint(50,25));
    
// Add a Text Box to enter a name
    NewGUIView *text_box_view = new NewGUIView(300,30);
    SDL_Color tb_bg = {0xee, 0xee, 0xee};
    text_box_view->draw_onto_self(GUIImage::create_filled(800,600,tb_bg), DispPoint());
    NewGUITextBox *text_box = new NewGUITextBox(300,30);
    text_box->set_text_size(text_size);
    text_box_view->attach_subview(text_box, DispPoint());
    
    view->attach_subview(text_box_view, DispPoint(50,50));
    
// Add a Text View that display's the user's age.
    NewGUITextView *name_display1 = new NewGUITextView(300, 30);
    name_display1->set_text(answer_text);
    name_display1->set_text_size(text_size);
    
    view->attach_subview(name_display1, DispPoint(150,100));
    
    NewGUITextView *name_display2 = new NewGUITextView(300, 30);
    name_display2->set_text_size(text_size);
    
    view->attach_subview(name_display2, DispPoint(100 + name_display1->get_w(), 100));
        
// Add a Button to accept the name
    NewGUITextButton *accept = NewGUI_create_button(bind(&NewGUITextView::set_text, name_display2, 
                                                         bind(action, 
                                                              bind(&NewGUITextView::get_text, text_box))));
    accept->set_text("Go");
    
    view->attach_subview(accept, DispPoint(100 + text_box_view->get_w(), 
                                           50 + text_box_view->get_h()/2 - accept->get_h()/2));
    
    
    return view;
}


template <typename T>
string my_itoa(T i) {
    stringstream ss;
    ss << i;
    return ss.str();    
}

