%{
#include <mio/matrix4x4.h>
%}

%include <mio/matrix4x4.h>

%extend mio::Matrix4x4
{
	float __getitem__(PyObject *key)
	{
		if (!PySequence_Check(key))
			throw mio::TypeException("key is not of sequence type");
		if (PySequence_Length(key) < 2)			
			throw mio::TypeException("key has fewer than 2 components");		
		
		PyObject *ii = PySequence_GetItem(key, 0);
		PyObject *jj = PySequence_GetItem(key, 1);
		
		if (!PyInt_Check(ii) || !PyInt_Check(jj))
		{
			Py_DECREF(ii);
			Py_DECREF(jj);
			throw mio::TypeException("one of the indices is not an integer");
		}
		
		long i = PyInt_AsLong(ii);
		long j = PyInt_AsLong(jj);
		
		Py_DECREF(ii);
		Py_DECREF(jj);
		
		if (i < 0 || i > 3 || j < 0 || j > 3)
			throw mio::IndexException("index to Matrix4x4 element out of range");
		else	
			return $self->_get(i,j);		
	};
	
	void __setitem__(PyObject *key, float value)
	{
		if (!PySequence_Check(key))
			throw mio::TypeException("key is not of sequence type");
		if (PySequence_Length(key) < 2)			
			throw mio::TypeException("key has fewer than 2 components");		
		
		PyObject *ii = PySequence_GetItem(key, 0);
		PyObject *jj = PySequence_GetItem(key, 1);
		
		if (!PyInt_Check(ii) || !PyInt_Check(jj))
		{
			Py_DECREF(ii);
			Py_DECREF(jj);
			throw mio::TypeException("one of the indices is not an integer");
		}
		
		long i = PyInt_AsLong(ii);
		long j = PyInt_AsLong(jj);
		
		Py_DECREF(ii);
		Py_DECREF(jj);
		
		if (i < 0 || i > 3 || j < 0 || j > 3)
			throw mio::IndexException("index to Matrix4x4 element out of range");
		else					
			$self->_set(i,j,value);
	};
	
	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Matrix4x4(self)
	%}
}