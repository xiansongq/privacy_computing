//
// Created by 17579 on 2023/5/10.
//

#ifndef KUNLUN_KEYWORDPIR_HPP
#define KUNLUN_KEYWORDPIR_HPP

#include "../../crypto/ec_point.hpp"
#include "../../crypto/hash.hpp"
#include "../../crypto/prg.hpp"
#include "../../crypto/block.hpp"
#include "../../netio/stream_channel.hpp"
#include "../../filter/bloom_filter.hpp"
#include "../ot/alsz_ote.hpp"
#include "../ot/iknp_ote.hpp"
//#include "../../utility/serialization.hpp
namespace keywordPIR {
    struct PP {
        bool malicious = false;
        std::string filter_type;
        size_t statistical_security_parameter;
        //size_t LOG_SERVER_LEN;
        size_t SERVER_LEN;
        //size_t LOG_CLIENT_LEN;
        size_t CLIENT_LEN;
    };

    // seriazlize
/*    std::ofstream &operator<<(std::ofstream &fout, const PP &pp) {
        fout << pp.filter_type;
        fout << pp.statistical_security_parameter;
        //fout << pp.LOG_SERVER_LEN;
        fout << pp.SERVER_LEN;
        //fout << pp.LOG_CLIENT_LEN;
        fout << pp.CLIENT_LEN;

        return fout;
    }*/

// load pp from file
/*    std::ifstream &operator>>(std::ifstream &fin, PP &pp) {
        fin >> pp.filter_type;
        fin >> pp.statistical_security_parameter;

        fin >> pp.LOG_SERVER_LEN;
        fin >> pp.SERVER_LEN;
        fin >> pp.LOG_CLIENT_LEN;
        fin >> pp.CLIENT_LEN;

        return fin;
    }*/

    PP Setup(std::string filter_type,
             size_t statistical_security_parameter,
             size_t SERVER_LEN,
             size_t CLIENT_LEN) {
        PP pp;
        pp.filter_type = filter_type;
        pp.statistical_security_parameter = statistical_security_parameter;
        //pp.LOG_SERVER_LEN = LOG_SERVER_LEN;
        pp.SERVER_LEN = SERVER_LEN;
        //pp.LOG_CLIENT_LEN = LOG_CLIENT_LEN;
        pp.CLIENT_LEN = CLIENT_LEN;
        return pp;
    }

/*
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
*/

    void Server(NetIO &io, PP &pp, std::vector<block> &vec_Y) {
        if (pp.SERVER_LEN != vec_Y.size()) {
            std::cerr << "input size of vec_Y does not match public parameters" << std::endl;
            exit(1);
        }

        PrintSplitLine('-');
        auto start_time = std::chrono::steady_clock::now();

        uint8_t k1[32];
        PRG::Seed seed = PRG::SetSeed(fixed_seed, 0); // initialize PRG
        GenRandomBytes(seed, k1, 32);  // pick a key k1

        std::vector<EC25519Point> vec_Hash_Y(pp.SERVER_LEN);
        std::vector<EC25519Point> vec_Fk1_Y(pp.SERVER_LEN);

#pragma omp parallel for num_threads(thread_count)
        //calculate H(y_i)^k-1
        for (auto i = 0; i < pp.SERVER_LEN; i++) {
            Hash::BlockToBytes(vec_Y[i], vec_Hash_Y[i].px, 32);
            x25519_scalar_mulx(vec_Fk1_Y[i].px, k1, vec_Hash_Y[i].px);
        }

        io.SendEC25519Points(vec_Fk1_Y.data(), pp.SERVER_LEN);

        std::cout << "ECC-based mqRPMT [step 1]: Server ===> F_k1(y_i) ===> Client";

        std::cout << " [" << 32 * pp.SERVER_LEN / (1024 * 1024) << " MB]" << std::endl;

        std::vector<EC25519Point> vec_Fk2_X(pp.CLIENT_LEN);
        io.ReceiveEC25519Points(vec_Fk2_X.data(), pp.CLIENT_LEN);

        std::vector<EC25519Point> vec_Fk1k2_X(pp.CLIENT_LEN);
#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.CLIENT_LEN; i++) {
            x25519_scalar_mulx(vec_Fk1k2_X[i].px, k1, vec_Fk2_X[i].px); // (H(x_i)^k2)^k1
        }
        if (pp.filter_type == "shuffle") {
            std::cerr << "does not support shuffle" << std::endl;
        }
        // generate and send bloom filter
        if (pp.filter_type == "bloom") {

            BloomFilter filter(vec_Fk1k2_X.size(), pp.statistical_security_parameter);

            filter.Insert(vec_Fk1k2_X);

            size_t filter_size = filter.ObjectSize();
            io.SendInteger(filter_size);

            char *buffer = new char[filter_size];
            filter.WriteObject(buffer);
            io.SendBytes(buffer, filter_size);
            std::cout << "ECC-based mqRPMT [step 2]: Server ===> BloomFilter(F_k2k1(y_i)) ===> Client";
            std::cout << " [" << (double) filter_size / (1024 * 1024) << " MB]" << std::endl;

            delete[] buffer;
        }


