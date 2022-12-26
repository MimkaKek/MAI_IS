#ifndef T_REV_INDEX_HPP
#define T_REV_INDEX_HPP

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "TPatriciaTrie.hpp"

class TRevIndex {
    private:
        
        // X - Filenames
        // Y - Tokens
        // bitStream[Y][X]

        TArray<TArray<std::size_t>> index;
        TArray<std::size_t>         counter;

        TPatriciaTrie<std::size_t> filenameToId;
        TPatriciaTrie<std::string> idToFilename;
        TPatriciaTrie<std::size_t> tokensToId;
        TPatriciaTrie<std::string> idToTokens;

        std::size_t tSize;
        std::size_t fSize;

    public:

        TRevIndex();
        ~TRevIndex();

        void Print();
        void Reset();
        void Reset(std::string&);

        std::size_t Add(std::string&, std::string&);

        std::string Get(std::string&);
};

#endif