#include "CookerMan.h"


CookerMan::CookerMan(int id_num,    std::string sourdough_channel_name, 
                                    std::string orders_channel_name, 
                                    std::string delivery_channel_name): 
    Employee(id_num),
    sourdough_channel_name(sourdough_channel_name),
    orders_channel_name(orders_channel_name),
    delivery_channel_name(delivery_channel_name){}

void CookerMan::run(){
    
    this->orders_channel = new FifoShared(this->orders_channel_name);
	this->orders_channel->abrir(); //blocked until recepcionist opem for write

    this->sourdough_channel = new FifoShared(this->sourdough_channel_name); 
	this->sourdough_channel->abrir(); //blocked until sourdough opem for write

    this->delivery_channel = new FifoEscritura(this->delivery_channel_name);
	this->delivery_channel->abrir(); //blocked until sourdough opem for read

	Recepcionist::Order order;
    std::cout << this->identify() << " a punto de leer orden la primera vez " <<std::endl;
	size_t read_bytes_order = orders_channel->leer(&order,sizeof(Recepcionist::Order));
	while(read_bytes_order > FIFO_EOF ){
		Logger::log(this, order.toString());
        std::cout << this->identify() << order.toString() << endl;

        Sourdough::Dough dough_piece;
	    size_t read_bytes_dough = this->sourdough_channel->leer(&dough_piece,sizeof(Sourdough::Dough));

        if(read_bytes_dough > FIFO_EOF ){
		    std::cout << this->identify() << " took dough pice " << dough_piece.toString() <<std::endl;
            Logger::log(this, " took dough pice " + dough_piece.toString());
            
            
            CookerMan::Product product;
            product.order = order;
            product.dough = dough_piece;
            this->delivery_channel->escribir(&product, sizeof(CookerMan::Product));
            

        } 
        if(read_bytes_dough == ERROR){
            std::cout << this->identify() << " error " <<  read_bytes_dough  << std::endl;
            throw std::runtime_error("Error reading sourdough fifo ");     
        }

        std::cout << this->identify() << " a punto de leer orden" <<std::endl;
        read_bytes_order = orders_channel->leer(&order,sizeof(Recepcionist::Order));
    } 
    if(read_bytes_order == ERROR){
        throw std::runtime_error("Error reading orders fifo");     
    } 
    std::cout << this->identify() << " loop ends " << std::endl;
    this->sourdough_channel->close_fifo();
    this->orders_channel->close_fifo();	 
    //this->orders_channel->eliminar();
    std::cout << this->identify() << " loop ends FIN " << std::endl;
}



void CookerMan::stop(){
    Employee::stop();
}
 