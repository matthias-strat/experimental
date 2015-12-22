#include "Tilemap.hpp"
#include <iostream>

bool Tilemap::load(const int* data, unsigned int width, unsigned int height)
{
    if (!m_Tileset.loadFromFile("Assets/tileset.png"))
        return false;

    static constexpr float tileWidthF(tileWidth), tileHeightF(tileHeight);

    // Init tilemap
    m_Vertices.resize(width*height*4);
    for (auto x(0); x < width; x++)
    {
        for (auto y(0); y < height; y++)
        {
            const auto tileIdx(get1DIndexFrom2D(x, y, width));

            auto& nw(m_Vertices[tileIdx*4 + 0]);
            auto& ne(m_Vertices[tileIdx*4 + 1]);
            auto& se(m_Vertices[tileIdx*4 + 2]);
            auto& sw(m_Vertices[tileIdx*4 + 3]);

            nw.position = {(x + 0)*tileWidthF, (y + 0)*tileHeightF};
            ne.position = {(x + 1)*tileWidthF, (y + 0)*tileHeightF};
            se.position = {(x + 1)*tileWidthF, (y + 1)*tileHeightF};
            sw.position = {(x + 0)*tileWidthF, (y + 1)*tileHeightF};

            // Calculate texture coordinate

            float tu, tv;
            switch (data[tileIdx])
            {
                case 0:
                {
                    tu = 1;
                    tv = 5;
                    break;
                }
                case 1:
                {
                    tu = 0;
                    tv = 4;
                    break;
                }
                case 2:
                {
                    tu = 1;
                    tv = 4;
                    break;
                }
                case 3:
                {
                    tu = 2;
                    tv = 4;
                    break;
                }
                case 4:
                {
                    tu = 2;
                    tv = 5;
                    break;
                }
                case 5:
                {
                    tu = 2;
                    tv = 6;
                    break;
                }
                case 6:
                {
                    tu = 1;
                    tv = 6;
                    break;
                }
                case 7:
                {
                    tu = 0;
                    tv = 6;
                    break;
                }
                case 8:
                {
                    tu = 0;
                    tv = 5;
                    break;
                }
            }

            nw.texCoords = {(tu + 0)*tileWidthF, (tv + 0)*tileHeightF};
            ne.texCoords = {(tu + 1)*tileWidthF, (tv + 0)*tileHeightF};
            se.texCoords = {(tu + 1)*tileWidthF, (tv + 1)*tileHeightF};
            sw.texCoords = {(tu + 0)*tileWidthF, (tv + 1)*tileHeightF};

        }
    }

    return true;
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_Tileset;
    target.draw(&m_Vertices[0], m_Vertices.size(), sf::Quads, states);
}