//
// SEAL pir
// Created by 17579 on 2023/5/30.
//
#include "message.hpp"
#include "seal/seal.h"
#include "seal/util/polyarithsmallmod.h"

#include "../mpc/pir/pir.hpp"
#include "../mpc/pir/pir_client.hpp"
#include "../mpc/pir/pir_server.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
//#include "../netio/stream_channel.hpp"
#include "../mpc/pir/seal_pir.hpp"
using namespace seal;
using namespace seal::util;
namespace PIR {


    Message localPIR(std::string filename1, std::string filename2) {
        //首先生成基本的参数
        uint64_t number_of_items = 1000000;
        uint64_t size_per_item = 20; // in bytes
        uint32_t N = 4096;
        // Recommended values: (logt, d) = (20, 2).
        uint32_t logt = 20;
        uint32_t d = 2;
        bool use_symmetric = false; // use symmetric encryption instead of public key
        // (recommended for smaller query)
        bool use_batching = true;  // pack as many elements as possible into a BFV
        // plaintext (recommended)
        bool use_recursive_mod_switching = true;
        EncryptionParameters enc_params(scheme_type::bfv);
        PirParams pir_params;
        cout << "Main: Generating SEAL parameters" << endl;
        gen_encryption_params(N, logt, enc_params);

        cout << "Main: Verifying SEAL parameters" << endl;
        verify_encryption_params(enc_params);
        cout << "Main: SEAL parameters are good" << endl;

        cout << "Main: Generating PIR parameters" << endl;
        gen_pir_params(number_of_items, size_per_item, d, enc_params, pir_params,
                       use_symmetric, use_batching, use_recursive_mod_switching);
        print_seal_params(enc_params);
        print_pir_params(pir_params);

//        //使用多线程分别调用 client 和 server
        Message message;
        //读取待文件
        std::ifstream fin;
        std::vector <uint64_t> index_list;
        fin.open(filename1, std::ios::binary);
        if (!fin) {
            message.code=0;
            message.msg="查询文件打开失败";
            return message;
        }
        std::string line;
        while (std::getline(fin, line)) {
            std::stringstream stream(line);
            uint64_t a;
            stream >> a;
            //std::cout<<"a "<<a<<std::endl;
            index_list.push_back(a);

        }
        fin.close();
        //读取数据库文件
        std::vector <std::string> tdb;
        std::ifstream fin1;
        fin1.open(filename2, std::ios::binary);
        if (!fin1) {
            message.code=0;
            message.msg="数据库文件打开失败";
            return message;
        }
        std::string line1;
        while (std::getline(fin1, line1)) {
            tdb.push_back(line1);
        }
        fin1.close();
        auto start_time = std::chrono::steady_clock::now();
        // Initialize PIR Server
        cout << "Main: Initializing server" << endl;
        PIRServer server(enc_params, pir_params);

        // Initialize PIR client....
        PIRClient client(enc_params, pir_params);
        cout << "Main: Generating galois keys for client" << endl;
        GaloisKeys galois_keys = client.generate_galois_keys();
        // Server maps the galois key to client 0. We only have 1 client,
        // which is why we associate it with 0. If there are multiple PIR
        // clients, you should have each client generate a galois key,
        // and assign each client an index or id, then call the procedure below.
        server.set_galois_key(0, galois_keys);

        cout << "Main: Creating the database with random data (this may take some "
                "time) ..."
             << endl;
        // Create test database
        auto db(make_unique<uint8_t[]>(number_of_items * size_per_item));
        for (int i = 0; i < tdb.size(); i++) {
            std::string str = tdb[i];
            for (int j = 0; j < str.size(); j++) {
                db.get()[(i * str.size()) + j] = str[j] - '0';
            }
        }
        server.set_database(move(db), number_of_items, size_per_item);
        server.preprocess_database();
        cout << "Main: database pre processed " << endl;
        std::vector<std::string> ans;
        for(int i=0;i<index_list.size();i++){
            /*Generate query*/
            //uint64_t ele_index = Block::BlockToUint64High(index_data[i]);
            uint64_t ele_index = index_list[i];
            uint64_t index = client.get_fv_index(ele_index);   // index of FV plaintext
            uint64_t offset = client.get_fv_offset(ele_index); // offset in FV plaintext
            PirQuery query = client.generate_query(index);
            PirReply reply = server.generate_reply(query, 0);
            cout << "Main: reply generated" << endl;
            // Serialize reply (useful for sending over the network)
            //int reply_size = server.serialize_reply(reply, server_stream);
            vector<uint8_t> elems = client.decode_reply(reply, offset);
            for(int i=0;i<elems.size(); i++) std::cout<<elems[i] <<" ";
            cout << "Main: reply decoded" << endl;
            std::string str="";
            for(int i=0;i<18; i++){
                 str+=std::to_string(static_cast<int>(elems[i]));
            }
            ans.push_back(str);
            std::cout<<str<<std::endl;

        }
        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "SealPIR: takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
        message.code=200;
        message.pirdata=ans;
        message.msg="执行成功！";
        message.sender_time=std::chrono::duration<double, std::milli>(running_time).count();
        return message;
    }
    Message RemotePIR(std::string ip,std::string port,std::string isclient,std::string filename ){
         //生成协议参数
        //首先生成基本的参数
        uint64_t number_of_items = 1000000;
        uint64_t size_per_item = 20; // in bytes
        uint32_t N = 4096;
        // Recommended values: (logt, d) = (20, 2).
        uint32_t logt = 20;
        uint32_t d = 2;
        bool use_symmetric = false; // use symmetric encryption instead of public key
        // (recommended for smaller query)
        bool use_batching = true;  // pack as many elements as possible into a BFV
        // plaintext (recommended)
        bool use_recursive_mod_switching = true;
        EncryptionParameters enc_params(scheme_type::bfv);
        PirParams pir_params;
        cout << "Main: Generating SEAL parameters" << endl;
        gen_encryption_params(N, logt, enc_params);

        cout << "Main: Verifying SEAL parameters" << endl;
        verify_encryption_params(enc_params);
        cout << "Main: SEAL parameters are good" << endl;

        cout << "Main: Generating PIR parameters" << endl;
        gen_pir_params(number_of_items, size_per_item, d, enc_params, pir_params,
                       use_symmetric, use_batching, use_recursive_mod_switching);
        print_seal_params(enc_params);
        print_pir_params(pir_params);

        Message message;
        // 判断是否是client
        if(isclient=="1"){
            NetIO client("client",ip, std::stoi(port));
           message=SEALPIR::Clinet(client,pir_params, enc_params,filename);

            return message;
        }else{
            NetIO server("server",ip, std::stoi(port));
            message=SEALPIR::Server(server,pir_params, enc_params,filename);
            return message;
        }
        return message;
    }

}
