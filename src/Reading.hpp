#pragma once

#include <fstream>
#include <type_traits>
#include <string>
#include <vector>

#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/endian/conversion.hpp>

#include "Descriptors.hpp"

namespace DLIS
{


template<typename ... T>
std::size_t
size(std::tuple<T ...> & tuple)
{
  std::size_t s = 0;

  auto sizeOfTuplePart =
    [&s](auto & part)
    {
      using Type = decltype(part);
      s += sizeof(Type);
    };

  boost::fusion::for_each(tuple, sizeOfTuplePart);

  return s;
}


template<typename T>
std::enable_if_t<std::is_arithmetic<T>::value>
convertEndian(T & t)
{
  boost::endian::big_to_native_inplace(t);
}


template<typename T>
std::enable_if_t<!std::is_arithmetic<T>::value>
convertEndian(T & t)
{}

template<typename ... T>
void
read(std::ifstream & file, std::tuple<T ...> * tuple)
{
  auto readTuplePart =
    [&file](auto & part)
    {
      using Type = decltype(part);
      file.read(reinterpret_cast<char *>(&part), sizeof(Type));

      convertEndian(part);
    };

  boost::fusion::for_each(*tuple, readTuplePart);
}


template<typename T>
void
read(std::ifstream & file, T * t)
{
  file.read(reinterpret_cast<char*>(t), sizeof(T));
  convertEndian(t);
}


std::string
read(std::ifstream & file, RepresentationCode rc)
{
  switch (rc)
  {
    case RepresentationCode::IDENT:
    {
      uint8_t length;
      read(file, &length);

      std::string s(length, '\0');

      file.read(&s[0], length);

      return s;
    }
    break;

    default:
      break;
  }
}


//template<typename ... T>
//void
//read(std::ifstream & file, T*...)
//{
////
//}
}
