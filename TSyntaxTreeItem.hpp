#ifndef T_SYNTAX_TREE_ITEM_HPP
#define T_SYNTAX_TREE_ITEM_HPP

#include <string>

class TSyntaxTreeItem {
    private:

        std::string      token;
        TSyntaxTreeItem* left;
        TSyntaxTreeItem* right;
        TSyntaxTreeItem* parent;

    public:

        TSyntaxTreeItem();
        TSyntaxTreeItem(std::string);
        TSyntaxTreeItem(std::string, TSyntaxTreeItem*, TSyntaxTreeItem*, TSyntaxTreeItem*);
        ~TSyntaxTreeItem();

        std::string* getTokenPtr();

        TSyntaxTreeItem* getLeft();
        TSyntaxTreeItem* getRight();
        TSyntaxTreeItem* getParent();

        void setLeft(TSyntaxTreeItem*);
        void setRight(TSyntaxTreeItem*);
        void setParent(TSyntaxTreeItem*);
        void setToken(std::string);
};

#endif