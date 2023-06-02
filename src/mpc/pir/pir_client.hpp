/***********************************************************************************
this hpp file implements the core code of Sealpir  Client
************************************************************************************

* @paper      https://eprint.iacr.org/2017/1142.pdf
***********************************************************************************/


#pragma once

#include "pir.hpp"
#include <memory>
#include <vector>

using namespace std;

class PIRClient {
public:
    PIRClient(const seal::EncryptionParameters &encparms,
              const PirParams &pirparams);

    PirQuery generate_query(std::uint64_t desiredIndex);

    // Serializes the query into the provided stream and returns number of bytes
    // written
    int generate_serialized_query(std::uint64_t desiredIndex,
                                  std::stringstream &stream);

    seal::Plaintext decode_reply(PirReply &reply);

    std::vector<uint64_t> extract_coeffs(seal::Plaintext pt);

    std::vector<uint64_t> extract_coeffs(seal::Plaintext pt,
                                         std::uint64_t offset);

    std::vector<uint8_t> extract_bytes(seal::Plaintext pt, std::uint64_t offset);

    std::vector<uint8_t> decode_reply(PirReply &reply, uint64_t offset);

    seal::Plaintext decrypt(seal::Ciphertext ct);

    seal::GaloisKeys generate_galois_keys();

    // Index and offset of an element in an FV plaintext
    uint64_t get_fv_index(uint64_t element_index);

    uint64_t get_fv_offset(uint64_t element_index);

    // Only used for simple_query
    seal::Ciphertext get_one();

    seal::Plaintext replace_element(seal::Plaintext pt,
                                    std::vector<std::uint64_t> new_element,
                                    std::uint64_t offset);

    //std::string serialize_galoiskeys(seal::Serializable <seal::GaloisKeys> g);
    PirReply deserialize_reply(stringstream &istream,seal::SEALContext context) ;
    std::shared_ptr<seal::SEALContext> context_;
private:
    seal::EncryptionParameters enc_params_;
    PirParams pir_params_;

    std::unique_ptr<seal::Encryptor> encryptor_;
    std::unique_ptr<seal::Decryptor> decryptor_;
    std::unique_ptr<seal::Evaluator> evaluator_;
    std::unique_ptr<seal::KeyGenerator> keygen_;
    std::unique_ptr<seal::BatchEncoder> encoder_;


    vector<uint64_t> indices_; // the indices for retrieval.
    vector<uint64_t> inverse_scales_;

    friend class PIRServer;
};
