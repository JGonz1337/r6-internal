#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <cstddef>
#include <d3d11.h>
#include <iostream>
#include <string>

#include "protection/xorstr.hpp"

#define O_SWAPTABLE 0x93E68
#define O_BASE utils::get().fetch_module_base("RainbowSix.exe")
template<typename T>
class singleton
{
public:
    static T& get()
    {
        static auto instance_ = new T;
        return *instance_;
    }
};

#endif // !TYPES_HPP