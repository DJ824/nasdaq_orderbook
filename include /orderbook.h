//
// Created by Devang Jaiswal on 7/25/24.
//

#ifndef NASDAQ_ORDERBOOK_ORDERBOOK_H
#define NASDAQ_ORDERBOOK_ORDERBOOK_H
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "order.h"
#include "limit.h"
//typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<2>> dec_float;



class orderbook {
private:
    std::map<uint32_t , limit *, std::greater<>> bids_;
    std::map<uint32_t , limit *, std::less<>> offers_;
    std::unordered_map<std::pair<uint32_t, bool>, limit*, boost::hash<std::pair<uint32_t, bool>>> limit_lookup_;
    std::unordered_map<uint64_t, order*> order_lookup_;
    std::shared_ptr<spdlog::logger> async_logger;


public:
    orderbook();
    void add_order(order* new_order) ;
    void execute_order(uint64_t id, uint32_t size, uint32_t locate);
    void execute_order_price(uint64_t id, uint32_t size, uint32_t price, uint32_t locate);
    void cancel_order(uint64_t id, uint32_t size, uint32_t locate);
    void delete_order(uint64_t id, uint32_t locate);
    void replace_order(uint64_t old_id, uint64_t new_id, uint32_t price, uint32_t size, uint32_t locate);
    limit* get_or_insert_limit(bool side, uint32_t price);

};


#endif
