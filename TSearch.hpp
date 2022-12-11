#ifndef T_SEARCH_HPP
#define T_SEARCH_HPP
#include <string>
#include <fstream>
#include <filesystem>
#include <cstring>

#include "TArray.hpp"
#include "TFindedData.hpp"
#include "TBitTable.hpp"
#include "TSyntaxTree.hpp"

namespace fs = std::filesystem;

class TSearch {
    private:
        std::string              root;
        std::string              search;

        TBitTable                index;
        TSyntaxTree              tree;

        void _BoolParse();

    public:

        TSearch();

        ~TSearch();

        void LoadIndex();

        TArray<std::string> BoolParse(std::string);

        TArray<std::string> StupidSearch(std::string);

        TArray<std::string> BooleanSearch(std::string);
};

const std::string DOC_PATH = "./ruwiki";

#endif