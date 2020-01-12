
#ifndef _HEADER_FILE_json11_shim_20200112154745_
#define _HEADER_FILE_json11_shim_20200112154745_

#include <ostream>
#include "json11.hpp"

std::ostream &operator<<(std::ostream &stream, json11::Json &j);

#endif