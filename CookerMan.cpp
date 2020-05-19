#include "CookerMan.h"


CookerMan::CookerMan(int id_num,
                                    std::string sourdough_channel_name, 
                                    std::string orders_channel_name, 
                                    std::string delivery_channel_name): 
    Employee(id_num),
    sourdough_channel_name(sourdough_channel_name),
    orders_channel_name(orders_channel_name),
    delivery_channel_name(delivery_channel_name){

    this->orders_channel = new FifoShared(this->orders_channel_name);
    this->sourdough_channel = new FifoShared(this->sourdough_channel_name);
    this->delivery_channel = new FifoEscritura(this->delivery_channel_name);
    this->logger = new Logger();
}

void CookerMan::run(){

    this->orders_channel->abrir(); //blocked until receptionist open for write
	this->sourdough_channel->abrir(); //blocked until sourdough open for write
	this->delivery_channel->abrir(); //blocked until delivery open for read


	Receptionist::Order order;
    CookerMan::Product product;
    
    //std::cout << this->identify() << " a punto de leer orden la primera vez " << std::strerror(errno) <<std::endl;
	size_t read_bytes_order = orders_channel->leer(&order, sizeof(Receptionist::Order), this->identify());
    std::cout << this->identify() << " despues de leer la primera vez fifo: "<<this->orders_channel_name << static_cast<int>(read_bytes_order) << std::strerror(errno) << std::endl;
	while(read_bytes_order > FIFO_EOF ){ // until receptionist  close other side

        
        this->logger->log(this, order.toString());
        //std::cout << this->identify() << order.toString() << endl;

        Sourdough::Dough dough_piece;
	    size_t read_bytes_dough = this->sourdough_channel->leer(&dough_piece, sizeof(Sourdough::Dough), this->identify());

        if(read_bytes_dough == FIFO_EOF || read_bytes_dough == ERROR){
            //std::cout << this->identify() << " Error reading sourdough fifo: " << static_cast<int>(read_bytes_dough) << std::strerror(errno) << std::endl;
            break; 
        } 
       
        //std::cout << this->identify() << " took dough pice " << dough_piece.toString() << std::endl;
        this->logger->log(this, " took dough pice " + dough_piece.toString());

        product.order = order;
        product.dough = dough_piece;
        product.made_by = this->identify();
        //std::cout << "por escribir "<< product.toString()<<endl;
        this->delivery_channel->escribir(&product, sizeof(CookerMan::Product));
        //std::cout << "escribiendo "<<  << endl;
            
        //std::cout << this->identify() << " a punto de leer orden" <<std::endl;
        read_bytes_order = orders_channel->leer(&order,sizeof(Receptionist::Order), this->identify());
    } 
    if(read_bytes_order == ERROR){
        std::cout << this->identify() << " Error reading orders fifo: " << std::strerror(errno) << std::endl;      
    } 
    //std::cout << this->identify() << " loop ends " << std::endl;
    std::cout << this->identify() << " loop ends FIN " << std::endl;
    this->delivery_channel->close_fifo(); //signal to end delivery loop
    this->sourdough_channel->close_fifo();
    this->orders_channel->close_fifo();
    this->orders_channel->eliminar();
    std::cout << this->identify() << " channels closed " << std::endl;
}


void CookerMan::waitMe(){
    Employee::waitMe();
}


CookerMan::~CookerMan(){
    delete this->orders_channel;
    delete this->sourdough_channel;
    delete this->delivery_channel;
    delete this->logger;
}

  