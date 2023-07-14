#pragma once
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



struct Header {
    char msg_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
} __attribute__((packed));

struct TradeMessage {
    Header header;
    uint64_t order_ref_number;
    char buy_sell;
    uint32_t shares;
    char stock[8];
    uint32_t price;
    uint64_t match_number;

    void reverseEndian() {
        match_number = __builtin_bswap64(match_number);
        header.timestamp = __builtin_bswap64(header.timestamp);
        header.stock_locate = __builtin_bswap16(header.stock_locate);
        header.tracking_number = __builtin_bswap16(header.tracking_number);
        shares = __builtin_bswap32(shares);
        price = __builtin_bswap32(price);
        order_ref_number = __builtin_bswap64(order_ref_number);

    }
} __attribute__((packed));