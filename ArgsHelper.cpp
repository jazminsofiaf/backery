#include "ArgsHelper.h"


using namespace std;

const std::string ArgsHelper::PIZZEROS = "pizzeros=";
const std::string ArgsHelper::PANADEROS= "panaderos=";
const std::string ArgsHelper::DELIVERY= "delivery=";
const std::string ArgsHelper::PEDIDOS= "pedido=";


ArgsHelper::args * ArgsHelper::parse(int argc, char** argv){ 
    if(argc <= 4){
        std::cerr << "Debe ingresar 4 argumentos: pizzeros=<n> panaderos=<n> delivery=<n> pedidos=<file.txt>" << std::endl;
        throw std::runtime_error("Error: Invalid Arguments");
    }

    remove(Logger::FILENAME.c_str());

    ArgsHelper::args * args = new ArgsHelper::args();
    args->pizzeros = ArgsHelper::get(argc, argv, ArgsHelper::PIZZEROS);
    args->panaderos = ArgsHelper::get(argc, argv, ArgsHelper::PANADEROS);
    args->delivery = ArgsHelper::get(argc, argv, ArgsHelper::DELIVERY);
    if(args->pizzeros < 1 || args->panaderos < 1|| args->delivery < 1){
        std::cerr <<"Error: more than one employee is needed for each type"<< endl;
        throw std::runtime_error("Error: Invalid Arguments");
    }
    args->pedido = ArgsHelper::getOrdersFile(argc, argv);
    args->file_size = ArgsHelper::getFileSize(args->pedido);
    return args;
}

int ArgsHelper::get(int argc, char** argv, std::string name){ 
    for (int i = 1; i < argc; ++i){
        std::string argument = argv[i];
        std::string::size_type n = argument.find(name);
        if(n != std::string::npos){
            std::string num = argument.erase(n, name.length());
            try{
		        return std::stoi(num);
		    } catch (std::invalid_argument const &e){
                std::cerr << "Bad input: not int value" << std::endl;
            } catch (std::out_of_range const &e){
		        std::cerr << "Integer overflow " << std::endl;
            }
        }  
    }
    std::cerr << "no se encontro "<<  name << std::endl;
    throw std::runtime_error("Error: Invalid Arguments");
}

std::string ArgsHelper::getOrdersFile(int argc, char** argv){ 
    for (int i = 1; i < argc; ++i){
        std::string argument = argv[i];
        std::string::size_type n = argument.find(ArgsHelper::PEDIDOS);
        if(n != std::string::npos){
            std::string name = argument.erase(n, ArgsHelper::PEDIDOS.length());
            return name;
        }  
    }
    std::cerr << "no se encontro "<<  ArgsHelper::PEDIDOS << std::endl;
    throw std::runtime_error("Error: Invalid Arguments");
}

int ArgsHelper::getFileSize(std::string filename){
    FILE * file = NULL;
    file  = fopen(filename.c_str(),"rb");
    if (file == NULL) {
        perror("No existe el archivo de pedidos");
        throw std::runtime_error("Error: Invalid Arguments");
    }
    fseek(file,0,SEEK_END);
    int size = ftell(file);
    fclose(file);
    return size;
}


