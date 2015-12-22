#pragma once
#include "../Common/Common.hpp"
#include <vector>

constexpr unsigned int tileWidth{32}, tileHeight{32};

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const int* data, unsigned int width, unsigned int height);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Texture m_Tileset;
    std::vector<sf::Vertex> m_Vertices;
};