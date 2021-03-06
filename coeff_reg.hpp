#pragma once
#include "coeff.hpp"
#include <SDL.h>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class BaseCoeff;
class CoeffReg
{
public:
  static CoeffReg &instance();

  template <typename T>
  void add(T &value, const char *name)
  {
    if (names.find(name) != std::end(names))
      throw std::runtime_error(std::string("Duplicate coefficient name: ") + name);
    data.push_back(std::make_unique<Coeff<T>>(value, name));
    currentIdx = data.size() - 1;
  }

  std::string display() const;
  void change(int);
  bool load();
  void rotate(bool);
  bool onKeyDown(SDL_Keycode);

private:
  CoeffReg() = default;
  std::vector<std::unique_ptr<BaseCoeff>> data;
  std::unordered_set<std::string> names;
  size_t currentIdx = 0;
};

namespace Internal
{
  class Declare
  {
  public:
    template <typename T>
    Declare(T &value, const char *name)
    {
      CoeffReg::instance().add(value, name);
    }
  };
} // namespace Internal

#define COEFF(x, y)  \
  static auto x = y; \
  Internal::Declare COEFF##x(x, #x);
