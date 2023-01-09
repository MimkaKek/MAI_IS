#ifndef T_FILE_DATA
#define T_FILE_DATA

#include <iostream>
#include <string>
#include "TPatriciaTrie.hpp"

class TFileData {
    public:
        std::string         filepath;
        std::string         title;
        TPatriciaTrie<int>  tokenToTF;
        float               score;
        std::size_t         id;

        TFileData();
        TFileData(TFileData&);
        TFileData(std::string, std::string, std::size_t);
        ~TFileData();

        friend std::ostream& operator << (std::ostream&, TFileData&);

        TFileData&     operator= (const TFileData& data) {
            
            this->filepath  = data.filepath;
            this->title     = data.title;
            this->tokenToTF = data.tokenToTF;
            this->score     = data.score;
            this->id        = data.id;

            return *this;
        }
};

#endif