#ifndef END_CHILD_H_
#define END_CHILD_H_
#include <iostream>
#include <exception>

using namespace std;

struct EndChildException : public std::exception {
	const char * what () const throw (){
    	return "Child end";
    }
};

#endif