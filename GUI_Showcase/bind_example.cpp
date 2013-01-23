//
//  bind_example.cpp
//  GUI_Showcase
//
//  Created by Nathan Daly on 1/22/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include <iostream>

#include <sstream>
#include <tr1/functional>
using namespace std;
using tr1::bind;
using namespace std::tr1;
using namespace std::tr1::placeholders;


string my_itoa(double i) {
    stringstream ss;
    ss << i;
    return ss.str();    
}

double times_2(double a) { 
    return 2*a;
}

template <typename Oper>
void print_after_op(Oper op) {
    
    for (int i = 0; i < 10; i++) {
        cout << my_itoa(op(i)) << endl;
    }
}

int main() {

    print_after_op(times_2);
    
    
    return 0;
}