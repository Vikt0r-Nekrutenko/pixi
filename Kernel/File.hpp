#ifndef FILE_HPP
#define FILE_HPP

#include "vector.hpp"
#include <vector>
#include <string>
#include <cstdio>

namespace pixi { namespace files {

using byte  = unsigned char;

constexpr math::dword FIELD_SIZE       = 256u;
constexpr math::dword SIGNATURE_SIZE   = 16u;

class KERNEL_EXPORT file
{
public:
    file(const std::string &fileName);
    ~file();

    std::string field(const size_t &indx, const size_t &fieldNumber) const;
    math::vector<> signature(const size_t &indx) const;
private:
    FILE *m_dataFile = nullptr;
};

}}

#endif // FILE_HPP
