###  可视化隐私计算平台

#### 1、项目介绍

本项目是基于C++ Qt实现的一个隐私计算软件。目前提供以下几个隐私计算协议：

- PSI
  - ECDH PSI
  - cm20
- PSU
  - mq-rpmt PSU
- PIR
  - sealPIR

所有的协议都提供本地执行和在线远程执行两个版本。filetest文件中提供了一些测试数据，大小分别为$$2^{10}$$ 和$$2^{20}$$。 

#### 2、源码编译安装

本项目依赖于openSSL、SEAL、openMP第三方库。

##### 安装openSSL

```bash
wget --no-check-certificate https://www.openssl.org/source/old/3.0/openssl-3.0.0.tar.gz
tar -zxvf openssl-3.0.0.tar.gz
```

修改openSSL的源码，将**crypto/ec/curve25519.c 的第211行的 "static "删除**。继续安装：

```bash
cd openssl-3.0.0
./config --prefix=/usr/local/openssl --openssldir=/usr/local/openssl 
```

此时openssl头文件的目录是：

```bash
/usr/local/openssl/lib64/include 
```

有时可能需要修改CMakeList.txt文件中openSSL的路径。

编译安装

```bash
make depend
sudo make install
```

##### 安装openMP

```
sudo apt-get install libomp-dev
```

##### 安装SEAL 4.0

```bash
git clone -b v4.0.0 https://github.com/microsoft/SEAL.git
# 进入 SEAL 目录
cd SEAL
# 创建构建目录
mkdir build
cd build
# 配置 SEAL
cmake..

# 编译 SEAL
make
# 安装 SEAL
sudo make install
```

##### 编译本项目

```bash
# 回到要拉取代码的目录
git clone https://github.com/xiansongq/privacy_computing.git
cd privacy_computing
cmake .
make
```

编译后的可执行文件输出到build目录。

##### 3、免责声明

此存储库中的代码尚未经过安全审核，**不适合在生产系统中部署**。作者不保证（包括但不限于本存储库中代码的安全性、安全性或正确性）。

##### 4、参考资料

该软件实现了以下论文描述的算法：

- Chase M, Miao P. Private set intersection in the internet setting from lightweight oblivious PRF[C]//Advances in Cryptology–CRYPTO 2020: 40th Annual International Cryptology Conference, CRYPTO 2020, Santa Barbara, CA, USA, August 17–21, 2020, Proceedings, Part III 40. Springer International Publishing, 2020: 34-63.
- Angel S, Chen H, Laine K, et al. PIR with compressed queries and amortized query processing[C]//2018 IEEE symposium on security and privacy (SP). IEEE, 2018: 962-979.
- Zhang C, Chen Y, Liu W, et al. Linear Private Set Union from Multi-Query Reverse Private Membership Test[J]. Cryptology ePrint Archive, 2022.

部分算法代码参考以下库：

[yuchen1024/Kunlun: A C++ library containing both easy-to-use lower level bigint/ecgroup interfaces and high-level cryptographical schemes/protocols. (github.com)](https://github.com/yuchen1024/Kunlun)


[![Security Status](https://www.murphysec.com/platform3/v31/badge/1671012518867001344.svg)](https://www.murphysec.com/console/report/1671012518770532352/1671012518867001344)


