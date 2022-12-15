#ifndef NUMCPP_DIAGNOSTICS_H
#define NUMCPP_DIAGNOSTICS_H

#include <vector>
#include <string>
#include <functional>

#include <numcpp/defs.hpp>

namespace npp
{
	double run(const std::string &name, u64 iterations, std::function<void(u64)> exec);
}

#endif
