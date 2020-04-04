#include "crypto/HorizonCrypto.hpp"

#ifdef __SWITCH__
#include <switch.h>
#else
#include <mbedtls/aes.h>
#endif

#include "crypto/SeadRand.hpp"

std::array<uint8_t, 0x10> getKeyOrIV(uint32_t* headerCryptSectionBuffer, uint64_t sourceIdx) {
    // Look up prng seed and advance it by some amount
    SeadRand rng(headerCryptSectionBuffer[headerCryptSectionBuffer[sourceIdx] & 0x7F]);
    uint32_t rngAdvanceCount =
        (headerCryptSectionBuffer[headerCryptSectionBuffer[sourceIdx + 1] & 0x7F] & 0xF) + 1;
    for (uint32_t i = 0; i < rngAdvanceCount; i++) {
        rng.getU64();
    }

    // Generate key
    std::array<uint8_t, 0x10> result;
    for (int i = 0; i < 0x10; i++) {
        result[i] = (uint8_t)(rng.getU32() >> 24);
    }
    return result;
}

void aesCryptCtr(const uint8_t* in_buf, std::array<uint8_t, 0x10> key, std::array<uint8_t, 0x10> iv,
                 size_t len, uint8_t* out_buf) {
#ifdef __SWITCH__
#warning "I have not tested this, please test to make sure this works"
    Aes128CtrContext ctx;
    aes128CtrContextCreate(&ctx, key.data(), iv.data());
    aes128CtrCrypt(&ctx, out_buf, in_buf, len);
#else
    size_t count = 0;
    std::array<uint8_t, 0x10> sb = {0};
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_enc(&ctx, key.data(), 128);
    mbedtls_aes_crypt_ctr(&ctx, len, &count, iv.data(), sb.data(), in_buf, out_buf);
    mbedtls_aes_free(&ctx);
#endif
}
