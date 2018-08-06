#pragma once

#include "Descriptors.hpp"

#include <bitset>
#include <iomanip>


namespace DLIS
{

template<typename T>
struct Accessor;


template<typename T>
Accessor<T>
accessor(T & t)
{
  return Accessor<T>(t);
}



//----------------------

template<>
struct Accessor<StorageUnitLabel>
{
  using Type = StorageUnitLabel;

  explicit Accessor(StorageUnitLabel const & sul)
    : storageUnitLabel(sul)
  {
  }

  std::string
  storageUnitSequenceNumber() const
  {
    auto size = sizeof(std::tuple_element<0, Type>::type);
    return std::string(std::get<0>(storageUnitLabel), size);
  }

  std::string
  dlisVersion() const
  {
    auto size = sizeof(std::tuple_element<1, Type>::type);
    return std::string(std::get<1>(storageUnitLabel), size);
  }

  std::string
  storageUnitStructure() const
  {
    auto size = sizeof(std::tuple_element<2, Type>::type);
    return std::string(std::get<2>(storageUnitLabel), size);
  }

  std::string
  maxRecordLength() const
  {
    auto size = sizeof(std::tuple_element<3, Type>::type);
    return std::string(std::get<3>(storageUnitLabel), size);
  }

  std::string
  storageSetIdentifier() const
  {
    auto size = sizeof(std::tuple_element<4, Type>::type);
    return std::string(std::get<4>(storageUnitLabel), size);
  }

  friend
  std::ostream &
  operator<<(std::ostream & stream, Accessor<StorageUnitLabel> const & a)
  {
    stream << "Storage Unit Label:" << "\n";
    stream << '\t' << a.storageUnitSequenceNumber() << "\n";
    stream << '\t' << a.dlisVersion() << "\n";
    stream << '\t' << a.storageUnitStructure() << "\n";
    stream << '\t' << a.maxRecordLength() << "\n";
    stream << '\t' << "'" << a.storageSetIdentifier() << "'" << "\n";

    return stream;
  }

  StorageUnitLabel const & storageUnitLabel;
};


template<>
struct Accessor<VisibleRecordHeader>
{
  using Type = VisibleRecordHeader;

  explicit Accessor(Type const & vrh)
    : visibleRecordHeader(vrh)
  {
  }

  unsigned int
  length() const
  {
    return std::get<0>(visibleRecordHeader);
  }

  unsigned int
  majorVersion() const
  {
    return std::get<2>(visibleRecordHeader);
  }


  friend
  std::ostream &
  operator<<(std::ostream & stream, Accessor<Type> const & a)
  {
    auto leftC = [](std::ostream & stream) -> std::ostream&
                 {
                   stream << '\t' << std::left << std::setw(20);
                   return stream;
                 };

    auto rightC = [](std::ostream & stream) -> std::ostream&
                  {
                    stream << '\t' << std::right << std::setw(10);
                    return stream;
                  };


    stream << "Visible Record Header:" << '\n';

    leftC(stream) << "Record Length:";
    rightC(stream) << a.length() << '\n';

    leftC(stream) << "Major Version:";
    rightC(stream) << a.majorVersion() << '\n';

    return stream;
  }


  Type const & visibleRecordHeader;
};


template<>
struct Accessor<LogicalRecordSegmentHeader>
{
  using Type = LogicalRecordSegmentHeader;

  explicit Accessor(Type const & lrsh)
    : lrshdr(lrsh)
  {
  }

  unsigned int
  length() const
  {
    return std::get<0>(lrshdr);
  }

  std::bitset<8>
  attributes() const
  {
    return std::bitset<8>(std::get<1>(lrshdr));
  }


  std::bitset<8>
  recordType() const
  {
    return std::bitset<8>(std::get<2>(lrshdr));
  }

  //

  bool
  explicitlyFormatted() const
  {
    return attributes().test(LogicalRecordSegmentHeaderAttributeBits::LOGICAL_RECORD_STRUCTURE);
  }

  bool
  firstSegment() const
  {
    return !attributes().test(LogicalRecordSegmentHeaderAttributeBits::PREDECESSOR);
  }

  bool
  lastSegment() const
  {
    return !attributes().test(LogicalRecordSegmentHeaderAttributeBits::SUCCESSOR);
  }

  bool
  hasEncryption() const
  {
    return attributes().test(LogicalRecordSegmentHeaderAttributeBits::ENCRYPTION);
  }

  bool
  hasChecksum() const
  {
    return attributes().test(LogicalRecordSegmentHeaderAttributeBits::CHECKSUM);
  }

  bool
  hasTrailingLength() const
  {
    return attributes().test(LogicalRecordSegmentHeaderAttributeBits::TRAILING_LENGTH);
  }

