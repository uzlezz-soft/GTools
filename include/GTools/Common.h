#ifndef GTOOLS_COMMON_H
#define GTOOLS_COMMON_H

#include <string>
#if __cplusplus >= 201703L
#include <string_view>
#endif

namespace GTools
{
#if __cplusplus >= 201703L
using ConstString = std::string_view;
#else
using ConstString = const std::string&;
#endif

using String = std::string;
}

#endif