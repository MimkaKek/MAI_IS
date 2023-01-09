#ifndef T_PATRICIA_TRIE_ITEM_HPP
#define T_PATRICIA_TRIE_ITEM_HPP

#include "TPatriciaTrieItem.hpp"
#include "TArray.hpp"
#include <string>
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
        int                     BitGet(const char*, int) const;
        int                     BitFirstDifferent(const char*, const char*) const;
        bool                    KeyCompare(const char*, const char*) const;
        void                    KeyCopy(TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        
        TPatriciaTrieItem<T>*   head;
        std::size_t             size;

    public:

        TPatriciaTrie();
        TPatriciaTrie(TPatriciaTrie<T>&);

        virtual ~TPatriciaTrie();
        
        TPatriciaTrieItem<T>*   LookupNode(std::string&) const;
        T*                      Lookup(std::string&) const;
        TPatriciaTrieItem<T>*   Insert(std::string&, T&);
        bool                    Delete(std::string&);
        
        TArray<std::string*>    KeyList() const;
        std::size_t             Size() const;

        TPatriciaTrieItem<T>*   GetHead() const;
        void                    Print(TPatriciaTrieItem<T>*, int) const;
        void                    Print() const;
        void                    ClearTrie();
        bool                    Empty() const;
        void                    SwapHead(TPatriciaTrie<T>*);
        
        bool                    SaveTrieBefore(TPatriciaTrieItem<T>*, std::ofstream*) const;
        bool                    SaveTrieCurrent(TPatriciaTrieItem<T>*, std::ofstream*) const;
        
        void                    LoadTrieBefore(std::string&, std::ifstream*);
        void                    LoadTrieCurrent(std::string&, TPatriciaTrieItem<T>*, std::ifstream*);

        TArray<TPatriciaTrieItem<T>*> ItemList() const;

        TPatriciaTrie<T>& operator= (const TPatriciaTrie<T>& trie) {
            
            this->ClearTrie();

            TArray<TPatriciaTrieItem<T>*> items = trie.ItemList();
            
            for (std::size_t i = 0; i < items.Size(); ++i) {
                this->Insert(items[i]->key, items[i]->data);
            }

            return *this;
        };

                T&  operator[] (std::string key)       {
            return *this->Lookup(key);
        };
};

#endif