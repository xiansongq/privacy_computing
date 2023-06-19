//
// Created by 17579 on 2023/6/16.
//

#ifndef PRIVATE_COMPUTATION_PSU_FACTORY_HPP
#define PRIVATE_COMPUTATION_PSU_FACTORY_HPP

#include "psu_mqrpmt.cpp"


namespace PSUFactory {
    /*
     * filename1 文件1
     * filename2 文件2
     * type  协议类型
     * */
    Message local_psu(std::string filename1, std::string filename2, std::string type) ;
    /*
     * ip ip地址
     * port 端口号
     * isserver 是否是服务端
     * filename 文件名称正确不
     * type 协议类型
     * */
    Message remote_psu(std::string ip, std::string port, std::string isserver, std::string filename, std::string type) ;

}
#endif //PRIVATE_COMPUTATION_PSU_FACTORY_HPP
