//
// Created by Артем Брежнев on 13/10/22.
//
#pragma once

#ifndef CPP_PILOT_HSE_BINARYTRIE_H
#define CPP_PILOT_HSE_BINARYTRIE_H
#include <vector>
#include <cstdint>
class BinaryTrie {
public:
    BinaryTrie(){};
    struct Node {
        uint64_t value = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        Node(Node* left, Node* right);
        explicit Node(uint64_t value);
        bool operator<(Node& rhs) const;
    };
    class TrieIterator {
    public:
        enum class Direction {
            FROM_PARENT,
            FROM_LEFT_CHILD,
            FROM_RIGHT_CHILD,
        };
        TrieIterator(){};
        TrieIterator(Node* node, uint64_t);
        TrieIterator operator++();
        bool operator!=(const TrieIterator& rhs) const;
        std::pair<Node*, uint64_t> operator*();

    private:
        Direction WhatDirectionAmI() const;
        Node* node_;
        uint64_t path_;
    };
    TrieIterator begin();  // NOLINT
    TrieIterator end();    // NOLINT
    //    template <typename ...ArgsT>
    //    Node* AddNode(ArgsT ...args);
    Node* AddNode(uint64_t v);
    Node* AddNode(BinaryTrie::Node* left, BinaryTrie::Node* right);
    ~BinaryTrie();
    void ClaimRoot(Node* root);

private:
    std::vector<Node*> nodes_;
    TrieIterator end_;
    TrieIterator begin_;
};

#endif  // CPP_PILOT_HSE_BINARYTRIE_H
