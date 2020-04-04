#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
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
        constexpr std::array supportedRev = {0x2, 0x4};

        if (std::find(supportedMajor.begin(), supportedMinor.end(), majorVer) == supportedMajor.end()) return false;
        if (std::find(supportedMinor.begin(), supportedMinor.end(), minorVer) == supportedMinor.end()) return false;
        if (std::find(supportedRev.begin(), supportedRev.end(), saveFileRevision) == supportedRev.end()) return false;
        return true;
    }
};

/// Header as in (decrypted) main.dat
struct AcnhHeader {
  public:
    AcnhHeaderVersion version;
    std::array<uint8_t, 276> unk;
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
        os << "r=" << unsigned(obj.red) << " g=" << unsigned(obj.green) << " b=" << unsigned(obj.blue);
        return os;
    }
};

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

    const std::string getName() const {
        return name.str();
    }

    /// Returns resolution in one dimension.
    constexpr int getResolution() const {
        return S == 512 ? 32 : 64; // Poor mans sqrt(S*2)
    }

    // TODO: Use this as implementation and let the other setPixel call this.
    uint8_t getPixel(int i) const {
        return getPixel(i / getResolution(), i % getResolution());
    }

    // TODO: Move this to both DesignPattern and ProDesignPattern with each their own implementation.
    uint8_t getPixel(int x, int y) const {
        if (x % 2 == 0) {
            return data[(x / 2) + y * (getResolution() / 2)] & 0x0F;
        } else {
            return (data[(x / 2) + y * (getResolution() / 2)] & 0xF0) >> 4;
        }
    }

    const AcnhColor getPixelColor(int i) const {
        return getPixelColor(i / getResolution(), i % getResolution());
    }

    // TODO: Move this to both DesignPattern and ProDesignPattern with each their own implementation.
    const AcnhColor getPixelColor(int x, int y) const {
        int idx = getPixel(x, y);
        if (idx == 15) return AcnhColor(); // Transparent
        return palette[idx];
    }

    void setPixel(int i, int value) {
        auto idx = i / 2;
        if (i % 2 == 0) {
            data[idx] = value & 0x0F;
        } else {
            data[idx] = value << 4 & 0xF0 | data[idx] & 0x0F;
        }
    }

    void setPixel(int x, int y, int value) {
        setPixel(y * getResolution() + x, value);
    }
};

typedef AcnhDesignPattern<512> DesignPattern;
typedef AcnhDesignPattern<2048> ProDesignPattern;

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

/// Header as in ___Header.dat
struct AcnhSaveHeader {
    AcnhHeaderVersion version;
    std::array<uint8_t, 240> unk;
    std::array<uint32_t, 128> crypto;
};
