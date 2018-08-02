#pragma once

#include <fstream>
#include <type_traits>
#include <string>

#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <boost/endian/conversion.hpp>


namespace DLIS
{


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
  std::cout << "OMMM" << std::endl;
  file.read(reinterpret_cast<char*>(t), sizeof(T));
  convertEndian(t);
}


//template<typename ... T>
//void
//read(std::ifstream & file, T*...)
//{
  ////
//}



//std::string
//read(std::ifstream & file, RepresentationCode rc)
//{
  //switch(rc)
  //{
  //case RepresentationCode::OBNAME:

    //break;
  //default:
    //break;
  //}

//}


}
