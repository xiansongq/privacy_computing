/***********************************************************************************
this hpp file implements the core code of Sealpir namespace SEALPIR
SEALPIR can be executed for index-based queries through network communication protocols
************************************************************************************
* @author     XianSong
* @paper      https://eprint.iacr.org/2017/1142.pdf
***********************************************************************************/

#ifndef KUNLUN_SEALPIR_HPP_
#define KUNLUN_SEALPIR_HPP_


#include "seal/seal.h"
#include "seal/util/polyarithsmallmod.h"
#include "../../netio/stream_channel.hpp"
#include "pir.hpp"
#include "pir_client.hpp"
#include "pir_server.hpp"
#include "../../qtapi/message.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <vector>


using namespace seal;
using namespace seal::util;
namespace SEALPIR {

    Message
    Clinet(NetIO &io, PirParams params, const EncryptionParameters &enc_params,
           std::string filename) {
        Message message;
        auto start_time = std::chrono::steady_clock::now();
        // Initialize PIR client....
        PIRClient client(enc_params, params);

        //读取文件
        std::ifstream fin;
        std::vector <uint64_t> index_list;
        fin.open(filename, std::ios::binary);
        if (!fin) {
            message.code = 0;
            message.msg = "文件打开失败";
            return message;
        }
        std::string line;
        while (std::getline(fin, line)) {
            std::stringstream stream(line);
            uint64_t a;
            stream >> a;
            index_list.push_back(a);

        }
        fin.close();


        cout << "Main: Generating galois keys for client" << endl;
        //create GaloisKeys
        GaloisKeys galois_keys = client.generate_galois_keys();
        std::ostringstream output;
        // Serialization  GaloisKeys sending by network
        galois_keys.save(output);
        //First send GaloisKeys length to Server
        io.SendBlock(Block::MakeBlock(0LL, output.str().size()));
        //send GaloisKeys
        std::string temp = output.str();
        io.SendString(temp);
        std::vector <std::string> ans;
        std::string isend = "1";
        io.SendString(isend);
        int query_toltal_size = 0;
        for (int i = 0; i < index_list.size(); i++) {
            /*Generate query*/
            //uint64_t ele_index = Block::BlockToUint64High(index_data[i]);
            uint64_t ele_index = index_list[i];
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
            vector <uint8_t> elems = client.decode_reply(reply, offset);
            std::string ele = "";
            for (int i = 0; i < 18; i++) {
                ele += std::to_string(static_cast<int>(elems[i]));
            }
            ans.push_back(ele);
            if (i != index_list.size() - 1)
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
        message.code = 200;
        message.msg = "执行成功";
        message.receiver_time = std::chrono::duration<double, std::milli>(running_time).count();
        message.pirdata = ans;
        return message;
    }

    Message Server(NetIO &io, PirParams params, const EncryptionParameters &enc_params,
                   std::string filename) {
        Message message;
        auto start_time = std::chrono::steady_clock::now();
        // Initialize PIR Server....
        PIRServer server(enc_params, params);
        //read file
        std::vector <std::string> tdb;
        std::ifstream fin;
        fin.open(filename, std::ios::binary);
        if (!fin) {
            message.code = 0;
            message.msg = "文件打开失败";
            return message;
        }
        std::string line;
        while (std::getline(fin, line)) {
            tdb.push_back(line);

        }
        fin.close();
        block c;
        io.ReceiveBlock(c);
        int gas_len = Block::BlockToInt64(c);
        std::string gas_key(gas_len, '0');
        // Receive GaloisKeys
        io.ReceiveString(gas_key);
        GaloisKeys *galois_keys1 = deserialize_galoiskeys(gas_key, server.context_);
        server.set_galois_key(0, *galois_keys1);
        /*Generate database */
        auto db(make_unique<uint8_t[]>(params.ele_num * params.ele_size));
        for (int i = 0; i < tdb.size(); i++) {
            std::string str = tdb[i];
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
        message.code = 200;
        message.msg = "执行成功";
        message.sender_time = std::chrono::duration<double, std::milli>(running_time).count();
        return message;
    }
}


#endif