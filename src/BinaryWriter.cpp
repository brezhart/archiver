#include "BinaryWriter.h"
#include <vector>
#include <cstdint>
#include <cassert>
BinaryWriter::BinaryWriter(std::string file_path, bool reverse) {
    is_reversed_ = reverse;
    support_buffer_.resize(BUFFER_SIZE, 0);
    file_.open(file_path, std::ios::binary);
}
void BinaryWriter::WriteChunk() {
    std::vector<char> char_buffer(BUFFER_SIZE, 0);
    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        for (size_t g = 0; g < BITS_IN_CHAR; ++g) {
            if (is_reversed_) {
                char_buffer[i] |= buffer_[i * BITS_IN_CHAR + g] << (BITS_IN_CHAR - g - 1);
            } else {
                char_buffer[i] |= buffer_[i * BITS_IN_CHAR + g] << (g);
            }
        }
    }
    file_.write(char_buffer.data(),
                static_cast<std::streamsize>((current_bit_offset_ + BITS_IN_CHAR - 1) / BITS_IN_CHAR));
    buffer_ &= 0;
}
void BinaryWriter::Close() {
    WriteChunk();
    file_.close();
}
BinaryWriter& BinaryWriter::Write(const uint64_t& to_write, size_t bits_to_write) {
    bits_written += bits_to_write;
    size_t can_write = std::min(bits_to_write, BUFFER_SIZE_IN_BITS - current_bit_offset_);
    for (size_t i = 0; i < can_write; ++i) {
        if (is_reversed_) {
            buffer_[current_bit_offset_ + i] = (to_write >> (bits_to_write - i - 1)) & 1ULL;
        } else {
            buffer_[current_bit_offset_ + i] = (to_write >> (i)) & 1ULL;
        }
    }
    current_bit_offset_ += can_write;
    assert(current_bit_offset_ <= BUFFER_SIZE_IN_BITS);
    if (current_bit_offset_ == BUFFER_SIZE_IN_BITS) {
        WriteChunk();
        assert(bits_to_write >= can_write);
        size_t tail_size = bits_to_write - can_write;
        assert(tail_size < BUFFER_SIZE_IN_BITS);
        for (size_t i = 0; i < tail_size; ++i) {
            if (is_reversed_) {
                buffer_[i] = (to_write >> ((bits_to_write - (i + can_write) - 1))) & 1ULL;
            } else {
                buffer_[i] = (to_write >> (i + can_write)) & 1ULL;
            }
        }
        current_bit_offset_ = tail_size;
    }
    return *this;
}
