#include "msg_struct.h"
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>

#include <iostream>
#include <string>
#include <time.h>
#include <inttypes.h> // PRIux
#include <cstring>
#include <zlib.h>
using  namespace std;

// S:11,R:38,H:24,Y:19,L:25,V:34,W:11,K:27,J:34,h:20,A:35,F:39,E:30,C:35,X:22,D:18,U:34,P:43,Q:39,B:18,I:49,N:19

const int chunksize = 64;

int main() {
    
    string filename = "01302019.NASDAQ_ITCH50.gz";
    ifstream file(filename,ios::binary);
    // gzFile in = gzopen(filename,"rb");
    char buffer[64];
    char *offset = buffer;
    gzFile gz = gzopen(filename, "rb");
    if(gz==NULL) cout<<"Null Data"<<endl;
    // file.open(filename);
    int cnt = 0;
    // file.ignore(9);
    // char k;
    // file.get(k);
    // cout<<k<<endl;
    while(gz) {
        file.ignore(2);
        char k;
        file.get(k);
        if (k=='S') {
            file.read(buffer,11);
        }
        if(k=='R') file.read(buffer,38);
        if(k=='H') file.read(buffer,24);
        if(k=='Y') file.read(buffer,19);
        if(k=='L') file.read(buffer,25);
        if(k=='V') file.read(buffer,34);
        if(k=='W') file.read(buffer,11);
        if(k=='K') file.read(buffer,27);
        if(k=='J') file.read(buffer,34);
        if(k=='h') file.read(buffer,20);
        if(k=='A') file.read(buffer,35);
        if(k=='F') file.read(buffer,39);
        if(k=='E') file.read(buffer,30);
        if(k=='C') file.read(buffer,35);
        if(k=='X') file.read(buffer,22);
        if(k=='D') file.read(buffer,18);
        if(k=='U') file.read(buffer,34);
        if(k=='P') {
            file.read(buffer,43);
            TradeMessagesNC* trades = (TradeMessagesNC*) buffer;
            // char mt = trades->header.msg_type;
            uint16_t sl = __builtin_bswap16(trades->header.stock_locate);
            // uint16_t tn = __builtin_bswap16(trades->header.tracking_number);
            uint64_t ts = __builtin_bswap64(trades->header.timestamp);
            uint64_t on = __builtin_bswap64(trades->order_ref_number);
            uint8_t bs = trades->buy_sell;
            uint32_t sh = __builtin_bswap32(trades->shares);
            uint8_t*st = trades->stock;
            uint32_t prc = __builtin_bswap32(trades->price);
            uint64_t mn = __builtin_bswap64(trades->match_number);
            cout<<sl<<" "<<ts<<" "<<on<<" "<<bs<<" "<<sh<<" "<<st<<" "<<prc<<" "<<mn<<endl;
            
        }
        if(k=='Q') file.read(buffer,39);
        if(k=='B') file.read(buffer,18);
        if(k=='I') file.read(buffer,49);
        if(k=='N') file.read(buffer,19);
        // cout<<cnt+1<<endl;
        cnt++;
        // Header*h = (Header*)buffer;
        // for(int i=0;i<64;i++) cout<<buffer[i]<<" ";
        // cout<<buffer<<endl;
        // cnt+=1;
        // cout<<endl;
        // cout<<h->stock_locate<<endl;
        // cout<<h->timestamp<<endl;
    }
    // boost::iostreams::filtering_istream gzip_stream;
    // unique_ptr<ifstream> input_stream;
    // gzip_stream.reset();
    // input_stream.reset(new ifstream(filename,ios::in | ios::binary));
    // if (input_stream->is_open()) {
    //     gzip_stream.push(boost::iostreams::gzip_decompressor());
    //     gzip_stream.push(*input_stream);
    //     gzip_stream.read((char*)buffer,chunksize);
    //     cout<<gzip_stream.gcount()<<endl;
    //     // Header*h = (Header*)buffer;
    //     cout<<buffer<<endl;
    // }
    // // input_stream->read((char*)buffer,chunksize);
    // else cout<<"Hello";

    return 0;
}