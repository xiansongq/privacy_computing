//
// Created by 17579 on 2023/6/14.
//

#ifndef PRIVATE_COMPUTATION_DH_PSI_HPP
#define PRIVATE_COMPUTATION_DH_PSI_HPP
#include "../crypto/setup.hpp"
#include "../mpc/psi/ecdh_psi.hpp"
#include "message.hpp"
#include <thread>
namespace ECDH_PSI{
    //创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
    void   localpsireceiver(ECDHPSI::PP pp, std::vector <block> vecx, Message &message) ;

//创建本地执行 psi 的api 需要两个函数 一边模拟 receiver 一边模拟 sender
     void  localpsisender( ECDHPSI::PP pp,  std::vector <block> vecy, Message &message) ;

    Message localhostPSI(std::string filename1, std::string filename2);

// 定义联机执行的 api
    Message remotepsi_(std::string ip, std::string port, std::string filename, std::string isserver,std::string type) ;

}
#endif //PRIVATE_COMPUTATION_DH_PSI_HPP
