//
// Created by Saipraveen B on 25/08/16.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <deque>

#ifndef CSD_ASSIGNMENT2_REGISTERFILE_H
#define CSD_ASSIGNMENT2_REGISTERFILE_H

/*
 * Standard Register file.
 * Can be any width and have any number of registers.
 */

// TODO: Add selective volatility removal.

template<typename T>
class RegisterFile {
 public:

  // Standard Registers
  std::vector<T> registers;
  // Special Registers (PC, CRx etc)
  std::vector<T> spls;

  // Init register file
  RegisterFile(int numRegs);

  // Returns data in register 's'
  T get(int s);

  // Sets data in register 's' to 'val'
  void set(int s, T val);

  // Index of PC in special registers.
  static const int REG_PC = 0;
  static const int REG_BSR = 1;

  // Returns a mutable reference to the i'th register.
  T &reg(int i);

  // Returns a mutable reference to the i'th special register.
  T &spl(int i);

  // returns null is data was obtained
  // returns a marker if data was blocked.
  void *try_get(int s, T &data, void *marker);
  bool mark_volatile(int s, void *marker);

  void forward_operand(int s, void *marker, T operand);
  std::vector<std::deque<void *>> volatility;

  bool clear_volatility();
  void remove_volatility(void *marker);
  std::map<std::pair<int, void *>, std::pair<T, int>> operand_table;
  void tick();
};

template<typename T>
T RegisterFile<T>::get(int s) {
  return registers[s];
}

template<typename T>
void *RegisterFile<T>::try_get(int s, T &data, void *marker) {

  if (operand_table.count(std::make_pair(s, marker))) {
    data = operand_table[std::make_pair(s, marker)].first;
    return nullptr;
  }

  if (marker == nullptr) {

    // First time, check for volatility.
    if (!volatility[s].size()) {
      data = registers[s];
      return nullptr;
    } else {

      auto k = volatility[s].back();
      // Check if it's already in the forwarding table.
      if (operand_table.count(std::make_pair(s, k))) {
        data = operand_table[std::make_pair(s, k)].first;
        return nullptr;
      } else
        return k;

    }

  } else {

    // Already in queue. Check if marker has finished.
    if (find(volatility[s].begin(), volatility[s].end(), marker) == volatility[s].end()) {
      // Marker finished execution.
      data = registers[s];
      return nullptr;
    } else
      // Marker not yet finished.
      return marker;

  }

}

template<typename T>
bool RegisterFile<T>::mark_volatile(int s, void *marker) {
  volatility[s].push_back(marker);
}

template<typename T>
bool RegisterFile<T>::clear_volatility() {

  for (int i = 0; i < volatility.size(); i++)
    volatility[i].clear();

}
template<typename T>
void RegisterFile<T>::set(int s, T val) {
  if (volatility[s].size())
    volatility[s].pop_front();
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
  for (int i = 0; i < numRegs; i++) {
    registers.push_back(0);
    volatility.push_back(std::deque<void *>());
  }
  for (int i = 0; i < 4; i++)
    spls.push_back(0);

}
template<typename T>
void RegisterFile<T>::remove_volatility(void *marker) {

  for (int i = 0; i < registers.size(); i++) {
    // Remove marker from all register volatility queues.
    auto vol = find(volatility[i].begin(), volatility[i].end(), marker);
    if (vol != volatility[i].end())
      volatility[i].erase(vol);

  }

}
template<typename T>
void RegisterFile<T>::forward_operand(int s, void *marker, T operand) {
  // Add (value,age) to (s,marker).
  operand_table[std::make_pair(s, marker)] = std::make_pair(operand, 0);
}

template<typename T>
// Flush operand table after X ticks.
void RegisterFile<T>::tick() {
  std::vector<std::pair<int, void *>> lst;
  for (auto p : operand_table) {

    int times = p.second.second;
    if (times > 2)
      // Drop the operand.
      lst.push_back(p.first);
    else {
      // Increase age.
      T k = p.second.first;
      operand_table[p.first] = std::make_pair(k, p.second.second + 1);
    }

  }
  for (auto k : lst) {
    operand_table.erase(k);
  }

}

#endif //CSD_ASSIGNMENT2_REGISTERFILE_H
