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
        int                     BitGet(const char*, int);
        int                     BitFirstDifferent(const char*, const char*);
        bool                    KeyCompare(const char*, const char*);
        void                    KeyCopy(TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        
        TPatriciaTrieItem<T>*   head;
        std::size_t             size;

    public:

        TPatriciaTrie();
        virtual ~TPatriciaTrie();
        
        TPatriciaTrieItem<T>*   LookupNode(std::string&);
        T*                      Lookup(std::string&);
        TPatriciaTrieItem<T>*   Insert(std::string&, T);
        bool                    Delete(std::string&);
        
        TArray<std::string*>    KeyList();
        std::size_t             Size();

        TPatriciaTrieItem<T>*   GetHead();
        void                    Print(TPatriciaTrieItem<T>*, int);
        void                    ClearTrie();
        bool                    Empty();
        void                    SwapHead(TPatriciaTrie<T>*);
        
        bool                    SaveTrieBefore(TPatriciaTrieItem<T>*, std::ofstream*);
        bool                    SaveTrieCurrent(TPatriciaTrieItem<T>*, std::ofstream*);
        
        void                    LoadTrieBefore(std::string&, std::ifstream*);
        void                    LoadTrieCurrent(std::string&, TPatriciaTrieItem<T>*, std::ifstream*);
};

#endif