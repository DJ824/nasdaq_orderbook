

#ifndef NASDAQ_ORDERBOOK_PARSER_H
#define NASDAQ_ORDERBOOK_PARSER_H

#include <string>
#include <cstdint>
#include <charconv>
#include <vector>
#include <memory>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "message.h"
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<2>> dec_float;

class parser {
public:
    explicit parser(const std::string &file_path);
    void parse();
    std::vector<std::unique_ptr<message>> message_stream;

private:
    std::string file_path_;
    void parse_line(const std::string &line);
    uint32_t fast_float_to_int(const std::string& s);
};

#endif