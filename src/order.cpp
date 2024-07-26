//
// Created by Devang Jaiswal on 7/24/24.
//

#include "../include /order.h"


order::order(uint64_t time, uint32_t size, uint32_t price, uint64_t id, bool side, uint32_t locate) : time_(time),
                                                                                                   size_(size),
                                                                                                   price_(price),
                                                                                                   id_(id), side_(side),
                                                                                                   next_(nullptr),
                                                                                                   prev_(nullptr),
                                                                                                   parent_(nullptr),
                                                                                                   locate_(locate) {}

void order::remove_qty(uint32_t shares) {
    size_ -= shares;
}

