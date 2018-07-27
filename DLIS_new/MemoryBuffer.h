#pragma once

// буфер реаллокации данных
struct MemoryBuffer
{
    char *data;
    std::size_t size;
    std::size_t max_size;

    bool Resize(std::size_t new_len);
    void Free();
};
