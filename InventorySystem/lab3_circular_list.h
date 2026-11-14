// showcase.h - LAB 3: circular linked list (brand banners)
#pragma once
#include <string>
#include "utils.h"

struct Banner { std::string brand, tagline; Banner* next; };

class Showcase : NoCopy {
    Banner* tail;
public:
    int count;
    Showcase();
    ~Showcase();
    void append(const std::string& brand, const std::string& tagline);
    Banner* head() const { return tail ? tail->next : nullptr; }
    std::string brandAt(int i) const;
};
