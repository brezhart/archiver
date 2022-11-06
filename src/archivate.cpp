#include <vector>
#include <set>
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "Utils.h"
#include "BinaryTrie.h"
#include <cstdint>
const uint64_t FILENAME_END = 256;
const uint64_t ONE_MORE_FILE = 257;
const uint64_t ARCHIVE_END = 258;

void Archivate(std::string write_path, std::vector<std::string> file_paths) {
    BinaryWriter writer(write_path, true);
    for (size_t current_file_path_index = 0; current_file_path_index < file_paths.size(); ++current_file_path_index) {
        std::string file_path = file_paths[current_file_path_index];
        BinaryReader reader(file_path, false);
        std::unordered_map<uint64_t, size_t> frequencies = CalculateFrequency(reader, 8);
        for (auto chr : file_path) {
            ++frequencies[chr];
        }
        frequencies[256] = 1;
        frequencies[257] = 1;
        frequencies[258] = 1;
        auto cmp = [](const std::pair<size_t, BinaryTrie::Node*>& lhs,
                      const std::pair<size_t, BinaryTrie::Node*>& rhs) {
            return std::tie(lhs.first, lhs.second->value) < std::tie(rhs.first, rhs.second->value);
        };
        std::set<std::pair<size_t, BinaryTrie::Node*>, decltype(cmp)> priority_queue(cmp);
        BinaryTrie trie;
        for (auto [key, value] : frequencies) {
            priority_queue.insert({value, trie.AddNode(key)});
        }
        while (priority_queue.size() > 1) {
            auto lhs = (*priority_queue.begin());
            priority_queue.erase(priority_queue.begin());
            auto rhs = (*priority_queue.begin());
            priority_queue.erase(priority_queue.begin());
            priority_queue.insert({rhs.first + lhs.first, trie.AddNode(lhs.second, rhs.second)});
        }
        trie.ClaimRoot(priority_queue.begin()->second);
        std::unordered_map<uint64_t, uint64_t> code_words;
        for (auto [node, path] : trie) {
            uint64_t len = MostSignificantBit(path);
            uint64_t new_path = 1LL << len;
            for (size_t bit = 0; bit < len; ++bit) {
                new_path |= (((path >> (len - bit - 1)) & 1) << bit);
            }
            path = new_path;
            code_words[node->value] = path;
        }
        auto words_order = CanonateCodeWords(code_words);
        reader.Close();
        BinaryReader reader2(file_path, false);
        writer.Write(static_cast<uint64_t>(code_words.size()), 9);
        for (auto word : words_order) {
            writer.Write(word, 9);
        }
        size_t word_max_len = 0;
        for (auto [key, value] : code_words) {
            word_max_len = std::max(word_max_len, static_cast<size_t>(MostSignificantBit(value)));
        }
        std::vector<int> amount_of_words_with_specefic_length(word_max_len, 0);
        for (auto [key, value] : code_words) {
            ++amount_of_words_with_specefic_length[MostSignificantBit(value) - 1];
        }
        for (size_t i = 0; i < amount_of_words_with_specefic_length.size(); ++i) {
            writer.Write(amount_of_words_with_specefic_length[i], 9);
        }
        std::string file_name = file_path;
        for (auto chr : file_name) {
            writer.Write(code_words[chr], MostSignificantBit(code_words[chr]));
        }
        writer.Write(code_words[FILENAME_END], MostSignificantBit(code_words[FILENAME_END]));
        uint64_t value = 0;
        while (reader2.Read(value, 8)) {
            writer.Write(code_words[value], MostSignificantBit(code_words[value]));
        }
        if (current_file_path_index + 1 == file_paths.size()) {
            writer.Write(code_words[ARCHIVE_END], MostSignificantBit(code_words[ARCHIVE_END]));
        } else {
            writer.Write(code_words[ONE_MORE_FILE], MostSignificantBit(code_words[ONE_MORE_FILE]));
        }
    }
}