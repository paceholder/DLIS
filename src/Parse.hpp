#pragma once

#include <iostream>
#include <exception>

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
          read(file, &componentDescriptor.byte);

          auto const & compAccessor = DLIS::accessor(componentDescriptor);
          std::cout << compAccessor << std::endl;

          if (compAccessor.isSet())
          {
            SetDescriptor setDescriptor { componentDescriptor.byte };

            if (setDescriptor.byte & SetDescriptorBits::TYPE)
            {
              std::string setType = read<RepresentationCode::IDENT>(file);
              std::cout << "Set Type: " << setType << std::endl;
            }

            if (setDescriptor.byte & SetDescriptorBits::NAME)
            {
              std::string setName = read<RepresentationCode::IDENT>(file);
              std::cout << "Set Name: " << setName << std::endl;
            }
          }
          else if (compAccessor.isAttribute())
          {
            std::cout << "ATTRIBUTE" << std::endl;

            Attribute attribute;

            if (componentDescriptor.byte & AttributeDescriptorBits::LABEL)
            {
              auto label = read<RepresentationCode::IDENT>(file);
              std::cout << "Label: " << label << std::endl;

              attribute.label = label;
            }
            if (componentDescriptor.byte & AttributeDescriptorBits::COUNT)
            {
              unsigned int count = read<RepresentationCode::UVARI>(file);
              std::cout << "Count: " << count << std::endl;

              attribute.count = count;
            }
            if (componentDescriptor.byte & AttributeDescriptorBits::REPRESENTATION_CODE)
            {
              RepresentationCode repCode =
                (RepresentationCode)read<RepresentationCode::USHORT>(file);
              std::cout << "Representation Code: " << (unsigned int)repCode << std::endl;

              attribute.repCode = repCode;
            }
            if (componentDescriptor.byte & AttributeDescriptorBits::UNITS)
            {
              std::string units =
                read<std::string, RepresentationCode::UNITS>(file);
              std::cout << "Units: " << units << std::endl;

              attribute.units = units;
            }
            if (componentDescriptor.byte & AttributeDescriptorBits::VALUE)
            {

              for(unsigned int i = 0; i < attribute.count; ++i)
              {
                if (attribute.repCode == RepresentationCode::IDENT)
                {
                  std::string value = read<RepresentationCode::IDENT>(file);
                  std::cout << "Value: " << value << std::endl;
                }
                else if (attribute.repCode == RepresentationCode::ASCII)
                {
                  std::string value = read<RepresentationCode::ASCII>(file);
                  std::cout << "Value: " << value << std::endl;


                }
                else
                {
                  std::cout << "REPCODE: " << (unsigned int)attribute.repCode << std::endl;
                  //throw std::logic_error("Value not implemented");
                }
              }
            }
          }
          else if (compAccessor.isObject())
          {
            std::cout << "OBJECT" << std::endl;

            if (componentDescriptor.byte & ObjecDescriptorBits::OBJECT_NAME)
            {
              Obname obname = read<Obname, RepresentationCode::OBNAME>(file);
              std::cout << "Object name: " << obname << std::endl;
            }

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
