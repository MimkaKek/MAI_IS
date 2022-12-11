#ifndef T_PATRICIA_TRIE_ITEM_HPP
#define T_PATRICIA_TRIE_ITEM_HPP

#include "TPatriciaTrieItem.hpp"
#include "TArray.hpp"
#include <fstream>

template <class T>
class TPatriciaTrie {
    private:
        
        struct TPack {
            char                    up;
            int                     length;
            T                       data;
            int                     index;
        };

        void                    RecursiveRemove(TPatriciaTrieItem<T>*);
        int                     BitGet(char*, int);
        int                     BitFirstDifferent(char*, char*);
        bool                    KeyCompare(char*, char*);
        void                    KeyCopy(TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        
        TPatriciaTrieItem<T>*   head;
        std::size_t             size;

    public:

        TPatriciaTrie();
        virtual ~TPatriciaTrie();
        
        TPatriciaTrieItem<T>*   LookupNode(char*);
        T*                      Lookup(char*);
        TPatriciaTrieItem<T>*   Insert(char*, T);
        bool                    Delete(char*);
        
        TArray<char*>           KeyList();
        std::size_t             Size();

        TPatriciaTrieItem<T>*   GetHead();
        void                    Print(TPatriciaTrieItem<T>*, int);
        void                    ClearTrie();
        bool                    Empty();
        void                    SwapHead(TPatriciaTrie<T>*);
        
        bool                    SaveTrieBefore(TPatriciaTrieItem<T>*, std::ofstream*);
        bool                    SaveTrieCurrent(TPatriciaTrieItem<T>*, std::ofstream*);
        
        void                    LoadTrieBefore(char*, std::ifstream*);
        void                    LoadTrieCurrent(char*, TPatriciaTrieItem<T>*, std::ifstream*);
};

#endif