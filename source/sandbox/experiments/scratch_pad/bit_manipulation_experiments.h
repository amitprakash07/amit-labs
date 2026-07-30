#ifndef SANDBOX_EXPERIMENTS_SCRATCH_PAD_BIT_MANIPULATION_EXPERIMENTS_H
#define SANDBOX_EXPERIMENTS_SCRATCH_PAD_BIT_MANIPULATION_EXPERIMENTS_H

#include <type_traits>
#include <bitset>
#include <iostream>

namespace amit::sandbox_experiments
{
    template <typename T>
    concept BitManipulationType = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template <BitManipulationType T>
    std::string PrintBinaryRepresentation(const T& value)
    {
        std::string    ret_string;
        const uint8_t* bytePtr = reinterpret_cast<const uint8_t*>(&value);

        for (size_t i = 0; i < sizeof(T); ++i)
        {
            std::bitset<8> bits(bytePtr[i]);
            ret_string += bits.to_string() + ' ';
        }

        return ret_string;
    }

    template <BitManipulationType T>
    uint8_t GetSetCountBit(T data)
    {
        uint8_t count = 0;
        while (data)
        {
            count++;
            data = data & (data - 1);
        }

        return count;
    }

    inline uint16_t BitReplication(uint8_t data)
    {
        uint16_t ret_data = data;
        ret_data          = (data << 8) | static_cast<uint16_t>(data);
        return ret_data;
    }

    inline void ExperimentBitManipulation()
    {
        std::vector<int8_t>  int_data  = {static_cast<int8_t>(0b1111'1111), 0b0000'0001};
        std::vector<uint8_t> uint_data = {0b1000'0001, 0b0000'0001};

        for (const auto& data : int_data)
        {
            std::cout << "Data:" << data << " Binary: " << PrintBinaryRepresentation(data) << "\n";
            std::cout << "Set bits count: " << static_cast<int>(GetSetCountBit(data)) << "\n";
        }

        for (const auto& data : uint_data)
        {
            std::cout << "Data:" << data << " Binary: " << PrintBinaryRepresentation(data) << "\n";
            std::cout << "Set bits count: " << static_cast<int>(GetSetCountBit(data)) << "\n";
        }

        uint8_t data = 0b1010'1010;
        std::cout << "Replicated: " << PrintBinaryRepresentation(BitReplication(data)) << "\n";
    }
}  // namespace amit::sandbox_experiments

#endif