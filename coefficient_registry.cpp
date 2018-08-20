#include "coefficient_registry.hpp"
#include <cpptoml/cpptoml.h>
#include <log/log.hpp>

CoefficientRegistry &CoefficientRegistry::instance()
{
  static CoefficientRegistry inst;
  return inst;
}

std::string CoefficientRegistry::display() const
{
  if (currentIdx >= data.size())
    return "Not selected";
  return data[currentIdx]->display();
}

void CoefficientRegistry::rotate(bool isUp)
{
  if (data.empty())
    return;
  if (isUp)
    currentIdx = (currentIdx + 1) % data.size();
  else
    currentIdx = (currentIdx + data.size() - 1) % data.size();
}

bool CoefficientRegistry::onKeyDown(SDL_Keycode key)
{
  switch (key)
  {
    case SDLK_PAGEUP:
      rotate(true);
      LOG(display());
      return true;
    case SDLK_PAGEDOWN:
      rotate(false);
      LOG(display());
      return true;
    case SDLK_KP_PLUS:
      change(1);
      LOG(display());
      return true;
    case SDLK_KP_MINUS:
      change(-1);
      LOG(display());
      return true;
  }
  return false;
}

void CoefficientRegistry::change(int value)
{
  if (currentIdx >= data.size())
    return;
  data[currentIdx]->change(value);
  auto &&root = cpptoml::make_table();
  for (auto &&coeff : data)
    coeff->serialize(root.get());
  std::ofstream f("coeff.toml");
  f << *root;
}

void CoefficientRegistry::load()
{
  auto &&config = cpptoml::parse_file("coeff.toml");
  for (auto &&coeff : data)
    coeff->deserialize(config.get());
}
