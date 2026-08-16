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
        "crelHello",
        [](const std::vector<Obj>& args) -> Obj {
            return std::string("Hello from CREL!");
        }
    );
}