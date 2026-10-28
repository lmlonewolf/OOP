#include "TextIndex.h"
#include "Intersection.h"
#include <algorithm>
#include <filesystem>

std::vector<std::string> TextIndex::splitWords(const std::string& text) const {
    const std::string upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    const std::string lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    std::vector<std::string> result;
    std::string word;

    for (std::size_t i = 0; i < text.size(); i++) {
        char letter = text[i];
        std::string symbol;
        if (letter >= 'A' && letter <= 'Z') {
            letter = static_cast<char>(letter + ('a' - 'A'));
        }
        if ((letter >= 'a' && letter <= 'z') ||
            (letter >= '0' && letter <= '9')) {
            symbol = letter;
        }
        else if (i + 1 < text.size()) {
            std::string current = text.substr(i, 2);
            std::size_t position = upper.find(current);
            if (position != std::string::npos) {
                symbol = lower.substr(position, 2);
            }
            else if (lower.find(current) != std::string::npos) {
                symbol = current;
            }
            if (!symbol.empty()) {
                i++;
            }
        }

        if (!symbol.empty()) {
            word += symbol;
        }
        else if (!word.empty()) {
            result.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) {
        result.push_back(word);
    }
    return result;
}

void TextIndex::loadDirectory(const std::string& directory) {
    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(
        std::filesystem::u8path(directory))) {
        if (!entry.is_regular_file()) {
            continue;
        }
        std::string extension = entry.path().extension().u8string();
        for (char& letter : extension) {
            if (letter >= 'A' && letter <= 'Z') {
                letter = static_cast<char>(letter + ('a' - 'A'));
            }
        }
        if (extension == ".txt") {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end());

    TextIndex loaded;
    int id = 1;
    for (const auto& file : files) {
        loaded.addDocument(Document(id, file.u8string()));
        id++;
    }
    *this = std::move(loaded);
}

const std::vector<Document>& TextIndex::getDocuments() const {
    return documents;
}

bool TextIndex::addDocument(const Document& document) {
    for (const Document& current : documents) {
        if (current.getId() == document.getId()) {
            return false;
        }
    }
    std::vector<std::string> list = splitWords(document.getText());
    documents.push_back(document);
    for (const std::string& word : list) {
        std::vector<int>* ids = words.find(word);
        if (ids == nullptr) {
            words.insert(word, std::vector<int>{document.getId()});
        }
        else if (std::find(ids->begin(), ids->end(), document.getId()) == ids->end()) {
            ids->push_back(document.getId());
        }
    }
    return true;
}

bool TextIndex::removeDocument(int id) {
    for (unsigned int i = 0; i < documents.size(); i++) {
        if (documents[i].getId() != id) {
            continue;
        }
        std::vector<std::string> list = splitWords(documents[i].getText());
        for (const std::string& word : list) {
            std::vector<int>* ids = words.find(word);
            if (ids == nullptr) {
                continue;
            }
            ids->erase(std::remove(ids->begin(), ids->end(), id), ids->end());
            if (ids->empty()) {
                words.erase(word);
            }
        }
        documents.erase(documents.begin() + i);
        return true;
    }
    return false;
}

std::vector<int> TextIndex::searchWord(const std::string& word) const {
    std::vector<std::string> list = splitWords(word);
    if (list.size() != 1) {
        return {};
    }
    const std::vector<int>* ids = words.find(list[0]);
    if (ids == nullptr) {
        return {};
    }
    return *ids;
}

std::vector<int> TextIndex::searchAll(const std::string& query) const {
    std::vector<std::string> list = splitWords(query);
    if (list.empty()) {
        return {};
    }
    std::vector<int> result = searchWord(list[0]);
    for (unsigned int i = 1; i < list.size(); i++) {
        result = intersection(result, searchWord(list[i]));
    }
    return result;
}

std::vector<std::pair<std::string, std::vector<int>>> TextIndex::getIndex() const {
    return words.inorder();
}
