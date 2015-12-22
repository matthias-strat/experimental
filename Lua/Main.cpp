#include <iostream>
#include <cassert>

extern "C"
{
    #include <Lua/lua.h>
    #include <Lua/lauxlib.h>
    #include <Lua/lualib.h>
}

int main()
{
    lua_State* l = luaL_newstate();
    luaL_dofile(l, "Assets/scripts/hello.lua");

    // Push function name to the stack
    lua_getglobal(l, "subtract_and_hello");
    // Push two numbers (parameters) to the stack
    lua_pushinteger(l, 1);
    lua_pushinteger(l, 3);

    // Call the function
    const int numArgs = 2;
    const int numReturnValues = 2;
    lua_call(l, numArgs, numReturnValues);

    // Note that the stack is 1-indexed from the bottom
    const int diff = lua_tointeger(l, 1);
    const std::string greeting = lua_tostring(l, 2);

    std::cout << "from lua:\ndiff = " << diff << "\ngreeting = " << greeting << "\n";

    // Pop the return values from the stack.
    lua_pop(l, 2);

    // Check that things worked correctly.
    assert(diff == -2 && greeting == "hello");

    // Clean up the lua context
    lua_close(l);

    return 0;
}