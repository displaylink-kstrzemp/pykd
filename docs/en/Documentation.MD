{anchor:table}
# Table of Contents
* [1. Introduction](#intro)
  * [1.1 General Information](#commoninfo)
  * [1.2 Quick Start](#gettingStarted)
  * [1.3 Building from Source](#Building)
  * [1.4 Manual Installation](#Installing)
  * [1.5 API Changes](#APIchange)
* [2. Commands for Windbg](#windbg)
  * [2.1 Loading the Plugin](#loadplugin)
  * [2.2 Running the Script](#runscript)
  * [2.3 Console Mode](#console)
* [3. Debugging Control](#debugging)
  * [3.1 Pausing and Resuming the Debugging Process](#break)
  * [3.2 Step Execution](#step)
  * [3.3 Debugging Control from Python Applications](#consoledebug)
  * [3.4 Printing Debug Information](#dbgprint)
  * [3.5 Executing Debugger Commands](#dbgcommand)
  * [3.6 Creating a Crash Dump](#createcrash)
* [4. Working with Memory and Registers](#memory)
  * [4.1 Accessing General-Purpose Registers](#reg)
  * [4.2 Accessing Model-Specific Registers](#msr)
  * [4.3 Normalizing Virtual Addresses](#addr64)
  * [4.4 Direct Memory Access](#memaccess)
  * [4.5 Memory Access Errors](#memerror)
  * [4.6 Reading Strings from Memory](#memstr)
* [5. Modules](#modules)
  * [5.1 The `module` Class](#moduleclass)
  * [5.2 Module Load and Unload Events](#moduleload)
* [6. Obtaining Symbolic Information](#syminfo)
  * [6.1 Symbol (PDB) Files](#pdbfile)
  * [6.2 Type Information](#types)
* [7. Typed Variables](#typedVar)
  * [7.1 The `typedVar` Class](#typedVar)
  * [7.2 Creating an Instance of the `typedVar` Class](#typedVarClass)
  * [7.3 Methods of the `typedVar` Class](#typedVarMethod)
  * [7.4 Classes and Structures](#typedVarUDT)
  * [7.5 Arrays and Pointers](#typedVarArray)
  * [7.6 Enumerators](#typedVarEnum)
  * [7.7 Casting to Other Types](#typedVarCast)
* [8. Processes and Threads](#ProcessThreads)
  * [8.1 User-Mode Threads](#UserModeThreads)
  * [8.2 Kernel-Mode Threads](#KernelModeThreads)
  * [8.3 Kernel-Mode Processes](#KernelModeProcess)
* [9. Local Variables](#Locals)
  * [9.1 Current Local Variables](#CurrentLocals)
* [10. Breakpoints](#breakpoints)
  * [10.1 Setting Breakpoints](#setBreakpoints)
  * [10.2 Conditional Breakpoints](#condBreakpoints)
* [11. Debugging Events](#eventHandler)
* [12. The `disasm` Class](#disasm)
* [API Reference](#PYKD-0.2-API-Reference)

{anchor:intro}
# 1. Introduction
{anchor:commoninfo}
## 1.1 General Information
The pykd project started in 2010. The main motivation for development was the inconvenience of the built-in tools for writing debugging scripts for windbg. Python was chosen as an alternative scripting engine for many reasons: the ease of learning the language itself, the availability of a large standard library, and a powerful and convenient framework for creating extension modules. Pykd is a module for the CPython interpreter. Pykd itself is written in C++ and uses Boost.Python to export functions and classes to Python. Pykd provides access to debugging control on the Windows platform through the Debug Engine library and to obtaining symbolic information through the MS DIA library. Note that pykd does not provide direct access to the COM interfaces of Debug Engine and MS DIA. Instead, it implements its own interface, making the development process faster and more convenient (we hope).

It is worth noting that pykd can operate in two modes: as a plugin for windbg, in which case it provides commands to run scripts in the context of a debugging session; as a separate module for the Python interpreter. The latter mode can be useful for creating automatic crash dump parsing tools, for example.

[Table of Contents](#table)

{anchor:gettingStarted}
## 1.2 Quick Start
For a quick start, it is best to download the automatic installer. It will install all the necessary components (including Python if it is not yet installed). To ensure the installation was successful, launch windbg and start debugging an application or analyzing a dump. Load *pykd*:

    .load pykd.pyd

If no error messages appear, everything is fine. But just to be sure, let's verify that everything is indeed working:

    >!pycmd
    Python 2.6.5 (r265:79096, Mar 19 2010, 18:02:59) [MSC v.1500 64 bit (AMD64)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    (InteractiveConsole)
    >>>print "Hello world!"
    Hello world!
    >>>quit()
    >

Try running the sample scripts:

    !py help
    !py samples

If everything worked, you can move on to writing your own scripts.

[Table of Contents](#table)

{anchor:Building}
## 1.3 Building from Source
### Download the source code from the [repository](http://pykd.codeplex.com/SourceControl/list/changesets).

### Install Python
You can download the required version [here](http://www.python.org).

### Install and configure Boost.
If you're not sure where to get it: [http://www.boost.org](http://www.boost.org). There is also an installation and build guide available.

### Set environment variables.
The following environment variables are required for building:
- `$(DIA_SDK_ROOT)` - Path to the MS DIA library. It should look something like this: C:\Program Files (x86)\Microsoft Visual Studio 9.0\DIA SDK. The MS DIA library is installed with Visual Studio.
- `$(DBG_SDK_ROOT)` - Path to the Debug Engine SDK. It should look something like this: C:\Program Files (x86)\Debugging Tools for Windows (x86)\sdk. The Debug Engine SDK is installed with the Debugging Tools for Windows (currently part of the Platform SDK).
- `$(BOOST_ROOT)` - Path to the directory where Boost is installed.
- `$(PYTHON_ROOT)` - Path to the directory where Python is installed. It is assumed that both x86 and x64 versions are installed on the system and that the Python directory structure is as follows:
  - C:\Python26\x86\...
  - C:\Python26\x64\...
  The `$(PYTHON_ROOT)` variable should be set to C:\Python26 in this case. If the platform specification is missing from the Python installation path, the project file will need to be adjusted.

### Build Boost.Python libraries.
You will need the static Boost.Python libraries for the build. The project file specifies the following paths to the Boost.Python libraries:
- `$(BOOST_ROOT)\stage` - for x86 build
- `$(BOOST_ROOT)\stage64` - for x64 build

You can build them using the following commands:

    bjam --stagedir=stage --with-python stage
    bjam address-model=64 --stagedir=stage64 --with-python stage

If you don't have bjam installed yet, you can download it [here](http://www.boost.org/users/download/boost_jam_3_1_18.html).

[Table of Contents](#table)

{anchor:Installing}
## 1.4 Manual Installation
For manual installation, you will need the pykd.pyd module and the C++ runtime redistributable (vcredist) from Visual Studio, specifically the one that the module was built with. If you built it yourself, there should be no problems. If you downloaded it from the site, there should also be no problems, as the zip archive contains the necessary vcredist (assuming we didn't mess up during the release :) ).

### Where to copy pykd.pyd?
It depends on the usage scenario. If pykd will be used as a plugin for windbg, it makes sense to copy it to the winext directory (located in the directory where windbg is installed). In this case, you can rename it to pykd.dll, so you can omit the file extension when loading:

    kd>.load pykd

If pykd will be used for writing your own Python programs, place it where the Python interpreter can find it. There are three options:
- The Lib subdirectory in the Python installation directory.
- Any custom directory. The path to it should be added to the `$(PYTHONPATH)` environment variable or set via the registry.
- Any custom directory without specifying paths anywhere, always launching Python from the directory containing pykd.pyd.

### Installing vcredist
Of course, you need to install vcredist. Otherwise, why download it?

### Registering MS DIA
The MS DIA library will be installed during the vcredist installation. But for it to work, it still needs to be registered. To do this, find the directory where the msdia90.dll module was installed and run the following command from that directory:

    regsvr32 msdia90.dll

If you built the module yourself and used Visual Studio, there is no need to take any actions with vcredist - it is already on your machine, and MS DIA is also in place.

[Table of Contents](#table)

{anchor:APIchange}
## 1.5 API Changes
### loadModule
The loadModule function has been removed. Instead, you should use the constructor of the *module* class:

    # mod = loadModule('mymodule')
    mod = module("mymodule")

[Table of Contents](#table)

{anchor:windbg}

# 2. Commands for Windbg

{anchor:loadplugin}
## 2.1 Loading the Plugin
To load the plugin in windbg, execute the following command:

    kd>.load pykd_path/pykd.pyd

If `pykd.pyd` is located in the `winext` directory (a subdirectory in Debugging Tools for Windows), you can omit the path to pykd:

    kd>.load pykd.pyd

If `pykd.pyd` is renamed to `pykd.dll`, you can omit the file extension:

    kd>.load pykd

To view the loaded windbg extensions, use the `.chain` command:

    kd>.chain

To unload the plugin, use one of the following commands:

    kd>.unload pykd_path/pykd.pyd
    kd>.unload pykd.pyd
    kd>.unload pykd

To avoid loading pykd each time, you can execute the "Save Workspace" command after loading the plugin. This will ensure that pykd loads automatically for the given workspace.

[Table of Contents](#table)

{anchor:runscript}
## 2.2 Running the Script
You can run a script using the `!py` command:

    kd>!py script_path/script_name.py  param1 param2 ...

The `.py` extension can be omitted. To avoid specifying the full path to the script, you can set it in the `PYTHONPATH` environment variable or (preferably) add the pykd key to the registry section:

    HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\2.6\PythonPath

In this case, the script paths are set in the Default value of the created key.

You can access the variables in the script through the `sys.argv` list:

    import sys
    print "script path: " + sys.argv[0]
    print "param1: " + sys.argv[1]
    print "param2: " + sys.argv[2]

[Table of Contents](#table)

{anchor:console}
## 2.3 Console Mode
To launch the Python console, use the `!pycmd` command:

    1: kd> !pycmd
    Python 2.6.5 (r265:79096, Mar 19 2010, 18:02:59) [MSC v.1500 64 bit (AMD64)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    (InteractiveConsole)
    >>>

Before starting, the import of pykd will be performed automatically, so you can immediately call pykd functions. Remember, you can exit the console mode using the `quit()` function. The state of the Python interpreter is preserved:

    >>> a = 10
    >>> quit(0)
    1: kd> !pycmd
    Python 2.6.5 (r265:79096, Mar 19 2010, 18:02:59) [MSC v.1500 64 bit (AMD64)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    (InteractiveConsole)
    >>> print a
    10
    >>>

[Table of Contents](#table)

# 3. Debugging Control

{anchor:break}
## 3.1 Pausing and Resuming the Debugging Process
In windbg, the commands Break (Ctrl+Break) and Go (F5) are used for this purpose. Their counterparts in pykd are:
- `go()`
- `breakin()`

The `go` function resumes the debugging process and returns control only when the debugger is halted again—either by hitting a breakpoint or by manually stopping the debugging with Ctrl+Break. This is important to keep in mind when writing scripts. The function may throw a `DbgException`, usually occurring when the debugged process terminates.

    try:
        while True:
            go()
            print "break"
    except:
        print "process terminated"

This script will handle any debugger stops and automatically resume execution.

The `breakin` function is unlikely to be needed during regular operations. Typically, scripts only run when the debugger is paused, making a call to `breakin` meaningless at that moment. To stop the debugging process from a script, you'll need to create a separate thread to call this function.

*Warning!* Do not attempt to use the `breakin`, `go`, or `trace` functions inside debugger event handlers (e.g., in conditional breakpoints).

[Table of Contents](#table)

{anchor:step}
## 3.2 Step Execution
For step debugging (tracing), there are two functions:
- `step()`
- `trace()`

Their actions are analogous to the debugger commands "trace into" and "trace over". Both functions may throw a `DbgException` if the debugged process has already terminated.

[Table of Contents](#table)

{anchor:debugstatus}
{anchor:consoledebug}
## 3.3 Debugging Control from Python Applications
If you want to execute your scripts outside of windbg, the first step is to create a debugging session. Session management will be discussed in more detail in a dedicated section. If your application does not plan to use multiple debugging sessions, you don't need to worry about it—the first session will be created automatically with the following calls:
- `loadDump(dumpName)` - loads a crash dump
- `id startProcess(imageName)` - starts a new process in debug mode
- `id attachProcess(processId)` - attaches the debugger to an existing process
- `attachKernel(parameterStr)` - attaches the debugger to the system kernel

To detach the debugger from the debugged process, use the `detachProcess(id)` call.

To stop debugging and terminate the debugged process, use the `killProcess(id)` function.

You can determine the debugger's mode with the following calls:
- `bool isDumpAnalyzing()`
- `bool isKernelDebugging()`

The first function determines whether the debugger is in a "live" debugging state or analyzing a memory dump. The second function distinguishes between kernel-mode and user-mode debugging. If a script uses specific system symbols (e.g., Windows kernel symbols), it may be helpful to insert such a check at the beginning of the script to inform the user that they are attempting to run the script in an inappropriate situation.

[Table of Contents](#table)

{anchor:dbgprint}
## 3.4 Printing Debug Information
To output information to the screen, you can use the standard `print` statement. However, it is recommended to use the following specialized functions:
- `dprint(message, dml=False)`
- `dprintln(message, dml=False)`

The second function differs from the first by automatically adding a newline character. The optional `dml` parameter enables output with DML markup (only works in windbg). DML markup is similar to a very simplified version of HTML. Text formatting is done with special tags:
- `<b></b>` - bold font
- `<i></i>` - italic font
- `<u></u>` - underlined font
- `<link cmd="cmdStr">command</link>` - executes a command (similar to the `<a>` tag in HTML)

    dprinln("<b><u>This command reloads all symbols</b></u>", True)
    dprinln("<link cmd=\".reload /f\">reload</link>", True)

[Table of Contents](#table)

{anchor:dbgcommand}
## 3.5 Executing Debugger Commands
To execute a debugger command, use the following function:
- `commandOutput dbgCommand(commandStr)`

    s = dbgCommand("!analyze -v")
    dprint(s)

To evaluate an expression (similar to the "?" debugger command), use the `expressionStr expr(expressionStr)` function:

    expr("@rax + 10")

When running a Python program, you may need commands from standard windbg extensions (e.g., `!analyze`). You will need to load them manually. Use the `long loadExt(extensionPath)` function for this purpose. This function returns an extension handle, which is needed to call extension functions:

- `str callExt(extHandle, command, params)`

And, if necessary, to unload the extension:

- `removeExt(extHandle)`

*Warning!* Working with windbg extensions differs from pykd version 0.1! In version 0.2, the `ext` class has been removed, and you no longer need to worry about the lifecycle of the loaded extension.

[Table of Contents](#table)

{anchor:createcrash}
## 3.6 Creating a Crash Dump
You can save the system state as a crash dump using the `writeDump` function. This function is available for both kernel-mode and user-mode. The second parameter specifies the type of dump (False - full dump, True - mini dump):

    writeDump(r"C:\dump\fulldump.dmp", False)
    writeDump(r"C:\dump\minidump.dmp", True)

[Table of Contents](#table)

{anchor:memory}

# 4. Working with Memory and Registers

{anchor:reg}
## 4.1 Accessing General-Purpose Registers
Access to General-Purpose Registers (GPR) is provided by the `reg` function:
- `cpuReg reg(regName)`
- `cpuReg reg(regIndex)`

The first variant takes the symbolic name of the register, while the second takes an integer index. The second form can be used to enumerate registers:

    import pykd

    try:
        i = 0
        while True:
            r = pykd.reg(i)
            pykd.dprintln("%s    %x ( %d )" % (r.name(), r, r))
            i += 1
    except pykd.BaseException:
           pass

Both variants of the `reg` function return an instance of the `cpuReg` class. If the register information cannot be retrieved, a `BaseException` will be thrown.

The `cpuReg` class has two methods:
- `name()`
- `index()`

The `cpuReg` class can be used in integer calculations without additional type casting:

    r = reg("eax")
    print r / 10 * 234

*Note:*  
The current implementation of `pykd` supports working only with integer registers. FPU, MMX, and SSE registers are not supported. Support is planned for future versions.

[Table of Contents](#table)

{anchor:msr}
## 4.2 Accessing Model-Specific Registers (MSR)
Access to Model-Specific Registers is provided by the `rdmsr(msrNumber)` function:

    >>> print findSymbol(rdmsr(0x176))
    nt!KiFastCallEntry

[Table of Contents](#table)

{anchor:addr64}
## 4.3 Normalizing Virtual Addresses
All `pykd` functions return virtual addresses in what is known as a normalized form, which is a 64-bit integer. For 32-bit platforms, the address is sign-extended to 64 bits. This operation in C looks like this:

    ULONG64 addr64 = (ULONG64)(LONG)addr;

Thus, addresses are converted as follows:
- 0x00100000 -> 0x00000000 00100000
- 0x80100000 -> 0xFFFFFFFF 80100000

This should be taken into account if the addresses returned by `pykd` functions are involved in arithmetic operations. To avoid potential comparison errors, it is recommended to use the `addr64()` function:

    import pykd
    nt = pykd.module("nt")
    if nt > addr64(0x80000000):
        print "nt module is in the highest address space"

[Table of Contents](#table)

{anchor:memaccess}
## 4.4 Direct Memory Access
`pykd` provides a large set of functions for accessing the memory of the debugged system.  
To read unsigned integers, the following functions are available:
- `ptrByte(va)`
- `ptrWord(va)`
- `ptrDWord(va)`
- `ptrQWord(va)`

For signed integers, equivalent functions are available:
- `ptrSignByte(va)`
- `ptrSignWord(va)`
- `ptrSignDWord(va)`
- `ptrSignQWord(va)`

For developing cross-platform scripts, the following functions are available:
- `ptrMWord(va)`
- `ptrSignMWord(va)`
- `ptrPtr(va)`

They return results based on the platform's bitness—32 or 64 bits.

Often, it is necessary to read a block of memory. The following functions are used for this:
- `loadBytes(va, count)`
- `loadWords(va, count)`
- `loadDWords(va, count)`
- `loadQWords(va, count)`
- `loadSignBytes(va, count)`
- `loadSignWords(va, count)`
- `loadSignDWords(va, count)`
- `loadSignQWords(va, count)`
- `loadPtrs(va, count)`

All functions return a `list` object.

[Table of Contents](#table)

{anchor:memerror}
## 4.5 Memory Access Errors
All memory access functions throw a `MemoryException` if the data cannot be read from the specified address:

    try:
        a = ptrByte(0)
    except MemoryException:
        print "memory exception occurred"

You can check the validity of a virtual address using the `isValid(va)` function.

[Table of Contents](#table)

{anchor:memstr}
## 4.6 Reading Strings from Memory
It is often necessary to read string data from memory. Of course, you could use the `loadBytes` function for this, but it's not always convenient. Therefore, `pykd` provides a set of functions that return data as a string.

The primary functions are:
- `loadChars(va, count)`
- `loadWChars(va, count)`

They work just like `loadBytes` and `loadWords`, with the only difference being the return value (`string` instead of `list`). This allows them to be used, for example, with the `struct` module:

    from struct import unpack
    shortField1, shortField2, longField = unpack('hhl', loadChars(addr, 8))

For reading null-terminated strings from memory, the following functions are available:
- `loadCStr(va)`
- `loadWStr(va)`

Both return strings (`loadWStr` returns UNICODE). Note the potential danger in using these functions—there is no guarantee of the presence of a terminating null character! *Note:* The maximum string length is limited to 64K. Attempting to read a longer string will result in a `MemoryException`.

In the Windows kernel, strings are represented using the `UNICODE_STRING` and `ANSI_STRING` structures. The following functions are available for working with them:
- `loadAnsiString`
- `loadUnicodeString`

[Table of Contents](#table)

{anchor:modules}

# 5. Modules

{anchor:moduleclass}
## 5.1 The `module` Class
A module is an executable file mapped into memory. A typical program consists of a main module (usually with a `.exe` extension) and a set of libraries. Modules are managed using the `module` class.

### 5.1.1 Creating an Instance of the `module` Class
The `module` class has two constructor forms:
- `module(moduleName)`
- `module(va)`

The first form creates a module by its name, and the second by a virtual address belonging to the module. If the module is not found, the constructor will throw a `BaseException`.

Example:

    from pykd import *
    try:
        ntdll = module("ntdll")
        print ntdll.name(), hex(ntdll.begin()), hex(ntdll.size())
    except BaseException:
        print "module not found"

### 5.1.2 Retrieving Information About the Module
The following methods of the `module` class are used for this purpose:
- `name()` - returns the module name
- `image()` - returns the executable file name
- `pdb()` - returns the name and full path to the symbol file
- `begin()` - returns the virtual address where the module is loaded
- `end()` - returns the virtual address of the module's end
- `checksum()` - returns the checksum
- `timestamp()` - returns the timestamp
- `getVersion()` - returns a tuple representing the module version, e.g., (1, 0, 6452, 0)
- `queryVersion(valueName)` - returns a value from the module's resources

### 5.1.3 Loading and Accessing Symbols
The `reload()` method is used to load symbol information.  
To find the virtual address corresponding to a symbol, use the `offset(symName)` method. If the specified symbol is not found, a `BaseException` will be thrown. Instead of explicitly calling the `offset` function, you can obtain the address corresponding to the symbol by accessing it as a property of the `module` class:

    >>> nt = module("nt")
    >>> print hex(nt.offset("PsLoadedModuleList"))
    0xfffff801acb5ae80L
    >>> print hex(nt.__getattr__("PsLoadedModuleList"))
    0xfffff801acb5ae80L
    >>> print hex(nt.PsLoadedModuleList)
    0xfffff801acb5ae80L

Sometimes, the RVA of a symbol is needed. You can obtain it using the `rva(symbolName)` function.

### 5.1.4 Conversion to Other Types
An instance of the `module` class has conversion operators to a string (`__str__`) and an integer:

    >>> nt = module("nt")
    >>> print nt
    Module: nt
    Start: fffff801ac882000 End: fffff801acfc8000 Size: 746000
    Image: ntkrnlmp.exe
    Pdb: c:\sym\ntkrnlmp.pdb\569F266AE67D457D969D92298F8F98082\ntkrnlmp.pdb
    Timestamp: 4f7118bb
    Check Sum: 6b3b15

    >>> print hex(nt)
    fffff801ac882000

Additionally, an instance of the `module` class can participate in arithmetic operations:

    >>> print hex(nt + 10)
    0xfffff801ac88200aL

### 5.1.5 Retrieving Type Information
In addition to symbols describing variables and functions (entities that have an RVA), there may be symbols describing types. Naturally, these do not have an RVA. If type information is available for a module, it can be obtained using the `type(typeName)` function. This function returns an instance of the `typeInfo` class, which will be discussed later.

    >>> nt = module("nt")
    >>> print nt.type("_MDL")
    struct/class: _MDL Size: 0x1c (28)
       +0000 Next                : _MDL*
       +0004 Size                : Int2B
       +0006 MdlFlags            : Int2B
       +0008 Process             : _EPROCESS*
       +000c MappedSystemVa      : Void*
       +0010 StartVa             : Void*
       +0014 ByteCount           : ULong
       +0018 ByteOffset          : ULong

### 5.1.6 Typed Variables
`pykd` simplifies working with complex types such as classes and structures through a special class called `typedVar`. You can obtain an instance of the `typedVar` class using the following methods of the `module` class:
- `typedVar(va)`
- `typedVar(symbolName)`
- `typedVar(typeName, va)`

    >>> nt = module("nt")
    >>> print nt.typedVar("_LIST_ENTRY", nt.PsLoadedModuleList)
    struct/class: _LIST_ENTRY at 0xfffff8000369c650
       +0000 Flink               : _LIST_ENTRY*   0xfffffa8003c64890
       +0008 Blink               : _LIST_ENTRY*   0xfffffa80092f8f30

[Table of Contents](#table)

{anchor:moduleload}
## 5.2 Handling Module Load and Unload Events
To handle module load and unload events, you need to create a subclass of the [eventHandler](#eventHandler) class.  
The module load event is handled by the [onLoadModule](#eventHandler_onLoadModule) method.  
The module unload event is handled by the [onUnloadModule](#eventHandler_onUnloadModule) method.

[Table of Contents](#table)

{anchor:syminfo}

# 6. Obtaining Symbolic Information

{anchor:pdbfile}
## 6.1 Symbol (PDB) Files
When a module is built, a file containing symbolic (debugging) information is created (usually with a `.pdb` extension). Depending on the compiler settings, it may contain full or partial information (so-called "public symbols"). Symbol files may contain the following information:
- Names, types, and relative offsets of global variables and constants
- Names, parameters, and relative offsets of functions and class methods
- Names of user-defined types (structures, classes, enumerations)
- Values of constants
- Information about local variables of functions and class methods

To work with symbol files, Microsoft provides a special library, MS DIA. *Pykd* uses this library to handle symbols. To directly access symbolic information, *pykd* implements its own interface.

[Table of Contents](#table)

{anchor:types}
## 6.2 Type Information

{anchor:typeInfo}
### 6.2.1 The `typeInfo` Class
The `typeInfo` class is exported to Python to represent type information. This class describes structures, classes, unions, enumerations, bit fields, pointers, and basic types.

The class provides the following methods:
- `name` - retrieves the name of the type
- `size` - retrieves the total size of the type
- `staticOffset` - retrieves the offset of a static field
- `fieldOffset` - retrieves the offset of a field
- `bitOffset` - retrieves the offset of a bit field
- `bitWidth` - retrieves the size of a bit field
- `field` - retrieves a field
- `asMap` - retrieves a dictionary for the value of an enumeration
- `deref` - dereferences a pointer
- `ptrTo` - creates a pointer to the type
- `arrayOf` - creates an array of the type
- `append` - adds a field (a method for structures and enumerations created using [typeBuilder](#typeBuilder))

[Table of Contents](#table)

{anchor:get_typeInfo}
### 6.2.2 Obtaining a Type Object
A type object can be obtained by calling the constructor with the type name. The passed string can contain either the full type specification ("module_name!type_name") or just "type_name". 

A type object can be obtained indirectly:
- `type` method on a `module` object - creates a type object by name
- `type` method on a `typedVar` object - creates a type object corresponding to the variable's type

Example (printing the structure `"_UNICODE_STRING"` from `ntdll`):

    >>> us = module("ntdll").type("_UNICODE_STRING")
    >>> print us
    class/struct : _UNICODE_STRING Size: 0x10 (16)
       +0000 Length                  : UInt2B
       +0002 MaximumLength           : UInt2B
       +0008 Buffer                  : UInt2B*

To retrieve all types of a module, you can use the `enumTypes` method on a `module` object, which returns a list of type names represented in the module's debug symbols.

[Table of Contents](#table)

{anchor:typeBuilder}
### 6.2.3 Creating Types Not Represented in Debug Symbols
Often during debugging, the necessary data types are not present in the debug symbols. To facilitate this, a separate class was created for building custom structures and unions—`typeBuilder`.

Complex data types are generally composed of basic types. To obtain basic types, the `typeBuilder` object provides the following fields:
- `UInt1B`
- `UInt2B`
- `UInt4B`
- `UInt8B`
- `Int1B`
- `Int2B`
- `Int4B`
- `Int8B`
- `Long`
- `ULong`
- `Bool`
- `Char`
- `WChar`
- `VoidPtr`

Special mention should be made of the `VoidPtr` field—a pointer to `void`. The size of this type depends on the platform and the current debugging mode. However, for convenience, when creating a `typeBuilder` object, you can pass the desired pointer size to the constructor.

With basic types and types from the module's debug information, you can build arbitrary types. The `typeBuilder` object has two methods for this purpose: `createStruct` and `createUnion`. Both methods take a string for the type name as the first parameter. When creating a structure, an optional second parameter can specify the desired alignment.

Example (creating and printing a custom structure `"_UNICODE_STRING"`):

    >>> tb = typeBuilder()
    >>> us = tb.createStruct("_UNICODE_STRING")
    >>> us.append("Length", tb.UInt2B)
    >>> us.append("MaximumLength", tb.UInt2B)
    >>> us.append("Buffer", tb.WChar.ptrTo())
    >>> print us
    class/struct : _UNICODE_STRING Size: 0x10 (16)
       +0000 Length                  : UInt2B
       +0002 MaximumLength           : UInt2B
       +0008 Buffer                  : WChar*

[Table of Contents](#table)

{anchor:typedVar}

# 7. Typed Variables

{anchor:typedVar}
## 7.1 The `typedVar` Class
Earlier, we looked at an example of how to read structured data from memory:

    from struct import unpack
    shortField1, shortField2, longField = unpack('hhl', loadChars(addr, 8))

Obviously, working with large structures containing hundreds of fields in this way is not very convenient. Therefore, *pykd* implements a special class: `typedVar`, which allows working with complex data structures. The `typedVar` class obtains its data type information from the symbol information.

[Table of Contents](#table)

{anchor:typedVarClass}
## 7.2 Creating an Instance of the `typedVar` Class
There are several overloaded constructors for the `typedVar` class:
- `typedVar(symbolName)`
- `typedVar(typeName, va)`
- `typedVar(typeInfo, va)`

    t1 = typedVar("MyModule!MyVar")
    t2 = typedVar("MyModule!MyType", addr)
    ti = typeInfo("MyModule!MyType")
    t3 = typedVar(ti, addr)

All three methods will yield the same result if _addr_ is the address of the `MyVar` variable. Note that all these methods (especially the first) are not optimal in terms of performance, as significant time may be spent searching for symbol information. If possible, it is better to use the methods of the `module` class:
- `module.typedVar(va)`
- `module.typedVar(symbolName)`
- `module.typedVar(typeName, va)`

    mod = module("MyModule")
    t4 = mod.typedVar(addr)
    t5 = mod.typedVar("MyVar")
    t6 = mod.typedVar("MyType", addr)

The result will be the same as a direct call to the constructor. However, the `module` class instance optimizes access to symbol information.

If there is an error in specifying the variable or type name, a `SymbolException` will be thrown.

    try:
        typedVar("MyModule!NotExistVar")
    except SymbolException:
        print "The var does not exist"

[Table of Contents](#table)

{anchor:typedVarMethod}
## 7.3 Methods of the `typedVar` Class
- `getAddress()` - returns the address of the variable
- `sizeof()` - returns the size of the variable
- `offset()` - if the variable is a field of a parent structure, returns the offset relative to the parent
- `field(fieldName)` - returns the structure field as an instance of the `typedVar` class
- `deref()` - dereferences pointers and returns the result as an instance of the `typedVar` class
- `type()` - returns the variable's type as an instance of the `typeInfo` class

[Table of Contents](#table)

{anchor:typedVarUDT}
## 7.4 Classes and Structures
You can access the fields of a structure using the `field` method. For convenience, a method to access fields as class attributes has been added:

    >>> tv = typedVar("structVar")
    >>> tv.field("m_field") == tv.m_field
    True

Additionally, you can access the fields of a structure by index:

    tv = typedVar("structVar")
    for i in range(0, len(tv)):
        fieldName, fieldValue = tv[i]
        print fieldName, fieldValue

As seen in the example, a tuple of the field name and its value is returned in this case. The same example can be written more concisely:

    tv = typedVar("structVar")
    for fieldName, fieldValue in tv:
        print fieldName, fieldValue

Variables of type `typedVar` can participate in arithmetic operations. The address of the variable is used as the value. *Note:* In arithmetic operations, the rules of C pointer arithmetic *do not apply*. The address is treated simply as a number, so `var + 1` just increments the address value.

[Table of Contents](#table)

{anchor:typedVarArray}
## 7.5 Arrays and Pointers
The `typedVar` class allows working with arrays, including multidimensional ones. To access array elements, use the `[]` index operator:

    >>> tv = typedVar("intMatrix")
    >>> print tv
    Int4B[2][3] at 0x13f159150
    >>> print tv[1]
    Int4B[3] at 0x13f15915c
    >>> print tv[1][2]
    Int4B at 0x13f159164 Value: 0x5 (5)

The `typedVar` class can also work with pointers. To "dereference" a pointer, use the `deref()` function:

    >>> tv = typedVar("ptrIntMatrix")
    >>> print tv
    Ptr Int4B(*)[2][3] at 0x13f1591c0 Value: 0x13f159150
    >>> print tv.deref()
    Int4B[2][3] at 0x13f159150
    >>> print tv.deref()[1][2]
    Int4B at 0x13f159164 Value: 0x5 (5)

`typedVar` variables can participate in arithmetic expressions. For arrays, the address is used as the value, and for pointers, the pointer value (i.e., where it points). *Note:* In arithmetic operations, the rules of C pointer arithmetic *do not apply*.

[Table of Contents](#table)

{anchor:typedVarEnum}

# 7. Typed Variables

{anchor:typedVarEnum}
## 7.6 Enumerators
When working with enumerators, it is useful to access the type information of the enumerator, as this is how you can get the correspondence between numeric constants and symbolic names. This can be done through the `type` method, which returns a reference to a `typeInfo` object:

    var = typedVar("myStruct")
    if var.structType == var.structType.type().TYPE_ONE:
        print "TYPE_ONE"
    else:
        print "ANOTHER_TYPE"

The `typeInfo` class has a method `asMap()`, which returns a `dict` object for enumerators, where the keys are numeric constants and the values are their symbolic representations:

    var = typedVar("myStruct")
    {
        "TYPE_ONE": lambda: var.field_one,
        "TYPE_TWO": lambda: var.field_two 
    }[var.type().asMap()[var.structType]]()

This example demonstrates two things: first, how to create a logical structure in Python similar to the `switch` statement in C using a `dict`, where the field values are lambda expressions; second, how to obtain the correspondence between a numeric constant and its symbolic name for an enumerator.

Instances of `typedVar` containing enumerators can be used in arithmetic operations:

    >>> var = typedVar("myStruct")
    >>> print var.structType * 2 + 10

[Table of Contents](#table)

{anchor:typedVarCast}
## 7.7 Casting to Other Types
The `typedVar` class has operators for casting to a string (`__str__`) and to an integer (`__long__`).

    >>> print str(typedVar("g_struct"))
    struct/class: struct3 at 0x13f4391f8
       +0000 m_arrayField            : Int4B[2]   
       +0008 m_noArrayField          : Int4B   0x3 (3)

The integer value returned by the `long()` function depends on the data type stored in `typedVar`:
- **Basic types**: The actual value is returned.
- **Structures, classes, and unions**: The value returned is the pointer to the start of the data.
- **Enumerators**: The actual value is returned.
- **Pointers**: The pointer's value is returned.
- **Arrays**: The pointer to the start of the data is returned.

    >>> long(typedVar("g_struct").m_noArrayField)
    3L
    >>> hex(long(typedVar("g_struct").m_arrayField))
    '0x13f4391f8L'
    >>> 
    >>> long(typedVar("g_struct").m_arrayField[1])
    2L

[Table of Contents](#table)

{anchor:ProcessThreads}
# 8. Processes and Threads

{anchor:UserModeThreads}
## 8.1 User-Mode Threads
In user mode, the debugger operates in the context of the debugged process. If the process has multiple threads, you can switch the debugging context to another thread. It is important to distinguish between the "current thread" (the thread that will continue execution when debugging is resumed) and the "implicit thread" (the thread in which the debugger is currently operating). The thread context refers to the set of processor registers, including the current instruction pointer and stack pointer.

To change the thread context, use the `setImplicitThread` function, which takes a pointer to the TEB (Thread Environment Block) as a parameter. You can obtain a pointer to the TEB using:
- `getImplicitThread` - TEB of the current thread
- `getProcessThreads` - list of TEBs for all threads in the process

[Table of Contents](#table)

{anchor:KernelModeThreads}
## 8.2 Kernel-Mode Threads
In kernel mode, there are some differences:
- The `setImplicitThread` and `getImplicitThread` functions work with pointers to `ETHREAD`, not `TEB` as in user mode.
- The `getProcessThreads` function is not available in kernel mode. If you need to obtain a list of threads for a process, you'll need to manually parse the `EPROCESS` structure. Fortunately, this is not difficult:

    nt = module("nt")
    process = nt.typedVar("_EPROCESS", processAddr)
    threadLst = nt.typedVarList(process.ThreadListHead, "_ETHREAD", "ThreadListEntry")

It is important to note that switching the context of the debugged thread does not switch the context of the process. More details on this are provided in the next section.

[Table of Contents](#table)

{anchor:KernelModeProcess}
## 8.3 Kernel-Mode Processes

[Table of Contents](#table)

{anchor:Locals}
# 9. Local Variables

{anchor:CurrentLocals}
## 9.1 Current Local Variables
When debugging an application, the system kernel, or analyzing a crash dump, there is always a current thread, and in that thread, a current frame. If there is debugging information for the module to which this frame belongs and that debugging information includes local variables, you can access them conveniently without explicit operations with registers and the stack. The `getLocals()` function is used for this purpose. It returns a `dict` object where the key is the variable name and the value is an instance of the `typedVar` class:

    # Print local variable "argc"
    print getLocals()["argc"]

    # Print all local variables in the current frame
    for varName, varValue in getLocals().items():
        print varName, varValue

[Table of Contents](#table)

{anchor:breakpoints}

# 10. Breakpoints

{anchor:setBreakpoints}
## 10.1 Setting Breakpoints
The `setBp` function is used to set breakpoints. It allows setting both software and hardware breakpoints. The function returns a numeric identifier that can later be used to remove the breakpoint with the `removeBp` function.

Setting a software breakpoint:

    nt = module("nt")
    bpid = setBp(nt.NtCreateFile)

Setting a hardware breakpoint:

    nt = module("nt")
    bpid = setBp(nt.NtCreateFile, 1, 4)

The second parameter specifies the size of the memory accessed, and the third parameter specifies the type of access (1 - read, 2 - write, 4 - execute). The access types work as flags and can be combined. For example, 3 means read + write.

[Table of Contents](#table)

{anchor:condBreakpoints}
## 10.2 Conditional Breakpoints
To set a conditional breakpoint, a callback function is used, which is passed as a parameter to the `setBp` call. This function must accept one parameter (the identifier of the triggered breakpoint is passed to it). For the breakpoint to trigger, the callback function must return `True`.

    import fnmatch
    from pykd import *

    nt = module('nt')
    objAttrType = nt.type("_OBJECT_ATTRIBUTES")

    def onCreateFile(id):
        objattr = typedVar(objAttrType, ptrPtr(reg('esp') + 0xC))
        return fnmatch.fnmatch(loadUnicodeString(objattr.ObjectName), '*.exe')

    setBp(nt.NtCreateFile, onCreateFile)

Note that a lambda function can be used as a callback function:

    setBp(myAddr, lambda id: reg('rax') > 0x1000)

Remember the lifetime of objects created in scripts! Functions are objects in Python, and they will be deleted when the Python virtual machine terminates. Here's a potential pitfall: if you run the previous script using the `!py setmybreak.py` command, you won't get the expected breakpoint trigger because it will be removed when the script finishes. What to do? There are two options:
1. Use debug control in the script, like this:

    setBp(nt.NtCreateFile, onCreateFile)
    go() 

In this case, you'll catch exactly one breakpoint trigger, and then the script will finish.

2. Use the `!pycmd` command to set the breakpoint. 

Recall that this command creates a _global_ Python interpreter, and all Python objects remain available even after executing the `_quit()` command:

    >!pycmd
    >>> import setmybreak
    >>> quit()
    > g

When the module is imported, all actions to set the breakpoint will be performed, and even after exiting the console, the Python callback functions will continue to work!

*Warning!*
Callback functions have some restrictions on the use of the *pykd* API:
- You cannot call functions that may change the state of the debugger: `go`, `breakin`, `trace`.
- You cannot call functions that may create or destroy debug sessions: `startProcess`, `killProcess`, `openDump`, etc.
- You cannot manipulate thread and process contexts (`setCurrentProcess`, `setImplicitThread`).

[Table of Contents](#table)

{anchor:eventHandler}
# 11. Debugging Events

[Table of Contents](#table)

{anchor:eventHandler_onBreakpoint}
## 11.1 Handling Breakpoints (onBreakpoint Method)

[Table of Contents](#table)

{anchor:eventHandler_onException}
## 11.2 Handling Exceptions (onException Method)

[Table of Contents](#table)

{anchor:eventHandler_onLoadModule}
## 11.3 Handling Module Load Events (onLoadModule Method)

[Table of Contents](#table)

{anchor:eventHandler_onUnloadModule}
## 11.4 Handling Module Unload Events (onUnloadModule Method)

[Table of Contents](#table)

{anchor:disasm}
# 12. The `disasm` Class
[disasm reference|PYKD 0.1. API Reference#disasm]

The `disasm` class is a wrapper around the disassembler from Debug Engine. Accordingly, the results of its work are the same as those of the `u` command in `kd/cdb/windbg`.

The `disasm` class has the following methods:
- `__init__()` - creates a disassembler that starts from the current instruction.
- `__init__(offset)` - creates a disassembler that starts from the specified offset.
- `disasm()` - returns the disassembled representation of the CPU instruction at the current offset and moves to the next instruction.
- `disasm(offset)` - returns the disassembled representation of the CPU instruction at the specified offset and moves to the next instruction.
- `asm(code)` - assembles the specified instruction and changes the machine code at the specified offset.
- `begin()` - returns the offset specified when the `disasm` instance was created.
- `current()` - returns the current offset.
- `length()` - returns the length of the current CPU instruction.
- `instruction()` - returns the disassembled representation of the CPU instruction at the current offset.
- `ea()` - returns the effective address of the last disassembled instruction, or 0.
- `reset()` - similar to calling `self.disasm(self.begin())`.

The effective address is the address of an operand located in memory. For example, for the instruction:

    mov ecx, [esi+0x10]

The effective address would be the value of `esi + 0x10`. Obviously, this value makes sense when disassembling the current instruction.

[Table of Contents](#table)
