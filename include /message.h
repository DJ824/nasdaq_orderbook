#ifndef NASDAQ_ORDERBOOK_MESSAGE_H
#define NASDAQ_ORDERBOOK_MESSAGE_H
#include <cstdint>
#include <memory>
#include <boost/multiprecision/cpp_dec_float.hpp>
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<2>> dec_float;

struct message {
    uint64_t id_;
    uint32_t size_;
    char type;
    uint32_t locate_;
    uint64_t time_;

    message(uint64_t id, uint32_t size, char type, uint32_t locate, uint64_t time)
            : id_(id), size_(size), type(type), locate_(locate), time_(time) {}

    virtual ~message() = default;
};

struct add_message : message {
    bool side_;
    uint32_t price_;

    add_message(uint64_t id, uint32_t size, char type, uint32_t locate, uint64_t time, bool side, dec_float price)
            : message(id, size, type, locate, time), side_(side), price_(price) {}
};

struct execute_message : message {
    execute_message(uint64_t id, uint32_t size, char type, uint32_t locate, uint64_t time)
            : message(id, size, type, locate, time) {}
};

struct execute_order_price_message : message {
    uint32_t price_;

    execute_order_price_message(uint64_t id, uint32_t size, char type, uint32_t locate, uint64_t time, dec_float price)
            : message(id, size, type, locate, time), price_(price) {}
};

struct cancel_message : message {
    cancel_message(uint64_t id, uint32_t size, char type, uint32_t locate, uint64_t time)
            : message(id, size, type, locate, time) {}
};

struct delete_message : message {
    delete_message(uint64_t id, char type, uint32_t locate, uint64_t time)
            : message(id, 0, type, locate, time) {}
};

struct replace_message : message {
    uint64_t new_id_;
    uint32_t price_;

    replace_message(uint64_t id, uint32_t size, char type, uint32_t locate, uint64_t time, uint64_t new_id, dec_float price)
            : message(id, size, type, locate, time), new_id_(new_id), price_(price) {}
};

#endif