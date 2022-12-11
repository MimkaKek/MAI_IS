#ifndef T_BIT_TABLE_HPP
#define T_BIT_TABLE_HPP

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "TPatriciaTrie.hpp"

class TBitTable {
    private:
        
        // X - Filenames
        // Y - Tokens
        // bitStream[Y][X]

        char**      bitStream;
        
        std::size_t nBits;
        std::size_t sizeX;
        std::size_t allocSizeX;

        std::size_t sizeY;
        std::size_t allocSizeY;

        TPatriciaTrie<std::size_t> filenames;
        TPatriciaTrie<std::size_t> tokens;

        std::size_t BitsToBytes(std::size_t);

    public:

        TBitTable();
        TBitTable(std::size_t, std::size_t);
        TBitTable(const TBitTable&);

        ~TBitTable();
        void Clear();
        void Init(std::size_t, std::size_t);

        std::size_t SizeInBytes();
        std::size_t SizeInBits();

        void Resize(std::size_t, std::size_t);

        void ReallocX();
        void ReallocY();

        void Print();

        std::size_t AddX(std::string);
        std::size_t AddY(std::string);
        std::size_t Add(std::string, std::string);

        TArray<char*> GetFileList();

        unsigned char BitSet(std::string*, std::string*, unsigned char);
        unsigned char BitSet(std::size_t, std::size_t, unsigned char);
        unsigned char BitSet(char*, char*, unsigned char);
        
        unsigned char BitGet(std::string*, std::string*);
        unsigned char BitGet(std::size_t, std::size_t);
        unsigned char BitGet(char*, char*);

        TBitTable& operator= (const TBitTable& table) {
            
            for(std::size_t i = 0; i < this->sizeY; ++i) {
                delete[] this->bitStream[i];
            }
            delete[] this->bitStream;
            

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

            return *this;
        }
};

#endif