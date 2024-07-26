//
// Created by Devang Jaiswal on 7/25/24.
//

#include "../include /orderbook.h"

orderbook::orderbook() : bids_(), offers_(), limit_lookup_(), order_lookup_() {}

void orderbook::add_order(order *new_order) {
    limit *curr_limit = get_or_insert_limit(new_order->side_, new_order->price_);
    order_lookup_[new_order->id_] = new_order;
    curr_limit->add_order(new_order);
    std::cout << "added order with id: " << new_order->id_ << " at price: " << new_order->price_ << " at limit price: "
              << curr_limit->price_ << std::endl;
}

void orderbook::execute_order(uint64_t id, uint32_t size, uint32_t locate) {
    order *target = order_lookup_[id];
    bool side = target->side_;
    uint32_t price = target->price_;
    auto current_limit = target->parent_;
    target->remove_qty(size);
    current_limit->volume_ -= size;
    if (target->size_ == 0) {
        current_limit->remove_order(target);
        if (current_limit->volume_ == 0) {
            if (side) {
                bids_.erase(price);
            } else {
                offers_.erase(price);
            }
            std::pair<uint32_t, bool> key = std::make_pair(price, side);
            limit_lookup_.erase(key);
            target->parent_ = nullptr;
        }
        order_lookup_.erase(target->id_);
        std::cout << "fully filled order with id: " << target->id_ << "at price: " << current_limit->price_
                  << " removing from book" << std::endl;
        delete target;
    }
}

void orderbook::execute_order_price(uint64_t id, uint32_t size, uint32_t price, uint32_t locate) {
    order *target = order_lookup_[id];
    bool side = target->side_;
    uint32_t curr_price = target->price_;
    auto current_limit = target->parent_;
    target->remove_qty(size);
    current_limit->volume_ -= size;
    std::cout << "order id: " << target->id_ << " filled for " << size << " shares" << std::endl;
    if (target->size_ == 0) {
        current_limit->remove_order(target);
        if (current_limit->volume_ == 0) {
            if (side) {
                bids_.erase(curr_price);
            } else {
                offers_.erase(curr_price);
            }
            std::pair<uint32_t, bool> key = std::make_pair(curr_price, side);
            limit_lookup_.erase(key);
            target->parent_ = nullptr;
        }
        order_lookup_.erase(target->id_);
        std::cout << "fully filled order id: " << target->id_ << "at price: " << current_limit->price_
                  << " removing from book" << std::endl;
        delete target;
    }
}

void orderbook::cancel_order(uint64_t id, uint32_t size, uint32_t locate) {
    order *target = order_lookup_[id];
    bool side = target->side_;
    uint32_t curr_price = target->price_;
    auto current_limit = target->parent_;
    target->remove_qty(size);
    current_limit->volume_ -= size;
    std::cout << "order id: " << target->id_ << " reduced size by " << size << " shares" << std::endl;
    if (target->size_ == 0) {
        current_limit->remove_order(target);
        if (current_limit->volume_ == 0) {
            if (side) {
                bids_.erase(curr_price);
            } else {
                offers_.erase(curr_price);
            }
            std::pair<uint32_t, bool> key = std::make_pair(curr_price, side);
            limit_lookup_.erase(key);
            target->parent_ = nullptr;
        }
        order_lookup_.erase(target->id_);
        std::cout << "fully filled order with id: " << target->id_ << "at price: " << current_limit->price_
                  << " removing from book" << std::endl;
        delete target;
    }
}

void orderbook::delete_order(uint64_t id, uint32_t locate) {
    order *target = order_lookup_[id];
    bool side = target->side_;
    auto curr_limit = target->parent_;
    auto curr_price = target->price_;
    curr_limit->remove_order(target);
    if (curr_limit->num_orders_ == 0) {
        if (side) {
            bids_.erase(curr_price);
        } else {
            offers_.erase(curr_price);
        }
        std::pair<uint32_t, bool> key = std::make_pair(curr_price, side);
        limit_lookup_.erase(key);
        target->parent_ = nullptr;
    }
    order_lookup_.erase(target->id_);
    std::cout << "deleted order id: " << target->id_ << " at price: " << target->price_ << " removing from book"
              << std::endl;
    delete target;
}

void orderbook::replace_order(uint64_t old_id, uint64_t new_id, uint32_t price, uint32_t size, uint32_t locate) {
    order *target = order_lookup_[old_id];
    bool side = target->side_;
    auto curr_limit = target->parent_;
    auto curr_price = target->price_;
    auto old_size = target->size_;
    curr_limit->remove_order(target);
    if (curr_limit->num_orders_ == 0) {
        if (side) {
            bids_.erase(curr_price);
        } else {
            offers_.erase(curr_price);
        }
        std::pair<uint32_t, bool> key = std::make_pair(curr_price, side);
        limit_lookup_.erase(key);
        target->parent_ = nullptr;
    }
    target->id_ = new_id;
    target->price_ = price;
    target->size_ = size;
    add_order(target);
    std::cout << "modified order id: " << target->id_ << " old price: " << curr_price << " new price: "
              << target->price_ << " old size: " << old_size << " new size: " << target->size_ << std::endl;
}

limit *orderbook::get_or_insert_limit(bool side, uint32_t price) {
    std::pair<uint32_t, bool> key = std::make_pair(price, side);
    auto it = limit_lookup_.find(key);
    if (it == limit_lookup_.end()) {
        limit *new_limit = new limit(price);
        if (side) {
            bids_[price] = new_limit;
        } else {
            offers_[price] = new_limit;
        }
        limit_lookup_[key] = new_limit;
        return new_limit;
    } else {
        return it->second;
    }

}
