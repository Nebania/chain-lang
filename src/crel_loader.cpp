#include "crel_loader.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using InitFn = void (*)(CREL*);

bool LibraryLoader::load(const std::string& path, CREL& api)
{
#ifdef _WIN32

    HMODULE handle = LoadLibraryA(path.c_str());

    if (!handle)
        return false;

    auto init = reinterpret_cast<InitFn>(
        GetProcAddress(handle, "LinkLibrary_Init")
    );

    if (!init)
    {
        FreeLibrary(handle);
        return false;
    }

    init(&api);

    return true;

#else

    void* handle = dlopen(path.c_str(), RTLD_NOW);

    if (!handle)
        return false;

    auto init = reinterpret_cast<InitFn>(
        dlsym(handle, "LinkLibrary_Init")
    );

    if (!init)
    {
        dlclose(handle);
        return false;
    }

    init(&api);

    return true;

#endif
}