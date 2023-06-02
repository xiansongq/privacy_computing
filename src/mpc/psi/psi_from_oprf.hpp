#ifndef KUNLUN_PSI_HPP_
#define KUNLUN_PSI_HPP_

#include "../../filter/bloom_filter.hpp"

#include "../oprf/ote_oprf.hpp"
#include "../ot/alsz_ote.hpp"
#include "../../qtapi/message.hpp"
//基于 bloom的 psi可以使用 直接使用plain的不行
//数据以string 的格式返回

namespace OPRFPSI {
    std::string uintTostring(std::vector<uint8_t> num) {
        std::string str = "";
        for (int i = 0; i < num.size(); i++) {
            str += std::to_string(static_cast<int>(num[i]));

        }
        return str;
    }

    Message Receive(NetIO &io, OTEOPRF::PP &pp, std::vector<block> &vec_Y) {
        Message message;
        if (vec_Y.size() != pp.LEN) {
            message.code=0;
            message.msg="|Y| does not match public parameter";
            return message;
        }
        auto start_time = std::chrono::steady_clock::now();
        std::vector<std::vector<uint8_t>> oprf_key = OTEOPRF::Server(io, pp);
        std::vector<std::vector<uint8_t>> oprf_value = OTEOPRF::Evaluate(pp, oprf_key, vec_Y, vec_Y.size());
        std::vector<uint8_t> vec_indication_bit(vec_Y.size());
        std::vector<std::string> ans;
        std::string choice = "bloom";
        if (choice == "bloom") {
            BloomFilter filter;
            size_t filter_size = filter.ObjectSize();
            io.ReceiveInteger(filter_size);
            char *buffer = new char[filter_size];
            io.ReceiveBytes(buffer, filter_size);
            filter.ReadObject(buffer);
            auto bloom_start_time = std::chrono::steady_clock::now();
            vec_indication_bit = filter.Contain(oprf_value);
            auto bloom_end_time = std::chrono::steady_clock::now();
            auto running_time1 = bloom_end_time - bloom_start_time;
            std::cout << "Receiver excute bloom filter query takes time= "
                      << std::chrono::duration<double, std::milli>(running_time1).count() << " ms" << std::endl;

            PrintSplitLine('-');
            std::cout << "mpOPRF-based PSI: Receive <=== BloomFilter(F_k(x_i)) <=== Sender ["
                      << (double) (filter_size) / (1024*1024) << " MB]" << std::endl;
            delete[] buffer;
            for (int i = 0; i < vec_indication_bit.size(); i++) {
                if ((int) vec_indication_bit[i] == 1) ans.push_back(std::to_string(Block::BlockToInt64(vec_Y[i])));
            }
            auto end_time = std::chrono::steady_clock::now();
            auto running_time = end_time - start_time;
            std::cout << "mpOPRF-based PSI [bloom filter]: Receiver side takes time "
                      << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
            message.receiver_time=std::chrono::duration<double, std::milli>(running_time).count();
            message.code=200;
            message.msg="执行成功";
            message.data=ans;
        }

        std::cout<<"oprf ans size:"<<ans.size()<<std::endl;


        return message;
    }

    Message Send(NetIO &io, OTEOPRF::PP &pp, std::vector<block> &vec_X) {
        Message message;
        if (vec_X.size() != pp.LEN) {
            message.code=0;
            message.msg="|X| does not match public parameter";
            return message;
        }
        auto start_time = std::chrono::steady_clock::now();
        std::vector<std::vector<uint8_t>> oprf_value = OTEOPRF::Client(io, pp, vec_X, vec_X.size());
        //std::vector<std::vector<uint8_t>> oprf_value = OTEOPRF::Evaluate(pp, oprf_key, vec_X, vec_X.size());
        std::string choice = "bloom";
        if (choice == "bloom") {
            BloomFilter filter(oprf_value.size(), 40);
            auto bloom_start_time = std::chrono::steady_clock::now();
            filter.Insert(oprf_value);
            auto bloom_end_time = std::chrono::steady_clock::now();
            auto running_time1 = bloom_end_time - bloom_start_time;
            std::cout << "Sender execute bloom filter insert takes time= "
                      << std::chrono::duration<double, std::milli>(running_time1).count() << " ms" << std::endl;

            size_t filter_size = filter.ObjectSize();
            io.SendInteger(filter_size);
            char *buffer = new char[filter_size];
            filter.WriteObject(buffer);
            io.SendBytes(buffer, filter_size);
            filter.ReadObject(buffer);

            std::cout << "mpOPRF-based PSI: Sender ===> BloomFilter(F_k(x_i)) ===> Receiver ["
                      << (double) (filter_size)/ (1024*1024) << " MB]" << std::endl;
            delete[] buffer;
            PrintSplitLine('-');
            auto end_time = std::chrono::steady_clock::now();
            auto running_time = end_time - start_time;
            std::cout << "mpOPRF-based PSI [bloom filter]: Sender side takes time "
                      << std::chrono::duration<double, std::milli>(running_time).count() << " ms" << std::endl;
            message.sender_time=std::chrono::duration<double, std::milli>(running_time).count();
        }


        message.code=200;
        message.msg="执行成功";
        return message;
    }

}


#endif
