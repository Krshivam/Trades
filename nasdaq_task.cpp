#include "trade_msg.h"

using namespace std;
namespace io = boost::iostreams;


tm convert_nanoseconds_to_datetime(uint64_t nanos) {
    chrono::nanoseconds duration(nanos);
    chrono::seconds seconds = chrono::duration_cast<chrono::seconds>(duration);
    time_t epoch_seconds = seconds.count();
    tm* timeinfo = gmtime(&epoch_seconds);

    return *timeinfo;
}

//msg type and their size-1
unordered_map<char,int>type_sze_map = {{'S',11},{'R',38},{'H',24},{'Y',19},{'L',25},{'V',34},{'W',11},{'K',27},{'J',34},{'h',20},{'A',35},{'F',39},{'E',30},{'C',35},{'X',22},{'D',18},{'U',34},{'P',43},{'Q',39},{'B',18},{'I',49},{'N',19}};


void parseTrades (istream & instream, ofstream& outdata) 
{
    map<string,map<int,double>> prices;    // to store stock price at particular hour
    map<string,map<int,int>> volume;       // to store stock volume at particular hour
    outdata.open("VWAP_output.txt");   // To stora VWAP

    char buffer[64];   //for any msg , struct max size will not exceed 64

    while (instream.good()) 
    {
        TradeMessage msg;
        instream.read(reinterpret_cast<char*>(&msg.header.msg_type),1);
        if(msg.header.msg_type == 'P')
        {
            // populating trade msg fields from buffer.

            instream.read(reinterpret_cast<char*>(buffer), sizeof(TradeMessage) - 1);    //1 byte already read
            memcpy(&msg.header.stock_locate,buffer,2);
            memcpy(&msg.header.tracking_number,buffer+2,2);
            memcpy(&msg.header.timestamp,buffer+4,6);
            memcpy(&msg.order_ref_number,buffer+10,8);
            memcpy(&msg.buy_sell,buffer+18,1);
            memcpy(&msg.shares,buffer+19,4);
            memcpy(msg.stock,buffer+23,8);
            msg.stock[7] = '\0';
            memcpy(&msg.price,buffer+31,4);
            memcpy(&msg.match_number,buffer+35,8);
            msg.reverseEndian();   // convert to little endian , comment it if your sys is big endian
            std::tm datetime = convert_nanoseconds_to_datetime(msg.header.timestamp);
            int hr = datetime.tm_hour;

            //Uncomment below line to print trade msg certain fields
            // cout<<msg.header.stock_locate<<" "<<msg.header.tracking_number<<" "<<msg.header.timestamp<<" "<<msg.order_ref_number<<" "<<msg.buy_sell<<" "<<msg.shares<<" "<<msg.stock<<" "<<msg.price<<" "<<msg.match_number<<endl;

            prices[msg.stock][hr] += ((msg.price)/10000.0) * msg.shares;
            volume[msg.stock][hr] += msg.shares;

            cout << "VWAP for "<< msg.stock <<" at "<< hr <<" hrs is "<< prices[msg.stock][hr]/volume[msg.stock][hr] << endl;
            outdata << "VWAP for "<< msg.stock <<" at "<< hr <<" hrs is "<< prices[msg.stock][hr]/volume[msg.stock][hr] << endl;
        }

        //Skip rest of msg types
        else if(type_sze_map.find(msg.header.msg_type) != type_sze_map.end()) {
            int sze = type_sze_map[msg.header.msg_type];
            instream.read(reinterpret_cast<char*>(buffer), sze);
        }
    }
    outdata.close();

    //uncomment below lines to get VWAP file for each stock
    // for(auto stk:prices) 
    // {
    //     outdata.open(stk.first+".txt");
    //     for (auto hr_price:stk.second) 
    //     {
    //         outdata << "VWAP for stk at " << hr_price.first<<" hrs is "<< hr_price.second/volume[stk.first][hr_price.first] <<endl;
    //     }
    //     outdata.close();
    // }
    
}


int main(int argc,char** argv) {
    if (argc < 2 ) {
        throw "Invalid Args";
    }
    // string filename = "01302019.NASDAQ_ITCH50.gz";
    ifstream file(argv[1], ios::binary);
    io::filtering_istreambuf inbuf;
    inbuf.push(io::gzip_decompressor());
    inbuf.push(file);
    istream instream(&inbuf);
    ofstream outdata;
    parseTrades(instream ,outdata);
    
    return 0;
}
