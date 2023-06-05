//
// Created by 17579 on 2023/5/26.
//

#ifndef PRIVATE_PSI_CM20_HPP
#define PRIVATE_PSI_CM20_HPP
#include "../mpc/psi/psi_from_oprf.hpp"
#include "../crypto/setup.hpp"
#include "message.hpp"
#include <thread>
//#include "psi_cm20.h"



namespace PSI{
//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
 void static  localpsireceiver(OTEOPRF::PP pp, std::vector <block> vecx, Message &message, int slen) ;

//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
inline void static localpsisender(OTEOPRF::PP pp, std::vector <block> vecy, Message &message) ;

 Message localhostPSI(std::string filename1, std::string filename2);


// 定义联机执行的 api
 Message remotepsi(std::string ip, std::string port, std::string filename, std::string isserver) ;


};

#endif