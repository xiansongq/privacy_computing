//
// Created by 17579 on 2023/6/11.
//

#ifndef KUNLUN_ECDH_PSI_HPP
#define KUNLUN_ECDH_PSI_HPP


#include "../../crypto/ec_point.hpp"
#include "../../crypto/hash.hpp"
#include "../../crypto/prg.hpp"
#include "../../crypto/block.hpp"
#include "../../netio/stream_channel.hpp"
#include "../../filter/bloom_filter.hpp"
#include "../../utility/serialization.hpp"
#include "../../qtapi/message.hpp"

#define USE_CURVE_25519
namespace ECDHPSI {

    using Serialization::operator<<;
    using Serialization::operator>>;

    struct PP {
        bool malicious = false;
        std::string filter_type; // shuffle, bloom
        size_t statistical_security_parameter; // default=40
        size_t LOG_SERVER_LEN;
        size_t SERVER_LEN;
        size_t LOG_CLIENT_LEN;
        size_t CLIENT_LEN;
    };

// seriazlize
    std::ofstream &operator<<(std::ofstream &fout, const PP &pp) {
        fout << pp.filter_type;
        fout << pp.statistical_security_parameter;
        fout << pp.LOG_SERVER_LEN;
        fout << pp.SERVER_LEN;
        fout << pp.LOG_CLIENT_LEN;
        fout << pp.CLIENT_LEN;

        return fout;
    }

// load pp from file
    std::ifstream &operator>>(std::ifstream &fin, PP &pp) {
        fin >> pp.filter_type;
        fin >> pp.statistical_security_parameter;

        fin >> pp.LOG_SERVER_LEN;
        fin >> pp.SERVER_LEN;
        fin >> pp.LOG_CLIENT_LEN;
        fin >> pp.CLIENT_LEN;

        return fin;
    }

    PP Setup(std::string filter_type,
             size_t statistical_security_parameter,
             size_t SERVER_LEN,
             size_t CLIENT_LEN) {
        PP pp;
        pp.filter_type = filter_type;
        pp.statistical_security_parameter = statistical_security_parameter;
        pp.SERVER_LEN = SERVER_LEN;
        pp.CLIENT_LEN = CLIENT_LEN;
        return pp;
    }

/*    PP SetupServer(std::string filter_type,
                   size_t statistical_security_parameter,
                   size_t SERVER_LEN) {
        PP pp;
        pp.filter_type = filter_type;
        pp.statistical_security_parameter = statistical_security_parameter;
        pp.SERVER_LEN = SERVER_LEN;
        return pp;
    }

    PP SetupClient(std::string filter_type,
                   size_t statistical_security_parameter,
                   size_t CLIENT_LEN) {
        PP pp;
        pp.filter_type = filter_type;
        pp.statistical_security_parameter = statistical_security_parameter;
        pp.CLIENT_LEN = CLIENT_LEN;
        return pp;
    }*/


    void SavePP(PP &pp, std::string pp_filename) {
        std::ofstream fout;
        fout.open(pp_filename, std::ios::binary);
        if (!fout) {
            std::cerr << pp_filename << " open error" << std::endl;
            exit(1);
        }
        fout << pp;
        fout.close();
    }

    void FetchPP(PP &pp, std::string pp_filename) {
        std::ifstream fin;
        fin.open(pp_filename, std::ios::binary);
        if (!fin) {
            std::cerr << pp_filename << " open error" << std::endl;
            exit(1);
        }
        fin >> pp;
        fin.close();
    }

#ifndef USE_CURVE_25519

