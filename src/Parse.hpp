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

  for(unsigned int i = 0; i < 100; ++i)
  {
    VisibleRecordHeader vrh;
    read(file, &vrh);

    auto const & headerAccessor = DLIS::accessor(vrh);

    if (headerAccessor.length() == 0)
      return;

    std::cout << headerAccessor << std::endl;


    //lav.lengh();

    file.ignore(headerAccessor.length() - DLIS::size(vrh) );
  }



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
      SetDescriptor setDescriptor = componentDescriptor;

      if (setDescriptor & SetDescriptorBits::TYPE)
      {
        auto setType = read(file, RepresentationCode::IDENT);
        std::cout << "Set Type: " << setType << std::endl;
      }

      if (setDescriptor & SetDescriptorBits::NAME)
      {
        auto setName = read(file, RepresentationCode::IDENT);
        std::cout << "Set Name: " << setName << std::endl;
      }


      // read attributes
      {
        ComponentDescriptor attr;
        read(file, &attr);

        auto const & a = DLIS::accessor(attr);
        std::cout << a << std::endl;

      }
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
