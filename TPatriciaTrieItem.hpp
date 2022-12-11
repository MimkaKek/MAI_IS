#ifndef T_PATRICIA_TRIE_HPP
#define T_PATRICIA_TRIE_HPP

template <class T> class TPatriciaTrie;

template <class T>
class TPatriciaTrieItem {
    private:
        
        friend class TPatriciaTrie<T>;
        
        T                       data;
        char*                   key;
        int                     index;
        TPatriciaTrieItem<T>*   left;
        TPatriciaTrieItem<T>*   right;

    public:

        TPatriciaTrieItem();
        TPatriciaTrieItem(char*, T, int, TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        virtual ~TPatriciaTrieItem();

        void Initialize(char*, T, int, TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
};

#endif