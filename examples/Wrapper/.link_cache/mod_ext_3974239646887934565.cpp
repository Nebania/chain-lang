#include <iostream>
#include <string>
#define print(x) std::cout << (x) << std::endl;
#define LINK_x (((int*)link_shm)[0])
#define LINK_z (((int*)link_shm)[1])
#define LINK_y (((double*)((char*)link_shm + 2048))[0])

extern "C" void link_entry(void* link_shm) {

    print(" ==== C++ Wrapping ==== "); 
    for(int i = 0; i < LINK_z; i++) {
        LINK_x -= 1;
        LINK_y -= 2.5;
        print("Decrement: " + std::to_string(LINK_x));
    }

}