    Message Server(NetIO &io, PP &pp, std::vector<block> &vec_Y) {
         Message message;
         if (pp.SERVER_LEN != vec_Y.size()) {
             message.code=0;
             message.msg="input size of vec_Y does not match public parameters";
             return message;
             //std::cerr << "input size of vec_Y does not match public parameters" << std::endl;
             //exit(1);
         }
std::cout<<"ECDH server  USE_CURVE_25519"<<std::endl;
         PrintSplitLine('-');
         auto start_time = std::chrono::steady_clock::now();

         BigInt k1 = GenRandomBigIntLessThan(order); // pick a key k1

         std::vector<ECPoint> vec_Fk1_Y(pp.SERVER_LEN);
#pragma omp parallel for num_threads(thread_count)
         for (auto i = 0; i < pp.SERVER_LEN; i++) {
             vec_Fk1_Y[i] = Hash::BlockToECPoint(vec_Y[i]) * k1; // H(x_i)^k1
         }

         io.SendECPoints(vec_Fk1_Y.data(), pp.SERVER_LEN);

         std::cout << "ECDHPSI [step 1]: Server ===> F_k1(y_i) ===> Client";
#ifdef ECPOINT_COMPRESSED
         std::cout << " [" << (double)POINT_COMPRESSED_BYTE_LEN * pp.SERVER_LEN/(1024*1024) << " MB]" << std::endl;
#else
         std::cout << " [" << (double) POINT_BYTE_LEN * pp.SERVER_LEN / (1024 * 1024) << " MB]" << std::endl;
#endif

         std::vector<ECPoint> vec_Fk2_X(pp.CLIENT_LEN);
         io.ReceiveECPoints(vec_Fk2_X.data(), pp.CLIENT_LEN);

         std::vector<ECPoint> vec_Fk1k2_X(pp.CLIENT_LEN);
#pragma omp parallel for num_threads(thread_count)
         for (auto i = 0; i < pp.CLIENT_LEN; i++) {
             vec_Fk1k2_X[i] = vec_Fk2_X[i] * k1;  // H(x_i)^k_2K_1
         }

         // compute the indication bit vector
         std::vector<uint8_t> vec_indication_bit(pp.CLIENT_LEN);

         if (pp.filter_type == "shuffle") {
             std::vector<ECPoint> vec_Fk2k1_Y(pp.SERVER_LEN);
             io.ReceiveECPoints(vec_Fk2k1_Y.data(), pp.SERVER_LEN);
             std::unordered_set<ECPoint, ECPointHash> S;
             for (auto i = 0; i < pp.SERVER_LEN; i++) {
                 S.insert(vec_Fk2k1_Y[i]);
             }
             for (auto i = 0; i < pp.CLIENT_LEN; i++) {
                 if (S.find(vec_Fk1k2_X[i]) == S.end()) vec_indication_bit[i] = 0;
                 else vec_indication_bit[i] = 1;
             }
         }

         if (pp.filter_type == "bloom") {
             BloomFilter filter;
             // get the size of filter
             size_t filter_size = filter.ObjectSize();
             io.ReceiveInteger(filter_size);
             // get the content of filter
             char *buffer = new char[filter_size];
             io.ReceiveBytes(buffer, filter_size);

             // reconstruct bloom filter
             filter.ReadObject(buffer);
             delete[] buffer;

             vec_indication_bit = filter.Contain(vec_Fk1k2_X);
         }
         std::vector<std::string> ans;
         for (int i = 0; i < vec_indication_bit.size(); i++) {
             if ((int) vec_indication_bit[i] == 1) ans.push_back(std::to_string(Block::BlockToInt64(vec_Y[i])));
         }

         message.data=ans;
         auto end_time = std::chrono::steady_clock::now();
         auto running_time = end_time - start_time;
         std::cout << "ECDHPSI: Server side takes time = "
                   << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
         message.receiver_time = std::chrono::duration<double, std::milli>(running_time).count();
         message.msg = "执行成功";
         PrintSplitLine('-');
         message.code=200;
         return message;
     }

