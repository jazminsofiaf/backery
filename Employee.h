#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <iostream>
#include <unistd.h>
using namespace std;

class Employee{
    protected:
        int id;
        
    public:
        //constructor
        Employee(int id);
        virtual std::string identify() const = 0;  
};

#endif