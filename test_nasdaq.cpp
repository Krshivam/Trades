#include <iostream>
#include <fstream>
#include <cstring>
#include <zlib.h>

struct ITCHHeader {
    char msgType;
    uint16_t msgSize;
    uint64_t timeStamp;
};

struct ITCHTrade {
    char msgType;
    uint16_t msgSize;
    uint64_t timeStamp;
    uint64_t orderRefNum;
    char buySellIndicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
    uint64_t matchNum;
};

int main(int argc, char* argv[]) {
    // Check for file argument
    const char* filename = "01302019.NASDAQ_ITCH50.gz";
    // if (argc < 2) {
    //     std::cout << "Usage: " << argv[0] << " [FILEPATH]\n";
    //     return 1;
    // }

    // Open gz file

    // ptime tim = from_nanos_std(temp);
    // int hr = tim.time_of_day().hours();
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file\n";
        return 1;
    }

    // Decompress gz file
    gzFile gz = gzopen(filename, "rb");
    if (gz == NULL) {
        std::cout << "Failed to open gz file\n";
        return 1;
    }

    // Read and parse file data
    char buffer[256];
    uint64_t offset = 0;
    while (gzgets(gz, buffer, 256) != NULL) {
        // Parse ITCH message header
        ITCHHeader header;
        std::memcpy(&header, buffer, sizeof(ITCHHeader));

        // Parse ITCH message data
        switch (header.msgType) {
            case 'P': {
                ITCHTrade trade;
                std::memcpy(&trade, buffer + sizeof(ITCHHeader), sizeof(ITCHTrade));
                if (trade.buySellIndicator == 'B') {
                    std::cout << "Buy Trade: " << trade.stock << " " << trade.shares << " shares at $" << (double)trade.price / 10000 << " (Match #" << trade.matchNum << ")\n";
                } else if (trade.buySellIndicator == 'S') {
                    std::cout << "Sell Trade: " << trade.stock << " " << trade.shares << " shares at $" << (double)trade.price / 10000 << " (Match #" << trade.matchNum << ")\n";
                }
                // offset += sizeof(ITCHHeader) + sizeof(ITCHTrade);
                break;
            }
            default:
                // offset += header.msgSize + sizeof(ITCHHeader);
                break;
        }

        // Seek to next message
        // gzseek(gz, offset, SEEK_SET);
    }

    // Close gz file
    gzclose(gz);

    // Close file
    file.close();

    return 0;
}
