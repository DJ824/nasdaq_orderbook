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
        main_book->process_msg(it.get());
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration);
    std::cout << "seconds: " << seconds.count() << std::endl;


    return 0;
}