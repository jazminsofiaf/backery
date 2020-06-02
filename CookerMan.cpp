#include "CookerMan.h"



CookerMan::CookerMan(int id_num,
                                    FifoEscritura * dough_order_channel,
                                    FifoLectura *  sourdough_channel,
                                    std::string orders_channel_name,
                                    std::string delivery_channel_name): 
    Employee(id_num),
    dough_order_channel(dough_order_channel),
    sourdough_channel(sourdough_channel){

    this->orders_channel = new FifoShared(orders_channel_name);
    this->delivery_channel = new FifoEscritura(delivery_channel_name);
    this->logger = new Logger();
}

void CookerMan::run(){
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler ( SIGINT,&sigint_handler, 0);

    this->orders_channel->abrir(); //blocked until receptionist open for write
	this->delivery_channel->abrir(); //blocked until delivery open for read


	Receptionist::Order order;
    Sourdough::DoughOrder dough_order;
    Sourdough::Dough dough_piece;
    CookerMan::Product product;

    char order_msg[ORDER_SIZE];
    int read_bytes_order = this->orders_channel->leer(&order_msg, ORDER_SIZE);
    order.deserialize(order_msg);
	while(read_bytes_order > FIFO_EOF && sigint_handler.getGracefulQuit() == 0){ // until receptionist  close other side

        this->logger->log(this, " gets "+ order.toString());

        dough_order.order_id = order.id;
        dough_order.cook_id = this->id;
        std::string meal = this->getMeal();
        memset(dough_order.product, '\0', sizeof(dough_order.product));
        std::copy(meal.begin(), meal.end(), dough_order.product);

        this->logger->log( this, " ask for piece of dough. "+ dough_order.toString());
        this->dough_order_channel->escribir(static_cast<const void *>(dough_order.serialize()), DOUGH_ORDER_SIZE);

        char dough_msg[DOUGH_SIZE];
	    size_t read_bytes_dough = this->sourdough_channel->leer(&dough_msg, DOUGH_SIZE);
        dough_piece.deserialize(dough_msg);

        if(read_bytes_dough == FIFO_EOF || read_bytes_dough == ERROR || sigint_handler.getGracefulQuit() != 0 ){
            break; 
        } 

        this->logger->log(this, " took dough pice " + dough_piece.toString());

        product.order = order;
        product.dough = dough_piece;
        product.cook_id = this->id;
        this->delivery_channel->escribir(static_cast<const void *>(product.serialize()), PRODUCT_SIZE);

        read_bytes_order = this->orders_channel->leer(&order_msg, ORDER_SIZE);
        order.deserialize(order_msg);
    } 
    if(read_bytes_order == ERROR){
        std::cerr << this->identify() << " Error reading orders fifo: " << std::strerror(errno) << std::endl;
    }
    this->delivery_channel->close_fifo(); //signal to end delivery loop
    this->sourdough_channel->close_fifo();
    this->orders_channel->close_fifo();
    this->orders_channel->eliminar();
    SignalHandler::destruir();
}


void CookerMan::waitMe(){
    Employee::waitMe();
}


CookerMan::~CookerMan(){
    delete this->orders_channel;
    delete this->delivery_channel;
    delete this->logger;
}

  