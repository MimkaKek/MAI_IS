#ifndef T_TARRAY_HPP
#define T_TARRAY_HPP

#include <iostream>
#include <cstdlib>

template <typename T>
class TArray {
    private:

        T** data;
        std::size_t n;
        std::size_t s;

        void ReallocArray();

    public:

        TArray();
        TArray(const TArray&);
        ~TArray();

        void            Clear();

        bool            IsEmpty();

        std::size_t     Size();
        void            Push(T);
        T               Pop();
        int             Remove(std::size_t);

        void            Print();

        T               Get(std::size_t);

                T&  operator[] (std::size_t i)       { 
            return *this->data[i]; 
        }

        const   T&  operator[] (std::size_t i) const { 
            return *this->data[i]; 
        }

        TArray&     operator= (const TArray& arr)    {
            
            for(std::size_t i = 0; i < this->n; ++i) {
                delete this->data[i];
            }
            delete[] this->data;

            this->n = arr.n;
            this->s = arr.s;

            this->data = new T*[this->s];
            if(this->data == nullptr) {
                std::cerr << "ERROR: malloc at TArray::TArray(const TArray&)!" << std::endl;
            }
            for(std::size_t i = 0; i < this->n; ++i) {
                this->data[i] = new T;
                *this->data[i] = *arr.data[i];
            }

            return *this;
        }
};

#endif