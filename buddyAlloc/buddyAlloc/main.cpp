#include <iostream>
#include "BuddyAllocator.h"


int main()
{
    auto a = new BuddyAllocator(10); //16 byte
    a->show();
    auto d = a->alloc(8);
    auto b = a->alloc(4);
    auto c = a->alloc(2);
    auto s = a->alloc(1);
    auto v = a->alloc(1);
    a->dealloc(b);
    a->dealloc(c);
    a->dealloc(d);
    a->dealloc(s);
    a->show();
    a->dealloc(v);
    a->show();
}