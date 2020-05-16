
#include "Recepcionist.h"
Recepcionist::Recepcionist(int id_num, 
                            Logger * logger, 
                            std::string bread_name, std::string pizza_name,
                            int start, int end, std::string file_name)
                            : Employee(id_num), logger(logger), 
                            bread_channel_name(bread_name), pizza_channel_name(pizza_name),
                            read_start(start), read_end(end), orders_file(file_name){
                        
    this->bread_channel = new FifoEscritura(this->bread_channel_name);
    this->pizza_channel = new FifoEscritura(this->pizza_channel_name);
    this->channel_map[BREAD] = this->bread_channel;
    this->channel_map[PIZZA] = this->pizza_channel;
}

void Recepcionist::run(){
    //other side already open
    this->bread_channel->abrir();
    //this->pizza_channel->abrir(); 

    std::ifstream file(this->orders_file, ios::in | ios::binary);
    int pos = this->read_start;
    file.seekg(pos);
    char c;
    std::string order = "";
  	while (pos < this->read_end && file.get(c)){
        if(this->isDelimiter(c)){
            this->tryToSend(order, pos); 
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
        if(this->isDelimiter(c)){  
            this->tryToSend(order, pos);
            break;
        } else{
           order += c;
        }
    }  
    //signal to stop loop  for baker and pizza man
    this->bread_channel->close_fifo();
    this->pizza_channel->close_fifo();    
}

bool Recepcionist::isDelimiter(char c){
    return (c == '\n' || c == ' ' || c == ',' || c== '-' || c== ';');
}

void Recepcionist::tryToSend(std::string str_order, int pos){
    str_order = this->toUpper(str_order);
    if(this->channel_map.count(str_order)){
        FifoEscritura * channel = this->channel_map[str_order];
        Recepcionist::Order order;
        order.id = pos; //set last read position as order id
        order.product = str_order;
        channel->escribir(&order, sizeof(Recepcionist::Order));
        this->logger->log(this, order.toString());
        std::cout << this->identify() << order.toString() << endl;
    }
}

std::string Recepcionist::toUpper(std::string str){
    std::for_each(str.begin(), str.end(), [](char & c){
	    c = ::toupper(c);
    });
    return str;
}

void Recepcionist::stop(){
    Employee::stop();
}

std::string Recepcionist::identify() const {
     return "Recepcionist "+std::to_string(this->id);

}