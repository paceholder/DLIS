#pragma once

#include <iostream>

class ParsingContext
{
public:

  ParsingContext(std::ifstream & file)
    : _inRecord(false)
  {
    std::size_t current = file.tellg();

    file.seekg(0, std::ios::end);

    std::size_t _fileLength = file.tellg();

    // set back
    file.seekg(current);
  }

  void setCurrentRecordRanges(std::size_t b, std::size_t e)
  {
    _inRecord = true;
    _currentRecordBegin = b;
    _currentRecordEnd   = e;
  }

  bool inRecord(std::ifstream & file)
  {
    return (file.tellg() >= _currentRecordBegin &&
            file.tellg() < _currentRecordEnd);
  }

public:

  std::size_t fileLength() const { return _fileLength; }

private:

  bool _inRecord;

  std::size_t _currentRecordBegin;
  std::size_t _currentRecordEnd;

  std::size_t _fileLength;
};
