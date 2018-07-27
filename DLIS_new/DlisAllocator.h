#pragma once

#include <cstddef> // std::size_t

class CDLISAllocator
{
public:

  CDLISAllocator();
  ~CDLISAllocator() = default;

private:

  struct PullChunk
  {
    char *data;
    std::size_t len;
    std::size_t max_size;
    PullChunk *next;
  };

  struct PullBase
  {
    std::size_t id;
    PullChunk *chunks;
    PullBase *next;
  };

public:

  std::size_t   PullCreate(std::size_t max_size);
  void     PullFree(unsigned int pull_id);
  void     PullRelease(PullBase *pull);
  void     PullFreeAll();

  char    *MemoryGet (std::size_t pull_id, std::size_t size);

private:

  std::size_t m_pull_id;

  PullBase *m_pulls;

private:
  char  *MemoryChunkGet(PullBase *pull, std::size_t size);
};