     Message Client(NetIO &io, PP &pp, std::vector<block> &vec_X) {
        Message message;
        std::cout<<"ECDH client USE_CURVE_25519"<<std::endl;
         if (pp.CLIENT_LEN != vec_X.size()) {
             message.code=0;
             message.msg="input size of vec_Y does not match public parameters";
             return message;
             //std::cerr << "input size of vec_Y does not match public parameters" << std::endl;
             //exit(1);
         }

         PrintSplitLine('-');
         auto start_time = std::chrono::steady_clock::now();

         BigInt k2 = GenRandomBigIntLessThan(order); // pick a key

         std::vector<ECPoint> vec_Fk2_X(pp.CLIENT_LEN);
#pragma omp parallel for num_threads(thread_count)
         for (auto i = 0; i < pp.CLIENT_LEN; i++) {
             vec_Fk2_X[i] = Hash::BlockToECPoint(vec_X[i]) * k2; // H(x_i)^k2
         }

         // first receive incoming data
         std::vector<ECPoint> vec_Fk1_Y(pp.SERVER_LEN);
         io.ReceiveECPoints(vec_Fk1_Y.data(), pp.SERVER_LEN); // receive Fk1_Y from Server

         // then send
         io.SendECPoints(vec_Fk2_X.data(), pp.CLIENT_LEN);

         std::cout << "ECDHPSI [step 2]: Client ===> F_k2(x_i) ===> Server";
#ifdef ECPOINT_COMPRESSED
         std::cout << " [" << (double)POINT_COMPRESSED_BYTE_LEN*pp.CLIENT_LEN/(1024*1024) << " MB]" << std::endl;
#else
         std::cout << " [" << (double) POINT_BYTE_LEN * pp.CLIENT_LEN / (1024 * 1024) << " MB]" << std::endl;
#endif

         std::vector<ECPoint> vec_Fk2k1_Y(pp.SERVER_LEN);
#pragma omp parallel for num_threads(thread_count)
         for (auto i = 0; i < pp.SERVER_LEN; i++) {
             vec_Fk2k1_Y[i] = vec_Fk1_Y[i] * k2;
         }

         // permutation
         if (pp.filter_type == "shuffle") {
             std::random_shuffle(vec_Fk2k1_Y.begin(), vec_Fk2k1_Y.end());
             io.SendECPoints(vec_Fk2k1_Y.data(), pp.SERVER_LEN);
             std::cout << "ECDHPSI [step 2]: Client ===> Permutation(F_k2k1(y_i)) ===> Server";
#ifdef ECPOINT_COMPRESSED
             std::cout << " [" << (double)POINT_COMPRESSED_BYTE_LEN * pp.SERVER_LEN/(1024*1024) << " MB]" << std::endl;
#else
             std::cout << " [" << (double) POINT_BYTE_LEN * pp.SERVER_LEN / (1024 * 1024) << " MB]" << std::endl;
#endif
         }

         // generate and send bloom filter
         if (pp.filter_type == "bloom") {

             BloomFilter filter(vec_Fk2k1_Y.size(), pp.statistical_security_parameter);

             filter.Insert(vec_Fk2k1_Y);

             size_t filter_size = filter.ObjectSize();
             io.SendInteger(filter_size);

             char *buffer = new char[filter_size];
             filter.WriteObject(buffer);
             io.SendBytes(buffer, filter_size);
             std::cout << "ECDHPSI [step 2]: Client ===> BloomFilter(F_k2k1(y_i)) ===> Server";
             std::cout << " [" << (double) filter_size / (1024 * 1024) << " MB]" << std::endl;

             delete[] buffer;
         }

         auto end_time = std::chrono::steady_clock::now();
         auto running_time = end_time - start_time;
         std::cout << "ECDHPSI: Client side takes time = "
                   << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;


         PrintSplitLine('-');
         message.sender_time = std::chrono::duration<double, std::milli>(running_time).count();
         message.msg = "执行成功";
         message.code=200;
         return message;
     }

#else


    Message Server(NetIO &io, PP &pp, std::vector <block> &vec_Y) {
        Message message;
        if (pp.SERVER_LEN != vec_Y.size()) {
            message.code = 0;
            message.msg = "input size of vec_Y does not match public parameters";
            //std::cerr << "input size of vec_Y does not match public parameters" << std::endl;
            //exit(1);
        }
        PrintSplitLine('-');
        auto start_time = std::chrono::steady_clock::now();
        uint8_t k1[32];
        PRG::Seed seed = PRG::SetSeed(fixed_seed, 0); // initialize PRG
        GenRandomBytes(seed, k1, 32);  // pick a key k1

        std::vector <EC25519Point> vec_Hash_Y(pp.SERVER_LEN);
        std::vector <EC25519Point> vec_Fk1_Y(pp.SERVER_LEN);

#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.SERVER_LEN; i++) {
            Hash::BlockToBytes(vec_Y[i], vec_Hash_Y[i].px, 32);
            x25519_scalar_mulx(vec_Fk1_Y[i].px, k1, vec_Hash_Y[i].px);
        }

        io.SendEC25519Points(vec_Fk1_Y.data(), pp.SERVER_LEN);

        std::cout << "ECDHPSI [step 1]: Server ===> F_k1(y_i) ===> Client";

        std::cout << " [" << 32 * pp.SERVER_LEN / (1024 * 1024) << " MB]" << std::endl;

        std::vector <EC25519Point> vec_Fk2_X(pp.CLIENT_LEN);
        io.ReceiveEC25519Points(vec_Fk2_X.data(), pp.CLIENT_LEN);

