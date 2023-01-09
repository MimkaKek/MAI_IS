#ifndef T_TOKEN_DATA
#define T_TOKEN_DATA

#include <iostream>
#include <string>
#include "TPatriciaTrie.hpp"
#include "TArray.hpp"
#include "TFileData.hpp"

class TTokenData {
    public:
        std::string         token;
        TArray<TFileData*>  files;
        std::size_t         df;
        std::size_t         id;

        TTokenData();
        TTokenData(TTokenData&);
        TTokenData(std::string);
        ~TTokenData();

        friend std::ostream& operator << (std::ostream&, TTokenData&);

        TTokenData&     operator= (const TTokenData& data) {
            
            this->token = data.token;
            this->files = data.files;
            this->df    = data.df;
            this->id    = data.id;

            return *this;
        }
};

#endif