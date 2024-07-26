// parser.cpp

#include "../include /parser.h"
#include "../include /message.h"
#include <fstream>
#include <sstream>
#include <iostream>

parser::parser(const std::string &file_path) : file_path_(file_path) {
    message_stream.reserve(70000000);
}

uint32_t parser::fast_float_to_int(const std::string &s) {
    const char* start = s.data();
    const char* end = start + s.size();

    uint32_t integer_part = 0;
    uint32_t fractional_part = 0;
    int fractional_digits = 0;

    auto [ptr, ec] = std::from_chars(start, end, integer_part);

    if (ptr != end && *ptr == '.') {
        start = ptr + 1;
        std::from_chars(start, end, fractional_part);
        fractional_digits = end - start;
    }

    while (fractional_digits < 4) {
        fractional_part *= 10;
        fractional_digits++;
    }

    return integer_part * 10000 + fractional_part;
}

void parser::parse() {
    std::ifstream file(file_path_);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path_ << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        parse_line(line);
    }

    file.close();
}

void parser::parse_line(const std::string &line) {
    std::istringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return;
    }

    char type = tokens[0][0];
    uint32_t locate = std::stoul(tokens[1]);
    uint64_t time = std::stoull(tokens[3]);
    uint64_t id = std::stoull(tokens[4]);

    switch (type) {
        case 'A': {
            bool side = tokens[5] == "B" ? 1 : 0;
            uint32_t size = std::stoul(tokens[6]);
            uint32_t price = fast_float_to_int(tokens[8]);
            message_stream.push_back(std::make_unique<add_message>(id, size, type, locate, time, side, price));
            break;
        }
        case 'F': {
            bool side = tokens[5] == "B" ? 1 : 0;
            uint32_t size = std::stoul(tokens[6]);
            uint32_t price = fast_float_to_int(tokens[8]);
            message_stream.push_back(std::make_unique<add_message>(id, size, type, locate, time, side, price));
            break;
        }
        case 'E': {
            uint32_t size = std::stoul(tokens[5]);
            message_stream.push_back(std::make_unique<execute_message>(id, size, type, locate, time));
            break;
        }
        case 'C': {
            uint32_t size = std::stoul(tokens[5]);
            uint32_t price = fast_float_to_int(tokens[8]);
            message_stream.push_back(std::make_unique<execute_order_price_message>(id, size, type, locate, time, price));
            break;
        }
        case 'X': {
            uint32_t size = std::stoul(tokens[5]);
            message_stream.push_back(std::make_unique<cancel_message>(id, size, type, locate, time));
            break;
        }
        case 'D': {
            message_stream.push_back(std::make_unique<delete_message>(id, type, locate, time));
            break;
        }
        case 'U': {
            uint64_t new_id = std::stoull(tokens[5]);
            uint32_t size = std::stoul(tokens[6]);
            uint32_t price = fast_float_to_int(tokens[7]);
            message_stream.push_back(std::make_unique<replace_message>(id, size, type, locate, time, new_id, price));
            break;
        }
        default:
            std::cerr << "Unknown message type: " << type << std::endl;
            break;
    }
}