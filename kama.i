%module kama
%include exception.i
%{
#include "kama.h"
#include <Python.h>
%}


%exception {
  try { $action }
  catch (char *e) { SWIG_exception (SWIG_RuntimeError, e); }
  catch (const char *e) { SWIG_exception (SWIG_RuntimeError, (char*)e); }
}

%include <std_vector.i>
%include <std_string.i>
%include <std_list.i>

%typemap(out) std::vector<Kama::Node>* %{
    $result = PyList_New($1->size()); // Create outer Python list of correct size
    for(size_t i = 0; i < $1->size(); ++i){
        PyObject* node = PyDict_New();
        PyDict_SetItem(node, PyString_FromString("nbest"), PyInt_FromLong((*$1)[i].nbest));
        PyDict_SetItem(node, PyString_FromString("id"),  PyInt_FromLong((*$1)[i].id)); 
        PyDict_SetItem(node, PyString_FromString("surface"),  PyString_FromString((*$1)[i].surface)); 
        PyDict_SetItem(node, PyString_FromString("feature"),  PyString_FromString((*$1)[i].feature)); 
        PyList_SetItem($result, i, node);
    }
%}

%typemap(in) std::list<const char*> %{
    if(PyList_Check($input)){
        std::list<const char*> strList;
        for(int i = 0; i < PyList_Size($input); i++){
           PyObject* pyItem = PyList_GetItem($input, i);
           PyObject* str = PyUnicode_AsEncodedString(pyItem, "utf-8", "strict");
           const char* tmpStr = PyBytes_AsString(str);
           strList.push_back(tmpStr);
        }
        $1 = strList;
    } else {
        PyErr_SetString(PyExc_TypeError,"Wrong argument type, list expected");
        return NULL;
    }
%}

%include version.h
%include kama.h


