#pragma once

#include <string>

class Document {
    int id;
    std::string fileName;
    std::string text;

public:
    Document(int documentId, const std::string& fileName);

    int getId() const;
    const std::string& getFileName() const;
    const std::string& getText() const;
};
