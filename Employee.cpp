#include "Employee.h"

Employee::Employee(int id_num): id(id_num){}

void Employee::start(){
    pid_t pid = fork ();
	if ( pid == CHILD_PD ) {
        this->run();
		exit(OK);
	}
	std::cout << this-> identify() <<" pid: "<<pid << std::endl;
	this->process_id = pid;
}


void Employee::stop(){
    int status;
    pid_t rc_pid = waitpid(this->process_id, &status, 0);   
    if (rc_pid < 0) {   
        if (errno == ECHILD) {
            std::cerr << this-> identify() << " child does not exist" << std::endl;
        }
        else {
            std::cerr << this-> identify() << " bad argument passed to waitpid" << std::endl;
        }
    }
	std::cout <<  this-> identify() << " stoped " << status << std::endl;

}
