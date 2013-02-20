//
//  main.cpp
//  GUI_Showcase
//
//  Created by Nathan Daly on 1/16/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "GUIApp.h"
#include "GUIWindow.h"
#include "GUIView.h"
#include "GUIButton.h"
#include "GUITextBox.h"

// This would be a library where the user doesn't have access to the code.
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
string get_msg(const std::exception& e);

string no_action(const string& str);

// Action should expect a string and return a string
template <typename Action>
GUIView* create_entry_form(const string &prompt_text, const string &answer_text,
                              Action action);
GUIView* create_entry_form(const string &prompt_text, const string &answer_text);


/********************************
 * Helper Class Declarations
 ********************************/

// A class to represent an entry form.
// Simply an aggregation of subviews that lessen code repetition.
class EntryForm : public GUITextView {
public:
    template <typename Action>
    EntryForm(const string &prompt_text, const string &answer_text,
              Action action);
    
    EntryForm(const string &prompt_text, const string &answer_text);

    // Public data members so they can be accessed.
    GUITextView *prompt;
    GUITextView *name_display1;
    GUITextButton *accept;
    
private:
    template <typename Action>
    void initialize(const string &prompt_text, const string &answer_text,
                    Action action);
};


// User-Defined Error Struct
struct MyError {
    std::string msg;
    MyError(const std::string &msg_) : msg(msg_) { }
};


// A View Class to display Error Messages until clicked.
class ErrorMsg : public GUIImageView {
public:
    template <typename Error_t>
    ErrorMsg(const Error_t& e)
    :GUIImageView(GUIImage::create_filled(450, 75, bg))
    { 
        GUITextView *msg = new GUITextView(425, 50);
        attach_subview(msg, DispPoint(12,20));
        msg->set_text(get_msg(e));
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
    return "Error: " + e.msg;
}
string get_msg(const FakeLib1Error& e) {
    return "Lib1 Error: " + e.error_msg;
}
string get_msg(const std::exception& e) {
    return "Error: " + string(e.what());
}

// A functor to display an error message onto a view
struct Display_Error {
    Display_Error(GUIView *view_) : view(view_) { }
    
    template <typename Error_t>
    void operator()(const Error_t& e) {

        GUIView *error_msg = new ErrorMsg(e);
        
        DispPoint center(view->get_w()/2 - error_msg->get_w()/2,
                         view->get_h()/2 - error_msg->get_h()/2);
                         
        view->attach_subview(error_msg, center);
    }
private:
    GUIView *view;
};


/****************************
 * Main!
 ****************************/

// Create a few question forms, register error handlers, and run
// The main exciting thing here is register_erorr_handlers, because
// it allows you to use errors from a library whose code you cannot change.
int main(int argc, char **argv) {

    initGUI();

    GUIWindow window(800,600);
    
// **** Create the Main View ***********
    SDL_Color bg = {0xcc, 0xcc, 0xcc};
    GUIView *view = new GUIView(800,600);
    view->draw_onto_self(GUIImage::create_filled(800, 600, bg), DispPoint());
    
    window.attach_subview(view, DispPoint(0,0));

    
// Create the Questions
    view->attach_subview(new EntryForm("Please enter your name: ", "Your name is: "),
                         DispPoint(15, 50));

    view->attach_subview(new EntryForm("Date of birth (\"Mmm dd yyyy\"): ",
                                       "Your age is: ", get_age),
                         DispPoint(15, 200));

    view->attach_subview(new EntryForm("Please enter your age: ",
                                       "Your date of birth is: ", get_birthdate),
                         DispPoint(15, 350));

// Register Error Handlers
    // In this case, both types of errors use the same Error Handler,
    // but that might not always be the case.
    GUIApp::get()->register_exception_handler<FakeLib1Error>(Display_Error(view));
    GUIApp::get()->register_exception_handler<MyError>(Display_Error(view));

// Run!
    GUIApp::get()->run(&window);
    
    return 0;
};

// Using FakeLib1 return age as a string from a birthdate.
string get_age(string birthdate) {
    if (birthdate.size() != 11) {
        throw MyError("Entry must follow format exactly!");
    }
    return my_itoa(age_from_dob(birthdate));
}

// Using FakeLib1 return a birthdate as a string from age.
string get_birthdate(string age) {
    
    for (int i = 0; i < age.length(); i++) {
        if (!(isdigit(age[i]) || age[i] == '.')) {
            throw MyError("Age must be a number!");
        }
    }
    
    return date_of_birth(atof(age.c_str()));
}

// simply return the string as is.
string no_action(const string& str) { 
    return str; 
}


/****************************
 * Entry Form Implementation
 ****************************/

// Constructors simply call initialize
EntryForm::EntryForm(const string &prompt_text, const string &answer_text)
: GUITextView(600, 200) 
{
    initialize<string(*)(const string&)>(prompt_text, answer_text, no_action);    
}
template <typename Action>
EntryForm::EntryForm(const string &prompt_text, const string &answer_text,
                     Action action)
: GUITextView(600, 200) 
{
    initialize(prompt_text, answer_text, action);
}

// Attaches all of the correct subviews (prompt text, entry form, answer text,
//  and Go button) and sets the button to the specified action.
//  the form's text is passed to the action on button press.
template <typename Action>
void EntryForm::initialize(const string &prompt_text, const string &answer_text,
                           Action action)
{
    
    const int text_size = 20;
    
    // Add a Text Prompt to show prompt_text.
    prompt = new GUITextView(300, 30);
    prompt->set_text(prompt_text);
    prompt->set_text_size(text_size);
    
    attach_subview(prompt, DispPoint(50,25));
    
    
    // Add a Text Box to enter a string
    GUIView *text_box_view = new GUIView(300,30);
    SDL_Color tb_bg = {0xee, 0xee, 0xee};
    text_box_view->fill_with_color(tb_bg);
    GUITextBox *text_box = new GUITextBox(300,30);
    text_box->set_text_size(text_size);
    text_box_view->attach_subview(text_box, DispPoint());
    
    attach_subview(text_box_view, DispPoint(50,50));
    
    
    // Add a Text View to show answer_text.
    name_display1 = new GUITextView(300, 30);
    name_display1->set_text(answer_text);
    name_display1->set_text_size(text_size);
    
    attach_subview(name_display1, DispPoint(150,100));
    
    
    // Add a Text View to display result.
    GUITextView *name_display2 = new GUITextView(300, 30);
    name_display2->set_text_size(text_size);
    
    attach_subview(name_display2, DispPoint(100 + name_display1->get_w(), 100));
    
    
    // Add a Button to accept input and display it in result.
    accept = GUI_create_button(bind(&GUITextView::set_text, name_display2, 
                                    bind(action, 
                                         bind(&GUITextView::get_text, text_box))));
    accept->set_text("Go");

    attach_subview(accept, DispPoint(100 + text_box_view->get_w(), 
                                     50 + text_box_view->get_h()/2 - accept->get_h()/2));
    
}

// Return a string from any value. (Uses stringstream)
template <typename T>
string my_itoa(T i) {
    stringstream ss;
    ss << i;
    return ss.str();    
}

