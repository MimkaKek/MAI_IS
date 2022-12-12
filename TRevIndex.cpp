#include "TRevIndex.hpp"

TRevIndex::TRevIndex() {
    this->tSize = 0;
    this->fSize = 0;
};

TRevIndex::~TRevIndex() {};

std::size_t TRevIndex::Add(std::string token, std::string filename) {
    char* cstrFn = new char[filename.length() + 1];
    char* cstrT  = new char[token.length() + 1];
    strcpy(cstrFn, filename.c_str());
    strcpy(cstrT, token.c_str());

    std::size_t fIndex = 0;
    std::size_t tIndex = 0;

    if(this->filenameToId.Insert(cstrFn, this->fSize) != nullptr) {

        std::string idStr = std::to_string(this->fSize);
        char* cstrI  = new char[idStr.length() + 1];
        strcpy(cstrI, idStr.c_str());
        this->idToFilename.Insert(cstrI, cstrFn);
        delete[] cstrI;

        fIndex = this->fSize;
        ++(this->fSize);
    }
    else {
        fIndex = *(this->filenameToId.Lookup(cstrFn));
    }

    if(this->tokensToId.Insert(cstrT, this->tSize) != nullptr) {

        std::string idStr = std::to_string(this->tSize);
        char* cstrI  = new char[idStr.length() + 1];
        strcpy(cstrI, idStr.c_str());
        this->idToTokens.Insert(cstrI, cstrT);
        delete[] cstrI;

        tIndex = this->tSize;
        ++(this->tSize);
    }
    else {
        tIndex = *(this->tokensToId.Lookup(cstrT));
    }

    delete[] cstrFn;
    delete[] cstrT;

    if (this->index.Size() <= tIndex) {
        this->index.Push(TArray<std::size_t>());
        this->counter.Push(0);
    }
    this->index[tIndex].Push(fIndex);
    
    std::size_t j = this->index[fIndex].Size() - 1;

    while(j != 0 && this->index[fIndex][j] < this->index[fIndex][j - 1]) {
        std::size_t tmp            = this->index[fIndex][j];
        this->index[fIndex][j]     = this->index[fIndex][j - 1];
        this->index[fIndex][j - 1] = tmp;
        --j;
    }

    return 0;
}

std::string TRevIndex::Get(std::string token) {
    char* cstrT  = new char[token.length() + 1];
    strcpy(cstrT, token.c_str());

    std::size_t tIndex = *(this->tokensToId.Lookup(cstrT));

    if(this->counter[tIndex] >= this->index[tIndex].Size()) {
        return std::string("");
    }

    std::size_t fIndex = this->index[tIndex][this->counter[tIndex]];
    this->counter[tIndex] += 1;

    std::string idStr = std::to_string(fIndex);
    char* cstrI  = new char[idStr.length() + 1];
    strcpy(cstrI, idStr.c_str());

    std::string filename = *(this->idToFilename.Lookup(cstrI));

    delete[] cstrT;
    delete[] cstrI;

    return filename;
}

void TRevIndex::Reset() {
    for(std::size_t i = 0; i < this->tSize; ++i) {
        this->counter[i] = 0;
    }
    return;
}

void TRevIndex::Reset(std::string token) {

    char* cstrT  = new char[token.length() + 1];
    strcpy(cstrT, token.c_str());

    std::size_t tIndex = *(this->tokensToId.Lookup(cstrT));

    this->counter[tIndex] = 0;

    delete[] cstrT;

    return;
}

void TRevIndex::Print() {

    for(std::size_t i = 0; i < this->tSize; ++i) {

        std::size_t size = this->index[i].Size();
        std::cout << //TODO
        for(std::size_t j = 0; j < size; ++j) {
            std::cout << 
        }
    }

    std::size_t tIndex = *(this->tokensToId.Lookup(cstrT));

    this->counter[tIndex] = 0;

    delete[] cstrT;
    
    return;
}