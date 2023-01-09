#include "TPatriciaTrieItem.hpp"
#include <cstring>
#include <string>
#include <cstdlib>

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::TPatriciaTrieItem() {
    Initialize(std::string(), T(), -1, this, this);
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::TPatriciaTrieItem(std::string& nKey, T& nData, int nIndex, TPatriciaTrieItem<T>* nLeft, TPatriciaTrieItem<T>* nRight) {
    Initialize(nKey, nData, nIndex, nLeft, nRight);
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrieItem<T>::Initialize(std::string nKey, T nData, int nIndex, TPatriciaTrieItem<T>* nLeft, TPatriciaTrieItem<T>* nRight) {
    key       = nKey;
    data      = nData;
    index     = nIndex;
    left      = nLeft;
    right     = nRight;
}

template <class T>
T* TPatriciaTrieItem<T>::GetData() {
    return &this->data;
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::~TPatriciaTrieItem() {}

template class TPatriciaTrieItem<std::size_t>;
template class TPatriciaTrieItem<std::string>;
template class TPatriciaTrieItem<int>;

#include "TFileData.hpp"
template class TPatriciaTrieItem<TFileData>;

#include "TTokenData.hpp"
template class TPatriciaTrieItem<TTokenData>;