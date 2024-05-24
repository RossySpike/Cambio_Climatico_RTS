#include <random>
std::random_device rd;

int aleat(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  int num = dist(rd);
  return num;
}
