#pragma once

// Packed Bits classes copied from the original Wind Waker Randomizer
class PackedBitsWriter
{
   public:
    PackedBitsWriter() = default;
    ~PackedBitsWriter() = default;

    uint8_t bits_left_in_byte = 8;
    size_t current_byte = 0;
    std::vector<char> bytes = {};

    template<typename T>
    void write(T value, size_t length)
    {
        size_t bits_to_read = 0;
        while (length > 0)
        {
            if (length >= bits_left_in_byte)
            {
                bits_to_read = bits_left_in_byte;
            }
            else
            {
                bits_to_read = length;
            }

            size_t mask = (1 << bits_to_read) - 1;
            current_byte |= (value & mask) << (8 - bits_left_in_byte);

            bits_left_in_byte -= bits_to_read;
            length -= bits_to_read;
            value >>= bits_to_read;

            if (bits_left_in_byte > 0)
            {
                continue;
            }

            flush();
        }
    }

    void flush()
    {
        bytes.push_back(current_byte);
        current_byte = 0;
        bits_left_in_byte = 8;
    }
};

class PackedBitsReader
{
   public:
    PackedBitsReader(const std::vector<char>& bytes_): bytes(bytes_) {}
    ~PackedBitsReader() = default;

    size_t current_bit_index = 0;
    size_t current_byte_index = 0;
    std::vector<char> bytes = {};

    size_t read(size_t length)
    {
        size_t bits_read = 0;
        size_t value = 0;
        size_t bits_left_to_read = length;

        while (bits_read != length)
        {
            size_t bits_to_read = 0;
            if (bits_left_to_read > 8)
            {
                bits_to_read = 8;
            }
            else
            {
                bits_to_read = bits_left_to_read;
            }

            if (bits_to_read + current_bit_index > 8)
            {
                bits_to_read = 8 - current_bit_index;
            }

            size_t mask = ((1 << bits_to_read) - 1) << current_bit_index;

            if (current_byte_index >= bytes.size())
            {
                return static_cast<size_t>(-1);
            }

            size_t current_byte = bytes[current_byte_index];
            value = ((current_byte & mask) >> current_bit_index) << bits_read | value;

            current_bit_index += bits_to_read;
            current_byte_index += current_bit_index >> 3;
            current_bit_index %= 8;
            bits_left_to_read -= bits_to_read;
            bits_read += bits_to_read;
        }

        return value;
    }

    bool reached_last_byte() const {
        return current_byte_index == bytes.size() - 1;
    }
};
