#include "TSearch.hpp"

TSearch::TSearch() {
    this->root = ".";
};

TSearch::TSearch(std::string& path) {
    this->root = path;
};

TSearch::~TSearch() {};

void TSearch::_BoolParse() {

    std::size_t size = this->search.length();
    std::string buffer = "";

    bool isQuote = false;

    for(std::size_t i = 0; i < size; ++i) {
        if(this->search[i] == '!' || this->search[i] == '(' || this->search[i] == ')') {
            if(isQuote) {
                buffer += this->search[i];
            }
            else {

                if (buffer != "") {
                    this->tree.Insert(buffer);
                    buffer = "";
                }

                buffer += this->search[i];
                this->tree.Insert(buffer);
                buffer = "";
            }
        }
        else if(this->search[i] == '\"') {
            buffer += this->search[i];
            isQuote = (isQuote) ? false : true;
        }
        else if(this->search[i] != ' ') {
            buffer += this->search[i];
        }
        else {
            if(isQuote) {
                buffer += this->search[i];
            }
            else {
                this->tree.Insert(buffer);
                buffer = "";
            }
        }
    }
    if (buffer != "") {
        this->tree.Insert(buffer);
    }

    return;
};

void TSearch::LoadIndex(TYPE type) {

    std::string  str;
    std::string  filePath = "";
    std::fstream file;
    TFileData    fileData;

    this->tree.SetIndexPtr(&this->revIndex);

    auto begin = std::chrono::steady_clock::now();

    for (const auto & entry : fs::directory_iterator(this->root)) {
        filePath = entry.path();
        file.open(filePath, std::fstream::in);
        if(file.is_open()) {
            std::size_t nLine = 0;

            //std::cout << "\rReading... " + filePath;

            fileData.filepath = "";
            std::size_t fPos = filePath.length();
            
            for(fPos = fPos - 6; filePath[fPos] != '/'; --fPos) {
                fileData.filepath = filePath[fPos] + fileData.filepath;
            }

            TTokenData  tokenData;
            int         tokenCount;

            while(std::getline(file, str)) {
                if (nLine == 0) {
                    fileData.title = str;
                    ++nLine;
                    continue;
                }

                if (nLine % 2 == 0) {
                    tokenCount = std::stoi(str);
                    switch(type) {
                        case TSearch::BIT_INDEX:
                            this->bitIndex.Add(tokenData.token, tokenCount, fileData);
                            break;
                        case TSearch::REV_INDEX:
                            this->revIndex.Add(tokenData, tokenCount, fileData);
                            break;
                    }

                }
                else {
                    tokenData.token = str;
                }

                ++nLine;
            }
            file.close();
        }
        else {
            std::cerr << "ERROR: failed open " << filePath << std::endl;
            return;
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "\33[2K\rReading finished! Time: " << elapsed_ms.count() << std::endl;
    return;
}

TArray<TFileData*> TSearch::Search(std::string strToSearch, TYPE type) {
    
    switch(type) {
        case REV_INDEX:
            return _RevSearch(strToSearch);
        case BIT_INDEX:
            return _BitSearch(strToSearch);
        default:
            return TArray<TFileData*>();
    }
}


TArray<TFileData*> TSearch::_RevSearch(std::string strToSearch) {
    
    this->tree.Clear();
    this->search = strToSearch;
    this->_BoolParse();

    return TArray<TFileData*>(this->tree.CalcBool());
}

TArray<TFileData*> TSearch::_BitSearch(std::string strToSearch) {

    this->tree.Clear();
    this->search = strToSearch;
    this->_BoolParse();
    
    return TArray<TFileData*>(this->tree.CalcBool(&(this->bitIndex)));
}