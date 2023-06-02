/***********************************************************************************
this hpp file implements the core code of Sealpir namespace SEALPIR
SEALPIR can be executed for keyword-based queries through network communication protocols
************************************************************************************
* @author      XianSong
* @paper      https://eprint.iacr.org/2017/1142.pdf
***********************************************************************************/


#ifndef KUNLUN_SEALPIRKEYWORD_HPP_
#define KUNLUN_SEALPIRKEYWORD_HPP_


#include "seal/seal.h"
#include "seal/util/polyarithsmallmod.h"
#include "../../netio/stream_channel.hpp"
#include "../../crypto/bigint.hpp"
#include "../../crypto/hash.hpp"
#include "../../include/global.hpp"
#include "../rpmt/ecc_mqrpmt.hpp"

#include "pir.hpp"
#include "pir_client.hpp"
#include "pir_server.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <vector>


using namespace seal;
using namespace seal::util;
namespace SEALPIRKEYWORD {

    std::vector<std::vector<uint8_t>>
    Clinet(NetIO &io, PirParams params, const EncryptionParameters &enc_params, std::string filename) {
        // Initialize PIR client....
        auto start_time = std::chrono::steady_clock::now();
        PIRClient client(enc_params, params);

        std::ifstream fin;
        std::vector<block> vec_x;
        fin.open("A_PIR_ID.txt", std::ios::binary);
        if (!fin) {
            std::cout << "Failed to open file" << std::endl;
            std::exit(-1);
        }
        std::string line;
        int i = 0;
        while (std::getline(fin, line, '\n')) {
            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);
            vec_x.push_back(Block::MakeBlock('0LL',std::stoull(line)));
        }
        fin.close();

        // start ecc_mqrpmt
        eccmqRPMT::PP pp;
        pp=eccmqRPMT::Setup("bloom",40,1000000,10);
        std::vector<uint8_t> result=eccmqRPMT::Client(io,pp,vec_x);
        std::cout<<"eccmqRPMT result vector size="<<result.size()<<std::endl;
        std::vector<std::uint64_t> keyword_index;
        for(int i=0;i<result.size();i++){
            if((int)result[i]==1) keyword_index.push_back(i);
        }
        for(auto a:keyword_index) std::cout<<a<<" ";
        std::cout<<std::endl;

        std::cout << "keyword_inde size= " << keyword_index.size() << std::endl;

        /*The preprocessing of the files has been completed */
        cout << "Main: Generating galois keys for client" << endl;
        /*create GaloisKeys*/
        GaloisKeys galois_keys = client.generate_galois_keys();
        std::ostringstream output;
        /*  Serialization  GaloisKeys sending by network */
        galois_keys.save(output);
        /*Send GaloisKeys size*/
        io.SendBlock(Block::MakeBlock(0LL, output.str().size()));
        /*Send Galoiskeys*/
        std::string temp = output.str();
        io.SendString(temp);

