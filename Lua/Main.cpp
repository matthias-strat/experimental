#include <iostream>
#include <cassert>
#include <algorithm>
#include <memory>

extern "C"
{
    #include <Lua/lua.h>
    #include <Lua/lauxlib.h>
    #include <Lua/lualib.h>
}

// Simple Lua Bind
namespace slub
{

}

int main()
{
    slub::State state{false};
    state.push(1, true, "hi", 3.5f);


    return 0;
}