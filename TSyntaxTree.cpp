#include "TSyntaxTree.hpp"

TSyntaxTree::TSyntaxTree() {
    this->root = nullptr;
    this->lastToken = "";
    this->currentNode = nullptr;
    this->isBool = false;
    this->revIndex = nullptr;
}

TSyntaxTree::TSyntaxTree(TRevIndex* revIndex) {
    this->root = nullptr;
    this->lastToken = "";
    this->currentNode = nullptr;
    this->isBool = false;
    this->revIndex = revIndex;
}

TSyntaxTree::~TSyntaxTree() {
    if(root) {
        delete root;
    }
}

void TSyntaxTree::SetIndexPtr(TRevIndex* revIndex) {
    this->revIndex = revIndex;
}

void TSyntaxTree::_InsertNewParentNode(TSyntaxTreeItem* newParentNode) {

    TSyntaxTreeItem* oldParent = this->currentNode->getParent();

    newParentNode->setLeft(this->currentNode);
    newParentNode->setParent(oldParent);
    this->currentNode->setParent(newParentNode);

    if(this->root == this->currentNode) {
        this->root = newParentNode;
    }
    else {
        if (oldParent->getLeft() == this->currentNode) {
            oldParent->setLeft(newParentNode);
        }
        else {
            oldParent->setRight(newParentNode);
        }
    }

    this->currentNode = newParentNode;
    return;
}

void TSyntaxTree::_LookForNegation() {

    if (this->currentNode == this->root) {
        return;
    }

    TSyntaxTreeItem* tmp = this->currentNode;

    while(tmp->getParent()) {
        
        tmp = tmp->getParent();
        std::string* currentToken = tmp->getTokenPtr();

        if(*currentToken == "!" && *(tmp->getLeft()->getTokenPtr()) != "(") {
            tmp->setToken("!!");
            break;
        }

    }

    if(tmp != this->root) {
        this->currentNode = tmp;
    }

    return;
}

void TSyntaxTree::_InsertNegation(TSyntaxTreeItem* newNegation) {
    std::string* lastToken = this->currentNode->getTokenPtr();
    if (*lastToken == "&&" || *lastToken == "||") {
        if(this->currentNode->getRight() == nullptr) {
            this->currentNode->setRight(newNegation);
            newNegation->setParent(this->currentNode);
        }
    } 
    else if (*lastToken == "(" || *lastToken == "!") {
        this->currentNode->setLeft(newNegation);
        newNegation->setParent(this->currentNode);
    }
    else {
        TSyntaxTreeItem* newANDNode = new TSyntaxTreeItem(std::string("&&"));
        this->_InsertAND(newANDNode);
        newANDNode->setRight(newNegation);
        newNegation->setParent(newANDNode);
    }
    this->currentNode = newNegation;
    return;
}

void TSyntaxTree::_InsertOR(TSyntaxTreeItem* newORNode) {
    std::string* lastToken = this->currentNode->getTokenPtr();
    if (*lastToken == "(") {
        std::cout << "ERROR: Bracket before AND/OR!" << std::endl;
        return;
    }
    else if (*lastToken == "!") {
        std::cout << "ERROR: Negation before AND/OR!" << std::endl;
        return;
    }
    else {
        this->_InsertNewParentNode(newORNode);
    }

    this->currentNode = newORNode;
    return;
}

void TSyntaxTree::_InsertAND(TSyntaxTreeItem* newANDNode) {
    std::string* lastToken = this->currentNode->getTokenPtr();

    if (*lastToken == "||") {
        TSyntaxTreeItem* tmp = this->currentNode->getRight();
        this->currentNode->setRight(newANDNode);
        newANDNode->setParent(this->currentNode);

        newANDNode->setLeft(tmp);
        tmp->setParent(newANDNode);
    }
    else if (*lastToken == "(") {
        std::cout << "ERROR: Bracket before AND/OR!" << std::endl;
        return;
    }
    else if (*lastToken == "!") {
        std::cout << "ERROR: Negation before AND/OR!" << std::endl;
        return;
    }
    else {
        this->_InsertNewParentNode(newANDNode);
    }

    this->currentNode = newANDNode;
    return;
}

void TSyntaxTree::_InsertToken(TSyntaxTreeItem* newTokenNode) {
    std::string* currentToken = this->currentNode->getTokenPtr();
    if ((*currentToken == "&&" || *currentToken == "||") && this->currentNode->getRight() == nullptr) {
        this->currentNode->setRight(newTokenNode);
        newTokenNode->setParent(this->currentNode);
    }
    else if (*currentToken == "!") {
            this->currentNode->setLeft(newTokenNode);
            newTokenNode->setParent(this->currentNode);
            this->currentNode->setToken("!!");
    } 
    else if (*currentToken == "(") {
        this->currentNode->setLeft(newTokenNode);
        newTokenNode->setParent(this->currentNode);
        this->currentNode = newTokenNode;
    } else {
        this->_LookForNegation();
        TSyntaxTreeItem* newANDNode = new TSyntaxTreeItem(std::string("&&"));
        this->_InsertAND(newANDNode);
        newANDNode->setRight(newTokenNode);
        newTokenNode->setParent(newANDNode);
    }

    return;
}


