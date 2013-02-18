////
////  bind_example.cpp
////  GUI_Showcase
////
////  Created by Nathan Daly on 1/22/13.
////  Copyright (c) 2013 Lions Entertainment. All rights reserved.
////
//
//#include <SDL/SDL.h>
//
//#include <iostream>
//
//#include <sstream>
//#include <tr1/functional>
//using namespace std;
//using tr1::bind;
//using namespace std::tr1;
//using namespace std::tr1::placeholders;
//
//
//string my_itoa(double i) {
//    stringstream ss;
//    ss << i;
//    return ss.str();    
//}
//
//double times_2(double a) { 
//    return 2*a;
//}
//
//template <typename Oper>
//void print_after_op(Oper op) {
//    
//    for (int i = 0; i < 10; i++) {
//        cout << my_itoa(op(i)) << endl;
//    }
//}
//
//int main(int, char **) {
//
//    print_after_op(times_2);
//    
//    
//    return 0;
//}

#include <SDL/SDL.h>

#include <iostream>
#include <algorithm>
//#include <tr1/functional>
#include <boost/bind.hpp>
using namespace std;
//using tr1::bind;
using boost::bind;
//using namespace std::tr1::placeholders;

double times_2(double a) { 
    return 2*a;
}
void print_num(double a) {
    cout << a << endl;
}

template <typename Oper>
void oper_on_list(Oper op) {
    
    int list[] = {0,1,2,3,4};
    
    for_each(list, list+5, bind(print_num, bind(op, _1)));      // This works!
    for_each(list, list+5, bind(print_num, bind(op, bind(op, _1)))); // This doesn't!
}

int main(int, char**) {
    oper_on_list(bind(times_2, _1));
    return 0;
}
