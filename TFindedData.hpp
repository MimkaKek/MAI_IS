#ifndef T_FINDED_DATA_HPP
#define T_FINDED_DATA_HPP
#include "TArray.hpp"
#include <cstdlib>
#include <string>

class TFindedData {

    private:
        TArray<std::string>   file;
        TArray<std::size_t>   strNumber;
        TArray<std::size_t>   strPos;

    public:

        TFindedData();

        ~TFindedData();

        std::string GetFile(std::size_t);

        std::size_t GetNumber(std::size_t);

        std::size_t GetPos(std::size_t);

        std::size_t Size();

        void Push(std::string, std::size_t, std::size_t);
};

#endif