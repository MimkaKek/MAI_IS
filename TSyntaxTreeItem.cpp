#include "TSyntaxTreeItem.hpp"

TSyntaxTreeItem::TSyntaxTreeItem() {
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
}

TSyntaxTreeItem::TSyntaxTreeItem(std::string str) {
    this->token = str;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
}

TSyntaxTreeItem::TSyntaxTreeItem(std::string str, TSyntaxTreeItem* left, TSyntaxTreeItem* right, TSyntaxTreeItem* parent) {
    this->token = str;
    this->left = left;
    this->right = right;
    this->parent = parent;
}

TSyntaxTreeItem::~TSyntaxTreeItem() {
    if(this->left) {
        delete this->left;
    }
    if(this->right) {
        delete this->right;
    }
}

std::string* TSyntaxTreeItem::getTokenPtr() {
    return &(this->token);
}

TSyntaxTreeItem* TSyntaxTreeItem::getLeft() {
    return this->left;
}

TSyntaxTreeItem* TSyntaxTreeItem::getRight() {
    return this->right;
}

TSyntaxTreeItem* TSyntaxTreeItem::getParent() {
    return this->parent;
}
void TSyntaxTreeItem::setLeft(TSyntaxTreeItem* left) {
    this->left = left;
    return;
}

void TSyntaxTreeItem::setRight(TSyntaxTreeItem* right) {
    this->right = right;
    return;
}

void TSyntaxTreeItem::setParent(TSyntaxTreeItem* parent) {
    this->parent = parent;
    return;
}

void TSyntaxTreeItem::setToken(std::string nToken) {
    this->token = nToken;
    return;
}