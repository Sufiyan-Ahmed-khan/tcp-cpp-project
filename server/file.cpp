#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
void save(std::string iocom) {
  std::fstream fout;
  std::string line = iocom;
  fout.open("sliaple.txt", std::ios::app);
  std::time_t result = std::time(nullptr);
  fout << std::ctime(&result) << line << std::endl;
  fout.close();
}
