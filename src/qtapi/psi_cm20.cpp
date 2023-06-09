//
// Created by 17579 on 2023/5/30.
//

#include "psi_cm20.hpp"

namespace PSI {
    //创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
    void localpsireceiver(NetIO &io, OTEOPRF::PP pp, std::vector <block> vecx, Message &message, int slen) {
        try {
            message = OPRFPSI::Receive(io, pp, vecx);
        } catch (std::exception &e) {
            message.msg = e.what();
            message.code = 0;
        }
        //去除多余的数据
        std::vector <std::string> ans(message.data.size() - slen);
        for (int i = 0; i < message.data.size() - slen; i++)
            ans[i] = message.data[i];
        message.data = ans;
        io.DeleteIo();
        return;
    }

//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
    void localpsisender(NetIO &io, OTEOPRF::PP pp, std::vector <block> vecy, Message &message) {
        try {
            message = OPRFPSI::Send(io, pp, vecy);
        } catch (std::exception &e) {
            message.code = 0;
            message.msg = e.what();
        }
        io.DeleteIo();
        return;


    }

    void create_netio(std::string party, OTEOPRF::PP pp, std::vector <block> vect, Message &message,
                      int slen) {
        NetIO io(party, "127.0.0.1", 8085);

        if (party == "server") {
            localpsireceiver(io, pp, vect, message, slen);
        }

        if (party == "client") {
            localpsisender(io, pp, vect, message);
        }

        // io.DeleteIo();
    }

    Message localhost_PSI(std::string filename1, std::string filename2) {
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

        // 快速判断集合大小是否是2的次方倍
        size_t len = pow(2, ceil(log2(vec_x.size())));
        size_t slen = len - vec_x.size();
        //数据补齐
        for (int i = 0; i < slen; i++) vec_x.push_back(Block::MakeBlock(0LL, 0LL));
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
        //数据补齐
        for (int i = 0; i < slen; i++) vec_y.push_back(Block::MakeBlock(0LL, 0LL));
        std::cout << vec_x.size() << " " << vec_y.size() << std::endl;
        CRYPTO_Initialize();
        OTEOPRF::PP pp;
        pp = OTEOPRF::Setup(ceil(log2(len)), 40);
        OTEOPRF::PP pp1;
        pp1 = OTEOPRF::Setup(ceil(log2(len)), 40);

//        std::thread receiverthread(PSI::localpsireceiver, std::move(pp), std::move(vec_x), std::ref(message1),
//                                   std::move(slen));
//

//        std::thread senderthread(PSI::localpsisender, std::move(pp), std::move(vec_y), std::ref(message2));
        std::thread receiverthread(PSI::create_netio, "server", std::move(pp), std::move(vec_x), std::ref(message1),
                                   std::move(slen));

        std::thread senderthread(PSI::create_netio, "client", std::move(pp1), std::move(vec_y), std::ref(message2),
                                 std::move(slen));
        // std::thread receiverthread(PSI::localpsireceiver, pp, vec_x,&message2,slen);
        //std::thread senderthread(PSI::localpsisender, pp, vec_y, &message1);
        receiverthread.join();
        senderthread.join();

        message1.sender_time = message2.sender_time;
        std::cout << message1.code << " " << message1.msg << " " << message1.data.size() << std::endl;
        CRYPTO_Finalize();
        return message1;

    }


// 定义联机执行的 api
    Message remotepsi(std::string ip, std::string port, std::string filename, std::string isserver, std::string type) {
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
        // 快速判断集合大小是否是2的次方倍
        size_t len = pow(2, ceil(log2(vec_y.size())));
        size_t slen = len - vec_y.size();
        //数据补齐
        for (int i = 0; i < slen; i++) vec_y.push_back(Block::MakeBlock(0LL, 0LL));
        std::cout << vec_y.size() << std::endl;
        //std::cout<<"io info: "<<ip<<" "<<std::stoi(port)<<std::endl;
        //初始化参数
        CRYPTO_Initialize();
        OTEOPRF::PP pp;
        pp = OTEOPRF::Setup(ceil(log2(len)), 40);
        // std::cout<<"ip info1 "<<isserver<<std::endl;
        // 执行协议
        if (isserver == "1") {
            try {

                NetIO server("server", ip, std::stoi(port));
                // 发送协议类型 type给客户端
                block len = Block::MakeBlock('0LL', type.size());
                server.SendBlock(len);
                server.SendString(type);
                size_t cmp;
                server.ReceiveInteger(cmp);
                std::cout<<"cmp value :"<<cmp<<std::endl;
                if(cmp==-1){
                    server.DeleteIo();
                    return Message(0, "两方所选协议类型不同，你所选协议为：" + type + "。");
                }
                message = OPRFPSI::Receive(server, pp, vec_y);
                server.DeleteIo();
            }
            catch (std::exception &e) {
                message.code = 0;
                message.msg = e.what();
                return message;
            }
            //提出 用来删除补充的数据
            std::vector <std::string> ans(message.data.size() - slen);
            for (int i = 0; i < message.data.size() - slen; i++) {
                ans[i] = message.data[i];
            }
            message.code = 200;
            message.msg = "执行成功";
            message.data = ans;
        } else {
            try {
                NetIO client("client", ip, std::stoi(port));
                block a;
                client.ReceiveBlock(a);
                int strlen = Block::BlockToInt64(a);
                //接收协议类型 进行校验
                std::string rectype(strlen, '0');
                client.ReceiveString(rectype);
                if (rectype != type) {
                    client.SendInteger(-1);
                    client.DeleteIo();
                    return Message(0, "两方所选协议类型不同，当前服务端所选协议类型为：" + rectype + "。");
                }
                message = OPRFPSI::Send(client, pp, vec_y);
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
