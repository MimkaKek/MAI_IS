#include "TFindedData.hpp"

TFindedData::TFindedData() {}

TFindedData::~TFindedData() {}

std::string TFindedData::GetFile(std::size_t pos) {
    return this->file[pos];
}

std::size_t TFindedData::GetNumber(std::size_t pos) {
    return this->strNumber[pos];
}

std::size_t TFindedData::GetPos(std::size_t pos) {
    return this->strPos[pos];
}

std::size_t TFindedData::Size() {
    return this->file.Size();
}

void TFindedData::Push(std::string filename, std::size_t strN, std::size_t strP) {
    this->file.Push(filename);
    this->strNumber.Push(strN);
    this->strPos.Push(strP);
    return;
}