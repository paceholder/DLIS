#pragma once

#include <iostream>


class ParsingContext
{
public:

  ParsingContext(std::ifstream & file)
    : _inRecord(false)
    , _inSegment(false)
  {
    std::size_t current = file.tellg();

    file.seekg(0, std::ios::end);

    std::size_t _fileLength = file.tellg();

    // set back
    file.seekg(current);
  }

  void setCurrentRecordRanges(std::ifstream & file,
                              std::size_t visibleRecordHeaderSize,
                              std::size_t visibleRecordLength)
  {
    std::size_t const b =
      (std::size_t)file.tellg() - visibleRecordHeaderSize;

    _inRecord           = true;
    _currentRecordBegin = b;
    _currentRecordEnd   = b + visibleRecordLength;
  }

  bool inRecord(std::ifstream & file) const
  {
    return (file.tellg() >= _currentRecordBegin &&
            file.tellg() < _currentRecordEnd);
  }

  void setCurrentSegmentRanges(std::ifstream & file,
                               std::size_t segmentHeaderSize,
                               std::size_t segmentLength)
  {
    std::size_t const b = (std::size_t)file.tellg() - segmentHeaderSize;

    _inSegment           = true;
    _currentSegmentBegin = b;
    _currentSegmentEnd   = b + segmentLength;
  }

  bool inSegment(std::ifstream & file) const
  {
    return (file.tellg() >= _currentSegmentBegin &&
            file.tellg() < _currentSegmentEnd);
  }

public:

  std::size_t fileLength() const {
    return _fileLength;
  }

private:

  bool _inRecord;

  std::size_t _currentRecordBegin;
  std::size_t _currentRecordEnd;


  bool _inSegment;

  std::size_t _currentSegmentBegin;
  std::size_t _currentSegmentEnd;

  std::size_t _fileLength;
};
