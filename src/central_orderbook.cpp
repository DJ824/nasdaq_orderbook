//
// Created by Devang Jaiswal on 7/25/24.
//

#include "../include /central_orderbook.h"

central_orderbook::central_orderbook() {
    central_book_[21] = new orderbook();
    central_book_[464] = new orderbook();
    central_book_[6603] = new orderbook();
}

void central_orderbook::send_add_msg(add_message *add_message) {
    uint32_t key = add_message->locate_;
    order *new_order = new order(add_message->time_, add_message->size_, add_message->price_, add_message->id_,
                                 add_message->side_, add_message->locate_);
    central_book_[key]->add_order(new_order);
}

void central_orderbook::send_execute_msg(execute_message *execute_message) {
    uint32_t key = execute_message->locate_;
    central_book_[key]->execute_order(execute_message->id_, execute_message->size_, execute_message->locate_);
}

void central_orderbook::send_execute_price_msg(execute_order_price_message *execute_price_message) {
    uint32_t key = execute_price_message->locate_;
    central_book_[key]->execute_order_price(execute_price_message->id_, execute_price_message->size_,
                                            execute_price_message->price_, execute_price_message->locate_);
}

void central_orderbook::send_cancel_msg(cancel_message *cancel_message) {
    uint32_t key = cancel_message->locate_;
    central_book_[key]->cancel_order(cancel_message->id_, cancel_message->size_, cancel_message->locate_);
}

void central_orderbook::send_delete_msg(delete_message *delete_message) {
    uint32_t key = delete_message->locate_;
    central_book_[key]->delete_order(delete_message->id_, delete_message->locate_);
}

void central_orderbook::send_replace_msg(replace_message *replace_message) {
    uint32_t key = replace_message->locate_;
    central_book_[key]->replace_order(replace_message->id_, replace_message->new_id_, replace_message->price_, replace_message->size_, replace_message->locate_);
}

void central_orderbook::process_msg(message *msg) {
    if (msg->type == 'A' || msg->type == 'F') {
        send_add_msg(dynamic_cast<add_message *>(msg));
    }

    if (msg->type == 'E') {
        send_execute_msg(dynamic_cast<execute_message*>(msg));
    }

    if (msg->type == 'C') {
        send_execute_price_msg(dynamic_cast<execute_order_price_message*>(msg));
    }

    if (msg->type == 'X') {
        send_cancel_msg(dynamic_cast<cancel_message*>(msg));
    }

    if (msg->type == 'D') {
        send_delete_msg(dynamic_cast<delete_message*>(msg));
    }

    if (msg->type == 'U') {
        send_replace_msg(dynamic_cast<replace_message*>(msg));
    }
}


