/*
** Modified from the following project
** 1. https://github.com/emp-toolkit/
*/

#ifndef KUNLUN_CRYPTO_BLOCK_HPP_   //防止头文件重复包含
#define KUNLUN_CRYPTO_BLOCK_HPP_

#include "../include/std.inc"
#include "../utility/serialization.hpp"


typedef __m128i block;  //定义块别名

//定义名字空间
namespace Block {
// 序列化和反序列函数
    using Serialization::operator<<;
    using Serialization::operator>>;

// generate a block from two uint64_t values  根据两个uint64_t 类型的值生成一个block
    __attribute__((target("sse2")))
    inline block MakeBlock(uint64_t high, uint64_t low) {
        return _mm_set_epi64x(high, low);    // 使用 _mm_set_epi64x 函数生成一个 __m128i 类型的数据
    }

// 返回低64位整数
    inline int64_t BlockToInt64(const block &a) {
        return _mm_cvtsi128_si64(a);
    }

//返回高64位整数  该函数 需要在编译指令中加入 -msse4.2
    inline uint64_t BlockToUint64High(const block &a) {
        return _mm_extract_epi64(a, 1);  // 将 block 中索引为1的64位整数单独提取出来，即返回高64位
    }
    //返回低64位整数  该函数 需要在编译指令中加入 -msse4.2
    inline uint64_t BlockToUint64Low(const block &a) {
        return _mm_extract_epi64(a, 0);  // 将 block 中索引为1的64位整数单独提取出来，即返回高64位
    }

    // char buffer to block
    inline void CharBufferToBlock(const unsigned char *in,block &a){
        a= _mm_loadu_si128(reinterpret_cast<const __m128i*>(in));
    }


