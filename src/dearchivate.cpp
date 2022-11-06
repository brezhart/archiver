#include <string_view>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "dearchivate.h"
#include "Utils.h"
#include <cassert>

const uint64_t FILENAME_END = 256;
const uint64_t ONE_MORE_FILE = 257;
const uint64_t ARCHIVE_END = 258;

void ReadOneWord(BinaryReader& reader, std::unordered_map<uint64_t, uint64_t>& dictionary, uint64_t& write_to) {
    uint64_t word = 0;
    uint64_t current_bit_index = 0;
    uint64_t fixed_len_word = 0;
    while (dictionary.find(fixed_len_word) == dictionary.end()) {
        uint64_t new_bit = 0;
        reader.Read(new_bit, 1);
        word |= (new_bit << current_bit_index);
        ++current_bit_index;
        fixed_len_word = word | (1LL << current_bit_index);
    }
    write_to = fixed_len_word;
}

void Dearchivate(std::string archive_name) {
    BinaryReader reader(archive_name, true);
    int file_now = 0;
    while (true) {
        file_now++;
        uint64_t symbols_count = 0;
        reader.Read(symbols_count, 9);
        std::vector<uint64_t> words(symbols_count);
        //        std::cout << symbols_count << std::endl;
        reader.notify = true;
        for (size_t word_index = 0; word_index < symbols_count; ++word_index) {
            reader.notify = word_index == 18 && file_now == 3;
            reader.Read(words[word_index], 9);
        }
        reader.notify = false;
        uint64_t sum_of_lengths = 0;
        std::vector<uint64_t> codewords_sizes;
        int ind = 1;
        while (sum_of_lengths < symbols_count) {
            uint64_t current_size = 0;
            reader.Read(current_size, 9);
            codewords_sizes.push_back(current_size);
            //            std::cout << ind << " - " << current_size << std::endl;
            ind++;
            sum_of_lengths += current_size;
        }
        assert(symbols_count == sum_of_lengths);
        auto codeword_to_word = DecanonateCodeWords(words, codewords_sizes);
        std::string file_name;
        uint64_t current_char = 0;
        do {
            ReadOneWord(reader, codeword_to_word, current_char);
            current_char = codeword_to_word[current_char];
            assert(((current_char >> 9) & 1) == 0);
            if (current_char == FILENAME_END) {
                break;
            }
            file_name += static_cast<char>(current_char);
        } while (true);
        BinaryWriter writer(file_name, false);
        do {
            ReadOneWord(reader, codeword_to_word, current_char);
            current_char = codeword_to_word[current_char];
            assert(((current_char >> 9) & 1) == 0);
            if (current_char == ARCHIVE_END || current_char == ONE_MORE_FILE) {
                break;
            }
            writer.Write(current_char, 8);
        } while (true);
        if (current_char == ARCHIVE_END) {
            return;
        }
    }
}