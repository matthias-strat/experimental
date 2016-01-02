#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "../Common/Aliases.hpp"
#include "../Common/Game.hpp"
#include "../Common/NinePatch.hpp"

inline constexpr int get1DIndexFrom2D(int x, int y, int width)
{
    return x + y * width;
}