
#include "Recepcionist.h"
Recepcionist::Recepcionist(int id_num, 
                            std::string bread_name, std::string pizza_name,
                            int start, int end, std::string file_name)
                            : Employee(id_num), bread_channel_name(bread_name), pizza_channel_name(pizza_name),
                            read_start(start), read_end(end), orders_file(file_name){
}

void Recepcionist::run(){
    std::ifstream file(this->orders_file);
    int pos = this->read_start;
    file.seekg(pos);
    char c;
    std::string order = "";
  	while (pos < this->read_end && file.get(c)){
        if(this->is_delimiter(c)){
            this->validate_order(order); 
            order = "";
        } else{
           order += c;
        }
        pos++;  
    }
    if(order.empty()){
        return; //no words cut
    }
    while(file.get(c)){ //read last order
        if(this->is_delimiter(c)){  
            this->validate_order(order);
            break;
        } else{
           order += c;
        }
    }
     
}

bool Recepcionist::is_delimiter(char c){
    return (c == '\n' || c == ' ' || c == ',' || c== '-' || c== ';');
}

void Recepcionist::validate_order(std::string order){
    if(order.compare("pan") == 0 || order.compare("pizza") == 0){
        std::cout << this->identify() << " read order: "<< order << endl; 
        Logger::log(this, "order received: "+ order);
    }
}



void Recepcionist::stop(){}
std::string Recepcionist::identify() const {
     return "Recepcionist "+std::to_string(this->id);

}