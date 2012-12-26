#include "pynetwork.h"
#include "pyprotocols.h"

#include <boost/python.hpp>

#include "protocol_dummy.h"

using namespace danet;
using namespace boost;
using namespace boost::python;

BOOST_PYTHON_MODULE(libdanet)
{
  class_<danet::pynetwork, noncopyable>
      ("network", init<std::shared_ptr<protocol>>())
      .def("listen", &pynetwork::listen)
      .def("connect", &pynetwork::connect)
      .def("close", &pynetwork::close)
      .def("send", &pynetwork::send)
      .def("recive", &pynetwork::receive)
      .def("id", &pynetwork::id)
      .def("users", &pynetwork::users)
      .def("address", &pynetwork::address);

  scope protocols = class_<pyprotocols>
      ("protocols")
      .def("create_dummy", &pyprotocols::create_dummy);
  //class_<danet::protocols::dummy, bases<danet::protocol>>
  //    ("protocols_dummy");
  //def("")
}
