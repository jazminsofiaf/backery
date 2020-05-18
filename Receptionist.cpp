
#include "Receptionist.h"
Receptionist::Receptionist(int id_num,
                           std::string bread_name, std::string pizza_name,
                           int start, int end, std::string file_name)
                            : Employee(id_num), 
                            bread_channel_name(bread_name), pizza_channel_name(pizza_name),
                            read_start(start), read_end(end), orders_file(file_name){
                        
    this->bread_channel = new FifoEscritura(this->bread_channel_name);
    this->pizza_channel = new FifoEscritura(this->pizza_channel_name);
    this->channel_map[BREAD] = this->bread_channel;
    this->channel_map[PIZZA] = this->pizza_channel;
    this->logger = new Logger();
    std::cout << "[RECEPCIONISTA]" << std::strerror(errno) << std::endl;
}

void Receptionist::init(){
    std::cout << "[RECEPCIONISTA]" << std::strerror(errno) << std::endl;
    this->bread_channel->abrir();//block until at least one cooker man open read side
    this->pizza_channel->abrir(); //block until at least one cooker man open read side
}

void Receptionist::run(){

    SharedFile file(this->orders_file, this->read_start, this->read_end);

    file.getSharedLock();
    int pos = this->read_start;
    std::string order = "";
    char c;
    while (pos < this->read_end && file.getChar(c) ){
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
    this->bread_channel->close_fifo(); //signal to stop loop for baker man
    this->pizza_channel->close_fifo(); //signal to stop loop for pizza man
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
        order.product = str_order;
        channel->escribir(&order, sizeof(Receptionist::Order));
        this->logger->log(this, order.toString());
        std::cout << this->identify() << order.toString() << endl;
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
     return "Receptionist "+std::to_string(this->id);

}
Receptionist::~Receptionist(){
    std::cout << "calling recepcionist detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    delete this->bread_channel;
    delete this->pizza_channel;
    delete this->logger;
}

