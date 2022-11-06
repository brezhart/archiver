#include <string_view>
#include <vector>
#include <unordered_map>
#include "BinaryReader.h"
#include <cstdint>
std::unordered_map<uint64_t, size_t> CalculateFrequency(BinaryReader& reader, size_t bits_to_read) {
    std::unordered_map<uint64_t, size_t> frequencies;
    uint64_t value = 0;
    while (reader.Read(value, bits_to_read)) {
        ++frequencies[value];
    }
    return frequencies;
}
uint64_t MostSignificantBit(uint64_t x) {
    return 63 - __builtin_clzll(x);
}
std::vector<uint64_t> CanonateCodeWords(std::unordered_map<uint64_t, uint64_t>& code_words) {
    std::vector<std::pair<uint64_t, uint64_t>> words;
    for (auto [key, value] : code_words) {
        words.push_back({value, key});
    }
    std::sort(words.begin(), words.end(),
         [&](const std::pair<uint64_t, uint64_t>& lhs, const std::pair<uint64_t, uint64_t>& rhs) {
             uint64_t first_size = MostSignificantBit(lhs.first);
             uint64_t second_size = MostSignificantBit(rhs.first);
             return std::tie(first_size, lhs.second) < std::tie(second_size, rhs.second);
         });
    uint64_t last = 0;
    bool last_was = false;
    std::vector<uint64_t> words_order;
    for (auto [code_word, word] : words) {
        words_order.push_back(word);
        if (!last_was) {
            last_was = true;
            uint64_t length = MostSignificantBit(code_word);
            last = 1LL << (length);
        } else {
            uint64_t length = MostSignificantBit(last);
            uint64_t length_needed = MostSignificantBit(code_word);
            last = (last + 1) << (length_needed - length);
        }
        code_words[word] = last;
    }
    return words_order;
}
std::unordered_map<uint64_t, uint64_t> DecanonateCodeWords(std::vector<uint64_t> words, std::vector<uint64_t> lengths) {
    std::unordered_map<uint64_t, uint64_t> codeword_to_word;
    size_t current_length = 1;
    size_t words_with_current_length = 0;
    uint64_t last_code_word = 0;
    bool last_was = false;
    for (auto word : words) {

        size_t new_current_length = current_length;
        if (words_with_current_length == lengths[new_current_length - 1]) {
            ++new_current_length;
            words_with_current_length = 0;
        }
        while (lengths[new_current_length - 1] == 0) {
            ++new_current_length;
        }

        if (!last_was) {
            last_code_word = 1LL << (new_current_length);
            last_was = true;
        } else {
            last_code_word = (last_code_word + 1) << (new_current_length - current_length);
        }
        uint64_t len = MostSignificantBit(last_code_word);
        uint64_t new_last = 1LL << len;
        for (size_t bit = 0; bit < len; ++bit) {
            new_last |= (((last_code_word >> (len - bit - 1)) & 1) << bit);
        }
        codeword_to_word[new_last] = word;
        ++words_with_current_length;
        current_length = new_current_length;
    }
    return codeword_to_word;
}