        auto stime = std::chrono::steady_clock::now();
        IKNPOTE::PP p1;
        p1 = IKNPOTE::Setup(128);
        std::cout << "start execute OT" << std::endl;
        IKNPOTE::OnesidedSend(io, p1, vec_Y, 1 << 20);
        auto etime = std::chrono::steady_clock::now();
        auto running_time1 = etime - stime;
        std::cout << "keywordPIR: Server send OT time "
                  << std::chrono::duration<double, std::milli>(running_time1).count() << " ms" << std::endl;
        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "ECC-mqRPMT: Server side takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;

    }

    std::vector<block> Client(NetIO &io, PP &pp, std::vector<block> &vec_X) {
        if (pp.CLIENT_LEN != vec_X.size()) {
            std::cerr << "input size of vec_Y does not match public parameters" << std::endl;
            exit(1);
        }

        PrintSplitLine('-');
        auto start_time = std::chrono::steady_clock::now();

        uint8_t k2[32];
        PRG::Seed seed = PRG::SetSeed(fixed_seed, 0); // initialize PRG
        GenRandomBytes(seed, k2, 32);  // pick a key k2

        std::vector<EC25519Point> vec_Hash_X(pp.CLIENT_LEN);
        std::vector<EC25519Point> vec_Fk2_X(pp.CLIENT_LEN);
#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.CLIENT_LEN; i++) {
            Hash::BlockToBytes(vec_X[i], vec_Hash_X[i].px, 32);
            x25519_scalar_mulx(vec_Fk2_X[i].px, k2, vec_Hash_X[i].px);
        }

        // first receive incoming data
        std::vector<EC25519Point> vec_Fk1_Y(pp.SERVER_LEN);
        io.ReceiveEC25519Points(vec_Fk1_Y.data(), pp.SERVER_LEN); // receive Fk1_Y from Server

        // then send
        io.SendEC25519Points(vec_Fk2_X.data(), pp.CLIENT_LEN);

        std::cout << "ECC-based mqRPMT [step 2]: Client ===> F_k2(x_i) ===> Server";

        std::cout << " [" << 32 * pp.CLIENT_LEN / (1024 * 1024) << " MB]" << std::endl;


        std::vector<EC25519Point> vec_Fk2k1_Y(pp.SERVER_LEN);
#pragma omp parallel for num_threads(thread_count)
        for (auto i = 0; i < pp.SERVER_LEN; i++) {
            x25519_scalar_mulx(vec_Fk2k1_Y[i].px, k2, vec_Fk1_Y[i].px); // (H(x_i)^k2)^k1
        }

        // permutation
        if (pp.filter_type == "shuffle") {
            std::cerr << "does not support shuffle" << std::endl;
        }

        // compute the indication bit vector
        std::vector<uint8_t> vec_indication_bit(pp.SERVER_LEN);

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
            vec_indication_bit = filter.Contain(vec_Fk2k1_Y);
        }

        //return vec_indication_bit;
        auto stime = std::chrono::steady_clock::now();
        IKNPOTE::PP p1;
        p1 = IKNPOTE::Setup(128);
        std::cout << "start execute OT" << std::endl;
        auto result = IKNPOTE::OnesidedReceive(io, p1, vec_indication_bit, 1 << 20);
        auto etime = std::chrono::steady_clock::now();
        auto running_time1 = etime - stime;
        std::cout << "keywordPIR: Client receive OT time "
                  << std::chrono::duration<double, std::milli>(running_time1).count() << " ms" << std::endl;
        auto end_time = std::chrono::steady_clock::now();
        auto running_time = end_time - start_time;
        std::cout << "ECC-mqRPMT: Client side takes time = "
                  << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
        return result;
    }


}
#endif //KUNLUN_KEYWORDPIR_HPP
