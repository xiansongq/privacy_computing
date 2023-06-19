//
// Created by 17579 on 2023/6/16.
//

#include "psi_factory.hpp"

namespace PSIFactory {
    /*
     * filename1 文件1
     * filename2 文件2
     * type  协议类型
     * */
    Message local_psi(std::string filename1, std::string filename2, std::string type) {
//        switch (type) {  //c++ 11以上
//            case "CM20":
//                return PSI::localhostPSI(filename1, filename2);
//                break;
//            case "ECDH":
//                return ECDH_PSI::localhostPSI(filename1, filename2);
//                break;
//            default:
//                return Message(0, "所选择协议类型不存在！");
//                break;
//        }
        if (type == "CM20") {
            return PSI::localhost_PSI(filename1, filename2);
        } else if (type == "ECDH") {
            return ECDH_PSI::localhostPSI(filename1, filename2);
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
    Message remote_psi(std::string ip, std::string port,std::string filename, std::string isserver,  std::string type) {
//        switch (type) {   //c++ 11
//            case "CM20":
//                return PSI::remotepsi(ip, port, filename, isserver,type);
//                break;
//            case "ECDH":
//                return ECDH_PSI::remotepsi(ip, port, filename, isserver,type);
//                break;
//            default:
//                return Message(0, "所选择协议类型不存在！");
//                break;
//        }
        if (type == "CM20") {
            return PSI::remotepsi(ip, port, filename, isserver, type);
        } else if (type == "ECDH") {
            return ECDH_PSI::remotepsi_(ip, port, filename, isserver, type);
        } else {
            return Message(0, "所选择协议类型不存在！");
        }
        return Message(0, "发生未知错误，请联系管理员");
    }

}