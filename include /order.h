//
// Created by Devang Jaiswal on 7/24/24.
//

#ifndef NASDAQ_ORDERBOOK_ORDER_H
#define NASDAQ_ORDERBOOK_ORDER_H
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <cstdint>
#include <string>
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<2>> dec_float;

class limit;


class order {
public:
    uint64_t time_;
    uint32_t locate_;
    uint32_t size_;
    uint32_t price_;
    uint64_t id_;
    bool side_;
    order* next_;
    order* prev_;
    limit* parent_;
    order(uint64_t time, uint32_t size, uint32_t price, uint64_t id, bool side, uint32_t locate);
    order();
    void remove_qty(uint32_t shares);


};


#endif
