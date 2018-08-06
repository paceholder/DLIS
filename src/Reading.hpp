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
tupleSize(std::tuple<T ...> & tuple)
{
  std::size_t s        = 0;

  auto sizeOfTuplePart =
    [&s](auto & part)
    {
      using Type = decltype(part);
      s         += sizeof(Type);
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
{
}

template<typename ... T>
void
read(std::ifstream & file,
     std::tuple<T ...> * tuple)
{
  auto readTuplePart =
    [&file](auto & part)
    {
      using Type = decltype(part);
      std::size_t s = sizeof(Type);
      file.read(reinterpret_cast<char *>(&part), s);

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


template<typename T, RepresentationCode RC>
T
read(std::ifstream & file);


template<RepresentationCode RC>
auto read(std::ifstream & file);


template<>
auto
read<RepresentationCode::UVARI>(std::ifstream & file)
{
  using Result = RCodeTraits_t<RepresentationCode::UVARI>;

  Result result{};

  //variable-length integer

  uint8_t firstByte;
  read(file, &firstByte);

  if ((firstByte & UvariBits::ONE_BYTE_NUMBER) == 0)
  {
    firstByte &= (~UvariBits::ONE_BYTE_NUMBER);

    result     = static_cast<Result>(firstByte);
  }
  else if (((firstByte & UvariBits::TWO_BYTE_MASK) ^
            UvariBits::TWO_BYTE_NUMBER) == 0)
  {
    firstByte &= (~UvariBits::TWO_BYTE_NUMBER);
    uint8_t two[2];

    two[0] = firstByte;

    // read one more byte
    file.read(reinterpret_cast<char*>(&two[1]), 1);

    uint16_t number = *reinterpret_cast<uint16_t*>(&two[0]);

    boost::endian::big_to_native_inplace(number);

    result = static_cast<Result>(number);
  }
  else if (((firstByte & UvariBits::TWO_BYTE_MASK) ^
            UvariBits::FOUR_BYTE_NUMBER) == 0)
  {
    firstByte &= (~UvariBits::TWO_BYTE_NUMBER);
    uint8_t four[4];

    four[0] = firstByte;

    // read anothre three bytes
    file.read(reinterpret_cast<char*>(&four[1]), 3);

    uint32_t number = *reinterpret_cast<uint32_t*>(&four[0]);

    boost::endian::big_to_native_inplace(number);

    result = static_cast<Result>(number);
  }

  return result;
}


template<>
auto
read<RepresentationCode::ORIGIN>
  (std::ifstream & file)
{
  return read<RepresentationCode::UVARI>(file);
}

template<>
auto
read<RepresentationCode::USHORT>
  (std::ifstream & file)
{
  using Result = RCodeTraits_t<RepresentationCode::USHORT>;

  uint8_t l;
  read(file, &l);

  return static_cast<Result>(l);
}


template<>
auto
read<RepresentationCode::ASCII>
  (std::ifstream & file)
{
  unsigned int const length =
    read<RepresentationCode::UVARI>(file);

  std::string s(length, '\0');

  file.read(reinterpret_cast<char*>(&s[0]), length);

  return s;
}


template<>
auto
read<RepresentationCode::IDENT>
  (std::ifstream & file)
{
  unsigned int const length =
    read<RepresentationCode::USHORT>(file);

  std::string s(length, '\0');

  file.read(reinterpret_cast<char*>(&s[0]), length);

  return s;
}

template<>
std::string
read<std::string, RepresentationCode::UNITS>
  (std::ifstream & file)
{
  return read<RepresentationCode::IDENT>(file);
}




template<>
Obname
read<Obname, RepresentationCode::OBNAME>(std::ifstream & file)
{
  Obname result =
  {
    read<RepresentationCode::ORIGIN>(file),
    read<RepresentationCode::USHORT>(file),
    read<RepresentationCode::IDENT>(file)
  };

  return result;
}

}
