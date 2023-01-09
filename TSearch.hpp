#ifndef T_SEARCH_HPP
#define T_SEARCH_HPP
#include <string>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <chrono>

#include "TArray.hpp"
#include "TFindedData.hpp"
#include "TBitTable.hpp"
#include "TSyntaxTree.hpp"
#include "TRevIndex.hpp"
#include "TFileData.hpp"

namespace fs = std::filesystem;

class TSearch {
    private:
        std::string              root;
        std::string              search;

        TBitIndex                bitIndex;
        TSyntaxTree              tree;
        TRevIndex                revIndex;

        void _BoolParse();

        TArray<TFileData*> _RevSearch(std::string);
        TArray<TFileData*> _BitSearch(std::string);

    public:

        enum TYPE { REV_INDEX, BIT_INDEX };

        TSearch();
        TSearch(std::string&);
        ~TSearch();

        void LoadIndex(TYPE);

        TArray<TFileData*> Search(std::string, TYPE);
};

#endif