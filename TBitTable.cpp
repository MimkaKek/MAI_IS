#include "TBitTable.hpp"

std::size_t TBitTable::BitsToBytes(std::size_t nBits) {
    return ((nBits & 0x7) != 0) ? (nBits >> 3) + 1 : (nBits >> 3);
}

TBitTable::TBitTable() {
    this->nBits = 0;

    this->sizeX = 0;
    this->sizeY = 0;
    
    this->allocSizeX = 0;
    this->allocSizeY = 0;

    this->bitStream = 0;
}

TBitTable::TBitTable(std::size_t nTokens, std::size_t nFilenames) {
    this->nBits = nFilenames;

    this->sizeX = BitsToBytes(nFilenames);
    this->sizeY = nTokens;
    
    for(this->allocSizeX = 1; this->allocSizeX <= this->sizeX; this->allocSizeX *= 2);
    for(this->allocSizeY = 1; this->allocSizeY <= this->sizeY; this->allocSizeY *= 2);

    this->bitStream = new char*[this->allocSizeY];
    for(std::size_t i = 0; i < this->allocSizeY; ++i) {
        this->bitStream[i] = new char[this->allocSizeX];
    }
}

TBitTable::TBitTable(const TBitTable& table) {
    
    this->allocSizeX = table.allocSizeX;
    this->allocSizeY = table.allocSizeY;
    this->sizeX = table.sizeX;
    this->sizeY = table.sizeY;
    this->nBits = table.nBits;

    this->bitStream = new char*[this->allocSizeY];
    if(this->bitStream == nullptr) {
        std::cerr << "ERROR: malloc at TBitTable::TBitTable(const TBitTable&)!" << std::endl;
    }
    for(std::size_t i = 0; i < this->sizeY; ++i) {
        this->bitStream[i] = new char[this->allocSizeX];
    }

    for(std::size_t i = 0; i < this->sizeY; ++i) {
        for(std::size_t j = 0; j < this->sizeX; ++j) {
            this->bitStream[i][j] = table.bitStream[i][j];
        }
    }
}

TBitTable::~TBitTable() {
    for(std::size_t i = 0; i < this->sizeY; ++i) {
        delete[] this->bitStream[i];
    }
    delete[] this->bitStream;
}

void TBitTable::Clear() {
    if(this->bitStream) {
        for(std::size_t i = 0; i < this->sizeY; ++i) {
            if (this->bitStream[i]) {
                delete[] this->bitStream[i];
            }
        }
        delete[] this->bitStream;
    }

    this->nBits = 0;

    this->sizeX = 0;
    this->sizeY = 0;
    
    this->allocSizeX = 0;
    this->allocSizeY = 0;

    this->bitStream = nullptr;

}

void TBitTable::Init(std::size_t nTokens, std::size_t nFilenames) {

    this->Clear();

    this->nBits = nFilenames;

    this->sizeX = BitsToBytes(nFilenames);
    this->sizeY = nTokens;
    
    for(this->allocSizeX = 1; this->allocSizeX <= this->sizeX; this->allocSizeX *= 2);
    for(this->allocSizeY = 1; this->allocSizeY <= this->sizeY; this->allocSizeY *= 2);

    this->bitStream = new char*[this->allocSizeY];
    for(std::size_t i = 0; i < this->allocSizeY; ++i) {
        this->bitStream[i] = new char[this->allocSizeX];
    }
    return;
}

std::size_t TBitTable::SizeInBytes() {
    return this->sizeX;
}

std::size_t TBitTable::SizeInBits() {
    return this->nBits;
}

void TBitTable::Resize(std::size_t nTokens, std::size_t nFilenames) {

    std::size_t nSizeX = BitsToBytes(nFilenames);
    std::size_t nSizeY = nTokens;
    std::size_t nBits  = nFilenames;
    std::size_t nAllocSizeX = 1;
    std::size_t nAllocSizeY = 1;
    for(;nAllocSizeX <= nSizeX; nAllocSizeX *= 2);
    for(;nAllocSizeY <= nSizeY; nAllocSizeY *= 2);
    char** tmp = new char*[nAllocSizeY];

    std::size_t minSizeX = this->sizeX > nSizeX ? nSizeX : this->sizeX;
    std::size_t minSizeY = this->sizeY > nSizeY ? nSizeY : this->sizeY;
    for (std::size_t i = 0; i < minSizeY; ++i) {
        for (std::size_t j = 0; j < minSizeX; ++j) {
            tmp[i] = this->bitStream[i];
        }
    }

    this->Clear();

    this->allocSizeX = nAllocSizeX;
    this->allocSizeY = nAllocSizeY;

    this->nBits = nBits;

    this->sizeX = nSizeX;
    this->sizeY = nSizeY;

    this->bitStream = tmp;

    return;
}

void TBitTable::ReallocY() {
    char** newAlloc = new char*[this->allocSizeY];
    if(newAlloc == nullptr) {
        std::cerr << "ERROR: new at TBitTable::ReallocY()!" << std::endl;
    }
    if(this->bitStream != nullptr) {
        for(std::size_t i = 0; i < this->sizeY; ++i) {
            newAlloc[i] = this->bitStream[i];
        }
        for(std::size_t i = this->sizeY; i < this->allocSizeY; ++i) {
            newAlloc[i] = new char[this->allocSizeX];
            if(newAlloc[i] == nullptr) {
                std::cerr << "ERROR: new at TBitTable::ReallocY()!" << std::endl;
            }
        }

        delete[] this->bitStream;
    }
    else {
        newAlloc[0] = new char[this->allocSizeX];
        newAlloc[1] = new char[this->allocSizeX];
        if(newAlloc[0] == nullptr || newAlloc[1] == nullptr) {
            std::cerr << "ERROR: new at TBitTable::ReallocY()!" << std::endl;
        }
    }
    this->bitStream = newAlloc;
    return;
}

