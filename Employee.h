#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "Constant.h"


using namespace std;

class Employee{
    protected:
        int id;
        pid_t process_id;
        virtual void run() = 0;
        
    public:
        //constructor
        Employee(int id);

        void start();
        virtual void waitMe();
        virtual std::string identify() const = 0;  
        virtual ~Employee();
};

#endif