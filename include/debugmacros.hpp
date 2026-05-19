#pragma once

#include <iostream>

#define ASSERT_TRUE(cond, msg)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            std::cerr << msg;                                                                                          \
            std::exit(1);                                                                                              \
        }                                                                                                              \
    } while (0)
