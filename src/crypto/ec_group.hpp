#ifndef CRYPTO_EC_GROUP_HPP_
#define CRYPTO_EC_GROUP_HPP_

// enable pre-computation for fixed point multiplication
#define PRECOMPUTE_ENABLE

/* 
** enable point compression
** will save bandwidth by half at the cost of expensive decompression 
*/

//#define ECPOINT_COMPRESSED

#include "../include/global.hpp"
#include "../utility/print.hpp"
#include "bigint.hpp"


static EC_GROUP *group;
const static EC_POINT *generator; 

static BIGNUM *order;
static BIGNUM *cofactor;  // The cofactor of this group
static BIGNUM *curve_params_p; 
static BIGNUM *curve_params_a; 
static BIGNUM *curve_params_b;
static BIGNUM *curve_params_q; // q = (p-1)/2

static size_t POINT_BYTE_LEN; // the byte length of ec point
static size_t POINT_COMPRESSED_BYTE_LEN; // the byte length of ec point in compressed form

static BN_CTX *ec_ctx; // define ctx for ecc operations

static int curve_id = NID_X9_62_prime256v1; 
// static int curve_id = NID_secp256k1; 


void ECGroup_Initialize(){
    #ifdef PARALLEL
        ec_ctx = nullptr;
    #else
        ec_ctx = bn_ctx; 
    #endif

    group = EC_GROUP_new_by_curve_name(curve_id);
    // If this fails, this is usually due to an invalid curve id.
    CRYPTO_CHECK(group !=nullptr);

    generator = EC_GROUP_get0_generator(group);

    order = BN_new(); 
    CRYPTO_CHECK(EC_GROUP_get_order(group, order, bn_ctx) == 1);

    cofactor = BN_new(); 
    CRYPTO_CHECK(EC_GROUP_get_cofactor(group, cofactor, bn_ctx) == 1); 

    curve_params_p = BN_new(); 
    curve_params_a = BN_new();
    curve_params_b = BN_new(); 

    CRYPTO_CHECK(EC_GROUP_get_curve_GFp(group, curve_params_p, curve_params_a, curve_params_b, bn_ctx) == 1); 

    size_t rounds = 100; 
    CRYPTO_CHECK(BN_is_prime_ex(curve_params_p, rounds, bn_ctx, nullptr) == 1);

    curve_params_q = BN_new(); 
    BN_rshift(curve_params_q, curve_params_p, 1); // p_minus_one_over_two = (p-1)/2

    // PrintSplitLine('-'); 
    // std::cout << "EC group info >>> " << std::endl;

    // std::cout << "a = " << BN_bn2hex(curve_params_a) << std::endl;  
    // std::cout << "b = " << BN_bn2hex(curve_params_b) << std::endl;  
    // std::cout << "p = " << BN_bn2hex(curve_params_p) << std::endl;  
    // std::cout << "q = " << BN_bn2hex(curve_params_q) << std::endl;  
    // PrintSplitLine('-'); 

    BN_BYTE_LEN = BN_num_bits(curve_params_p)/8 + BN_num_bits(curve_params_p)%8;
    POINT_BYTE_LEN = BN_BYTE_LEN * 2 + 1; 
    POINT_COMPRESSED_BYTE_LEN = BN_BYTE_LEN + 1; 

     
    
    #ifdef PRECOMPUTE_ENABLE
        EC_GROUP_precompute_mult((EC_GROUP*) group, bn_ctx); // pre-compute the table of g    
        // check if precomputation have been done properly
        if(EC_GROUP_have_precompute_mult((EC_GROUP*) group) == 0){
            std::cerr << "precomputation is not done properly" << std::endl;
        }; 
    #endif
}

void ECGroup_Finalize(){
    EC_GROUP_free(group);
    
    BN_free(curve_params_p); 
    BN_free(curve_params_a); 
    BN_free(curve_params_b);
    BN_free(curve_params_q); 

    BN_free(order); 
    BN_free(cofactor); 
}

#endif //_CRYPTO_EC_GROUP_HPP_







