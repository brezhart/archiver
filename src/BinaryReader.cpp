//
// Created by Артем Брежнев on 12/10/22.
//

#include "BinaryReader.h"
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>

BinaryReader::BinaryReader(std::string file_path, bool reverse) {
    is_reversed_ = reverse;
    file_name_ = file_path;
    file_.open(file_path, std::ios::binary);
    ReadChunk();
}
void BinaryReader::ReadChunk() {
    std::vector<char> char_buffer(BUFFER_SIZE, 0);
    file_.read(char_buffer.data(), static_cast<std::streamsize>(char_buffer.size()));
    position_of_eof_ = file_.gcount() * BITS_IN_CHAR;
    has_eof_in_this_buffer_ = position_of_eof_ != BUFFER_SIZE_IN_BITS;
    buffer_ &= 0;
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        for (size_t g = 0; g < BITS_IN_CHAR; g++) {
            if (is_reversed_) {
                buffer_[((i + 1ULL) * BITS_IN_CHAR - g) - 1ULL] = (char_buffer[i] >> g) & 1ULL;
            } else {
                buffer_[BITS_IN_CHAR * i + g] = (char_buffer[i] >> g) & 1ULL;
            }
        }
    }
}
void BinaryReader::Close() {
    file_.close();
}
void BinaryReader::Reload() {
    current_bit_offset_ = 0;
    buffer_ &= 0;
    file_.close();
    file_.open(file_name_, std::ios::binary);
}
bool BinaryReader::Read(uint64_t& write_to, size_t bits_to_read) {
    if (has_eof_in_this_buffer_ && current_bit_offset_ >= position_of_eof_) {
        return false;
    }
    uint64_t word = 0;
    size_t can_read = std::min(BUFFER_SIZE_IN_BITS - current_bit_offset_, bits_to_read);
    for (size_t i = 0; i < can_read; ++i) {
        word |= (buffer_[current_bit_offset_ + i] << (is_reversed_ ? bits_to_read - i - 1ULL : i));
    }
    current_bit_offset_ += can_read;
    if (can_read != bits_to_read) {
        size_t tail_size = bits_to_read - can_read;
        ReadChunk();
        for (int i = 0; i < tail_size; ++i) {
            if (is_reversed_) {
                word |= buffer_[i] << (bits_to_read - (i + bits_to_read - tail_size) - 1ULL);
            } else {
                word |= buffer_[i] << (i + bits_to_read - tail_size);
            }
        }
        current_bit_offset_ = tail_size;
    }
    write_to = word;
    return true;
}