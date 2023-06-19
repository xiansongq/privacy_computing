//
// Created by 17579 on 2023/6/16.
//


#include "../qtapi/ecdh_psi.hpp"
int main(){
        Message message=ECDH_PSI::localhostPSI("A_PSI_DATA_2_10.txt","B_PSI_DATA_2_10.txt");
        std::cout<<message.code<<std::endl;
        std::cout<<message.msg<<std::endl;
        std::cout<<message.data.size()<<std::endl;
        return 0;
}
