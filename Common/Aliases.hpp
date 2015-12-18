#pragma once

// Macro for std::forward
#define FWD(x) std::forward<decltype(x)>(x)

// SFML vector shortcuts
template <typename T>
using Vec2 = sf::Vector2<T>;
using Vec2f = Vec2<float>;
using Vec2u = Vec2<unsigned int>;
using Vec2i = Vec2<int>;

// STL shortcuts
template <typename T>
using Func = std::function<T>;

// Smart pointer shortcuts
template <typename T>
using UPtr = std::unique_ptr<T>;
template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T, typename... TArgs>
inline decltype(auto) mkUPtr(TArgs&&... args)
{
    return std::make_unique<T>(FWD(args)...);
}

template <typename T, typename... TArgs>
inline decltype(auto) mkSPtr(TArgs&&... args)
{
    return std::make_shared<T>(FWD(args)...);
}