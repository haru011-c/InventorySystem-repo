// utils.h - helpers: string formatting and validated console input
#pragma once
#include <string>
#include <cstddef>

struct InputClosed {};                      // thrown when stdin reaches EOF

struct NoCopy {                             // base class that forbids copying
    NoCopy() {}
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
};

std::string str(long v);
std::string money(double v);
std::string lower(std::string s);
std::string fit(const std::string& s, std::size_t w);
std::string trim(const std::string& s);

std::string readLine(const std::string& prompt);
int         readInt(const std::string& prompt, int lo, int hi);
double      readDouble(const std::string& prompt, double lo, double hi);
std::string readText(const std::string& prompt, std::size_t maxLen = 40);
