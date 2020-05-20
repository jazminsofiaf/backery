
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
    std::cout << this->identify() << " shared lock from "<< this->read_start <<" to "<< this->read_end << endl;
    file.getSharedLock();
    int pos = this->read_start;
    std::string order = "";
    char c;
    while (pos < this->read_end && file.getChar(c) && sigint_handler.getGracefulQuit() == 0 ){
        if(this->isDelimiter(c)){
            std::cout << this->identify() << " es una orden? "<< order <<endl;
            this->tryToSend(order, pos);
            order = ""; 
        } else{
           order += c;
        }
        pos++;  
    }
    if( !order.empty() ){ //a  words cut
        std::cout << this->identify() << " palabra cortada: "<< order <<endl;
        while(file.getChar(c)){ //read last order
            if(this->isDelimiter(c)){
                std::cout << this->identify() << " palabra cortada es una orden? "<< order <<endl;
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
        std::cout <<this->identify() << " mandando orden" << order.toString() << endl;
        size_t sent = channel->escribir(static_cast<const void *>(&order), sizeof(Receptionist::Order));

        if ( sent > 0){
            std::cout <<this->identify() << " mandando orden " <<sent << order.toString() << endl;
        }else{
            std::cout <<this->identify() << "No se mando " <<sent << endl;
        }
        std::cout <<this->identify() << " orden mandada" << order.toString() << endl;
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
    //std::cout << "calling recepcionist detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    delete this->logger;
}