void TSyntaxTree::_InsertCloseBracket() {
    if (this->root == nullptr || this->currentNode == nullptr) {
        return;
    }

    TSyntaxTreeItem* tmp = this->currentNode;

    while(*(tmp->getTokenPtr()) != "(") {
        tmp = tmp->getParent();
        if (tmp == nullptr) {
            return;
        }
    }

    tmp->setToken(")");
    this->currentNode = tmp;

    this->_LookForNegation();
    return;
}

void TSyntaxTree::_InsertOpenBracket(TSyntaxTreeItem* newBRNode) {
    std::string* lastToken = this->currentNode->getTokenPtr();
    if ((*lastToken == "&&" || *lastToken == "||") && this->currentNode->getRight() == nullptr) {
        this->currentNode->setRight(newBRNode);
        newBRNode->setParent(this->currentNode);
    } 
    else if (*lastToken == "(" || *lastToken == "!") {
        this->currentNode->setLeft(newBRNode);
        newBRNode->setParent(this->currentNode);
    }
    else {
        TSyntaxTreeItem* newANDNode = new TSyntaxTreeItem(std::string("&&"));
        this->_InsertAND(newANDNode);
        newANDNode->setRight(newBRNode);
        newBRNode->setParent(newANDNode);
    }
    
    this->currentNode = newBRNode;
    return;
}

void TSyntaxTree::Insert(std::string nToken) {

    if (nToken == ")") {
        this->_InsertCloseBracket();
        return;
    }

    TSyntaxTreeItem* newNode = new TSyntaxTreeItem(nToken, nullptr, nullptr, nullptr);

    if(this->root == nullptr) {
        this->root = newNode;
        this->currentNode = newNode;
        if(nToken != "&&" && nToken != "||" && nToken != "!" && nToken != "(") {
            this->tokenList.Push(this->revIndex->GetTokenData(nToken));
        }
        return;
    }

    if(nToken == "&&") {
        this->isBool = true;
        this->_InsertAND(newNode);
    }
    else if(nToken == "||") {
        this->isBool = true;
        this->_InsertOR(newNode);
    }
    else if(nToken == "!") {
        this->_InsertNegation(newNode);
    }
    else if(nToken == "(") {
        this->_InsertOpenBracket(newNode);
    }
    else {
        this->_InsertToken(newNode);
        this->tokenList.Push(this->revIndex->GetTokenData(nToken));
    }

    return;
}

unsigned char TSyntaxTree::_RecCalcBool(TSyntaxTreeItem* node, std::size_t fileID, TBitIndex* index) {

    if (!node) {
        return 1;
    }

    std::string* token = node->getTokenPtr();

    if (*token == "||") {
        return this->_RecCalcBool(node->getLeft(), fileID, index) | this->_RecCalcBool(node->getRight(), fileID, index);
    }
    else if (*token == "&&") {
        return this->_RecCalcBool(node->getLeft(), fileID, index) & this->_RecCalcBool(node->getRight(), fileID, index);
    }
    else if (*token == "!!") {
        return !this->_RecCalcBool(node->getLeft(), fileID, index);
    }
    else if (*token == ")") {
        return this->_RecCalcBool(node->getLeft(), fileID, index);
    }
    else {
        return index->BitGet(*token, index->GetFileDataByID(fileID)->filepath);
    }
}

TArray<TTokenData*> TSyntaxTree::GetTokenList() {
    return this->tokenList;
}

TArray<TFileData*> TSyntaxTree::_RecCalcBool(TSyntaxTreeItem* node) {
    
    if (!node) {
        return TArray<TFileData*>();
    }

    std::string* token = node->getTokenPtr();

    if (*token == "||") {
        TArray<TFileData*> a = this->_RecCalcBool(node->getLeft());
        TArray<TFileData*> b = this->_RecCalcBool(node->getRight());
        TArray<TFileData*> c;

        std::size_t sizeA = a.Size();
        std::size_t sizeB = b.Size();

        std::size_t iterA = 0;
        std::size_t iterB = 0;

        while(iterA < sizeA && iterB < sizeB) {
            if(a[iterA]->id > b[iterB]->id) {
                c.Push(b[iterB]);
                ++iterB;
            }
            else if (a[iterA]->id < b[iterB]->id) {
                c.Push(a[iterA]);
                ++iterA;
            }
            else {
                c.Push(a[iterA]);
                ++iterA;
                ++iterB;
            }
        }

        while(iterB < sizeB) {
            c.Push(b[iterB]);
            ++iterB;
        }

        while(iterA < sizeA) {
            c.Push(a[iterA]);
            ++iterA;
        }

        return TArray<TFileData*>(c);
    }
    else if (*token == "&&") {
        TArray<TFileData*> a = this->_RecCalcBool(node->getLeft());
        TArray<TFileData*> b = this->_RecCalcBool(node->getRight());
        TArray<TFileData*> c;

        std::size_t sizeA = a.Size();
        std::size_t sizeB = b.Size();

        std::size_t iterA = 0;
        std::size_t iterB = 0;

        while(iterA < sizeA && iterB < sizeB) {
            if(a[iterA]->id > b[iterB]->id) {
                if(this->isBool == false) {
                    c.Push(b[iterB]);
                }
                ++iterB;
            }
            else if (a[iterA]->id < b[iterB]->id) {
                if(this->isBool == false) {
                    c.Push(a[iterA]);
                }
                ++iterA;
            }
            else {
                c.Push(a[iterA]);
                ++iterA;
                ++iterB;
            }
        }

        if (this->isBool == false) {
            while(iterB < sizeB) {
                c.Push(b[iterB]);
                ++iterB;
            }
            while(iterA < sizeA) {
                c.Push(a[iterA]);
                ++iterA;
            }
        }
        
        return TArray<TFileData*>(c);
    }
    else if (*token == "!!") {
        return this->_RecCalcBool(node->getLeft()); //TODO
    }
    else if (*token == ")") {
        return this->_RecCalcBool(node->getLeft());
    }
    else {
        return this->revIndex->GetArray(*token);
    }
 }

