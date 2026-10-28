#include "Document.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <stdexcept>


Document::Document(int documentId, const std::string& path) :
    id(documentId), fileName(path) {
    if (id <= 0) {
        throw std::invalid_argument("Идентификатор документа должен быть положительным");
    }

    std::ifstream file(std::filesystem::u8path(fileName), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + fileName);
    }

    std::ostringstream content;
    content << file.rdbuf();
    if (file.bad()) {
        throw std::runtime_error("Не удалось прочитать файл: " + fileName);
    }
    text = content.str();

}

int Document::getId() const {
    return id;
}

const std::string& Document::getFileName() const {
    return fileName;
}

const std::string& Document::getText() const {
    return text;
}
