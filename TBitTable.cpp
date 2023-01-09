#include "TBitTable.hpp"

std::size_t TBitIndex::BitsToBytes(std::size_t nBits) {
    return ((nBits & 0x7) != 0) ? (nBits >> 3) + 1 : (nBits >> 3);
}

TBitIndex::TBitIndex() {
    this->nBits = 0;

    this->nBytes = 0;
    this->nTokens = 0;
    
    this->allocBytes = 0;
    this->allocTokens = 0;

    this->bitTable = nullptr;
}

TBitIndex::TBitIndex(std::size_t nTokens, std::size_t nFilenames) {
    this->nBits = nFilenames;

    this->nBytes = BitsToBytes(nFilenames);
    this->nTokens = nTokens;
    
    for(this->allocBytes = 1; this->allocBytes <= this->nBytes; this->allocBytes *= 2);
    for(this->allocTokens = 1; this->allocTokens <= this->nTokens; this->allocTokens *= 2);

    this->bitTable = new char*[this->allocTokens];
    for(std::size_t i = 0; i < this->allocTokens; ++i) {
        this->bitTable[i] = new char[this->allocBytes];
    }
}

TBitIndex::TBitIndex(const TBitIndex& table) {
    
    this->allocBytes = table.allocBytes;
    this->allocTokens = table.allocTokens;
    this->nBytes = table.nBytes;
    this->nTokens = table.nTokens;
    this->nBits = table.nBits;

    this->bitTable = new char*[this->allocTokens];
    if(this->bitTable == nullptr) {
        std::cerr << "ERROR: malloc at TBitTable::TBitTable(const TBitTable&)!" << std::endl;
    }
    for(std::size_t i = 0; i < this->nTokens; ++i) {
        this->bitTable[i] = new char[this->allocBytes];
    }

    for(std::size_t i = 0; i < this->nTokens; ++i) {
        for(std::size_t j = 0; j < this->nBytes; ++j) {
            this->bitTable[i][j] = table.bitTable[i][j];
        }
    }
}

TBitIndex::~TBitIndex() {
    if(this->bitTable) {
        for(std::size_t i = 0; i < this->allocTokens; ++i) {
            delete[] this->bitTable[i];
        }
        delete[] this->bitTable;
    }
}

void TBitIndex::Clear() {
    if(this->bitTable) {
        for(std::size_t i = 0; i < this->allocTokens; ++i) {
            if (this->bitTable[i]) {
                delete[] this->bitTable[i];
            }
        }
        delete[] this->bitTable;
    }

    this->nBits = 0;

    this->nBytes = 0;
    this->nTokens = 0;
    
    this->allocBytes = 0;
    this->allocTokens = 0;

    this->bitTable = nullptr;

}

void TBitIndex::Init(std::size_t nTokens, std::size_t nFilenames) {

    this->Clear();

    this->nBits = nFilenames;

    this->nBytes = BitsToBytes(nFilenames);
    this->nTokens = nTokens;
    
    for(this->allocBytes = 1; this->allocBytes <= this->nBytes; this->allocBytes *= 2);
    for(this->allocTokens = 1; this->allocTokens <= this->nTokens; this->allocTokens *= 2);

    this->bitTable = new char*[this->allocTokens];
    for(std::size_t i = 0; i < this->allocTokens; ++i) {
        this->bitTable[i] = new char[this->allocBytes];
    }
    return;
}

std::size_t TBitIndex::SizeInBytes() {
    return this->nBytes;
}

std::size_t TBitIndex::SizeInBits() {
    return this->nBits;
}

void TBitIndex::Realloc() {

    if (this->nBytes >= (this->allocBytes / 2)) {
        this->ReallocX();
    }

    if (this->nTokens >= (this->allocTokens / 2)) {
        this->ReallocY();
    }
}

void TBitIndex::ReallocY() {

    std::size_t prevAllocTokens = this->allocTokens;
    this->allocTokens = (this->allocTokens == 0) ? 2 : this->allocTokens * 2;

    char** newBitTable = new char*[this->allocTokens];
    if(newBitTable == nullptr) {
        std::cerr << "ERROR: new at TBitTable::ReallocY()!" << std::endl;
    }

    if(this->bitTable == nullptr) {
        for(std::size_t i = 0; i < this->allocTokens; ++i) {
            
            newBitTable[i] = new char[this->allocBytes];
            if(newBitTable[i] == nullptr) {
                std::cerr << "ERROR: new at TBitTable::ReallocY()!" << std::endl;
            }

            for(std::size_t j = 0; j < this->allocBytes; ++j) {
                newBitTable[i][j] = 0;
            }
        }
    }
    else {
        
        for(std::size_t i = 0; i < prevAllocTokens; ++i) {
            newBitTable[i] = this->bitTable[i];
        }
        for(std::size_t i = prevAllocTokens; i < this->allocTokens; ++i) {
            
            newBitTable[i] = new char[this->allocBytes];
            if(newBitTable[i] == nullptr) {
                std::cerr << "ERROR: new at TBitTable::ReallocY()!" << std::endl;
            }

            for(std::size_t j = 0; j < this->allocBytes; ++j) {
                newBitTable[i][j] = 0;
            }
        }

        delete[] this->bitTable;
    }

    this->bitTable = newBitTable;
    return;
}

