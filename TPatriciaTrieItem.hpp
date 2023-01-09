#ifndef T_PATRICIA_TRIE_HPP
#define T_PATRICIA_TRIE_HPP

#include <string>

template <class T> class TPatriciaTrie;

template <class T>
class TPatriciaTrieItem {
    private:
        
        friend class TPatriciaTrie<T>;
        
        T                       data;
        std::string             key;
        int                     index;
        TPatriciaTrieItem<T>*   left;
        TPatriciaTrieItem<T>*   right;

    public:

        TPatriciaTrieItem();
        TPatriciaTrieItem(std::string&, T&, int, TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        virtual ~TPatriciaTrieItem();

        T* GetData();

        void Initialize(std::string, T, int, TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
};

#endif