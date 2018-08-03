#pragma once

#include <iostream>

#include "Accessors.hpp"
#include "Descriptors.hpp"
#include "ParsingContext.hpp"
#include "Reading.hpp"

namespace DLIS
{

void
parse(std::string filename)
{
  std::ifstream file (filename,
                      std::ios::in | std::ios::binary);

  ParsingContext parsingContext(file);

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

  while (!file.eof())
  {
    // Visible record header
    {
      VisibleRecordHeader vrh;
      read(file, &vrh);
      auto const & headerAccessor = DLIS::accessor(vrh);
      std::cout << headerAccessor << std::endl;


      parsingContext.setCurrentRecordRanges(file,
                                            DLIS::tupleSize(vrh),
                                            headerAccessor.length());

      //file.ignore(headerAccessor.length() - DLIS::size(vrh) );
    }

    // Logical record Segment Header

    std::size_t l = 0;

    while(parsingContext.inRecord(file))
    {
      LogicalRecordSegmentHeader segHeader;
      read(file, &segHeader);

      auto const & segHeaderAccessor = DLIS::accessor(segHeader);
      std::cout << segHeaderAccessor << std::endl;

      parsingContext.setCurrentSegmentRanges(file,
                                             DLIS::tupleSize(segHeader),
                                             segHeaderAccessor.length());

      if (segHeaderAccessor.explicitlyFormatted())
      {
        while(parsingContext.inSegment(file))
        {
          ComponentDescriptor componentDescriptor;
          read(file, &componentDescriptor);

          auto const & compAccessor = DLIS::accessor(componentDescriptor);
          std::cout << compAccessor << std::endl;

          if (compAccessor.isSet())
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
            //{
              //ComponentDescriptor attr;
              //read(file, &attr);

              //auto const & a = DLIS::accessor(attr);
              //std::cout << a << std::endl;

            //}
          }
          else if (compAccessor.isAttribute())
          {
            if (componentDescriptor & AttributeDescriptorBits::LABEL)
            {
              auto setType = read(file, RepresentationCode::IDENT);
              std::cout << "Label: " << setType << std::endl;
            }
            if (componentDescriptor & AttributeDescriptorBits::COUNT)
            {
//
            }
            if (componentDescriptor & AttributeDescriptorBits::REPRESENTATION_CODE)
            {
              auto repCode = read(file, RepresentationCode::USHORT);
              std::cout << "Representation Code: " << repCode << std::endl;
//
            }




            std::cout << "ATTRIBUTE" << std::endl;
          }
          else if (compAccessor.isObject())
          {
            std::cout << "OBJECT" << std::endl;
          }
        }
      }


      l += segHeaderAccessor.length();

      //file.ignore(a.length() - DLIS::tupleSize(segHeader) );
    }

    std::cout << "LENGTH: " << l << std::endl;


    break;

  }

  //else // Indirectly Formatted
  //{
  //// Next step is toread OBNAME in form:
  //// k&j&n'xxxx where
  //// k - origin reference
  //// j - copy number
  //// length of string

  ////ObnameHeader obnameHeader;
  ////read(file, &obnameHeader);

  ////auto const & ah = DLIS::accessor(obnameHeader);

  ////std::cout << ah.fiveSymbols() << std::endl;
  //}
}
}
