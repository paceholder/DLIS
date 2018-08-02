#pragma once

#include <iostream>

#include "Reading.hpp"
#include "Descriptors.hpp"
#include "Accessors.hpp"

namespace DLIS
{


void
parse(std::string filename)
{
  std::ifstream file (filename,
                      std::ios::in | std::ios::binary);

  if (file.fail())
  {
    std::cout << "Failed to read the file." << std::endl;

    return;
  }

  auto print =
    [](auto & part)
    {
      std::string s(part, sizeof(part));
      std::cout << s << std::endl;
    };

  //-----------

  // DLIS header

  StorageUnitLabel storageUnitLabel;
  read(file, &storageUnitLabel);

  std::cout << DLIS::accessor(storageUnitLabel) << std::endl;

  // Visible record header

  VisibleRecordHeader lav;
  read(file, &lav);

  std::cout << DLIS::accessor(lav) << std::endl;

  // Logical record Segment Header


  LogicalRecordSegmentHeader segHeader;
  read(file, &segHeader);

  auto const & a = DLIS::accessor(segHeader);

  std::cout << a << std::endl;

  if (a.explicitlyFormatted())
  {
    ComponentDescriptor componentDescriptor;
    read(file, &componentDescriptor);

    auto const & a = DLIS::accessor(componentDescriptor);
    std::cout << a << std::endl;

    if (a.isSet())
    {
      uint8_t length;

      read(file, &length);

      std::cout << "LENGTH: " << (unsigned int)length << std::endl;

    }

  }
  else // Indirectly Formatted
  {
    // Next step is toread OBNAME in form:
    // k&j&n'xxxx where
    // k - origin reference
    // j - copy number
    // length of string

    //ObnameHeader obnameHeader;
    //read(file, &obnameHeader);

    //auto const & ah = DLIS::accessor(obnameHeader);

    //std::cout << ah.fiveSymbols() << std::endl;
  }



}


}
