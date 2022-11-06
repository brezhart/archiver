//
// Created by Артем Брежнев on 12/10/22.
//

#pragma once

#ifndef CPP_PILOT_HSE_BINARYREADER_H
#define CPP_PILOT_HSE_BINARYREADER_H

#include "string_view"
#include <iostream>
#include <fstream>
#include <bitset>

class BinaryReader {
public:
    BinaryReader() = default;
    BinaryReader(std::string file_path, bool reverse);
    ~BinaryReader() {
        file_.close();
    };
    void Close();
    void Reload();
    bool Read(uint64_t& write_to, size_t bits_to_read);
    bool notify = false;

private:
    static const uint64_t BITS_IN_CHAR = 8;
    void ReadChunk();
    std::ifstream file_;
    std::string file_name_;
    static const size_t BUFFER_SIZE = 1024;
    static const size_t BUFFER_SIZE_IN_BITS = BUFFER_SIZE * BITS_IN_CHAR;
    bool is_reversed_ = false;
    std::bitset<BUFFER_SIZE * 8> buffer_;
    size_t current_bit_offset_ = 0;
    size_t position_of_eof_ = BUFFER_SIZE;  //
    bool has_eof_in_this_buffer_ = false;
};

#endif  // CPP_PILOT_HSE_BINARYREADER_H