void TBitTable::ReallocX() {
    for(std::size_t i = 0; i < this->sizeY; ++i) {
        char* newAlloc = new char[this->allocSizeX]();
        if(newAlloc == nullptr) {
            std::cerr << "ERROR: new at TArray::reallocArray()!" << std::endl;
        }

        for(std::size_t j = 0; j < this->sizeX; ++j) {
            newAlloc[j] = this->bitStream[i][j];
        }
        
        delete[] this->bitStream[i];
        
        this->bitStream[i] = newAlloc;
    }
    
    return;
}

std::size_t TBitTable::Add(std::string& token, std::string& filename) {

    std::size_t i = 0;
    std::size_t j = 0;

    if(this->filenames.Insert(filename, this->nBits) != nullptr) {
        j = this->nBits;
        ++(this->nBits);
        this->sizeX = BitsToBytes(this->nBits);
    }
    else {
        j = *(this->filenames.Lookup(filename));
    }

    if(this->tokens.Insert(token, this->sizeY) != nullptr) {
        i = this->sizeY;
        ++(this->sizeY);
    }
    else {
        i = *(this->tokens.Lookup(token));
    }

    if (this->sizeY >= (this->allocSizeY / 2)) {
        this->allocSizeY = (this->allocSizeY == 0) ? 2 : this->allocSizeY * 2;
        this->ReallocY();
    }

    if (this->sizeX >= (this->allocSizeX / 2)) {
        this->allocSizeX = (this->allocSizeX == 0) ? 2 : this->allocSizeX * 2;
        this->ReallocX();
    }

    this->BitSet(i, j, 1);
    return 0;
}

void TBitTable::Print() {
    TArray<std::string*> tokenList = this->tokens.KeyList();
    TArray<std::string*> fileList  = this->filenames.KeyList();
    for(std::size_t i = 0; i < tokenList.Size(); ++i) {
        std::cout << "Token: " << *tokenList[i] << " | Table: ";
        for(std::size_t j = 0; j < fileList.Size(); ++j) {
            std::cout << (int) this->BitGet(*tokenList[i], *fileList[j]);
            if (j % 8 == 0 && j > 0) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void TBitTable::WriteToFile(std::string& filename) {

    std::fstream file(filename, std::ios::out);

    TArray<std::string*> tokenList = this->tokens.KeyList();
    TArray<std::string*> fileList = this->filenames.KeyList();
    for(std::size_t i = 0; i < tokenList.Size(); ++i) {
        file << "| Token: " << *tokenList[i] << std::endl << "| Table:";
        for(std::size_t j = 0; j < fileList.Size(); ++j) {
            file << (int) this->BitGet(*tokenList[i], *fileList[j]);
            if (j % 8 == 0 && j > 0) {
                file << " ";
            }
        }
        file << std::endl;
        file << "=======" << std::endl;
    }

    file << std::endl;

    for(std::size_t j = 0; j < fileList.Size(); ++j) {
        file << j << ": " << *fileList[j] << std::endl;
    }

    file.close();

    return;
}

TArray<std::string*> TBitTable::GetFileList() {
    return TArray(this->filenames.KeyList());
}

unsigned char TBitTable::BitGet(std::string& token, std::string& filename) {

    std::size_t* tmpI = this->tokens.Lookup(token);
    std::size_t* tmpJ = this->filenames.Lookup(filename);

    if(tmpI == nullptr || tmpJ == nullptr) {
        return 2;
    }
    
    std::size_t i = *tmpI;
    std::size_t j = *tmpJ;

    if (j >= this->nBits || i >= this->sizeY) {
        std::cerr << "ERROR: out of border in TBitTable::BitGet(std::string, std::string)!" << std::endl;
        return 2;
    }
    int k = (j & 0x7);
    return ( (*(this->bitStream[i] + (j >> 3))) >> k) & 0x01;
}

unsigned char TBitTable::BitSet(std::string& token, std::string& filename, unsigned char value) {

    std::size_t i = *(this->tokens.Lookup(token));
    std::size_t j = *(this->filenames.Lookup(filename));

    if (j >= this->nBits || i >= this->sizeY) {
        std::cerr << "ERROR: out of border in TBitTable::BitSet(std::string, std::string, unsigned char)!" << std::endl;
        return 2;
    }
    int k = (j & 0x7);
    value = value ? 0x80 : 0x0;
    *(this->bitStream[i] + (j >> 3)) = *(this->bitStream[i] + (j >> 3)) | value >> k;
    return 0;
}

unsigned char TBitTable::BitGet(std::size_t i, std::size_t j) {
    if (j >= this->nBits || i >= this->sizeY) {
        std::cerr << "ERROR: out of border in TBitTable::BitGet(std::size_t, std::size_t)!" << std::endl;
        return 2;
    }
    int k = (j & 0x7);
    return ( (*(this->bitStream[i] + (j >> 3))) >> k) & 0x01;
}

unsigned char TBitTable::BitSet(std::size_t i, std::size_t j, unsigned char value) {
    if (j >= this->nBits || i >= this->sizeY) {
        std::cerr << "ERROR: out of border in TBitTable::BitSet(std::size_t, std::size_t, unsigned char)!" << std::endl;
        return 2;
    }
    int k = (j & 0x7);
    if (value) {
        *(this->bitStream[i] + (j >> 3)) = *(this->bitStream[i] + (j >> 3)) | 0x01 << k;
    }
    else {
        *(this->bitStream[i] + (j >> 3)) = *(this->bitStream[i] + (j >> 3)) & 0x01 << k;
    }
    
    return 0;
}