void TBitIndex::ReallocX() {

    std::size_t prevAllocBytes  = this->allocBytes;
    this->allocBytes            = (this->allocBytes == 0) ? 2 : this->allocBytes * 2;

    if(this->bitTable == nullptr) {
        return;
    }
    
    for(std::size_t i = 0; i < this->allocTokens; ++i) {
        char* newLine = new char[this->allocBytes];
        if(newLine == nullptr) {
            std::cerr << "ERROR: new at TArray::reallocArray()!" << std::endl;
        }

        for(std::size_t j = 0; j < prevAllocBytes; ++j) {
            newLine[j] = this->bitTable[i][j];
        }
        for(std::size_t j = prevAllocBytes; j < this->allocBytes; ++j) {
            newLine[j] = 0;
        }

        delete[] this->bitTable[i];
        this->bitTable[i] = newLine;
    }
    
    return;
}

TFileData* TBitIndex::GetFileDataByID(std::size_t id) {
    std::string strID = std::to_string(id);
    return this->iDToFiledata.Lookup(strID);
}

std::size_t TBitIndex::Add(std::string& token, int count, TFileData& filedata) {

    std::size_t i = 0;
    std::size_t j = 0;

    if(this->filenameToID.Insert(filedata.filepath, this->nBits) != nullptr) {
        j = this->nBits;
        std::string id = std::to_string(this->nBits);
        this->iDToFiledata.Insert(id, filedata);
        ++(this->nBits);
        this->nBytes = BitsToBytes(this->nBits);
    }
    else {
        j = *(this->filenameToID.Lookup(filedata.filepath));
    }

    if(this->tokensToID.Insert(token, this->nTokens) != nullptr) {
        i = this->nTokens;
        ++(this->nTokens);
    }
    else {
        i = *(this->tokensToID.Lookup(token));
    }

    this->Realloc();

    this->BitSet(i, j, 1);
    return 0;
}

void TBitIndex::Print() {
    TArray<std::string*> tokenList = this->tokensToID.KeyList();
    TArray<std::string*> fileList  = this->filenameToID.KeyList();
    for(std::size_t i = 0; i < tokenList.Size(); ++i) {
        std::cout << "Token: " << *tokenList[i] << " | Table: ";
        for(std::size_t j = 0; j < fileList.Size(); ++j) {
            std::cout << (int) this->BitGet(i, j);
            if (j % 8 == 0 && j > 0) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void TBitIndex::WriteToFile(std::string& filename) {

    std::fstream file(filename, std::ios::out);

    TArray<std::string*> tokenList = this->tokensToID.KeyList();
    TArray<std::string*> fileList = this->filenameToID.KeyList();
    for(std::size_t i = 0; i < tokenList.Size(); ++i) {
        file << "| Token: " << *tokenList[i] << std::endl << "| Table:";
        for(std::size_t j = 0; j < fileList.Size(); ++j) {
            file << (int) this->BitGet(i, j);
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

TArray<std::string*> TBitIndex::GetFileList() {
    return TArray(this->filenameToID.KeyList());
}

unsigned char TBitIndex::BitGet(std::size_t i, std::size_t j) {
    if (j >= this->nBits || i >= this->nTokens) {
        std::cerr << "ERROR: out of border in TBitIndex::BitGet(std::size_t, std::size_t)!" << std::endl;
        return 2;
    }
    int k = (j & 0x7);
    return ( (*(this->bitTable[i] + (j >> 3))) >> k) & 0x01;
}

unsigned char TBitIndex::BitGet(std::string& token, std::string& filename) {
    
    std::size_t i = *(this->tokensToID.Lookup(token));
    std::size_t j = *(this->filenameToID.Lookup(filename));

    return this->BitGet(i, j);
}

unsigned char TBitIndex::BitSet(std::size_t i, std::size_t j, unsigned char value) {
    if (j >= this->nBits || i >= this->nTokens) {
        std::cerr << "ERROR: out of border in TBitIndex::BitSet(std::size_t, std::size_t, unsigned char)!" << std::endl;
        return 2;
    }

    int k = (j & 0x7);
    int byte = j >> 3;

    if (value) {
        *(this->bitTable[i] + byte) = *(this->bitTable[i] + byte) | 0x01 << k;
    }
    else {
        *(this->bitTable[i] + byte) = *(this->bitTable[i] + byte) & ~(0x01 << k);
    }
    
    return 0;
}
