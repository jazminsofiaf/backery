#include "Baker.h"


Baker::Baker(int id_num,
        FifoEscritura * dough_order_channel,
        FifoLectura * sourdough_channel,
        std::string orders_channel_name,
        std::string delivery_channel_name)
: CookerMan(id_num, dough_order_channel, sourdough_channel, orders_channel_name, delivery_channel_name) {}

std::string Baker::identify() const {
    return "Baker "+std::to_string(this->id);
}
std::string Baker::getMeal() const {
    return BREAD;
}

Baker:: ~Baker(){}