  bool
  hasPadding() const
  {
    return attributes().test(LogicalRecordSegmentHeaderAttributeBits::PADDING);
  }

  friend
  std::ostream &
  operator<<(std::ostream & stream, Accessor<Type> const & a)
  {
    stream << "Logical Record Segment Header:" << "\n";
    stream << std::boolalpha;

    auto leftC = [](std::ostream & stream) -> std::ostream&
                 {
                   stream << '\t' << std::left << std::setw(20);
                   return stream;
                 };

    auto rightC = [](std::ostream & stream) -> std::ostream&
                  {
                    stream << '\t' << std::right << std::setw(10);
                    return stream;
                  };

    leftC(stream) << "Length:";
    rightC(stream) << a.length() << "\n";

    leftC(stream) << "Attributes:";
    rightC(stream) << a.attributes() << "\n";

    leftC(stream) << "Type:";
    rightC(stream) << a.recordType() << "\n";

    stream << std::endl;

    leftC(stream) << "Explicitly Formatted:";
    rightC(stream) << a.explicitlyFormatted() << "\n";

    leftC(stream) << "First Segment:";
    rightC(stream) << a.firstSegment() << "\n";

    leftC(stream) << "Last Segment:";
    rightC(stream) << a.lastSegment() << "\n";

    leftC(stream) << "Has Encryption:";
    rightC(stream) << a.hasEncryption() << "\n";

    leftC(stream) << "Has Checksum:";
    rightC(stream) << a.hasChecksum() << "\n";

    leftC(stream) << "Has Trailing Length:";
    rightC(stream) << a.hasTrailingLength() << "\n";

    leftC(stream) << "Has Padding:";
    rightC(stream) << a.hasPadding() << "\n";

    return stream;
  }


  Type const & lrshdr;
};


template<>
struct Accessor<ComponentDescriptor>
{
  using Type = uint8_t;

  explicit Accessor(ComponentDescriptor const & t)
    : bitset(t.byte)
    , type(t.byte & BOOST_BINARY(111 00000))
    , characts(t.byte & BOOST_BINARY(0001 1111))
  {
  }


  bool isAbsentAttribute() const
  {
    return ((type ^ ComponentDescriptorRoleBits::ABSATR) == 0);
  }


  bool isSet() const
  {
    return ((type ^ ComponentDescriptorRoleBits::SET) == 0);
  }

  bool isAttribute() const
  {
    return ((type ^ ComponentDescriptorRoleBits::ATTRIB) == 0);
  }

  bool isInvariantAttribute() const
  {
    return ((type ^ ComponentDescriptorRoleBits::INVATR) == 0);
  }

  bool isObject() const
  {
    return ((type ^ ComponentDescriptorRoleBits::OBJECT) == 0);
  }

  bool isRedundantSet() const
  {
    return ((type ^ ComponentDescriptorRoleBits::RDSET) == 0);
  }

  bool isReplacementSet() const
  {
    return ((type ^ ComponentDescriptorRoleBits::RSET) == 0);
  }



  friend
  std::ostream &
  operator<<(std::ostream & stream,
             Accessor<ComponentDescriptor> const & a)
  {
    stream << "Component Descriptor:" << std::bitset<8>(a.bitset) << "\n";

    auto leftC = [](std::ostream & stream) -> std::ostream&
                 {
                   stream << '\t' << std::left << std::setw(20);
                   return stream;
                 };

    auto rightC = [](std::ostream & stream) -> std::ostream&
                  {
                    stream << '\t' << std::right << std::setw(10);
                    return stream;
                  };

    stream << std::boolalpha;

    leftC(stream) << "Absent Attribute:";
    rightC(stream) << a.isAbsentAttribute() << "\n";

    leftC(stream) << "Attribute:";
    rightC(stream) << a.isAttribute() << "\n";

    leftC(stream) << "Invariant Attribute:";
    rightC(stream) << a.isInvariantAttribute() << "\n";

    leftC(stream) << "Object:";
    rightC(stream) << a.isObject() << "\n";

    leftC(stream)<< "Redundant Set:";
    rightC(stream) << a.isRedundantSet() << "\n";

    leftC(stream) << "Replacement Set:";
    rightC(stream) << a.isReplacementSet() << "\n";

    leftC(stream) << "Set:";
    rightC(stream) << a.isSet() << "\n";

    return stream;
  }


  Type const & bitset;
  Type const type;
  Type const characts;
};


std::ostream &
operator<<(std::ostream & o, Obname const &obname)
{
  o << obname.origin << "'"
    << obname.version << "'"
    << obname.ident << '\n';
  return o;
}


}
