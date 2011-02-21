#pragma once

#include <string>
#include <map>

#include <boost/python.hpp>
#include <boost/python/object.hpp>

/////////////////////////////////////////////////////////////////////////////////

class dbgModuleClass {

public:

    dbgModuleClass() :
        m_base( 0 ),
        m_end( 0 )     
    {}

    dbgModuleClass( const std::string &name, ULONG64 base, ULONG size );
    
    ULONG64
    getBegin() const {
        return m_base;
    }

    ULONG64
    getEnd() const {
        return m_end;
    }

    bool
    contain( ULONG64  addr ) const {
        if ( *( (ULONG*)&addr + 1 ) == 0 )
            *( (ULONG*)&addr + 1 ) = 0xFFFFFFFF;
                          
        return m_base <= addr && addr <= m_end;
    }

    std::string
    getName() const {
        return m_name;    
    }

    void
    reloadSymbols();

    ULONG64
    getOffset( const std::string  &symName );

    std::wstring
    getImageSymbolName() const {
        return m_imageFullName;
    }

    std::wstring    
    getPdbName() const {
        return std::wstring( m_debugInfo.LoadedPdbName );
    }

    void 
    addSyntheticSymbol( ULONG64 offset, ULONG size, const std::string &symName );

private:

    ULONG64         m_base;
    
    ULONG64         m_end;
    
    std::string     m_name;
    
    std::wstring    m_imageFullName;

    IMAGEHLP_MODULEW64        m_debugInfo;
    
    typedef std::map<std::string, ULONG64>  OffsetMap;
    OffsetMap       m_offsets;
    
    void
    getImagePath();
};

/////////////////////////////////////////////////////////////////////////////////

boost::python::object
loadModule( const std::string &moduleName );


boost::python::object
findModule( ULONG64 addr );

void
addSyntheticSymbol( ULONG64 addr, ULONG size, const std::string &symName );

/////////////////////////////////////////////////////////////////////////////////