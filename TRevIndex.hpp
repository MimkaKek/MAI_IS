#ifndef T_REV_INDEX_HPP
#define T_REV_INDEX_HPP

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cmath>
#include "TPatriciaTrie.hpp"
#include "TArray.hpp"
#include "TFileData.hpp"
#include "TTokenData.hpp"

class TRevIndex {
    private:

        TPatriciaTrie<TTokenData>  tokenToTokenData;
        TPatriciaTrie<TArray<TTokenData*>> trToTokenData;
        TPatriciaTrie<TFileData>   filenameToFileData;

    public:

        TRevIndex();
        ~TRevIndex();

        void Print();

        TArray<TFileData*>   IDToFileData(TArray<std::size_t>&);

        std::size_t          Add(TTokenData&, int, TFileData&);
        std::size_t          AddTranslation(std::string&, std::string&);

        TArray<TFileData*>   GetArray(std::string&);

        TArray<TTokenData*>  GetTokenData(std::string&);
        TFileData*           GetFileData(std::string&);

        void                 CalcTFxIDF(TArray<TTokenData*>&, TArray<TFileData*>&);
};

#endif