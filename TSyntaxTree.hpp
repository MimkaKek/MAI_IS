#ifndef T_SYNTAX_TREE
#define T_SYNTAX_TREE

#include <iostream>
#include "TSyntaxTreeItem.hpp"
#include "TBitTable.hpp"

class TSyntaxTree {
    private:
        TSyntaxTreeItem* root;
        TSyntaxTreeItem* currentNode;
        std::string      lastToken;

        void InsertAND(TSyntaxTreeItem*);
        void InsertOR(TSyntaxTreeItem*);
        void InsertOpenBracket(TSyntaxTreeItem*);
        void InsertCloseBracket();
        void InsertToken(TSyntaxTreeItem*);
        void InsertNegation(TSyntaxTreeItem*);
        void InsertNewParentNode(TSyntaxTreeItem*);
        void InsertNewQuote(TSyntaxTreeItem*);
        void LookForQuote();
        void LookForNegation();
        void RecPrint(TSyntaxTreeItem*, int);
        unsigned char RecCalcBool(TSyntaxTreeItem*, std::string*, TBitTable*);
        unsigned char RecCalcBool(TSyntaxTreeItem*, char*, TBitTable*);

    public:

        TSyntaxTree();
        ~TSyntaxTree();
        
        unsigned char CalcBool(std::string*, TBitTable*);
        unsigned char CalcBool(char*, TBitTable*);
        void Clear();
        void Insert(std::string);
        void Print();
};

#endif