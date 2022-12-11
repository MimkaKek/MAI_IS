#include "TSyntaxTree.hpp"

TSyntaxTree::TSyntaxTree() {
    this->root = nullptr;
    this->lastToken = "";
    this->currentNode = nullptr;
}

TSyntaxTree::~TSyntaxTree() {
    if(root) {
        delete root;
    }
}

void TSyntaxTree::InsertNewParentNode(TSyntaxTreeItem* newParentNode) {

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

void TSyntaxTree::LookForNegation() {

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

void TSyntaxTree::InsertNegation(TSyntaxTreeItem* newNegation) {
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
        this->InsertAND(newANDNode);
        newANDNode->setRight(newNegation);
        newNegation->setParent(newANDNode);
    }
    this->currentNode = newNegation;
    return;
}

void TSyntaxTree::InsertOR(TSyntaxTreeItem* newORNode) {
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
        this->InsertNewParentNode(newORNode);
    }

    this->currentNode = newORNode;
    return;
}

void TSyntaxTree::InsertAND(TSyntaxTreeItem* newANDNode) {
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
        this->InsertNewParentNode(newANDNode);
    }

    this->currentNode = newANDNode;
    return;
}

void TSyntaxTree::InsertToken(TSyntaxTreeItem* newTokenNode) {
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
        this->LookForNegation();
        TSyntaxTreeItem* newANDNode = new TSyntaxTreeItem(std::string("&&"));
        this->InsertAND(newANDNode);
        newANDNode->setRight(newTokenNode);
        newTokenNode->setParent(newANDNode);
    }

    return;
}


void TSyntaxTree::InsertCloseBracket() {
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

    this->LookForNegation();
    return;
}

void TSyntaxTree::InsertOpenBracket(TSyntaxTreeItem* newBRNode) {
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
        this->InsertAND(newANDNode);
        newANDNode->setRight(newBRNode);
        newBRNode->setParent(newANDNode);
    }
    
    this->currentNode = newBRNode;
    return;
}

void TSyntaxTree::Insert(std::string nToken) {

    if (nToken == ")") {
        this->InsertCloseBracket();
        return;
    }

    TSyntaxTreeItem* newNode = new TSyntaxTreeItem(nToken, nullptr, nullptr, nullptr);

    if(this->root == nullptr) {
        this->root = newNode;
        this->currentNode = newNode;
        return;
    }

    if(nToken == "&&") {
        this->InsertAND(newNode);
    }
    else if(nToken == "||") {
        this->InsertOR(newNode);
    }
    else if(nToken == "!") {
        this->InsertNegation(newNode);
    }
    else if(nToken == "(") {
        this->InsertOpenBracket(newNode);
    }
    else {
        this->InsertToken(newNode);
    }

    return;
}

unsigned char TSyntaxTree::RecCalcBool(TSyntaxTreeItem* node, std::string* filename, TBitTable* index) {

    if (!node) {
        return 1;
    }

    std::string* token = node->getTokenPtr();

    if (*token == "||") {
        return this->RecCalcBool(node->getLeft(), filename, index) | this->RecCalcBool(node->getRight(), filename, index);
    }
    else if (*token == "&&") {
        return this->RecCalcBool(node->getLeft(), filename, index) & this->RecCalcBool(node->getRight(), filename, index);
    }
    else if (*token == "!!") {
        return !this->RecCalcBool(node->getLeft(), filename, index);
    }
    else if (*token == ")") {
        return this->RecCalcBool(node->getLeft(), filename, index);
    }
    else {
        return index->BitGet(node->getTokenPtr(), filename);
    }
}

unsigned char TSyntaxTree::RecCalcBool(TSyntaxTreeItem* node, char* filename, TBitTable* index) {

    if (!node) {
        return 1;
    }

    std::string* token = node->getTokenPtr();

    if (*token == "||") {
        return this->RecCalcBool(node->getLeft(), filename, index) | this->RecCalcBool(node->getRight(), filename, index);
    }
    else if (*token == "&&") {
        return this->RecCalcBool(node->getLeft(), filename, index) & this->RecCalcBool(node->getRight(), filename, index);
    }
    else if (*token == "!!") {
        return (this->RecCalcBool(node->getLeft(), filename, index)) ? 0 : 1;
    }
    else if (*token == ")") {
        return this->RecCalcBool(node->getLeft(), filename, index);
    }
    else {
        std::string str(filename);
        return index->BitGet(node->getTokenPtr(), &str);
    }
}

unsigned char TSyntaxTree::CalcBool(std::string* filename, TBitTable* index) {
    return RecCalcBool(this->root, filename, index);
}

unsigned char TSyntaxTree::CalcBool(char* filename, TBitTable* index) {
    return RecCalcBool(this->root, filename, index);
}

void TSyntaxTree::Clear() {
    if(this->root) {
        delete this->root;
        this->root = nullptr;
        this->currentNode = nullptr;
    }
}

const int MAX_DEEP = 10;

void TSyntaxTree::RecPrint(TSyntaxTreeItem* root, int deep) {
    
    int pDeep;
    
    if ( root->getLeft() != nullptr ) {
        this->RecPrint(root->getLeft(), deep + 1);
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
        this->RecPrint(root->getRight(), deep + 1);
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
    this->RecPrint(this->root, 0);
    return;
}