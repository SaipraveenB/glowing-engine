//
// Created by Saipraveen B on 25/08/16.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>

#ifndef CSD_ASSIGNMENT2_REGISTERFILE_H
#define CSD_ASSIGNMENT2_REGISTERFILE_H

/*
 * Standard Register file.
 * Can be any width and have any number of registers.
 */

template<typename T>
class RegisterFile {
 public:
  std::vector<T> registers;
  std::vector<T> spls;
  RegisterFile(int numRegs);
  T get(int s);
  void set(int s, T val);

  static const int REG_PC = 0;

  T &reg(int i);
  T &spl(int i);
};

template<typename T>
T RegisterFile<T>::get(int s) {
  return registers[s];
}

template<typename T>
void RegisterFile<T>::set(int s, T val) {
  registers[s] = val;
}

template<typename T>
T &RegisterFile<T>::spl(int i) {
  return spls[i];
}

template<typename T>
T &RegisterFile<T>::reg(int i) {
  return registers[i];
}

template<typename T>
RegisterFile<T>::RegisterFile(int numRegs) {
  // Initialize register file for simplicity.
  for (int i = 0; i < numRegs; i++)
    registers.push_back(0);
  for (int i = 0; i < 4; i++)
    spls.push_back(0);

}

#endif //CSD_ASSIGNMENT2_REGISTERFILE_H
