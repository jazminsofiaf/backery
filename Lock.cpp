#include "Lock.h"

Lock :: Lock ( const std::string nombre ) {

	this->nombre = nombre;
	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	this->fd = open ( this->nombre.c_str(), O_CREAT|O_WRONLY, 0777 );
}

Lock :: Lock (const std::string nombre, FILE ** file, int type, int start, int end) {
	this->nombre = nombre;
	this->fl.l_type = type;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = start;
	this->fl.l_len = end - start;

    *file = fopen(this->nombre.c_str(),"rb"); 
	this->fd = fileno(*file);
}



int Lock :: tomarLock () {
	this->fl.l_type = F_WRLCK;
	return fcntl ( this->fd,F_SETLKW,&(this->fl) );
}

ssize_t Lock :: escribir ( const void* buffer,const ssize_t buffsize ) const {
	lseek ( this->fd,0,SEEK_END );
	return write ( this->fd,buffer,buffsize );
}

int Lock :: liberarLock () {
	this->fl.l_type = F_UNLCK;
	return fcntl ( this->fd,F_SETLK,&(this->fl) );
}

Lock :: ~Lock () {
	close ( this->fd );
}
 