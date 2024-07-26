//
// Created by Devang Jaiswal on 7/24/24.
//

#ifndef NASDAQ_ORDERBOOK_LIMIT_H
#define NASDAQ_ORDERBOOK_LIMIT_H
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <cstdint>
#include "order.h"
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<2>> dec_float;


class limit {
public:
    explicit limit(order* new_order);
    explicit limit(uint32_t price);
    uint32_t price_;
    uint64_t volume_;
    uint32_t num_orders_;
    order* head_;
    order* tail_;

    void add_order(order* new_order);
    void remove_order(order* target);
    uint32_t get_price();
    uint64_t get_volume();
    uint32_t get_size();
    bool is_empty();
    void reset();
    void set(uint32_t price);
};


#endif //NASDAQ_ORDERBOOK_LIMIT_H
