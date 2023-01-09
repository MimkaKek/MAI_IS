#include "TFileData.hpp"

TFileData::TFileData(): filepath(), title(), tokenToTF() {
    this->score     = 0;
    this->id        = 0;
};

TFileData::~TFileData() {};

TFileData::TFileData(TFileData& toCopy) {
    this->filepath  = toCopy.filepath;
    this->title     = toCopy.title;
    this->tokenToTF = toCopy.tokenToTF;
    this->score     = toCopy.score;
    this->id        = toCopy.id;
}

TFileData::TFileData(std::string filepath, std::string title, std::size_t id): tokenToTF() {
    this->filepath  = filepath;
    this->title     = title;
    this->score     = 0.0;
    this->id        = id;
}

std::ostream& operator<<(std::ostream& os, TFileData& fileData) {
    os << "Filepath: " << fileData.filepath << std::endl;
    os << "Title: " << fileData.title << std::endl;
    os << "Score: " << fileData.score << std::endl;
    return os;
}