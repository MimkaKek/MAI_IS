#include "TSearch.hpp"

TSearch::TSearch() {
    this->root = DOC_PATH;
};

TSearch::~TSearch() {};

TArray<std::string> TSearch::BoolParse(std::string str) {

    std::size_t size = str.length();
    std::string buffer = "";
    
    TArray<std::string> tokens;

    bool isQuote = false;

    for(std::size_t i = 0; i < size; ++i) {
        if(str[i] == '!' || str[i] == '(' || str[i] == ')') {
            if(isQuote) {
                buffer += str[i];
            }
            else {
                
                if (buffer != "") {
                    tokens.Push(buffer);
                    buffer = "";
                }

                buffer += str[i];
                tokens.Push(buffer);
                buffer = "";
            }
        }
        else if(str[i] == '\"') {
            buffer += str[i];
            isQuote = (isQuote) ? false : true;
        }
        else if(str[i] != ' ') {
            buffer += str[i];
        }
        else {
            if(isQuote) {
                buffer += str[i];
            }
            else {
                tokens.Push(buffer);
                buffer = "";
            }
        }
    }
    if (buffer != "") {
        tokens.Push(buffer);
    }

    return tokens;
};

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

void TSearch::LoadIndex() {
    std::string  str;
    std::string  filePath = "";
    std::fstream file;

    for (const auto & entry : fs::directory_iterator(this->root)) {
        filePath = entry.path();
        if(filePath.find(".html") != std::string::npos) {
            // std::string cmd = "python3 extract_text_from_html.py " + filePath;
            // char* cStrCmd  = new char[cmd.length() + 1];
            // strcpy(cStrCmd, cmd.c_str());
            // system(cStrCmd);
            // delete[] cStrCmd;
            // std::size_t fSize = filePath.length();
            // filePath[fSize - 4] = 'd';
            // filePath[fSize - 3] = 'a';
            // filePath[fSize - 2] = 't';
            // filePath[fSize - 1] = 'a';
            continue;
        }
        file.open(filePath, std::fstream::in);
        if(file.is_open()) {
            std::cout << "\rReading " << filePath << "...";
            std::size_t nLine = 0;
            while(std::getline(file, str)) {
                ++nLine;
                std::size_t fSize = filePath.length();
                filePath[fSize - 4] = 'h';
                filePath[fSize - 3] = 't';
                filePath[fSize - 2] = 'm';
                filePath[fSize - 1] = 'l';
                this->index.Add(str, filePath);
            }
            file.close();
        }
        else {
            std::cerr << "ERROR: failed open " << filePath << std::endl;
            return;
        }
    }
    std::cout << "\33[2K\rReading finished!" << std::endl;
    std::string sFilename = "IndexInfo.data";
    this->index.WriteToFile(sFilename);
    return;
}

TArray<std::string> TSearch::BooleanSearch(std::string strToSearch) {

    this->search = strToSearch;
    this->_BoolParse();
    std::cout << "Syntax tree:" << std::endl;
    this->tree.Print();
    
    unsigned char result;

    TArray<std::string*> files = this->index.GetFileList();
    std::size_t sizeF = files.Size();

    TArray<std::string> data;

    for(std::size_t j = 0; j < sizeF; ++j) {
        
        result = this->tree.CalcBool(files[j], &(this->index));

        if(result) {
            data.Push(std::string(*files[j]));
        }
    }

    this->tree.Clear();
    
    std::cout << "\33[2K\rSearch finished!" << std::endl;
    return TArray(data);
}

TArray<std::string> TSearch::StupidSearch(std::string strToSearch) {

    std::string str;
    std::string filePath = "";
    std::fstream file;
    TArray<std::string> data;

    for (const auto & entry : fs::directory_iterator(this->root)) {
        filePath = entry.path();
        if(filePath.find(".data") != std::string::npos) {
            continue;
        }
        file.open(filePath, std::fstream::in);
        if(file.is_open()) {
            // std::cout << "\rReading " << filePath << "...";
            std::size_t nLine = 0;
            while(std::getline(file, str)) {
                ++nLine;
                std::size_t p = str.find(strToSearch);
                if(p != std::string::npos) {
                    // std::cout << "\rFinded in " << filePath << " at " << nLine << ":" << p << std::endl;
                    data.Push(filePath);
                }
            }
            file.close();
        }
        else {
            std::cerr << "ERROR: failed open " << filePath << std::endl;
            return TArray(data);
        }
    }
    std::flush(std::cout);
    std::cout << "\33[2K\rSearch finished!" << std::endl;
    return TArray(data);
}