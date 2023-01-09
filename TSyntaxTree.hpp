#ifndef T_SYNTAX_TREE
#define T_SYNTAX_TREE

#include <iostream>
#include "TSyntaxTreeItem.hpp"
#include "TBitTable.hpp"
#include "TRevIndex.hpp"
#include "TArray.hpp"

class TSyntaxTree {
    private:
        TSyntaxTreeItem*    root;
        TSyntaxTreeItem*    currentNode;
        std::string         lastToken;
        bool                isBool;
        TRevIndex*          revIndex;

        TArray<TTokenData*> tokenList;

        void _InsertAND(TSyntaxTreeItem*);
        void _InsertOR(TSyntaxTreeItem*);
        void _InsertOpenBracket(TSyntaxTreeItem*);
        void _InsertCloseBracket();
        void _InsertToken(TSyntaxTreeItem*);
        void _InsertNegation(TSyntaxTreeItem*);
        void _InsertNewParentNode(TSyntaxTreeItem*);
        void _InsertNewQuote(TSyntaxTreeItem*);
        void _LookForQuote();
        void _LookForNegation();
        void _RecPrint(TSyntaxTreeItem*, int);
        void _SortArray(TArray<TFileData*>&);
        void _RecSortArray(TArray<TFileData*>&, std::size_t, std::size_t);

        unsigned char _RecCalcBool(TSyntaxTreeItem*, std::size_t, TBitIndex*);
        TArray<TFileData*> _RecCalcBool(TSyntaxTreeItem*);

    public:

        TSyntaxTree();
        TSyntaxTree(TRevIndex*);
        ~TSyntaxTree();
        
        TArray<TFileData*> CalcBool(TBitIndex*);
        TArray<TFileData*> CalcBool();

        TArray<TTokenData*> GetTokenList();

        void Clear();
        void Insert(std::string);
        void Print();

        void SetIndexPtr(TRevIndex*);
};

#endif