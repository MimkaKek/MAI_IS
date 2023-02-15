#include "TRevIndex.hpp"

TRevIndex::TRevIndex(): tokenToTokenData(), trToTokenData(), filenameToFileData() {};

TRevIndex::~TRevIndex() {};

std::size_t TRevIndex::Add(TTokenData& tokenData, int count, TFileData& filedata) {

    filedata.id = this->filenameToFileData.Size();
    tokenData.id = this->tokenToTokenData.Size();

    TPatriciaTrieItem<TTokenData>* tData = this->tokenToTokenData.Insert(tokenData.token, tokenData);
    if(tData != nullptr) {
        tData->GetData()->df = 1;
    }
    else {
        tData = this->tokenToTokenData.LookupNode(tokenData.token);
        tData->GetData()->df += 1;
    }

    TPatriciaTrieItem<TFileData>* fData = this->filenameToFileData.Insert(filedata.filepath, filedata);
    if(fData == nullptr) {
        fData = this->filenameToFileData.LookupNode(filedata.filepath);
    }

    std::string tokenID = std::to_string(tData->GetData()->id);

    fData->GetData()->tokenToTF.Insert(tokenID, count);
    tData->GetData()->files.Push(fData->GetData());

    return 0;
}

std::size_t TRevIndex::AddTranslation(std::string& token, std::string& translation) {
    TPatriciaTrieItem<TTokenData>* tItem = this->tokenToTokenData.LookupNode(token);

    if (tItem == nullptr) {
        return 1;
    }

    TTokenData* data  = tItem->GetData();
    data->translation = translation;

    TPatriciaTrieItem<TArray<TTokenData*>>* trItem = this->trToTokenData.LookupNode(translation);
    if (trItem == nullptr) {
        TArray<TTokenData*> array = TArray<TTokenData*>();
        trItem = this->trToTokenData.Insert(translation, array);
    }

    trItem->GetData()->Push(data);
    return 0;
}

TArray<TTokenData*> TRevIndex::GetTokenData(std::string& token) {
    TArray<TTokenData*> array;
    TTokenData* tokenData = this->tokenToTokenData.Lookup(token);
    if (tokenData == nullptr) {

        std::cout << "Can't find <" << token << "> in RU map" << std::endl;

        TArray<TTokenData*>* tmp = this->trToTokenData.Lookup(token);
        if (tmp != nullptr) {
            for(std::size_t n = 0; n < tmp->Size(); ++n) {
                array.Push(tmp->Get(n));
            }
        }
        else {
            std::cout << "Can't find <" << token << "> in EN map" << std::endl;
        }
    }
    else {
        array.Push(tokenData);
    }

    return TArray<TTokenData*>(array);
}


TFileData* TRevIndex::GetFileData(std::string& filename) {
    return this->filenameToFileData.Lookup(filename);
}

void TRevIndex::CalcTFxIDF(TArray<TTokenData*>& tokenList, TArray<TFileData*>& filesList) {

    std::size_t tokenSum = tokenList.Size();
    std::size_t fileSum  = filesList.Size();

    for (std::size_t fileID = 0; fileID < fileSum; ++fileID) {

        float score = 0.0;

        for (std::size_t tokenID = 0; tokenID < tokenSum; ++tokenID) {
            
            std::string id = std::to_string(tokenList[tokenID]->id);

            int* ptrTf = filesList[fileID]->tokenToTF.Lookup(id);
            float weightTf = (ptrTf == nullptr) ? 0.0 : (1 + std::log10((float) *ptrTf));
            
            float df = this->tokenToTokenData[tokenList[tokenID]->token].df;
            float weightDf = std::log10((float) this->filenameToFileData.Size() / df);

            score += weightTf * weightDf;

        }
        
        filesList[fileID]->score = score;
    }
}

TArray<TFileData*> TRevIndex::GetArray(std::string& token) {
    TArray<TTokenData*> array;
    TTokenData* tokenData = this->tokenToTokenData.Lookup(token);
    if (tokenData == nullptr) {

        std::cout << "Can't find <" << token << "> in RU map" << std::endl;

        TArray<TTokenData*>* tmp = this->trToTokenData.Lookup(token);
        if (tmp != nullptr) {
            for(std::size_t n = 0; n < tmp->Size(); ++n) {
                array.Push(tmp->Get(n));
            }
        }
        else {
            std::cout << "Can't find <" << token << "> in EN map" << std::endl;
        }
    }
    else {
        array.Push(tokenData);
    }

    TArray<TFileData*> callback;

    for(std::size_t n = 0; n < array.Size(); ++n) {
        TArray<TFileData*>* tmp = &(array[n]->files);
        for(std::size_t m = 0; m < tmp->Size(); ++m) {
            callback.Push(tmp->Get(m));
        }
    }

    return TArray<TFileData*>(callback);
}

void TRevIndex::Print() {

    TArray<std::string*> tokenList = this->tokenToTokenData.KeyList();
    std::size_t tSize = tokenList.Size();

    for(std::size_t tokenID = 0; tokenID < tSize; ++tokenID) {
        
        TTokenData* tokenData = this->tokenToTokenData.Lookup(*tokenList[tokenID]);
        std::size_t fSize = tokenData->files.Size();

        std::cout << "Token: " << tokenData->token << std::endl;
        std::cout << "Files: ";

        for(std::size_t fileID = 0; fileID < fSize; ++fileID) {

            TFileData* fileData = tokenData->files[fileID];

            std::cout << fileData->filepath << ":" << fileData->title << " | ";
        }

        std::cout << std::endl;
    }
    
    return;
}