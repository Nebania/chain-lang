#include <iostream>
#include <string>
#define print(x) std::cout << (x) << std::endl;
#define LINK_data (((int*)link_shm)[0])
    #include <iostream>
    #include <cmath>

extern "C" void link_entry(void* link_shm) {


    LINK_data = std::pow(LINK_data, 2);
    std::cout << "[Native C++] Root Result : " << LINK_data << std::endl;

}
