//
// Created by Devang Jaiswal on 7/25/24.
//

#include "../include /orderbook.h"
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


orderbook::orderbook() : bids_(), offers_(), limit_lookup_(), order_lookup_() {
    try {
        async_logger = spdlog::get("async_logger");

        if (!async_logger) {

            if (!spdlog::thread_pool()) {
                spdlog::init_thread_pool(8192, 1);
            }

            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/async_log.txt", true);

            std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};

            async_logger = std::make_shared<spdlog::async_logger>("async_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

            spdlog::register_logger(async_logger);
        }

        spdlog::set_default_logger(async_logger);

        spdlog::set_level(spdlog::level::info);

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

void orderbook::add_order(order *new_order) {
    limit *curr_limit = get_or_insert_limit(new_order->side_, new_order->price_);
    order_lookup_[new_order->id_] = new_order;
    curr_limit->add_order(new_order);
    spdlog::info("Added order with id: {} at price: {} at limit price: {}", new_order->id_, new_order->price_, curr_limit->price_);

}

void orderbook::execute_order(uint64_t id, uint32_t size, uint32_t locate) {
    order *target = order_lookup_[id];
    bool side = target->side_;
    uint32_t price = target->price_;
    auto current_limit = target->parent_;
    target->remove_qty(size);
    current_limit->volume_ -= size;
    spdlog::info("Order id: {} filled for {} shares", target->id_, size);

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
        spdlog::info("Fully filled order with id: {} at price: {}. Removing from book", target->id_, current_limit->price_);
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
    spdlog::info("Order id: {} filled for {} shares", target->id_, size);
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
       spdlog::info("Fully filled order id: {} at price: {}. Removing from book", target->id_, current_limit->price_);
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
    //spdlog::info("Order id: {} reduced size by {} shares", target->id_, size);
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
        spdlog::info("Cancelled order with id: {} at price: {}. Removing from book", target->id_, current_limit->price_);
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
    spdlog::info("Deleted order id: {} at price: {}. Removing from book", target->id_, target->price_);
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
   spdlog::info("Modified order id: {} old price: {} new price: {} old size: {} new size: {}", target->id_, curr_price, target->price_, old_size, target->size_);
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
