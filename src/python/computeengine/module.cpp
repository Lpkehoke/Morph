#include "computeengine/computeengine.h"

#include <boost/python.hpp>

#include <exception>

using namespace boost::python;
using namespace computeengine;

BOOST_PYTHON_MODULE(_computeengine)
{

class_<ComputeEngine>("ComputeEngine", no_init);

}

