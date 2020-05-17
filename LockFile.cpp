#include "LockFile.h"


LockFile :: LockFile ( const std::string nombre, int start, int end ) : Lock(nombre, &this->file, start, end, F_RDLCK){
	fseek(this->file, start, SEEK_SET);	
}


int LockFile :: getSharedLock(){
	this->fl.l_type = F_UNLCK;
	return fcntl (this->fd,F_SETLK,&(this->fl) );
	
}

bool LockFile :: getChar(char & c){
	int char_read;
	if( (char_read = fgetc(file)) == EOF ){
		return false;
	} 
	putchar(char_read);
	c = char_read;
	return true;
}

LockFile :: ~LockFile () {
	fclose ( this->file);
}
 