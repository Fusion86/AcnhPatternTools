// Taken from https://github.com/3096/effective-guacamole/blob/master/save_cryptor/main.cpp
#pragma once

#include <array>
#include <memory>

std::array<uint8_t, 0x10> getKeyOrIV(uint32_t* headerCryptSectionBuffer, uint64_t sourceIdx);
void aesCryptCtr(const uint8_t* in_buf, std::array<uint8_t, 0x10> key, std::array<uint8_t, 0x10> iv,
                 size_t len, uint8_t* out_buf);
