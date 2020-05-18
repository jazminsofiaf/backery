#ifndef LOCKFILE_H
#define LOCKFILE_H


#include <unistd.h>
#include <fcntl.h>
#include <string>


class LockFile {
protected:
    struct flock fl{};
    int fd;
    FILE * file;
    std::string name;

public:

    LockFile ( const std::string name, int type, int start, int end, std::string mode);
    ~LockFile();

    int getLock ();

    int freeLock ();
};


#endif //LOCKFILE_H
