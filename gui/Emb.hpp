// Header for Emb.cpp.
#ifndef EMB_HPP
#define EMB_HPP

#include <Python.h>

#include <functional>
#include <string>

namespace emb
{

typedef std::function<void(const std::string&)> stdout_write_type;
PyMODINIT_FUNC PyInit_emb();
void set_stdout(const stdout_write_type& write);


}


#endif // EMB_HPP
