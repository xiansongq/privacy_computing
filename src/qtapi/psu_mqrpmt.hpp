//
// Created by 17579 on 2023/5/26.
//

#ifndef PRIVATE_mqRPMT_PSU_HPP
#define PRIVATE_mqRPMT_PSU_HPP
#include "../mpc/psu/mqrpmt_psu.hpp"
#include "../crypto/setup.hpp"
#include "message.hpp"
#include <thread>
//#include "psi_cm20.h"



namespace PSU{
//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
void static  localpsureceiver(NetIO &io,mqRPMTPSU::PP &pp, std::vector <block> vecx, Message &message, int slen) ;

//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
inline void static localpsusender(NetIO &io,mqRPMTPSU::PP &pp, std::vector <block> vecy, Message &message) ;

 Message localhost_PSU(std::string filename1, std::string filename2);

// 定义联机执行的 api
 Message remotepsu(std::string ip, std::string port, std::string filename, std::string isserver,std::string type) ;


};

#endif //PRIVATE_mqRPMT_PSU_HPP