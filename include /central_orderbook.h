//
// Created by Devang Jaiswal on 7/25/24.
//

#ifndef NASDAQ_ORDERBOOK_CENTRAL_ORDERBOOK_H
#define NASDAQ_ORDERBOOK_CENTRAL_ORDERBOOK_H
#include <unordered_map>
#include "orderbook.h"
#include "message.h"

class central_orderbook {
private:
    std::unordered_map<uint32_t, orderbook*> central_book_;
    std::unordered_map<uint64_t , std::pair<uint32_t , order*>> central_order_lookup_;
public:
    central_orderbook();
    void send_add_msg(add_message* add_message);
    void send_execute_msg(execute_message* execute_message);
    void send_execute_price_msg(execute_order_price_message* execute_price_message);
    void send_cancel_msg(cancel_message* cancel_message);
    void send_delete_msg(delete_message* delete_message);
    void send_replace_msg(replace_message* replace_message);
    void process_msg(message* msg);

};

#endif