        std::vector<std::vector<uint8_t>> ans;
        /*Used to indicate whether the query is terminated. When the server-side receives 0,
         * it means the query is finished and the network communication is disconnected*/
        std::string isend = "1";
        io.SendString(isend);
        int query_toltal_size = 0;
        for (int i = 0; i < keyword_index.size(); i++) {
            /*Generate query*/
            //uint64_t ele_index = Block::BlockToUint64High(index_data[i]);
            uint64_t ele_index = keyword_index[i];
            uint64_t index = client.get_fv_index(ele_index);   // index of FV plaintext
            uint64_t offset = client.get_fv_offset(ele_index); // offset in FV plaintext

            /*Serialization query sending by network*/
            stringstream client_stream;
            int query_size = client.generate_serialized_query(index, client_stream);
            query_toltal_size += query_size;
            /*First send query size to server*/
            io.SendBlock(Block::MakeBlock(0LL, query_size));
            std::cout << "query_size" << query_size << std::endl;
            std::string str = client_stream.str();
            /*Send query*/
            io.SendString(str);
            block b;
            io.ReceiveBlock(b);
            int reply_size = Block::BlockToInt64(b);
            std::string str_reply(reply_size, '0');
            /*Receive Reply from server*/
            io.ReceiveString(str_reply);
            stringstream sreply(str_reply);
            /* Construct SEALContext*/
            seal::SEALContext context(enc_params);
            PirReply reply = client.deserialize_reply(sreply, context);
            vector<uint8_t> elems = client.decode_reply(reply, offset);
//            std::string ele="";
//            for(auto e:elems){
//                ele+=std::to_string(e);
//            }
//            std::cout<<"elems: "<<ele<<std::endl;
            ans.push_back(elems);
            if (i != keyword_index.size() - 1)
                isend = "1";
            else isend = "0";
            io.SendString(isend);
        }
        isend = "0";
        io.SendString(isend);
        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "SealPIR:Client side takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
        std::cout << "SealPIR:Client Query size= ["
                  << (double) (query_toltal_size) / (1024 * 1024) << " MB]" << std::endl;
        return ans;
    } //client

    void Server(NetIO &io, PirParams params, const EncryptionParameters &enc_params, std::string filename) {
        auto start_time = std::chrono::steady_clock::now();
        PIRServer server(enc_params, params);

        std::ifstream fin;
        std::vector<std::vector<std::string>> file_data;
        std::vector<block> vec_y;
        fin.open("B_PIR_DATA.txt", std::ios::binary);
        if (!fin) {
            std::cout << "Failed to open file" << std::endl;
            std::exit(-1);
        }
        std::string line;
        int i = 0;
        while (std::getline(fin, line)) {
            vector<string> row;
            row.push_back(to_string(i++));   /*add index colunms*/
            size_t pos = 0;
            string token;
            /* Split each row of data into multiple columns.*/
            while ((pos = line.find(",")) != string::npos) {
                token = line.substr(0, pos);
                row.push_back(token);
                line.erase(0, pos + 1);
            }
            row.push_back(line);
            vec_y.push_back(Block::MakeBlock('0LL',std::stoull(row[1])));
            file_data.push_back(row); /*save data*/

        }
        fin.close();

        eccmqRPMT::PP pp;
        pp=eccmqRPMT::Setup("bloom",40,1000000,10);
        eccmqRPMT::Server(io,pp,vec_y);

        //auto start_time = std::chrono::steady_clock::now();
        block c;
        io.ReceiveBlock(c);
        int gas_len = Block::BlockToInt64(c);
        std::string gas_key(gas_len, '0');
        io.ReceiveString(gas_key);
        GaloisKeys *galois_keys1 = deserialize_galoiskeys(gas_key, server.context_);
        server.set_galois_key(0, *galois_keys1);
        /*Before generating the database file, it is necessary to remove the special
         * symbol '-' from the second column of the raw data*/
//#pragma omp parallel for num_threads(thread_count)
        for (int i = 0; i < file_data.size(); i++) {
            std::string temp = file_data[i][2];
            temp.erase(std::remove(temp.begin(), temp.end(), '-'), temp.end());
            file_data[i][2] = temp;
        }
        /*Generate database */
        auto db(make_unique<uint8_t[]>(params.ele_num * params.ele_size));
        for (int i = 0; i < file_data.size(); i++) {
            std::string str = file_data[i][1] + file_data[i][2] + file_data[i][3];
            for (int j = 0; j < str.size(); j++) {
                db.get()[(i * str.size()) + j] = str[j] - '0';
            }
        }

        // Measure database setup
        cout << "Main: database pre processed " << endl;
        server.set_database(move(db), params.ele_num, params.ele_size);
        server.preprocess_database();

        std::string isend(1, '0');
        io.ReceiveString(isend);
        int reply_toltal_size = 0;
        while (isend == "1") {
            block a;
            io.ReceiveBlock(a);
            int query_size = Block::BlockToInt64(a);
            std::cout << "rec_query_size " << query_size << std::endl;
            std::string query(query_size, '0');
            io.ReceiveString(query);
            std::stringstream server_stream;
            std::stringstream server_stream1(query);
            PirQuery query2 = server.deserialize_query(server_stream1);
            PirReply reply = server.generate_reply(query2, 0);
            int reply_size = server.serialize_reply(reply, server_stream);
            reply_toltal_size += reply_size;
            io.SendBlock(Block::MakeBlock(0LL, reply_size));
            std::string str1 = server_stream.str();
            io.SendString(str1);
            io.ReceiveString(isend);
            //if (isend == "0") return;
        }
        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "SealPIR:Server side takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
        std::cout << "SealPIR:Server Reply size= ["
                  << (double) (reply_toltal_size) / (1024 * 1024) << " MB]" << std::endl;
    }// server
}// namespace


#endif