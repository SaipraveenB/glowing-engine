//
// Created by Saipraveen B on 25/08/16.
//
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <memory>
#include <iomanip>
#include <bitset>

#ifndef CSD_ASSIGNMENT2_MEMORY_H
#define CSD_ASSIGNMENT2_MEMORY_H

template<typename T>
class Memory {
 public:
  Memory(const int size);

  // Convenience functions for size;
  int getSize() const;
  void setSize(const int size);

  // Access arbitrary blocks of code. Granularity depends on T.
  T &block(const int idx);
  // Gets a block of data.
  T getBlock(const int idx) const;
  // Sets a block of data.
  void setBlock(const int idx, const T &new_value);

  // Writes an arbitrary array into data.
  void writeMem(const int idx, const void *loc, const int num_bytes);
  // Reads memory into a generic buffer.
  void readMem(const int idx, void *buffer, const int num_bytes) const;

  // Writes BigEndian shorts.
  unsigned short readShort(const int idx);
  // Reads BigEndian shorts.
  void writeShort(const int idx, unsigned short value);

  // Writes memory to standard output.
  std::string dumpMem() const;

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

template<typename T>
std::string Memory<T>::dumpMem() const {
  std::stringstream ss;
  char *read_ptr = (char *) buffer_;
  int num_bytes = sizeof(T) * size_;
  int cur_addr = 0;
  while (cur_addr < num_bytes) {
    ss << "$" << std::setfill('0') << std::setw(4) << cur_addr << "$";
    ss << std::bitset<8>(*read_ptr).to_string() << "\n";
    read_ptr++;
    cur_addr++;
  }

  return ss.str();
}

template<typename T>
unsigned short Memory<T>::readShort(const int idx) {
  return static_cast<unsigned short>(this->buffer_[idx]) << 8 | static_cast<unsigned char>(this->buffer_[idx + 1]);
}

template<typename T>
void Memory<T>::writeShort(const int idx, unsigned short value) {
  this->buffer_[idx] = static_cast<char>(value >> 8);
  unsigned short val = value >> 8;
  this->buffer_[idx + 1] = static_cast<char>(value & 0xFF);
  //std::cout << std::bitset<16>(value >> 8).to_string();
}
#endif //CSD_ASSIGNMENT2_MEMORY_H
