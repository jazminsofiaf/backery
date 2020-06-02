
#include "Receptionist.h"
#include "Handlers/SIGINT_Handler.h"

Receptionist::Receptionist(int id_num,
                           FifoEscritura * bread_channel,
                           FifoEscritura * pizza_channel,
                           int start, int end, std::string file_name)
                            : Employee(id_num), 
                            bread_channel(bread_channel), pizza_channel(pizza_channel),
                            read_start(start), read_end(end), orders_file(file_name){

    this->channel_map[BREAD] = this->bread_channel;
    this->channel_map[PIZZA] = this->pizza_channel;
    this->logger = new Logger();
}

void Receptionist::run(){
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler ( SIGINT,&sigint_handler, 0);

    SharedFile file(this->orders_file, this->read_start, this->read_end);
    file.getSharedLock();
    int pos = this->read_start;
    std::string order = "";
    char c;
    while (pos < this->read_end && file.getChar(c) && sigint_handler.getGracefulQuit() == 0 ){
        if(this->isDelimiter(c)){
            this->tryToSend(order, pos);
            order = ""; 
        } else{
           order += c;
        }
        pos++;  
    }
    if( !order.empty() ){ //a  words cut
        while(file.getChar(c)){ //read last order
            if(this->isDelimiter(c)){
                this->tryToSend(order, pos);
                break;
            } else{
                order += c;
            }
        }
    }
    file.freeLock();
    SignalHandler::destruir();

}

bool Receptionist::isDelimiter(char c){
    return (c == '\n' || c == ' ' || c == ',' || c== '-' || c== ';');
}

void Receptionist::tryToSend(std::string str_order, int pos){
    str_order = this->toUpper(str_order);
    if(this->channel_map.count(str_order)){
        FifoEscritura * channel = this->channel_map[str_order];
        Receptionist::Order order;
        order.id = pos; //set last read position as order id
        memset(order.product, '\0', sizeof(order.product));
        std::copy(str_order.begin(), str_order.end(), order.product);
        channel->escribir(static_cast<const void *>(&order), sizeof(Receptionist::Order));
        this->logger->log(this, " gets new " + order.toString());

    }
}

std::string Receptionist::toUpper(std::string str){
    std::for_each(str.begin(), str.end(), [](char & c){
	    c = ::toupper(c);
    });
    return str;
}

void Receptionist::waitMe(){
    Employee::waitMe();
}

std::string Receptionist::identify() const {
     return "Receptionist "+ std::to_string(this->id);

}
Receptionist::~Receptionist(){
    delete this->logger;
}

