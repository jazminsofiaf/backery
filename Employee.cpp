#include "Employee.h"

Employee::Employee(int id_num): id(id_num){}

void Employee::start(){
    std::cout << "before fork " << std::strerror(errno) << endl;
    pid_t pid = fork ();
    if(pid < OK){
        std::cout << "fork error" << std::strerror(errno)   << endl;
        exit(EXIT_FAILURE);
    }
	if ( pid == CHILD_PD ) {
        std::cout << "before run " << std::strerror(errno) << endl;
        this->run();
        exit(OK); //no detructor called
	}
	//std::cout << this-> identify() <<" pid: "<<pid << std::endl;
	this->process_id = pid;
}


void Employee::stop(){
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
    std::cout <<"[" << this-> identify() << "] " << status << std::endl;
    if(WIFEXITED(status)){//if child is completed normally after sleep is over
       std::cerr << this-> identify() << " terminates normally : " << WEXITSTATUS(status) << std::endl;
    }
    if(WIFSIGNALED(status)){//true if child was killed by any signal from other process or same process
        std::cerr << this-> identify() <<" terminates by signal : " << WTERMSIG(status) << std::endl;
        //Signal 13 means something is written to a pipe where nothing is read from anymore
        //Signal 11 (SIGSEGV, also known as segmentation violation) means that the program accessed a memory location that was not assigned to it
    }
}

Employee::~Employee() {
    //std::cout << "calling employee detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
}
