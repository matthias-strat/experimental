#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "../Common/Game.hpp"

template <typename T>
using Vec2 = sf::Vector2<T>;
using Vec2f = Vec2<float>;
using Vec2u = Vec2<unsigned int>;
using Vec2i = Vec2<int>;