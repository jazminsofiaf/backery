#include "Employee.h"

Employee::Employee(int id_num): id(id_num){}

void Employee::start(){
    pid_t pid = fork ();
    if(pid < OK){
        std::cerr << "fork error" << std::strerror(errno)   << endl;
        exit(EXIT_FAILURE);
    }
	if ( pid == CHILD_PD ) {
        this->run();
        exit(OK); //no detructor called
	}
	this->process_id = pid;
}


void Employee::waitMe(){
    int status;
    pid_t rc_pid = waitpid(this->process_id, &status, 0);
    if (rc_pid < 0) {
        if (errno == ECHILD) {
            std::cerr << this-> identify() << " child does not exist" << std::endl;
            return;
        }
        else {
            std::cerr << this-> identify() << " bad argument passed to waitpid" << this->process_id<<std::endl;
            return;
        }
    }
    if(WIFSIGNALED(status)){//true if child was killed by any signal from other process or same process
        std::cerr << this-> identify() <<" terminates by signal : " << WTERMSIG(status) << std::endl;
    }
}

Employee::~Employee() {}
