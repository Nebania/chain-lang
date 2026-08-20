#include "crel_loader.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <cstdint>
#endif

using InitFn = void (*)(CREL*);

bool LibraryLoader::load(const std::string& path, CREL& api)
{
#ifdef _WIN32

    HMODULE handle = LoadLibraryA(path.c_str());

    if (!handle) {
        std::cerr << "[CREL] LoadLibraryA failed for: " << path 
                  << " | Windows error: " << GetLastError() << std::endl;
        return false;
    }

    auto init = reinterpret_cast<InitFn>(
        GetProcAddress(handle, "CREL_Init")
    );

    if (!init)
    {
        std::cerr << "[CREL] CREL_Init not found in: " << path 
                  << " | Windows error: " << GetLastError() << std::endl;
        FreeLibrary(handle);
        return false;
    }

    init(&api);

    return true;

#else

    void* handle = dlopen(path.c_str(), RTLD_NOW);

    if (!handle) {
        std::cerr << "[CREL] dlopen failed for: " << path 
                  << " | Error: " << dlerror() << std::endl;
        return false;
    }

    auto init = reinterpret_cast<InitFn>(
        reinterpret_cast<intptr_t>(dlsym(handle, "CREL_Init"))
    );

    if (!init)
    {
        std::cerr << "[CREL] CREL_Init not found in: " << path 
                  << " | Error: " << dlerror() << std::endl;
        dlclose(handle);
        return false;
    }

    init(&api);

    return true;

#endif
}