//
// Created by Артем Брежнев on 14/10/22.
//
#pragma once
#ifndef CPP_PILOT_HSE_BINARYWRITER_H
#define CPP_PILOT_HSE_BINARYWRITER_H

#include "string_view"
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <cstdint>

class BinaryWriter {
public:
    BinaryWriter() = default;
    BinaryWriter(std::string file_path, bool reverse);
    ~BinaryWriter() {
        WriteChunk();
        file_.close();
    };
    BinaryWriter& Write(const uint64_t& to_write, size_t bits_to_write);
    void Close();
    uint64_t bits_written = 0;

private:
    static const uint64_t BITS_IN_CHAR = 8;
    void WriteChunk();
    std::ofstream file_;
    static const size_t BUFFER_SIZE = 1024;
    static const size_t BUFFER_SIZE_IN_BITS = BUFFER_SIZE * BITS_IN_CHAR;
    std::vector<char> support_buffer_;
    std::bitset<BUFFER_SIZE_IN_BITS> buffer_;
    size_t current_bit_offset_ = 0;
    bool is_reversed_ = false;
};

#endif  // CPP_PILOT_HSE_BINARYWRITER_H
