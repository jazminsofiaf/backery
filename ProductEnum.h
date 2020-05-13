#ifndef PRODUCT_H_
#define PRODUCT_H_

#define PIZZA "PIZZA"
#define BREAD "PAN"

#include<iostream> 
#include <exception>
#include "Constant.h"
using namespace std; 

class ProductEnum{

public:
    enum Value {
        Bread,
        Pizza
    };

    struct EnumValueError : public exception {
        const char * what () const throw () {
            return "Not valid enum value";
        }
    };

    static Value fromString(std::string str) { 
        
        if(str.compare(PIZZA) == OK){
            return Pizza;
        }
        if(str.compare(BREAD) == OK){
            return Bread;
        }  
        throw EnumValueError();      
    }

    static std::string toString(Value product) { 
        switch (product) {
            case Bread:
                return BREAD;
                break;
            case Pizza:
                return PIZZA;
                break; 
        }
        throw EnumValueError();      
    }

};

#endif