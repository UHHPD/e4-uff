#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>


Data::Data(const std::string& filename) {
  std::ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

 for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_error.push_back(errors);
  }

  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& in, int n){
  int res = 0;
  double sigma = 0;
  for (int i = 0; i < m_data.size(); i++) {
    double a = m_data[i];
    double err_a = m_error[i];
    for (int j = 0; j < in.m_data.size(); j++) {
      double b = in.m_data[j];
      double err_b = in.m_error[j];
      double delta = abs(a - b);
      double sigma = sqrt(err_a * err_a + err_b * err_b);
      if (delta > n * sigma) {
        res++;
      }
    }
  }
  return res;
}
Data Data::operator+(const Data& A) {
  Data res;
  for (int i = 0; i < m_data.size(); i++) {
    double y1 = m_data[i];
    double y2 = A.m_data[i];
    double w1 = 1 / (m_error[i] * m_error[i]);
    double w2 = 1 / (A.m_error[i] * A.m_error[i]);

    double y = (w1 * y1 + w2 * y2) / (w1 + w2);
    double sigma = sqrt(1/(w1 + w2));

    res.m_data.push_back(y);
    res.m_error.push_back(sigma);
  }
  return res;
}