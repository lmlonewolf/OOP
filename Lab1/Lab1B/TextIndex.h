#pragma once

#include "Document.h"
#include "BinaryTree.h"

class TextIndex {
    std::vector<Document> documents;
    BinaryTree<std::string, std::vector<int>> words;

    std::vector<std::string> splitWords(const std::string& text) const;

public:
    void loadDirectory(const std::string& directory);
    const std::vector<Document>& getDocuments() const;
    bool addDocument(const Document& document);
    bool removeDocument(int id);
    std::vector<int> searchWord(const std::string& word) const;
    std::vector<int> searchAll(const std::string& query) const;
    std::vector<std::pair<std::string, std::vector<int>>> getIndex() const;
};
