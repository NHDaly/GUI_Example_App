//
//  main.cpp
//  GUI_Showcase
//
//  Created by Nathan Daly on 1/16/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "gui/GUIApp.h"
#include "gui/GUIWindow.h"
#include "gui/GUIView.h"
#include "gui/GUIButton.h"
#include "gui/GUITextViews.h"

// This would be a library where the user doesn't have access to the code.
#include "fake_lib1.h" 

#include <iostream>
#include <cctype>
#include <sstream>
#include <functional>
using namespace std;
using namespace std::placeholders;


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
class ErrorMsg : public GUIView {
public:
    template <typename Error_t>
    ErrorMsg(const Error_t& e)
    :GUIView(450, 75)
    { 
        const SDL_Color bg = {0xbb, 0xbb, 0xbb};
        fill_with_color(bg);
        
        GUITextView *msg = new GUITextView(425, 50);
        attach_subview(msg, DispPoint(12,20));
        msg->set_text(get_msg(e));
        msg->set_text_size(18);

        GUITextView *dismiss_text = new GUITextView(425, 25);
        attach_subview(dismiss_text, DispPoint(140,50));
        dismiss_text->set_text("(Click this box to dismiss)");
        dismiss_text->set_text_size(10);
    }
private:
    
    virtual bool handle_mouse_down(DispPoint coord) {
        get_parent()->remove_subview(this);
        delete this;    // Self deletion! :D
        return true;
    }
};


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
    view->fill_with_color(bg);
    
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

const int entry_w_c = 650;
const int entry_h_c = 200;

// Constructors simply call initialize
EntryForm::EntryForm(const string &prompt_text, const string &answer_text)
: GUITextView(entry_w_c, entry_h_c) 
{
    initialize<string(*)(const string&)>(prompt_text, answer_text, no_action);    
}
template <typename Action>
EntryForm::EntryForm(const string &prompt_text, const string &answer_text,
                     Action action)
: GUITextView(entry_w_c, entry_h_c) 
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
    const int field_w = 350;
    const int field_h = 30;

    
    // Add a Text Prompt to show prompt_text.
    prompt = new GUITextView(field_w, field_h);
    prompt->set_text(prompt_text);
    prompt->set_text_size(text_size);
    
    attach_subview(prompt, DispPoint(50,25));
    
    
    // Add a Text Box to enter a string
    GUITextBox *text_box = new GUITextBox(field_w,field_h);
    text_box->set_text_size(text_size);
    
    attach_subview(text_box, DispPoint(50,50));
    
    
    // Add a Text View to show answer_text.
    name_display1 = new GUITextView(field_w, field_h);
    name_display1->set_text(answer_text);
    name_display1->set_text_size(text_size);
    
    attach_subview(name_display1, DispPoint(150,100));
    
    
    // Add a Text View to display result.
    GUITextView *name_display2 = new GUITextView(field_w, field_h);
    name_display2->set_text_size(text_size);
    
    attach_subview(name_display2, DispPoint(100 + name_display1->get_w(), 100));
    
    
    // Add a Button to accept input and display it in result.
    accept = GUI_create_button(bind(&GUITextView::set_text, name_display2, 
                                    bind(action, 
                                         bind(&GUITextBox::get_text, text_box))));
    accept->set_text("Go");

    attach_subview(accept, DispPoint(100 + text_box->get_w(), 
                                     50 + text_box->get_h()/2 - accept->get_h()/2));
    
}

// Return a string from any value. (Uses stringstream)
template <typename T>
string my_itoa(T i) {
    stringstream ss;
    ss << i;
    return ss.str();    
}