    const block zero_block = _mm_set_epi64x(0, 0);  // 初始化一个所有位均为 0 的 block 常量
    const block all_one_block = _mm_set_epi64x(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // 初始化一个所有位均为 1 的 block 常量
    const block select_mask[2] = {zero_block, all_one_block};   // 初始化两个用于选择的 block 常量，分别对应 0 和 1

// copy from https://devblogs.microsoft.com/oldnewthing/20141222-00/?p=43333
// Setting, clearing, and testing a single bit in an SSE register


// generate a mask block with n-th bit = 1
// 生成一段二进制数据，其中第 n 位为 1，其他位均为 0
    block GenMaskBlock(size_t n) {
        block onesLowHigh = _mm_slli_epi64(all_one_block,
                                           63);     // 将所有位均为 1 的 block 常量往左位移 63 位，得到一个左侧为 1 右侧为 0 的 block
        block singleOne = n < 64 ? _mm_slli_si128(onesLowHigh, 64 / 8) : _mm_srli_si128(onesLowHigh, 64 /
                                                                                                     8);  // 将上述 block 常量右移 n 位或者左移 n-64 位得到一个其中只有第 n 位为 1 的 block
        return _mm_srli_epi64(singleOne, n & 63);  // 将上述 block 常量右移 n & 63 位，得到一个其中只有第 n 位为 1 的 block
    }

// set the n-th bit = 1
    void SetBit(block &a, size_t n) {
        a = _mm_or_si128(a, GenMaskBlock(n));  // 使用 _mm_or_si128 函数将第 n 位设置为 1
    }

// set the n-th bit = 0
    void ClearBit(block &a, size_t n) {
        a = _mm_andnot_si128(a, GenMaskBlock(n));  // 使用 _mm_andnot_si128 函数将第 n 位设置为 0
    }

    inline std::vector<block> AND(std::vector<block> &vec_a, std::vector<block> &vec_b) {
        if (vec_a.size() != vec_b.size()) {
            std::cerr << "XORBlocks: size does not match" << std::endl;
        }
        size_t LEN = vec_a.size();

        std::vector<block> vec_result(LEN);
        for (auto i = 0; i < LEN; i++) {
            vec_result[i] = _mm_and_si128(vec_a[i], vec_b[i]);
        }
        return vec_result;
    }


    inline std::vector<block> XOR(std::vector<block> &vec_a, std::vector<block> &vec_b) {
        if (vec_a.size() != vec_b.size()) {
            std::cerr << "XORBlocks: size does not match" << std::endl;
        }
        size_t LEN = vec_a.size();

        std::vector<block> vec_result(LEN);
        for (auto i = 0; i < LEN; i++) {
            vec_result[i] = vec_a[i] ^ vec_b[i];
        }
        return vec_result;
    }

    inline std::vector<block> FixXOR(std::vector<block> &vec_a, block &b) {
        size_t LEN = vec_a.size();
        std::vector<block> vec_result(LEN);
        for (auto i = 0; i < LEN; i++) {
            vec_result[i] = vec_a[i] ^ b;
        }
        return vec_result;
    }

    __attribute__((target("sse4")))
    inline bool Compare(std::vector<block> &vec_a, std::vector<block> &vec_b) {
        if (vec_a.size() != vec_b.size()) {
            std::cerr << "size of block vector does not match" << std::endl;
        }

        bool EQUAL = true;
        for (auto i = 0; i < vec_a.size(); i++) {
            __m128i vcmp = _mm_xor_si128(vec_a[i], vec_b[i]);
            if (!_mm_testz_si128(vcmp, vcmp)) {
                std::cerr << "blocks differ at position: " << i << std::endl;
                EQUAL = false;
            }
        }
        return EQUAL;
    }

    inline bool Compare(const block &a, const block &b) {
        __m128i vcmp = _mm_xor_si128(a, b);
        if (!_mm_testz_si128(vcmp, vcmp)) return false;
        else return true;
    }


    bool IsLessThan(const block &a, const block &b) {
        /* Compare 8-bit lanes for ( a < b ), store the bits in the low 16 bits of thescalar value: */
        int less = _mm_movemask_epi8(_mm_cmplt_epi8(a, b));

        /* Compare 8-bit lanes for ( a > b ), store the bits in the low 16 bits of thescalar value: */
        int greater = _mm_movemask_epi8(_mm_cmpgt_epi8(a, b));

        /* It's counter-intuitive, but this scalar comparison does the right thing.
           Essentially, integer comparison searches for the most significant bit that differs... */
        return less > greater;
    }

    inline std::string ToString(const block &var) {
        std::string str(16, '0');
        memcpy(&str[0], &var, 16);
        return str;
    }

// shrink 128*n bits into n block
    inline void FromSparseBytes(const uint8_t *byte_data, size_t BYTE_LEN, block *block_data, size_t BLOCK_LEN) {
        if (BYTE_LEN != BLOCK_LEN * 128) {
            std::cerr << "FromSparseBytes: size does not match" << std::endl;
        }

        for (auto i = 0; i < BLOCK_LEN; i++) {
            block_data[i] = zero_block;
            for (auto j = 0, k = 127; j < 128 && k >= 0; j++, k--)
                if (byte_data[128 * i + j]) {
                    SetBit(block_data[i], k);
                }
        }
    }

    // __m128i to __uint128_t
    __uint128_t TO__uint128_t(block a) {
        __uint128_t b;
        memcpy(&b, &a, sizeof(a));
        return b;
    }

    // __uint128_t to __m128i
    block TO_block(__uint128_t a) {
        unsigned long long high = (unsigned long long) (a >> 64);  // 高 64 位
        unsigned long long low = (unsigned long long) (a & 0xffffffffffffffff); // 低 64 位
        block b = MakeBlock(high, low);
        return b;
    }


    inline void PrintBlock(const block &a) {
        std::cout << std::hex;
        uint64_t *data = (uint64_t *) &a;

        std::cout << std::setw(16) << std::setfill('0') << data[1] << " ";
        std::cout << std::setw(16) << std::setfill('0') << data[0] << std::endl;

        std::cout << std::dec << std::setw(0);
    }


    void PrintBlocks(block *var, size_t LEN) {
        for (auto i = 0; i < LEN; i++) {
            PrintBlock(var[i]);
            //std::cout << std::endl;
        }
    }

    void PrintBlocks(std::vector<block> vec_B) {
        for (auto i = 0; i < vec_B.size(); i++) {
            PrintBlock(vec_B[i]);
            std::cout << std::endl;
        }
    }

    std::ofstream &operator<<(std::ofstream &fout, const block &a) {
        char buffer[16];
        _mm_storeu_si128((block *) buffer, a);
        fout.write(buffer, 16);
        return fout;
    }

    std::ifstream &operator>>(std::ifstream &fin, block &a) {
        char buffer[16];
        fin.read(buffer, 16);
        a = _mm_load_si128((block *) buffer);
        return fin;
    }

    std::ofstream &operator<<(std::ofstream &fout, const std::vector<block> &vec_a) {
        size_t LEN = vec_a.size() * 16;
        char *buffer = new char[LEN]();
        memcpy(reinterpret_cast<block *>(buffer), vec_a.data(), LEN);
        fout.write(buffer, LEN);
        delete[] buffer;
        return fout;
    }

    std::ifstream &operator>>(std::ifstream &fin, std::vector<block> &vec_a) {
        size_t LEN = vec_a.size() * 16;
        char *buffer = new char[LEN]();
        fin.read(buffer, LEN);
        memcpy(reinterpret_cast<block *>(vec_a.data()), buffer, LEN);
        delete[] buffer;
        return fin;
    }

}


// custom hash
class BlockHash {
public:
    size_t operator()(const block &a) const {
        return std::hash<std::string>{}(Block::ToString(a));
    }
};

// custom compare
class BlockCompare {
public:
    bool operator()(const block &a, const block &b) const {
        return Block::IsLessThan(a, b);
    }
};

std::set<block, BlockCompare> ComputeSetDifference(std::vector<block> &vec_A, std::vector<block> &vec_B) {
    std::set<block, BlockCompare> set_A;
    for (auto var: vec_A) set_A.insert(var);

    std::set<block, BlockCompare> set_B;
    for (auto var: vec_B) set_B.insert(var);

    BlockCompare blockcmp;
    std::set<block, BlockCompare> set_diff_result;
    std::set_difference(set_A.begin(), set_A.end(), set_B.begin(), set_B.end(),
                        std::inserter<std::set<block, BlockCompare>>(set_diff_result, set_diff_result.end()),
                        blockcmp);

    return set_diff_result;
}



// Modified from
// https://mischasan.wordpress.com/2011/10/03/the-full-sse2-bit-matrix-transpose-routine/
// with inner most loops changed to _mm_set_epi8 and _mm_set_epi16

/*
** ROW_NUM and COLUMN_NUM are meant for matrix after transpose 
** When call this function, be aware of the order of ROW_NUM and COLUMN_NUM 
*/
#define INPUT(x, y) input[(x)*COLUMN_NUM/8 + (y)/8]
#define OUTPUT(x, y) output[(y)*ROW_NUM/8 + (x)/8]

__attribute__((target("sse2")))
inline void BitMatrixTranspose(uint8_t const *input, uint64_t ROW_NUM, uint64_t COLUMN_NUM, uint8_t *output) {
    int rr, cc, i, h;
    union {
        __m128i x;
        uint8_t b[16];
    } tmp;
    __m128i vec;
    assert(ROW_NUM % 8 == 0 && COLUMN_NUM % 8 == 0);

    // Do the main body in 16x8 blocks:
    for (rr = 0; rr <= ROW_NUM - 16; rr += 16) {
        for (cc = 0; cc < COLUMN_NUM; cc += 8) {
            vec = _mm_set_epi8(INPUT(rr + 15, cc), INPUT(rr + 14, cc), INPUT(rr + 13, cc),
                               INPUT(rr + 12, cc), INPUT(rr + 11, cc), INPUT(rr + 10, cc),
                               INPUT(rr + 9, cc), INPUT(rr + 8, cc), INPUT(rr + 7, cc),
                               INPUT(rr + 6, cc), INPUT(rr + 5, cc), INPUT(rr + 4, cc),
                               INPUT(rr + 3, cc), INPUT(rr + 2, cc), INPUT(rr + 1, cc),
                               INPUT(rr + 0, cc));
            for (i = 8; --i >= 0; vec = _mm_slli_epi64(vec, 1))
                *(uint16_t *) &OUTPUT(rr, cc + i) = _mm_movemask_epi8(vec);
        }
    }
    if (rr == ROW_NUM) return;

    // The remainder is a block of 8x(16n+8) bits (n may be 0).
    // Do a PAIR of 8x8 blocks in each step:
    if ((COLUMN_NUM % 8 == 0 && COLUMN_NUM % 16 != 0) || (ROW_NUM % 8 == 0 && ROW_NUM % 16 != 0)) {
        // The fancy optimizations in the else-branch don't work if the above if-condition
        // holds, so we use the simpler non-simd variant for that case.
        for (cc = 0; cc <= COLUMN_NUM - 16; cc += 16) {
            for (i = 0; i < 8; ++i) {
                tmp.b[i] = h = *(uint16_t const *) &INPUT(rr + i, cc);
                tmp.b[i + 8] = h >> 8;
            }
            for (i = 8; --i >= 0; tmp.x = _mm_slli_epi64(tmp.x, 1)) {
                OUTPUT(rr, cc + i) = h = _mm_movemask_epi8(tmp.x);
                OUTPUT(rr, cc + i + 8) = h >> 8;
            }
        }
    } else {
        for (cc = 0; cc <= COLUMN_NUM - 16; cc += 16) {
            vec = _mm_set_epi16(*(uint16_t const *) &INPUT(rr + 7, cc),
                                *(uint16_t const *) &INPUT(rr + 6, cc),
                                *(uint16_t const *) &INPUT(rr + 5, cc),
                                *(uint16_t const *) &INPUT(rr + 4, cc),
                                *(uint16_t const *) &INPUT(rr + 3, cc),
                                *(uint16_t const *) &INPUT(rr + 2, cc),
                                *(uint16_t const *) &INPUT(rr + 1, cc),
                                *(uint16_t const *) &INPUT(rr + 0, cc));
            for (i = 8; --i >= 0; vec = _mm_slli_epi64(vec, 1)) {
                OUTPUT(rr, cc + i) = h = _mm_movemask_epi8(vec);
                OUTPUT(rr, cc + i + 8) = h >> 8;
            }
        }
    }
    if (cc == COLUMN_NUM) return;

    //  Do the remaining 8x8 block:
    for (i = 0; i < 8; ++i)
        tmp.b[i] = INPUT(rr + i, cc);
    for (i = 8; --i >= 0; tmp.x = _mm_slli_epi64(tmp.x, 1))
        OUTPUT(rr, cc + i) = _mm_movemask_epi8(tmp.x);
}


#endif

