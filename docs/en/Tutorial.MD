# Introduction

## Step 1. Getting Started
The best way to install pykd is by using the automatic installer. It will install pykd in the correct location and register all necessary components.

If the installation completes without errors, it’s time to get acquainted with pykd. Start by launching windbg and beginning a debugging session (open a process, a dump, or establish a connection with the kernel debugger). Now, you can load pykd by running the command:
.load pykd.pyd
If any errors occur during the loading process, windbg will display a message. The absence of any messages indicates that the extension was successfully loaded.

Now, you can start working. Execute the command !pycmd. After executing this command, the debugger will enter user input mode. All user input will be processed by the Python interpreter.

0:000> !pycmd
Python 2.6.6 (r266:84297, Aug 24 2010, 18:13:38) [MSC v.1500 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
(InteractiveConsole)
>>> print "Hello world!"
Hello world!
>>>

This is a good time to get familiar with Python syntax if you haven't already. Rest assured, it won’t take long: Python is very easy to learn.

Let’s recall the basic syntax of Python:

>>> def printHello():
...     i = 0
...     while i < 4:
...         print "Hello #%d" % i
...         i += 1
... 
>>> printHello()
Hello #0
Hello #1
Hello #2
Hello #3
>>> 

Notice: block indentation is determined by the number of leading spaces. This is a "signature" feature of Python. For now, this knowledge will be sufficient. Let's move on.

## Step 2. Accessing Registers
Any debugger must provide three basic capabilities: reading processor registers, reading memory, and controlling the debugging mode. Let’s start with registers. With pykd, this is quite simple:

>>> print hex(reg("eip"))
0x778ecb60
>>> print hex(reg("esp"))
0x1ef0e0
>>> print hex(reg("esp")+4)
0x1ef0e4

In this case, we are using the function *reg* from PYKD. It reads the processor registers by name. An inquisitive reader might ask: how are we using functions from PYKD without importing the module itself? The module does need to be imported. However, PYKD does this automatically when the Python console is initialized.

Let’s write a small example and see where the current instruction pointer is pointing:

>>> print findSymbol(reg("eip"))
ntdll!LdrpDoDebuggerBreak+30

The *findSymbol* function tries to find a debug symbol for the given address. In this case, we see that the instruction pointer is equal to the offset 0x30 relative to the *LdrpDoDebuggerBreak* function, meaning we are simply inside the *LdrpDoDebuggerBreak* function located in the *ntdll* module. We were able to determine this because we have debugging information for the *ntdll.dll* module (the corresponding pdb file). If symbols are not displaying for some reason, check the symbol path settings in windbg.

## Step 4. Accessing Memory
PYKD provides a wide range of functions for accessing memory. These can be divided into three groups:
- **Reading values from memory:**
  - *ptrByte*
  - *ptrWord*
  - *ptrDWord*
  - *ptrQWord*
  
  Other functions are available; see the [API Reference for PYKD 0.2](#api-reference) for a complete list. All functions take an address as a parameter and return the value stored at that address.
  
- **Reading arrays:**
  - *loadBytes*
  - *loadWords*
  - *loadDWords*
  - *loadQWords*
  
  All functions take a pointer to the start of the array and its length in elements as parameters. They return a *list* object containing the array elements.
  
- **Reading strings:**
  - *loadCStr*
  - *loadWStr*
  
  These functions read null-terminated strings from memory and return Python strings.

Let’s modify the previous example and print the function arguments. Let’s assume the function follows the stdcall calling convention and its parameters are addressed by the *ebp* register:

>>> def printFunc():
...     print findSymbol(reg("eip"))
...     params = [ptrDWord(reg("ebp") + 4*(i+1)) for i in range(0, 3)]
...     print "var1: %x  var2: %x  var3:  %x" % (params[0], params[1], params[2])
... 
>>> print printFunc()
ntdll32!LdrpDoDebuggerBreak+2c
var1: 774b1383  var2: fffdd000  var3:  fffde000
None

Note the following construction:

params = [ptrDWord(reg("ebp") + 4*(i+1)) for i in range(0, 3)]

This is a list comprehension—a special Python construct that can be used for list initialization. This construct is equivalent to:

[ptrDWord(reg("ebp") + 4), ptrDWord(reg("ebp") + 8), ptrDWord(reg("ebp") + 0xC)]

## Step 5. Accessing Memory with Type Information
When debugging programs, we often deal with typed variables. PYKD has rich capabilities for accessing variables with type information. In essence, this is the main "feature" of the entire project: access to the fields of structures and classes is done in a way that closely resembles the original code. For example, suppose we have the following C code:

struct STRUCT_A {
  int field1;
  char field2;
};

STRUCT_A a = {100, 2};

Now, during debugging, we want to check the state of the variable 'a' using PYKD:

a = typedVar("module!STRUCT_A", getOffset("module!a"))
if a.field1 != 100 or a.field2 != 2:
    print "ERROR! a is not properly initialized!"

[Table of Contents](#table)
