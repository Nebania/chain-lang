```
░█████╗░██████╗░███████╗██╗░░░░░
██╔══██╗██╔══██╗██╔════╝██║░░░░░
██║░░╚═╝██████╔╝█████╗░░██║░░░░░
██║░░██╗██╔══██╗██╔══╝░░██║░░░░░
╚█████╔╝██║░░██║███████╗███████╗
░╚════╝░╚═╝░░╚═╝╚══════╝╚══════╝
Chain Runtime Extensions Library(Public API)
================================
Version: 0.1
Creater: tohar777
project:Chain
language:C++
status:active development
```
 CREL is a library that lets developers creating low-level,native libraries using C++

## Hello CREL
#### 1. create a C++ dynamic library and a chain file
helloCREL.cpp
```C++
#include "CREL.hpp"

#ifdef _WIN32
#define CREL_EXPORT __declspec(dllexport)
#else
#define CREL_EXPORT
#endif

extern "C" CREL_EXPORT
void CREL_Init(CREL* crel)
{
    crel->registerFunction(
        "helloCREL",
        [](const std::vector<Obj>& args) -> Obj {
            return std::string("Hello from CREL!");
        }
    );
}
```
crel.chain
```
print(helloCREL)
```
#### 2.Compile it using G++
```bash
    g++ -shared -o hello.dll helloCREL.cpp
```
#### Execute it with chainlang

```bash
chainlang crel.chain --crel ./hello.dll
```

output:
```
[CREL] Loading: ./hello.dll
[CREL] Loaded successfully: ./hello.dll
Hello from CREL!
```