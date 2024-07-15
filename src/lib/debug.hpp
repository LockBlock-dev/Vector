#pragma once
#include <iostream>

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(msg) std::cout << "DEBUG: " << msg << "\n";
#else
    #define DEBUG_PRINT(msg)
#endif
