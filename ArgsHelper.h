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
    static std::string getPedido(int argc, char** argv);

    public:
    struct args{
        int pizzeros, panaderos, delivery;
        std::string pedido;
    };
    static args * parse(int argc, char** argv);   
};

#endif