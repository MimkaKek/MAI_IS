#include "TArray.hpp"

template <typename T>
void TArray<T>::ReallocArray() {
        T** newAlloc = new T*[this->s];
        if(newAlloc == nullptr) {
            std::cerr << "ERROR: new at TArray::reallocArray()!" << std::endl;
        }
        for(std::size_t i = 0; i < this->n; ++i) {
            newAlloc[i] = this->data[i];
        }
        delete[] this->data;
        this->data = newAlloc;
}

template <typename T>
TArray<T>::TArray() {
    this->data  = new T*[1];
    if(this->data == nullptr) {
        std::cerr << "ERROR: malloc at TArray::TArray()!" << std::endl;
    }
    this->n = 0;
    this->s = 1;
}

template <typename T>
TArray<T>::TArray(std::size_t size) {
    this->data  = new T*[size];
    if(this->data == nullptr) {
        std::cerr << "ERROR: malloc at TArray::TArray()!" << std::endl;
    }
    for(std::size_t i = 0; i < size; ++i) {
        this->data[i] = new T();
    }

    this->n = size;
    this->s = 1;
    for(;this->s < this->n; this->s *= 2);
}

template <typename T>
TArray<T>::TArray(const TArray& arr) {
    this->n = arr.n;
    this->s = arr.s;

    this->data = new T*[this->s];
    if(this->data == nullptr) {
        std::cerr << "ERROR: malloc at TArray::TArray(const TArray&)!" << std::endl;
    }
    for(std::size_t i = 0; i < this->n; ++i) {
        this->data[i] = new T();
        *this->data[i] = *arr.data[i];
    }
}

template <typename T>
TArray<T>::~TArray() {
    for(std::size_t i = 0; i < this->n; ++i) {
        delete this->data[i];
    }
    delete[] this->data;
}

template <typename T>
void TArray<T>::Clear() {
    for(std::size_t i = 0; i < this->n; ++i) {
        delete this->data[i];
    }
    delete[] this->data;

    this->data = new T*[1];
    if(this->data == nullptr) {
        std::cerr << "ERROR: malloc at TArray::Clear()!" << std::endl;
    }
    this->n = 0;
    this->s = 1;
    return;
}

template <typename T>
std::size_t TArray<T>::Size() {
    return this->n;
}

template <typename T>
void TArray<T>::Push(T item) {

    this->data[this->n]  = new T;
    if(this->data[this->n] == nullptr) {
        std::cerr << "ERROR: new at TArray::push()!" << std::endl;
    }
    *this->data[n] = item;

    ++(this->n);

    if (this->n >= (this->s / 2)) {
        this->s *= 2;
        this->ReallocArray();
    }

    return;
}

template <typename T>
T TArray<T>::Pop() {
    
    if (this->s == 1) {
        std::cerr << "ERROR: Array is empty, but pop() called!" << std::endl;
        return T();
    }

    T* ptrData = this->data[n - 1];
    T poppedData = *ptrData;

    delete this->data[n - 1];

    --(this->n);

    if (this->n < (this->s / 2)) {
        this->s /= 2;
        this->ReallocArray();
    }

    return poppedData;
}
template <typename T>
bool TArray<T>::IsEmpty() {
    return (this->n == 0) ? true : false;
}

template <typename T>
int TArray<T>::Remove(std::size_t pos) {

    if (this->s == 1) {
        std::cerr << "ERROR: Array is empty, but remove() called!" << std::endl;
        return 1;
    }

    if (pos >= this->n || pos < 0) {
        std::cerr << "ERROR: Certain position is of array!" << std::endl;
        return 2;
    }

    delete this->data[pos];

    for(std::size_t i = pos; i < this->n; ++i) {
        this->data[i] = this->data[i + 1];
    }

    --(this->n);

    if (this->n < (this->s / 2)) {
        this->s /= 2;
        this->ReallocArray();
    }

    return 0;
}

template <typename T>
void TArray<T>::Print() {
    for(std::size_t i = 0; i < this->n; ++i) {
        std::cout << *this->data[i] << std::endl;
    }
    return;
}

template <typename T>
T TArray<T>::Get(std::size_t i) {
    if (i >= this->n || i < 0) {
        return T();
    }

    return *this->data[i];
}

template class TArray<std::string>;
template class TArray<std::size_t>;
template class TArray<TArray<std::size_t>>;
template class TArray<std::string*>;

#include "TPatriciaTrieItem.hpp"
template class TArray<TPatriciaTrieItem<std::size_t>*>;
template class TArray<TPatriciaTrieItem<std::string>*>;
template class TArray<TPatriciaTrieItem<int>*>;

#include "TFileData.hpp"
template class TArray<TFileData>;
template class TArray<TFileData*>;
template class TArray<TPatriciaTrieItem<TFileData>*>;

#include "TTokenData.hpp"
template class TArray<TTokenData>;
template class TArray<TTokenData*>;
template class TArray<TPatriciaTrieItem<TTokenData>*>;