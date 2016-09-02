//
// Created by Saipraveen B on 25/08/16.
//
#include <iostream>
#include <vector>
#include <cstring>
#include <memory>

#ifndef CSD_ASSIGNMENT2_MEMORY_H
#define CSD_ASSIGNMENT2_MEMORY_H

template<typename T>
class Memory {
 public:
  Memory(const int size);

  int getSize() const;
  void setSize(const int size);

  T &block(const int idx);
  T getBlock(const int idx) const;
  void setBlock(const int idx, const T &new_value);

  void writeMem(const int idx, const void *loc, const int num_bytes);
  void readMem(const int idx, void *buffer, const int num_bytes) const;

 private:
  int size_;
  T *buffer_;
};

template<typename T>
int Memory<T>::getSize() const {
  return size_;
}

template<typename T>
void Memory<T>::setSize(const int size) {
  size_ = size;
}

template<typename T>
Memory<T>::Memory(const int size) {
  this->size_ = size;
  this->buffer_ = new T[size];
}

template<typename T>
T &Memory<T>::block(const int idx) {
  return this->buffer_[idx];
}

template<typename T>
T Memory<T>::getBlock(const int idx) const {
  return this->buffer_[idx];
}

template<typename T>
void Memory<T>::setBlock(const int idx, const T &new_value) {
  this->buffer_[idx] = new_value;
}

template<typename T>
void Memory<T>::writeMem(const int idx, const void *arr, const int num_bytes) {
  memcpy(buffer_ + idx, arr, num_bytes * sizeof(T));
}

template<typename T>
void Memory<T>::readMem(const int idx, void *buffer, const int num_bytes) const {
  memcpy(buffer, this->buffer_ + idx, num_bytes * sizeof(T));
}

#endif //CSD_ASSIGNMENT2_MEMORY_H
