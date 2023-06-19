//
// Created by 17579 on 2023/6/16.
//

#ifndef PRIVATE_COMPUTATION_PSI_FACTORY_HPP
#define PRIVATE_COMPUTATION_PSI_FACTORY_HPP

#include "psi_cm20.cpp"
#include "ecdh_psi.cpp"

namespace PSIFactory {
    /*
     * filename1 文件1
     * filename2 文件2
     * type  协议类型
     * */
    Message local_psi(std::string filename1, std::string filename2, std::string type) ;
    /*
     * ip ip地址
     * port 端口号
     * isserver 是否是服务端
     * filename 文件名称正确不
     * type 协议类型
     * */
    Message remote_psi(std::string ip, std::string port, std::string isserver, std::string filename, std::string type) ;

}
#endif //PRIVATE_COMPUTATION_PSI_FACTORY_HPP
