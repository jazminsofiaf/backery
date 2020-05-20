#include "Sourdough.h"

Sourdough::Sourdough(std::string channel_name, int pizza, int bread): Employee(0), dough_index(1) {
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

    std::cout << "[Sourdough] por abrir fifos de ordenes de masa" << std::endl;
	this->dough_order_channel->abrir();

	int n=1;
    std::cout << "[Sourdough] por abrir fifos de escritura" << std::endl;
    for (FifoEscritura * write_channel : this->write_channels_sorted){
        std::cout << "[Sourdough] por abrir"<< n++ << std::endl;
        write_channel->abrir();
        std::cout << "[Sourdough] fifos de escritura abierto"<< n++ << std::endl;
    }

    Sourdough::DoughOrder dough_order;
    int read_bytes_order = this->dough_order_channel->leer(&dough_order, sizeof(Sourdough::DoughOrder));
    //std::cout << "[Sourdough] despues de abrir fifo" << sigusr_handler.getGracefulQuit() << std::endl;
	while( read_bytes_order > 0 && sigusr_handler.getGracefulQuit() == 0 ) {

        if(read_bytes_order < 0 ){
            std::cerr << "[Sourdough] error at read dough orders channel " << std::endl;
            break;
        }
        std::cout << "[Sourdough] read " << read_bytes_order << dough_order.toString() << std::endl;

        std::string key = dough_order.product+std::to_string(dough_order.cook_id);
        if (!this->all_write_channels.count(key)){
            std::cerr << "[Sourdough] not valid dough order " << key <<std::endl;
            continue;
        }
        this->logger->log(this, " receive " + dough_order.toString());
	    FifoEscritura * write_channel = this->all_write_channels[key];
        this->cookSourDough(write_channel, dough_order.order_id);

        read_bytes_order = this->dough_order_channel->leer(&dough_order, sizeof(Sourdough::DoughOrder));

	}
	//std::cout << "[Sourdough] loop ends " << sigusr_handler.getGracefulQuit() << std::endl;
    for (FifoEscritura * write_channel : this->write_channels_sorted){
        write_channel->close_fifo();
        write_channel->eliminar();
    }
    SignalHandler::destruir();
}

void Sourdough::cookSourDough(FifoEscritura * write_channel, int num){

    Sourdough::Dough dough;
    dough.num = num;
    dough.index = this->dough_index ++;
    std::cout << "[Sourdough] looping write " << dough.toString() << std::endl;
    this->logger->log(this, " sending " + dough.toString());
    write_channel->escribir(static_cast<const void *>(&dough), sizeof(Sourdough::Dough));
}

void Sourdough::waitMe(){
    //std::cout << "[Sourdough] stoping..." <<this->process_id << std::endl;
	kill(this->process_id, SIGUSR1);
	Employee::waitMe();
}

Sourdough :: ~Sourdough() {
    //std::cout << "calling sourfough detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    delete this->dough_order_channel;
    for (FifoEscritura * write_channel : this->write_channels_sorted ){
        delete write_channel;
    }
}
