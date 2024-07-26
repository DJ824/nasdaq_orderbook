#include "../include /parser.h"
#include "../include /central_orderbook.h"
#include <iostream>
#include <chrono>

int main() {
    std::string path = "combined_output.csv";
    parser p(path);
    p.parse();
    std::cout << p.message_stream.size() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    central_orderbook* main_book = new central_orderbook();
    auto start = std::chrono::high_resolution_clock::now();
    int count = 0;
    for (const auto &it : p.message_stream) {
        ++count;
        //if (count == 3000000) break;
        main_book->process_msg(it.get());
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration);
    std::cout << "seconds: " << seconds.count() << std::endl;




/*
    for (const auto& msg : p.message_stream) {
        std::cout << "Message ID: " << msg.id_ << "\n";
        std::cout << "Size: " << msg.size_ << "\n";
        std::cout << "Type: " << msg.type << "\n";
        std::cout << "Locate: " << msg.locate_ << "\n";
        std::cout << "Time: " << msg.time_ << "\n";

        switch (msg.type) {
            case 'A':
            case 'F': {
                auto add_msg = static_cast<const add_message&>(msg);
                std::cout << "Side: " << (add_msg.side_ ? "Buy" : "Sell") << "\n";
                std::cout << "Price: " << add_msg.price_ << "\n";
                break;
            }
            case 'E': {
                auto exec_msg = static_cast<const execute_message&>(msg);
                std::cout << "Size: " << exec_msg.size_ << "\n";
                break;
            }
            case 'C': {
                auto exec_price_msg = static_cast<const execute_order_price_message&>(msg);
                std::cout << "Size: " << exec_price_msg.size_ << "\n";
                std::cout << "Price: " << exec_price_msg.price_ << "\n";
                break;
            }
            case 'X': {
                auto cancel_msg = static_cast<const cancel_message&>(msg);
                std::cout << "Size: " << cancel_msg.size_ << "\n";
                break;
            }
            case 'D': {
                auto delete_msg = static_cast<const delete_message&>(msg);
                break;
            }
            case 'U': {
                auto replace_msg = static_cast<const replace_message&>(msg);
                std::cout << "New ID: " << replace_msg.new_id_ << "\n";
                std::cout << "Price: " << replace_msg.price_ << "\n";
                break;
            }
            default:
                std::cerr << "Unknown message type: " << msg.type << std::endl;
                break;
        }

        std::cout << "--------------------\n";
    }
*/
    return 0;
}