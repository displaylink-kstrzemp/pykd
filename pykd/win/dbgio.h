#pragma once

#include "dbgengine.h"

namespace pykd {

///////////////////////////////////////////////////////////////////////////////

class DbgOut  {

public:

    void
    write( const std::wstring  &str ) {
        dprint( str );
    }

};

///////////////////////////////////////////////////////////////////////////////

class DbgErr {

public:

    void
    write( const std::wstring  &str )
    {
        eprint( str );
    }
};

///////////////////////////////////////////////////////////////////////////////


class DbgIn {

public:

    std::string
    readline() {
        return dreadline();
    }

};

///////////////////////////////////////////////////////////////////////////////

} // end pykd namespace