TArray<TFileData*> TSyntaxTree::CalcBool(TBitIndex* index) {

    unsigned char result;

    std::size_t sizeF = index->SizeInBits();

    TArray<TFileData*> data;

    for(std::size_t j = 0; j < sizeF; ++j) {
        
        result = this->_RecCalcBool(this->root, j, index);

        if(result) {
            data.Push(index->GetFileDataByID(j));
        }
    }

    return TArray<TFileData*>(data);
}

void TSyntaxTree::_RecSortArray(TArray<TFileData*>& array, std::size_t lPos, std::size_t rPos) {
    if (lPos == rPos) {
        return;
    }
    int mid = (lPos + rPos) / 2;

    this->_RecSortArray(array, lPos   , mid );
    this->_RecSortArray(array, mid + 1, rPos);

    int lap = lPos;
    int rap = mid + 1;
    TArray<TFileData*> tmp(rPos - lPos + 1);

    for (int step = 0; step < rPos - lPos + 1; ++step) {
        if ((rap > rPos) || ((lap <= mid) && (array[lap]->score > array[rap]->score))) {
            tmp[step] = array[lap];
            ++lap;
        }
        else {
            tmp[step] = array[rap];
            ++rap;
        }
    }

    for (int step = 0, pos = lPos; step < rPos - lPos + 1; ++step, ++pos) {
        array[pos] = tmp[step];
    }
}

TArray<TFileData*> TSyntaxTree::CalcBool() {
    TArray<TFileData*>  filesList = this->_RecCalcBool(this->root);
    if (filesList.Size() == 0) {
        return TArray<TFileData*>(filesList);
    }
    this->revIndex->CalcTFxIDF(this->tokenList, filesList);
    this->_RecSortArray(filesList, 0, filesList.Size() - 1);
    return TArray<TFileData*>(filesList);
}

void TSyntaxTree::Clear() {
    if(this->root) {
        delete this->root;
    }
    this->root = nullptr;
    this->lastToken = "";
    this->currentNode = nullptr;
    this->isBool = false;
    this->tokenList.Clear();
}

const int MAX_DEEP = 10;

void TSyntaxTree::_RecPrint(TSyntaxTreeItem* root, int deep) {
    
    int pDeep;
    
    if ( root->getLeft() != nullptr ) {
        this->_RecPrint(root->getLeft(), deep + 1);
    }
    else {
        for(int i = 0; i < deep + 1; ++i) {
            std::cout << ".\t";
        }
        std::cout << ".";
        pDeep = deep + 1;
        while(pDeep < MAX_DEEP) {
            ++pDeep;
            std::cout << "\t.";
        }
        std::cout << std::endl;
    }
    
    for(int i = 0; i < deep; ++i) {
        std::cout << ".\t";
    }
    if (root == this->currentNode) {
        std::cout << "[" << *(root->getTokenPtr()) << "]";
    }
    else {
        std::cout << *(root->getTokenPtr());
    }
    if (root->getParent()) {
        //std::cout << "\t" << *(root->getParent()->getTokenPtr());
        std::cout << "\t" << ".";
    }
    else {
        std::cout << "\t" << ".";
    }
    pDeep = deep + 1;
    while(pDeep < MAX_DEEP) {
        ++pDeep;
        std::cout << "\t.";
    }
    std::cout << std::endl;

    if ( root->getRight() != nullptr ) {
        this->_RecPrint(root->getRight(), deep + 1);
    }
    else {
        for(int i = 0; i < deep + 1; ++i) {
            std::cout << ".\t";
        }
        std::cout << ".";
        pDeep = deep + 1;
        while(pDeep < MAX_DEEP) {
            ++pDeep;
            std::cout << "\t.";
        }
        std::cout << std::endl;
    }
}

void TSyntaxTree::Print() {
    this->_RecPrint(this->root, 0);
    return;
}