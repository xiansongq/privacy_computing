//
// Created by 17579 on 2023/6/14.
//


#include "ecdh_psi.hpp"

namespace ECDH_PSI {
    //创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
    void localpsireceiver(NetIO &io, ECDHPSI::PP pp, std::vector <block> vecx, Message &message) {
        try {
            message = ECDHPSI::Server(io, pp, vecx);
            io.DeleteIo();
        } catch (std::exception &e) {
            message.msg = e.what();
            message.code = 0;
        }
        return;
    }


//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
    void localpsisender(NetIO &io, ECDHPSI::PP pp, std::vector <block> vecy, Message &message) {

        try {
            message = ECDHPSI::Client(io, pp, vecy);
            io.DeleteIo();

        } catch (std::exception &e) {
            message.code = 0;
            message.msg = e.what();
        }
        return;
    }

    void create_netio(std::string party, ECDHPSI::PP pp, std::vector <block> vect, Message &message) {
        NetIO io(party, "127.0.0.1", 8085);

        if (party == "server") {
            localpsireceiver(io, pp, vect, message);
        }

        if (party == "client") {
            localpsisender(io, pp, vect, message);
        }

        // io.DeleteIo();
    }

    Message localhostPSI(std::string filename1, std::string filename2) {
        // 定义两个vector 存放文件数据
        std::vector <block> vec_x;
        //定义一个消息对象
        Message message1;
        Message message2;
        //从文件读取数据
        std::ifstream fin;
        fin.open(filename1, std::ios::binary);
        if (!fin) {
            message1.code = -1;
            message1.msg = "文件打开失败！";
            return message1;
        }
        std::string line;
        while (std::getline(fin, line)) {
            std::stringstream stream(line);
            uint64_t a;
            stream >> a;
            vec_x.push_back(Block::MakeBlock(0LL, a));
        }
        fin.close();
        // 定义两个vector 存放文件数据
        std::vector <block> vec_y;

        //从文件读取数据
        std::ifstream fin1;
        fin1.open(filename2, std::ios::binary);
        if (!fin1) {
            message1.code = -1;
            message1.msg = "文件打开失败！";
            return message1;
        }
        std::string line1;

        while (std::getline(fin1, line1)) {
            std::stringstream stream(line1);
            uint64_t a;
            stream >> a;
            vec_y.push_back(Block::MakeBlock(0LL, a));
        }
        fin1.close();

        if (vec_x.size() != vec_y.size()) {
            message1.code = 0;
            message1.msg = "两个集合大小不同！";
            return message1;
        }
        CRYPTO_Initialize();
#ifdef USE_CURVE_25519
        std::cerr << "Using Curve 25519" << std::endl;
#endif
        ECDHPSI::PP pp;
        pp = ECDHPSI::Setup("bloom", 40, vec_x.size(), vec_y.size());
        ECDHPSI::PP pp1;
        pp1 = ECDHPSI::Setup("bloom", 40, vec_x.size(), vec_y.size());

        std::thread receiverthread(ECDH_PSI::create_netio, "server", std::move(pp), std::move(vec_x),
                                   std::ref(message1));

        std::thread senderthread(ECDH_PSI::create_netio, "client", std::move(pp1), std::move(vec_y),
                                 std::ref(message2));

        // std::thread receiverthread(PSI::localpsireceiver, pp, vec_x,&message2,slen);
        //std::thread senderthread(PSI::localpsisender, pp, vec_y, &message1);
        receiverthread.join();
        senderthread.join();
        message1.sender_time = message2.sender_time;
        CRYPTO_Finalize();
        return message1;

    }


// 定义联机执行的 api
    Message remotepsi_(std::string ip, std::string port, std::string filename, std::string isserver, std::string type) {
        std::cout << filename << std::endl;
        // 读取文件
        // 定义两个vector 存放文件数据
        std::vector <block> vec_y;
        //定义一个消息对象
        Message message;
        //从文件读取数据
        std::ifstream fin1;
        fin1.open(filename, std::ios::binary);
        if (!fin1) {
            std::cout << "Failed to open file" << std::endl;
            message.code = -1;
            message.msg = "文件打开失败！";
            return message;
        }
        std::string line1;

        while (std::getline(fin1, line1)) {
            std::stringstream stream(line1);
            uint64_t a;
            stream >> a;
            vec_y.push_back(Block::MakeBlock(0LL, a));
        }
        fin1.close();


        //初始化参数
        CRYPTO_Initialize();
#ifdef USE_CURVE_25519
        std::cerr << "Using Curve 25519" << std::endl;
#endif
        ECDHPSI::PP pp;


        // std::cout<<"ip info1 "<<isserver<<std::endl;
        // 执行协议
        if (isserver == "1") {
            pp = ECDHPSI::Setup("bloom", 40, vec_y.size(),vec_y.size());
            try {

                NetIO server("server", ip, std::stoi(port));
                // 发送协议类型 type给客户端
                block len = Block::MakeBlock('0LL', type.size());
                server.SendBlock(len);
                server.SendString(type);
                message = ECDHPSI::Server(server, pp, vec_y);
                server.DeleteIo();
                std::cout<<"data size::"<<message.data.size()<<std::endl;
            }
            catch (std::exception &e) {
                message.code = 0;
                message.msg = e.what();
                return message;
            }
            message.code = 200;
            message.msg = "执行成功";

        } else {
            pp = ECDHPSI::Setup("bloom", 40, vec_y.size(),vec_y.size());
            try {
                NetIO client("client", ip, std::stoi(port));
                block a;
                client.ReceiveBlock(a);
                int strlen = Block::BlockToInt64(a);
                //接收协议类型 进行校验
                std::string rectype(strlen, '0');
                client.ReceiveString(rectype);
                if (rectype != type) {
                    client.DeleteIo();
                    return Message(0, "两方所选协议类型不同，当前服务端所选协议类型为：" + rectype + "。");
                }
                message = ECDHPSI::Client(client, pp, vec_y);
                client.DeleteIo();
            } catch (std::exception &e) {
                message.code = 0;
                message.msg = e.what();
                return message;
            }
            message.code = 200;
            message.msg = "执行成功";

        }
        CRYPTO_Finalize();
        return message;
    }

}
