#ifndef T_BIT_TABLE_HPP
#define T_BIT_TABLE_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include "TPatriciaTrie.hpp"
#include "TFileData.hpp"

class TBitIndex {
    private:

        char**      bitTable;
        bool        check;
        std::size_t nBits;
        std::size_t nBytes;
        std::size_t allocBytes;

        std::size_t nTokens;
        std::size_t allocTokens;

        TPatriciaTrie<std::size_t> filenameToID;
        TPatriciaTrie<std::size_t> tokensToID;
        TPatriciaTrie<TFileData>   iDToFiledata;

        std::size_t BitsToBytes(std::size_t);

    public:

        TBitIndex();
        TBitIndex(std::size_t, std::size_t);
        TBitIndex(const TBitIndex&);

        ~TBitIndex();
        void Clear();
        void Init(std::size_t, std::size_t);

        std::size_t SizeInBytes();
        std::size_t SizeInBits();

        TFileData* GetFileDataByID(std::size_t);

        void Resize(std::size_t, std::size_t);

        void ReallocX();
        void ReallocY();
        void Realloc();

        void WriteToFile(std::string&);
        void Print();

        std::size_t Add(std::string&, int, TFileData&);

        TArray<std::string*> GetFileList();

        unsigned char BitSet(std::size_t, std::size_t, unsigned char);
        
        unsigned char BitGet(std::string&, std::string&);
        unsigned char BitGet(std::size_t, std::size_t);

        TBitIndex& operator= (const TBitIndex& table) {
            
            for(std::size_t i = 0; i < this->nTokens; ++i) {
                delete[] this->bitTable[i];
            }
            delete[] this->bitTable;
            

            this->allocBytes = table.allocBytes;
            this->allocTokens = table.allocTokens;
            this->nBytes = table.nBytes;
            this->nTokens = table.nTokens;
            this->nBits = table.nBits;

            this->bitTable = new char*[this->allocTokens];
            if(this->bitTable == nullptr) {
                std::cerr << "ERROR: malloc at TBitIndex::TBitIndex(const TBitIndex&)!" << std::endl;
            }
            for(std::size_t i = 0; i < this->nTokens; ++i) {
                this->bitTable[i] = new char[this->allocBytes];
            }

            for(std::size_t i = 0; i < this->nTokens; ++i) {
                for(std::size_t j = 0; j < this->nBytes; ++j) {
                    this->bitTable[i][j] = table.bitTable[i][j];
                }
            }

            return *this;
        }
};

#endif