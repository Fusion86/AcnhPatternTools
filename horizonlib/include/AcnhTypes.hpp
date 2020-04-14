#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

struct AcnhHeaderVersion {
    uint32_t majorVer;
    uint32_t minorVer;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t saveFileRevision;

    friend std::ostream& operator<<(std::ostream& os, const AcnhHeaderVersion& obj) {
        os << obj.majorVer << "." << obj.minorVer << " (rev: " << obj.saveFileRevision << ")";
        return os;
    }

    bool isSupported() const {
        constexpr std::array supportedMajor = {0x6D};
        constexpr std::array supportedMinor = {0x78};
        constexpr std::array supportedRev = {0x2, 0x4, 0x5};

        if (std::find(supportedMajor.begin(), supportedMinor.end(), majorVer) ==
            supportedMajor.end())
            return false;
        if (std::find(supportedMinor.begin(), supportedMinor.end(), minorVer) ==
            supportedMinor.end())
            return false;
        if (std::find(supportedRev.begin(), supportedRev.end(), saveFileRevision) ==
            supportedRev.end())
            return false;
        return true;
    }
};

/// Header as in (decrypted) main.dat
struct AcnhHeader {
  public:
    AcnhHeaderVersion version;
    std::array<uint8_t, 276> unk;

    bool isEncrypted() const {
        // HACK: Shitty way to check if data is encrypted
        return !version.isSupported();
    }
};

template <size_t STRLEN>
struct AcnhString {
  public:
    std::array<uint16_t, STRLEN> bytes{};

    AcnhString() {}

    AcnhString(const std::string& str) {
        // HACK:
        size_t i = 0;
        for (char c : str) {
            if (i >= STRLEN) break;
            bytes[i++] = c;
        }
    }

    bool operator==(const AcnhString& rhs) const {
        return bytes == rhs.bytes;
    }

    friend std::ostream& operator<<(std::ostream& os, const AcnhString<STRLEN>& obj) {
        os << obj.str();
        return os;
    }

    const std::string str() const {
        // HACK:
        char cstr[STRLEN + 1];
        for (size_t i = 0; i < STRLEN; i++) {
            cstr[i] = bytes[i];
        }
        cstr[STRLEN] = 0; // Zero terminator
        return std::string(cstr);
    }
};

template <size_t STRLEN>
struct AcnhUniqueIdentifier {
  public:
    uint32_t id;
    AcnhString<STRLEN> string;

    friend std::ostream& operator<<(std::ostream& os, const AcnhUniqueIdentifier<STRLEN>& obj) {
        os << obj.string;
        return os;
    }
};

struct AcnhColor {
  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    bool operator==(const AcnhColor& rhs) const {
        return red == rhs.red && green == rhs.green && blue == rhs.blue;
    }

    friend std::ostream& operator<<(std::ostream& os, const AcnhColor& obj) {
        os << "r=" << unsigned(obj.red) << " g=" << unsigned(obj.green)
           << " b=" << unsigned(obj.blue);
        return os;
    }
};

// Unknown actually
enum class AcnhPatternType : uint8_t {

};

template <size_t S>
struct AcnhDesignPattern {
  public:
    uint32_t uid;
    uint32_t num1;
    uint8_t zero1[8];
    AcnhString<20> name;
    AcnhUniqueIdentifier<10> island;
    uint32_t unk3;
    AcnhUniqueIdentifier<10> creator;
    uint8_t zero2[4];
    uint8_t unk1; // Type/datasize?
    uint8_t unk2;
    uint8_t unk4[6];
    std::array<AcnhColor, 15> palette;
    std::array<uint8_t, S> data;
    AcnhPatternType type;
    uint16_t zero3;

    // const std::string getName() const {
    //     return name.str();
    // }

    /// Returns resolution in one dimension.
    constexpr int getResolution() const {
        return S == 512 ? 32 : 64; // Poor mans sqrt(S*2)
    }

    // TODO: Use this as implementation and let the other setPixel call this.
    uint8_t getPixel(int i) const {
        return getPixel(i % getResolution(), i / getResolution());
    }

    // TODO: Move this to both DesignPattern and ProDesignPattern with each their own
    // implementation. This does NOT work for pro patterns.
    uint8_t getPixel(int x, int y) const {
        if (x % 2 == 0) {
            return data[(x / 2) + y * (getResolution() / 2)] & 0x0F;
        } else {
            return (data[(x / 2) + y * (getResolution() / 2)] & 0xF0) >> 4;
        }
    }

    /// Get RGB array.
    std::unique_ptr<uint8_t[]> getRgbData() const {
        // TODO: This does NOT work for pro patterns.
        size_t pixel_count = getResolution() * getResolution();
        auto data = std::make_unique<uint8_t[]>(pixel_count * 3); // x3 because RGB = 3 bytes

        size_t ptr = 0;
        for (size_t i = 0; i < pixel_count; i++) {
            int paletteIdx = getPixel(i);
            if (paletteIdx == 0xF) {
                // Transparent
                // TODO: Maybe make this bright pink, just like game engines do?
                data[ptr++] = 0;
                data[ptr++] = 0;
                data[ptr++] = 0;
            } else {
                // Not transparent
                data[ptr++] = palette[paletteIdx].red;
                data[ptr++] = palette[paletteIdx].green;
                data[ptr++] = palette[paletteIdx].blue;
            }
        }

        return data;
    }

