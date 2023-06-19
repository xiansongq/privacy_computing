//
// Created by 17579 on 2023/6/16.
//

#include "psu_factory.hpp"

namespace PSUFactory {
    /*
     * filename1 文件1
     * filename2 文件2
     * type  协议类型
     * */
    Message local_psu(std::string filename1, std::string filename2, std::string type) {

        if (type == "ecc_mqrpmt") {
            return PSU::localhost_PSU(filename1, filename2);
        } else {
            return Message(0, "所选择协议类型不存在！");
        }
        return Message(0, "发生未知错误，请联系管理员");
    }

    /*
     * ip ip地址
     * port 端口号
     * isserver 是否是服务端
     * filename 文件名称正确不
     * type 协议类型
     * */
    Message remote_psu(std::string ip, std::string port,std::string filename, std::string isserver,  std::string type) {
        if (type == "ecc_mqrpmt") {
            return PSU::remotepsu(ip, port, filename, isserver, type);
        } else {
            return Message(0, "所选择协议类型不存在！");
        }
        return Message(0, "发生未知错误，请联系管理员");
    }

}