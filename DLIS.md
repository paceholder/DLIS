File Structure
==============


    [__________________________DLIS file________________________]

    [Storage Unit Label][Storage Unit Lenght & Version][Records ]
                                                           |
                                    _______________________|
                                   |
    [____________________________Record_________________________]

    [Visible Record Header][              Segments              ]
                                              |
                                    __________|
                                   |
    [___________________________Segment_________________________]

    [Logical Record Segment Header][    Objects   ][   Trailer  ]


1. Storage Unit Label - 80 bytes
  - `char[4]` - Storage Unit Sequence Number
  - `char[5]` - DLIS Version
  - `char[6]` - Storage Unit Structure
  - `char[5]` - Maximum Record Length
  - `char[60]` - Storage Set Identifier

2. Storage Unit Length & Version:
  - `uint16_t` - length
  - `uint8_t` - 0xFF
  - `uint8_t` - major version

    1. Logical Record Segment

        1. Logical Record Segment Header
          - `uint16_t` - Logical Record Segment Length
          - `uint8_t` - Logical Record Segment Attributes
          - `uint8_t` - Logical Record Type


        2. Explicitly Formatted Logical Record (EFLR)
          - Set
            - Object1: [ Attribute1, Attribute2, ... Attribute N ]
            - Object1: [ Attribute1, Attribute2, ... Attribute N ]
            - ...
            - ObjectM: [ Attribute1, Attribute2, ... Attribute N ]


Components are used to describe structural properties
of Set, Objects, and Attributes.

Set Component is a first Component.
Object Component defines the beginning of the first Object.
Object Component: [Object Name]
Then sequence of Attribute Components that is terminated
by the end of the Logical Record or by another Object Component.


EFLR Example
------------

~~~~~~

Logical Record Segment Header
  Set Component
  Object Component
  Attribute Component
  Absent Attribute Component
  Invariant Attribute Component
Logical Record Segment Trailer


