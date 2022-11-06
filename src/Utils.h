//
// Created by Артем Брежнев on 12/10/22.
//

#ifndef CPP_PILOT_HSE_UTILS_H
#define CPP_PILOT_HSE_UTILS_H

#endif  // CPP_PILOT_HSE_UTILS_H

#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <map>
#include "BinaryReader.h"
std::unordered_map<uint64_t, size_t> CalculateFrequency(BinaryReader& reader, size_t bits_to_read);
uint64_t MostSignificantBit(uint64_t x);
std::vector<uint64_t> CanonateCodeWords(std::unordered_map<uint64_t, uint64_t>& code_words);
std::unordered_map<uint64_t, uint64_t> DecanonateCodeWords(std::vector<uint64_t> words, std::vector<uint64_t> lengths);