        std::vector <EC25519Point> vec_Fk1k2_X(pp.CLIENT_LEN);
#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.CLIENT_LEN; i++) {
            x25519_scalar_mulx(vec_Fk1k2_X[i].px, k1, vec_Fk2_X[i].px); // (H(x_i)^k2)^k1
        }

        // compute the indication bit vector
        std::vector <uint8_t> vec_indication_bit(pp.CLIENT_LEN);

        if (pp.filter_type == "shuffle") {
            std::cerr << "does not support shuffle" << std::endl;
        }

        if (pp.filter_type == "bloom") {
            BloomFilter filter;
            // get the size of filter
            size_t filter_size = filter.ObjectSize();
            io.ReceiveInteger(filter_size);
            // get the content of filter
            char *buffer = new char[filter_size];
            io.ReceiveBytes(buffer, filter_size);

            // reconstruct bloom filter
            filter.ReadObject(buffer);
            delete[] buffer;

            vec_indication_bit = filter.Contain(vec_Fk1k2_X);
        }
        std::vector <std::string> ans;
        for (int i = 0; i < vec_indication_bit.size(); i++) {
            if ((int) vec_indication_bit[i] == 1) ans.push_back(std::to_string(Block::BlockToInt64(vec_Y[i])));
        }
        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "ECDHPSI: Server side takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;

        PrintSplitLine('-');
        message.receiver_time = std::chrono::duration<double, std::milli>(running_time).count();
        message.msg = "执行成功";
        message.data = ans;
        message.code = 200;
        return message;

    }

    Message Client(NetIO &io, PP &pp, std::vector <block> &vec_X) {
        Message message;

        if (pp.CLIENT_LEN != vec_X.size()) {
            message.code = 0;
            message.msg = "input size of vec_Y does not match public parameters";
            //std::cerr << "input size of vec_Y does not match public parameters" << std::endl;
            // exit(1);
        }
        std::cout << "ECDH client" << std::endl;
        std::cout<<pp.SERVER_LEN<<"  "<<pp.CLIENT_LEN<<" "<<pp.filter_type<<std::endl;
        PrintSplitLine('-');
        auto start_time = std::chrono::steady_clock::now();

        uint8_t k2[32];
        PRG::Seed seed = PRG::SetSeed(fixed_seed, 0); // initialize PRG
        GenRandomBytes(seed, k2, 32);  // pick a key k2

        std::vector <EC25519Point> vec_Hash_X(pp.CLIENT_LEN);
        std::vector <EC25519Point> vec_Fk2_X(pp.CLIENT_LEN);
#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.CLIENT_LEN; i++) {
            Hash::BlockToBytes(vec_X[i], vec_Hash_X[i].px, 32);
            x25519_scalar_mulx(vec_Fk2_X[i].px, k2, vec_Hash_X[i].px);
        }

        // first receive incoming data
        std::vector <EC25519Point> vec_Fk1_Y(pp.SERVER_LEN);
        io.ReceiveEC25519Points(vec_Fk1_Y.data(), pp.SERVER_LEN); // receive Fk1_Y from Server

        // then send
        io.SendEC25519Points(vec_Fk2_X.data(), pp.CLIENT_LEN);

        std::cout << "ECDHPSI [step 2]: Client ===> F_k2(x_i) ===> Server";

        std::cout << " [" << 32 * pp.CLIENT_LEN / (1024 * 1024) << " MB]" << std::endl;


        std::vector <EC25519Point> vec_Fk2k1_Y(pp.SERVER_LEN);
#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.SERVER_LEN; i++) {
            x25519_scalar_mulx(vec_Fk2k1_Y[i].px, k2, vec_Fk1_Y[i].px); // (H(x_i)^k2)^k1
        }

        // permutation
        if (pp.filter_type == "shuffle") {
            std::cerr << "does not support shuffle" << std::endl;
        }

        // generate and send bloom filter
        if (pp.filter_type == "bloom") {

            BloomFilter filter(vec_Fk2k1_Y.size(), pp.statistical_security_parameter);

            filter.Insert(vec_Fk2k1_Y);

            size_t filter_size = filter.ObjectSize();
            io.SendInteger(filter_size);

            char *buffer = new char[filter_size];
            filter.WriteObject(buffer);
            io.SendBytes(buffer, filter_size);
            std::cout << "ECDHPSI [step 3]: Client ===> BloomFilter(F_k2k1(y_i)) ===> Server";
            std::cout << " [" << (double) filter_size / (1024 * 1024) << " MB]" << std::endl;

            delete[] buffer;
        }

        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "ECDHPSI: Client side takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
        PrintSplitLine('-');
        message.sender_time = std::chrono::duration<double, std::milli>(running_time).count();
        message.code = 200;
        message.msg = "执行成功";
        return message;


    }

#endif

}


#endif //KUNLUN_ECDH_PSI_HPP
