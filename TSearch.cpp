#include "TSearch.hpp"

TSearch::TSearch() {
    this->root = ".";
    this->totalPath = "./total.data";
};

TSearch::TSearch(std::string& path) {
    this->root = path;
    this->totalPath = "./total.data";
};

TSearch::~TSearch() {};

void TSearch::_BoolParse(std::string& search) {

    std::size_t size = search.length();
    std::string buffer = "";

    bool isQuote = false;

    for(std::size_t i = 0; i < size; ++i) {
        if(search[i] == '!' || search[i] == '(' || search[i] == ')') {
            if(isQuote) {
                buffer += search[i];
            }
            else {

                if (buffer != "") {
                    this->tree.Insert(buffer);
                    buffer = "";
                }

                buffer += search[i];
                this->tree.Insert(buffer);
                buffer = "";
            }
        }
        else if(search[i] == '\"') {
            isQuote = (isQuote) ? false : true;
        }
        else if(search[i] != ' ') {
            buffer += search[i];
        }
        else {
            if(isQuote) {
                buffer += search[i];
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

    std::cout << "Begin generate index... " << std::endl;

    std::string  str;
    std::string  filePath = "";
    std::fstream file, log;
    TFileData    fileData;

    std::size_t  fileN = 0;
    std::size_t  totalFiles = 0;
    float percent;
    float prevPercent = 0.0;

    this->tree.SetIndexPtr(&this->revIndex);

    for (const auto & entry : fs::directory_iterator(this->root)) {
        filePath = entry.path();
        ++totalFiles;
    }

    auto begin = std::chrono::steady_clock::now();
    log.open("logger.log", std::fstream::app);
    std::cout << "Reading <" << totalFiles << "> files... " << std::endl;
    for (const auto & entry : fs::directory_iterator(this->root)) {
        ++fileN;
        percent = std::floor(((float) fileN / (float) totalFiles) * 10000) / 100.0;
        if ((percent - prevPercent) > 0.21) {
            std::cout << "\33[2K\rReady " << percent << "%";
            std::cout.flush();
            prevPercent = percent;
        }
        filePath = entry.path();
        file.open(filePath, std::fstream::in);
        if(file.is_open()) {
            std::size_t nLine = 0;

            fileData.filepath = "";
            std::size_t fPos = filePath.length();
            
            for(fPos = fPos - 6; filePath[fPos] != '/'; --fPos) {
                fileData.filepath = filePath[fPos] + fileData.filepath;
            }

            TTokenData  tokenData;
            int         tokenCount;

            log << "Reading from " + fileData.filepath + "\n";
            while(std::getline(file, str)) {
                if (nLine == 0) {
                    log << "Title: <" + str + ">\n";
                    fileData.title = str;
                    ++nLine;
                    continue;
                }

                if (nLine % 2 == 0) {
                    tokenCount = std::stoi(str);
                    log << "Add Token: <" + tokenData.token + "> | Count: " + str + "\n";
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
    std::cout << "\33[2K\rReading finished! Time: " << ((float) elapsed_ms.count() / 60000) << " min" << std::endl;
    log << "Reading finished! Time: " << ((float) elapsed_ms.count() / 60000) << " min" << std::endl;
    log.close();
    this->_LoadTranslation();
    return;
}

void TSearch::_LoadTranslation() {

    std::cout << "Adding translations to index... " << std::endl;

    std::string  str;
    std::fstream file, log;

    auto begin = std::chrono::steady_clock::now();

    log.open("logger.log", std::fstream::app);

    file.open(this->totalPath, std::fstream::in);
    std::cout << "Reading <" << this->totalPath << ">..." << std::endl;
    if(file.is_open()) {
        while(std::getline(file, str)) {
            
            std::size_t strLength = str.length();
            std::string translation = "";
            std::size_t n = 0;
            for(; str[n] != ' ' && n < strLength; ++n);
            std::string token = str.substr(0, n);
            std::size_t prevN = n + 1;
            for(n = strLength - 1; str[n] != ' ' && n > 0; --n);
            translation = str.substr(prevN, n - prevN);

            this->revIndex.AddTranslation(token, translation);
        }
        file.close();
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    log.close();
    std::cout << "\33[2K\rDone!" << std::endl;
    std::cout << "Load Translates finished! Time: " << ((float) elapsed_ms.count() / 60000) << " min" << std::endl;
    return;
}

TArray<TFileData*> TSearch::Search(std::string strToSearch, TYPE type) {
    
    std::cout << "Searching: <" << strToSearch << ">" << std::endl;

    switch(type) {
        case REV_INDEX:
            return _RevSearch(strToSearch);
        case BIT_INDEX:
            return _BitSearch(strToSearch);
        default:
            return TArray<TFileData*>();
    }
}

TArray<std::string> TSearch::GetAlternate() {
    return this->tree.GetAlternate();
}

TArray<TFileData*> TSearch::_RevSearch(std::string strToSearch) {
    
    this->tree.Clear();
    this->_BoolParse(strToSearch);
    this->tree.Print();

    return TArray<TFileData*>(this->tree.CalcBool());
}

TArray<TFileData*> TSearch::_BitSearch(std::string strToSearch) {

    this->tree.Clear();
    this->_BoolParse(strToSearch);
    this->tree.Print();
    
    return TArray<TFileData*>(this->tree.CalcBool(&(this->bitIndex)));
}