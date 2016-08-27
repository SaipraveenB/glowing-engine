//
// Created by Saipraveen B on 25/08/16.
//
#include <iostream>
#include <vector>

#ifndef CSD_ASSIGNMENT2_MEMORY_H
#define CSD_ASSIGNMENT2_MEMORY_H


template<typename T>
class Memory {
public:
    Memory( int );

    int getSize() const;

    void setSize(int size);

    T& block( int i );

    T getBlock( int i );

    void setBlock( int i, T s );

    void writeMem( int i, void* , int n );

    void readMem( int i, void* buffer, int n );
private:
    T* buffer;
    int size;

};

template<typename T>
int Memory<T>::getSize() const {
    return size;
}

template<typename T>
void Memory<T>::setSize(int size) {
    Memory::size = size;
}

template<typename T>
Memory<T>::Memory( int size ) {
    this->size = size;
    this->buffer = (T *) new short[size];
}

template<typename T>
T& Memory<T>::block(int i) {
    return *(this->buffer + i);
}

template<typename T>
T Memory<T>::getBlock(int i) {
    return this->buffer[i];
}

template<typename T>
void Memory<T>::setBlock( int i, T s ){
    this->buffer[i] = s;
}
template<typename T>
void Memory<T>::writeMem( int i, void* arr, int n ){
    memcpy( buffer, arr, n * sizeof(T) );
}
template<typename T>
void Memory<T>::readMem( int i, void* buffer, int n ){
    memcpy( buffer, this->buffer, n * sizeof(T) );
}


#endif //CSD_ASSIGNMENT2_MEMORY_H
