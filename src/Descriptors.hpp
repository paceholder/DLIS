#pragma once

#include <tuple>
#include <cstdint> // fixed size int types

#include <boost/utility.hpp>

namespace DLIS
{

/**
 * - Storage Unit Sequence Number
 * - DLIS Version
 * - Storage Unit Structure
 * - Maximum Record Length
 * - Storage Set Identifier
 */
using StorageUnitLabel = std::tuple<char[4], char[5], char[6], char[5], char[60]>;

/**
 * Logical Records consist of Logical Record Segments
 * Logical Record Segments consist of four mutually disjoint parts:
 * - Logical Record Segment Header
 * - Logical Record Segment Encryption Packet
 * - Logical Record Segment Body
 * - Logical Record Segment Trailer (LRST)
 */

/**
 * - Length - unsigned int 2 bytes
 * - Format Version Field - unsigned int 2 bytes;
 *   - Byte FF
 *   - Major version in uint8_t
 */
using VisibleRecordHeader = std::tuple<uint16_t, char, uint8_t>;

/**
 * - Logical Record Segment Length
 * - Logical Record Segment Attributes
 * - Logical Record Type
 */
using LogicalRecordSegmentHeader = std::tuple<uint16_t, uint8_t, uint8_t>;


enum LogicalRecordSegmentHeaderAttributeBits : uint8_t
{
  LOGICAL_RECORD_STRUCTURE = 7, // 0 - indirectly formatted logical record, 1 - explicitly
  PREDECESSOR              = 6, // 0 - first segment, 1 - not first segment
  SUCCESSOR                = 5, // 0 - last segment, 1 - not last segment
  ENCRYPTION               = 4, // 0 - no ecryption, 1 - encryption
  ENCRYPTION_PACKET        = 3,
  CHECKSUM                 = 2, // 0 - no checksum, 1 - checksum in LRST
  TRAILING_LENGTH          = 1, // 0 - no trailing length, 1 - copy of LRS length in LRST
  PADDING                  = 0  // 0 - no record paddign, 1 - pad bytes are in LRST
};

enum class IndirectlyFormattedLogicalRecordType : uint8_t
{
  FRAME_DATA       = 0,
  UNFORMATTED_DATA = 1,
  // 2-126 - reserverd, undefined
  END_OF_DATA      = 127
};

enum class ExplicitlyFormattedLogicalRecordType : uint8_t
{
  FILE_HEADER                 = 0,
  ORIGIN                      = 1, // origin or well-reference
  AXIS                        = 2,
  CHANNEL                     = 3,
  FRAME                       = 4, // frame or path
  STATIC_DATA                 = 5, // calibration coef, computation, equipment, group, param, tool...
  TEXTUAL_DATA                = 6, // comment
  UPDATE_DATA                 = 7,
  UNFORMATTED_DATA_IDENTIFIER = 8,
  LONG_NAME                   = 9,
  SPECIFICATION               = 10, // attribute, code, eflr, iflr, object-type, specification
  DICTIONARY                  = 11  // base dictionary, idenfier, lexicon, option
                                    // 12 - 127 reserverd, undefined
};

//using LogicalRecordSegmentEncryptionPacket = std::tuple<uint16_t, uint16_t, ....>

struct ComponentDescriptor { uint8_t byte; };

// First three bits in ComponentDescriptor
enum ComponentDescriptorRoleBits : uint8_t
{
  ABSATR   = BOOST_BINARY(000 00000),
  ATTRIB   = BOOST_BINARY(001 00000),
  INVATR   = BOOST_BINARY(010 00000),
  OBJECT   = BOOST_BINARY(011 00000),
  RESERVED = BOOST_BINARY(100 00000),
  RDSET    = BOOST_BINARY(101 00000),
  RSET     = BOOST_BINARY(110 00000),
  SET      = BOOST_BINARY(111 00000)
};

struct SetDescriptor { uint8_t byte; };

enum SetDescriptorBits : uint8_t
{
  TYPE = BOOST_BINARY(0001 0000),
  NAME = BOOST_BINARY(0000 1000)
};

enum AttributeDescriptorBits : uint8_t
{
  LABEL               = BOOST_BINARY(000 10000),
  COUNT               = BOOST_BINARY(000 01000),
  REPRESENTATION_CODE = BOOST_BINARY(000 00100),
  UNITS               = BOOST_BINARY(000 00010),
  VALUE               = BOOST_BINARY(000 00001)
};


struct ObjectDescriptor { uint8_t byte; };

enum ObjecDescriptorBits : uint8_t
{
  OBJECT_NAME = BOOST_BINARY(000 10000)
};

struct Set
{
  std::string type;
  std::string name;
};


enum class RepresentationCode : uint8_t
{
  FSHORT = 1,
  FSINGL = 2,
  FSING1 = 3,
  FSING2 = 4,
  ISINGL = 5,
  VSINGL = 6,
  FDOUBL = 7,
  FDOUB1 = 8,
  FDOUB2 = 9,
  CSINGL = 10,
  CDOUBL = 11,
  SSHORT = 12,
  SNORM  = 13,
  SLONG  = 14,
  USHORT = 15,
  UNORM  = 16,
  ULONG  = 17,
  UVARI  = 18,
  IDENT  = 19,
  ASCII  = 20,
  DTIME  = 21,
  ORIGIN = 22,
  OBNAME = 23,
  OBJREF = 24,
  ATTREF = 25,
  STATUS = 26,
  UNITS  = 27
};

template<RepresentationCode rc>
struct RCodeTraits;


template<>
struct RCodeTraits<RepresentationCode::ASCII>
{
  using ClientType = std::string;
};

template<>
struct RCodeTraits<RepresentationCode::IDENT>
{
  using ClientType = std::string;
};

template<>
struct RCodeTraits<RepresentationCode::UVARI>
{
  using ClientType = unsigned int;
};

template<>
struct RCodeTraits<RepresentationCode::ORIGIN>
{
  using ClientType = unsigned int;
};

template<>
struct RCodeTraits<RepresentationCode::USHORT>
{
  using ClientType = unsigned int;
};

template <RepresentationCode rc>
using RCodeTraits_t =
  typename RCodeTraits<rc>::ClientType;







enum UvariBits : uint8_t
{
  ONE_BYTE_NUMBER  = BOOST_BINARY(1 000 0000),
  TWO_BYTE_NUMBER  = BOOST_BINARY(10 00 0000),
  FOUR_BYTE_NUMBER = BOOST_BINARY(11 00 0000),

  TWO_BYTE_MASK    = BOOST_BINARY(11 00 0000)
};

struct Obname
{
  unsigned int origin;
  unsigned int version;
  std::string ident;
};


struct Attribute
{
  std::string label;

  unsigned int count = 1;

  RepresentationCode repCode = RepresentationCode::IDENT;
  //RepresentationCode repCode = RepresentationCode::UVARI;

  std::string units; // the same as IDENT
};


}
