#include "CookerMan.h"


CookerMan::CookerMan(int id_num,  Logger * logger, 
                                    std::string sourdough_channel_name, 
                                    std::string orders_channel_name, 
                                    std::string delivery_channel_name): 
    Employee(id_num),
    logger(logger),
    sourdough_channel_name(sourdough_channel_name),
    orders_channel_name(orders_channel_name),
    delivery_channel_name(delivery_channel_name){}

void CookerMan::run(){
    
    this->orders_channel = new FifoLectura(this->orders_channel_name);
	this->orders_channel->abrir(); //blocked until recepcionist opem for write

    this->sourdough_channel = new FifoLectura(this->sourdough_channel_name); 
	this->sourdough_channel->abrir(); //blocked until sourdough opem for write

    this->delivery_channel = new FifoEscritura(this->delivery_channel_name);
	this->delivery_channel->abrir(); //blocked until sourdough opem for read

	Recepcionist::Order order;
    CookerMan::Product product;
    std::cout << this->identify() << " a punto de leer orden la primera vez " <<std::endl;
	size_t read_bytes_order = orders_channel->leer(&order,sizeof(Recepcionist::Order));
    std::cout << this->identify() << "reading orders fifo: " << static_cast<int>(read_bytes_order) << std::strerror(errno) << std::endl;
	while(read_bytes_order > FIFO_EOF ){
		this->logger->log(this, order.toString());
        std::cout << this->identify() << order.toString() << endl;

        Sourdough::Dough dough_piece;
	    size_t read_bytes_dough = this->sourdough_channel->leer(&dough_piece, sizeof(Sourdough::Dough));

        if(read_bytes_dough == FIFO_EOF || read_bytes_dough == ERROR){
            std::cout << this->identify() << " Error reading sourdough fifo: " << static_cast<int>(read_bytes_dough) << std::strerror(errno) << std::endl;
            throw std::runtime_error("Error reading sourdough fifo");   
        } 
       
        std::cout << this->identify() << " took dough pice " << dough_piece.toString() <<std::endl;
        this->logger->log(this, " took dough pice " + dough_piece.toString());
            
 
        product.order = order;
        product.dough = dough_piece;
        std::cout << "por escribir "<< product.toString()<<endl;
        std::cout << "escribiendo "<< this->delivery_channel->escribir(&product, sizeof(CookerMan::Product)) << endl;
            
        std::cout << this->identify() << " a punto de leer orden" <<std::endl;
        read_bytes_order = orders_channel->leer(&order,sizeof(Recepcionist::Order));
    } 
    if(read_bytes_order == ERROR){
        throw std::runtime_error("Error reading orders fifo");     
    } 
    std::cout << this->identify() << " loop ends " << std::endl;
    //this->sourdough_channel->close_fifo();
    this->orders_channel->close_fifo();	 
    //this->orders_channel->eliminar();
    std::cout << this->identify() << " loop ends FIN " << std::endl;
}



void CookerMan::stop(){
    Employee::stop();
}
 