#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <functional>


template<class Key, class Value>
struct Node {
    Key key;
    Value value;
    std::unique_ptr<Node<Key, Value>> left = nullptr;
    std::unique_ptr<Node<Key, Value>> right = nullptr;

    Node(const Key& k, const Value& val) : key(k), value(val) {
    }
};


template<class Key, class Value, class Compare = std::less<Key>>
class BinaryTree {
    std::unique_ptr<Node<Key, Value>> root;
    Compare compare;

    bool eraseNode(std::unique_ptr<Node<Key, Value>>& node, const Key& key) {
        if (!node) {
            return false;
        }
        if (compare(key, node->key)) {
            return eraseNode(node->left, key);
        }
        if (compare(node->key, key)) {
            return eraseNode(node->right, key);
        }
        if (!node->left) {
            std::unique_ptr<Node<Key, Value>> replacement = std::move(node->right);
            node = std::move(replacement);
        }
        else if (!node->right) {
            std::unique_ptr<Node<Key, Value>> replacement = std::move(node->left);
            node = std::move(replacement);
        }
        else {
            Node<Key, Value>* next = node->right.get();
            while (next->left) {
                next = next->left.get();
            }
            node->key = next->key;
            node->value = next->value;
            Key nextKey = next->key;
            eraseNode(node->right, nextKey);
        }
        return true;
    }

    void inorder(const Node<Key, Value>* node, std::vector<std::pair<Key, Value>>& vec) const {
        if (node == nullptr) {
            return;
        }
        inorder(node->left.get(), vec);
        vec.push_back(std::make_pair(node->key, node->value));
        inorder(node->right.get(), vec);
    }

public:
    BinaryTree(Compare rule = Compare()) : root(nullptr), compare(rule) {
    }


    bool insert(const Key& key, const Value& value) {
        if (!root) {
            root = std::make_unique<Node<Key, Value>>(key, value);
            return true;
        }
        Node<Key, Value>* curr = root.get();
        while (curr != nullptr) {
            if (compare(key, curr->key)) {
                if (!curr->left) {
                    curr->left = std::make_unique<Node<Key, Value>>(key, value);
                    return true;
                }
                curr = curr->left.get();
            }
            else if (compare(curr->key, key)) {
                if (!curr->right) {
                    curr->right = std::make_unique<Node<Key, Value>>(key, value);
                    return true;
                }
                curr = curr->right.get();
            }
            else {
                return false;
            }
        }
        return false;
    }

    const Value* find(const Key& key) const {
        const Node<Key, Value>* current = root.get();
        while (current != nullptr) {
            if (compare(key, current->key)) {
                current = current->left.get();
            }
            else if (compare(current->key, key)) {
                current = current->right.get();
            }
            else {
                return &current->value;
            }
        }
        return nullptr;
    }

    Value* find(const Key& key) {
        Node<Key, Value>* current = root.get();
        while (current != nullptr) {
            if (compare(key, current->key)) {
                current = current->left.get();
            }
            else if (compare(current->key, key)) {
                current = current->right.get();
            }
            else {
                return &current->value;
            }
        }
        return nullptr;
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    bool erase(const Key& key) {
        return eraseNode(root, key);
    }

    std::vector<std::pair<Key, Value>> inorder() const {
        std::vector<std::pair<Key, Value>> vec;
        inorder(root.get(), vec);
        return vec;
    }
};
