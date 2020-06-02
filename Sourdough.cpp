#include "Sourdough.h"

Sourdough::Sourdough(std::string channel_name, int pizza, int bread): Employee(0), dough_index(1), dough_size(0){
    this->logger = new Logger();
    
    this->dough_order_channel = new FifoLectura(channel_name);
    for(int index = 1; index <= pizza; index++ ){
        FifoEscritura * pizza_fifo = new FifoEscritura(PIZZA + std::to_string(index));
        this->all_write_channels[PIZZA + std::to_string(index)] = pizza_fifo;
        this->write_channels_sorted.push_back(pizza_fifo);
    }
    for(int index = 1; index <= bread; index++ ){
        FifoEscritura * bread_fifo = new FifoEscritura(BREAD + std::to_string(index));
        this->all_write_channels[BREAD + std::to_string(index)] = bread_fifo;
        this->write_channels_sorted.push_back(bread_fifo);

    }
}

std::string Sourdough::identify() const {
    return "Sourdough";
}

void Sourdough::run(){
    SIGUSR_Handler sigusr_handler;
	SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr_handler,0);

	this->dough_order_channel->abrir();

    for (FifoEscritura * write_channel : this->write_channels_sorted){
        write_channel->abrir();
    }

    Sourdough::DoughOrder dough_order;
    char dough_order_msg[DOUGH_ORDER_SIZE];
    int read_bytes_order = this->dough_order_channel->leer(&dough_order_msg, DOUGH_ORDER_SIZE);
    dough_order.deserialize(dough_order_msg);

	while( read_bytes_order > 0 && sigusr_handler.getGracefulQuit() == 0 ) {
        this->feedDough();
        std::string key = dough_order.product+std::to_string(dough_order.cook_id);
        if (!this->all_write_channels.count(key)){
            continue;
        }
        this->logger->log(this, " receive " + dough_order.toString());
	    FifoEscritura * write_channel = this->all_write_channels[key];
        this->cookSourDough(write_channel, dough_order.order_id);

        char dough_order_msg[DOUGH_ORDER_SIZE];
        read_bytes_order = this->dough_order_channel->leer(&dough_order_msg, DOUGH_ORDER_SIZE);
        dough_order.deserialize(dough_order_msg);
	}

    for (FifoEscritura * write_channel : this->write_channels_sorted){
        write_channel->close_fifo();
        write_channel->eliminar();
    }
    this->dough_order_channel->close_fifo();
    SignalHandler::destruir();
}

void Sourdough::feedDough(){
    this->dough_size = this->dough_size + 5;
    Sourdough::Dough dough;
    dough.num = 0;
    dough.index = this->dough_index;
    dough.dough_size = this->dough_size;
    this->logger->log(this, " feeding " + dough.toString());
}

void Sourdough::cookSourDough(FifoEscritura * write_channel, int num){
    Sourdough::Dough dough;
    dough.num = num;
    dough.index = this->dough_index ++;
    this->logger->log(this, " sending " + dough.toString());
    write_channel->escribir(static_cast<const void *>(dough.serialize()), DOUGH_SIZE);
}

void Sourdough::waitMe(){
	kill(this->process_id, SIGUSR1);
	Employee::waitMe();
}

Sourdough :: ~Sourdough() {
    delete this->dough_order_channel;
    for (FifoEscritura * write_channel : this->write_channels_sorted ){
        delete write_channel;
    }
}