    /// Get RGBA array.
    std::unique_ptr<uint8_t[]> getRgbaData() const {
        // TODO: This does NOT work for pro patterns.
        size_t pixel_count = getResolution() * getResolution();
        auto data = std::make_unique<uint8_t[]>(pixel_count * 4); // x4 because RGBA = 4 bytes

        size_t ptr = 0;
        for (size_t i = 0; i < pixel_count; i++) {
            int paletteIdx = getPixel(i);
            if (paletteIdx == 0xF) {
                // Transparent
                *(uint32_t*)data.get() = 0; // R,G,B,A = 0
                ptr += 4;
            } else {
                // Not transparent
                data[ptr++] = palette[paletteIdx].red;
                data[ptr++] = palette[paletteIdx].green;
                data[ptr++] = palette[paletteIdx].blue;
                data[ptr++] = 0xFF; // Alpha
            }
        }

        return data;
    }

    /// Get alpha array.
    std::unique_ptr<uint8_t[]> getAlphaData() const {
        size_t pixel_count = getResolution() * getResolution();
        auto data = std::make_unique<uint8_t[]>(pixel_count);

        for (size_t i = 0; i < pixel_count; i++) {
            data[i] = getPixel(i) == 0xF ? 0 : 0xFF;
        }

        return data;
    }

    const AcnhColor getPixelColor(int i) const {
        return getPixelColor(i % getResolution(), i / getResolution());
    }

    const AcnhColor getPixelColor(int x, int y) const {
        int idx = getPixel(x, y);
        if (idx >= 0xF) throw std::runtime_error("Palette index out of range!");
        return palette[idx];
    }

    // Probably doesn't work for pro patterns.
    void setPixel(int i, int value) {
        auto idx = i / 2;
        if (i % 2 == 0) {
            data[idx] = value & 0x0F;
        } else {
            data[idx] = (value << 4 & 0xF0) | (data[idx] & 0x0F);
        }
    }

    void setPixel(int x, int y, int value) {
        setPixel(y * getResolution() + x, value);
    }
};

typedef AcnhDesignPattern<512> DesignPattern;
typedef AcnhDesignPattern<2048> ProDesignPattern;

// Should this be named EncryptedUInt32?
struct EncryptedInt32 {
  private:
    static constexpr uint32_t ENCRYPTION_CONSTANT = 0x80E32B11;
    static constexpr uint8_t SHIFT_BASE = 3;

  public:
    uint32_t encryptedValue;
    uint16_t adjust;
    uint8_t shift;
    uint8_t checksum;

    uint32_t get() const {
        // Untested
        if (encryptedValue == 0 && adjust == 0 && shift == 0 && checksum == 0) {
            return 0;
        }

        if (!valid()) {
            std::cout << "EncryptedInt32 value is not valid, can't read!" << std::endl;
            return 0;
        }

        // Magic
        uint64_t a = ((uint64_t)encryptedValue) << ((32 - SHIFT_BASE - shift) & 0x3F);
        int32_t b = (int32_t)a + (int32_t)(a >> 32);
        return (uint32_t)(ENCRYPTION_CONSTANT - adjust + b);
    }

    void set() {}

    bool valid() const {
        return checksum == calculateChecksum();
    }

    uint8_t calculateChecksum() const {
        const uint8_t* ptr = (uint8_t*)&encryptedValue;
        return ptr[0] + ptr[1] + ptr[2] + ptr[3] - 0x2D;
    }
};

struct PersonalPhoto {
  public:
    uint32_t size;

    /// JPG data. Not all bytes might be used, see `size` for the actual count of used bytes.
    std::array<uint8_t, 0x2300C> data;
};

struct AcnhMainData {
  public:
    AcnhHeader header;
    AcnhUniqueIdentifier<10> island;
    uint32_t unk1;
    AcnhUniqueIdentifier<10> character;
    std::array<uint8_t, 0x1D71B8> unk2;
    std::array<DesignPattern, 50> designPatterns;
    std::array<ProDesignPattern, 50> proDesignPatterns;
    DesignPattern townFlag;
    std::array<ProDesignPattern, 8> ableSisters;
    std::array<uint8_t, 0x8C3A08> unk3;
};

struct AcnhPersonalData {
  public:
    AcnhHeader header;
    std::array<uint8_t, 44948> unk1;
    AcnhUniqueIdentifier<10> island;
    uint32_t unk2;
    AcnhUniqueIdentifier<10> character;

    std::array<uint8_t, 25756> unk3;
    EncryptedInt32 nookMiles;
    std::array<uint8_t, 48> unk4;
    PersonalPhoto photo;

    std::array<uint8_t, 0x1660> unk5; // Again character + island? + unks
    std::array<uint8_t, 0x33004> unk6; // Pockets, item storage and unks
    EncryptedInt32 bank;
    std::array<uint8_t, 12948> unk7;
};

/// Header as in ___Header.dat
struct AcnhSaveHeader {
    AcnhHeaderVersion version;
    std::array<uint8_t, 240> unk;
    std::array<uint32_t, 128> crypto;
};
