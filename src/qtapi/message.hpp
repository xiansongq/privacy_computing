//
// Created by 17579 on 2023/5/26.
// 返回协议执行状态的结构体
//

#ifndef PRIVATE_MESSGE_HPP
#define PRIVATE_MESSGE_HPP
#include <iostream>



struct Message{
    int code; // 执行状态
    std::string msg ; //返回消息
    std::vector<std::string> data; //返回数据
    std::vector<std::string> pirdata; // pir查询返回的数据
    double receiver_time; //receiver执行时间
    double sender_time; //sender 执行时间
    Message(int code1,std::string msg1){
        code=code1;
        msg=msg1;
    }
    Message(){};
};

#endif //PRIVATE_MESSGE_HPP
