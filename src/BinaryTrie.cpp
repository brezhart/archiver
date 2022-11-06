//
// Created by Артем Брежнев on 13/10/22.
//

#include "BinaryTrie.h"
#include <cstdint>

BinaryTrie::Node::Node(BinaryTrie::Node* left, BinaryTrie::Node* right) {
    this->left = left;
    left->parent = this;
    this->right = right;
    right->parent = this;
    value = std::min(left->value, right->value);
}
BinaryTrie::Node::Node(uint64_t value) {
    this->value = value;
}
bool BinaryTrie::Node::operator<(BinaryTrie::Node& rhs) const {
    return value < rhs.value;
}
BinaryTrie::Node* BinaryTrie::AddNode(uint64_t v) {
    Node* node = new Node(v);
    nodes_.push_back(node);
    return node;
}
BinaryTrie::Node* BinaryTrie::AddNode(BinaryTrie::Node* left, BinaryTrie::Node* right) {
    Node* node = new Node(left, right);
    nodes_.push_back(node);
    return node;
}
void BinaryTrie::ClaimRoot(BinaryTrie::Node* root) {
    uint64_t path = 1;
    Node* now = root;
    end_ = TrieIterator(now, path);
    while (now->right != nullptr || now->left != nullptr) {
        if (now->left != nullptr) {
            path <<= 1;
            now = now->left;
        } else {
            path <<= 1;
            path += 1;
            now = now->right;
        }
    }
    begin_ = TrieIterator(now, path);
}
BinaryTrie::~BinaryTrie() {
    for (auto to_del : nodes_) {
        delete to_del;
    }
}

BinaryTrie::TrieIterator::TrieIterator(Node* node, uint64_t path) {
    node_ = node;
    path_ = path;
}
BinaryTrie::TrieIterator BinaryTrie::begin() {
    return begin_;
}
BinaryTrie::TrieIterator BinaryTrie::end() {
    return end_;
}
BinaryTrie::TrieIterator BinaryTrie::TrieIterator::operator++() {
    Direction direction = WhatDirectionAmI();
    node_ = node_->parent;
    path_ >>= 1;
    while (node_->right != nullptr || node_->left != nullptr) {
        switch (direction) {
            case Direction::FROM_PARENT:
                if (node_->left != nullptr) {
                    node_ = node_->left;
                    path_ <<= 1;
                } else {
                    node_ = node_->right;
                    path_ <<= 1;
                    path_ += 1;
                }
                direction = Direction::FROM_PARENT;
                break;
            case Direction::FROM_RIGHT_CHILD:
                if (node_->parent != nullptr) {
                    direction = WhatDirectionAmI();
                    node_ = node_->parent;
                    path_ >>= 1LL;
                } else {
                    return *this;
                }
                break;
            case Direction::FROM_LEFT_CHILD:
                if (node_->right != nullptr) {
                    node_ = node_->right;
                    path_ <<= 1LL;
                    path_ += 1;
                    direction = Direction::FROM_PARENT;
                } else if (node_->parent != nullptr) {
                    direction = WhatDirectionAmI();
                    node_ = node_->parent;
                    path_ >>= 1LL;
                } else {
                    return *this;
                }
                break;
        }
    }
    return *this;
}
BinaryTrie::TrieIterator::Direction BinaryTrie::TrieIterator::WhatDirectionAmI() const {
    return node_->parent->right == node_ ? Direction::FROM_RIGHT_CHILD : Direction::FROM_LEFT_CHILD;
}
bool BinaryTrie::TrieIterator::operator!=(const BinaryTrie::TrieIterator& rhs) const {
    return node_ != rhs.node_;
}
std::pair<BinaryTrie::Node*, uint64_t> BinaryTrie::TrieIterator::operator*() {
    return {node_, path_};
}
