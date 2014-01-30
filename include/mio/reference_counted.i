%{
#include <mio/reference_counted.h>
%}

%feature("ref")   mio::ReferenceCounted "$this->inc_ref();"
%feature("unref") mio::ReferenceCounted "$this->dec_ref();"

%include <mio/reference_counted.h>