#ifndef ARGSHELPER_H_
#define ARGSHELPER_H_

#include <iostream>
#include <stdexcept>



class ArgsHelper {
    private:
    static const std::string PIZZEROS;
    static const std::string PANADEROS;
    static const std::string DELIVERY;
    static const std::string PEDIDOS;
    static int get(int argc, char** argv, std::string name);
    static std::string getOrdersFile(int argc, char** argv);
    static int getFileSize(std::string filename);

    public:
    struct args{
        int pizzeros, panaderos, delivery, file_size;
        std::string pedido;
    };
    static args * parse(int argc, char** argv);   
};

#endif