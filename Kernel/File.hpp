#ifndef FILE_HPP
#define FILE_HPP

#include "kernel_global.hpp"
#include <vector>
#include <string>
#include <cstdio>

namespace pixi { namespace files {

using byte  = unsigned char;
using word  = unsigned short;
using dword = unsigned long;
using qword = unsigned long long;

constexpr word FIELD_SIZE       = 256u;
constexpr word SIGNATURE_SIZE   = 16u;

class KERNEL_EXPORT file
{
public:
    file(const std::string &fileName);
    ~file();

    std::string field(const size_t &indx, const size_t &fieldNumber);
    std::vector<byte> signature(const size_t &indx);
private:
    FILE *m_dataFile;
};

}}

#endif // FILE_HPP
