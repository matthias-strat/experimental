#pragma once

class NinePatch : public sf::Drawable, public sf::Transformable
{
public:
    inline NinePatch() = default;
    inline NinePatch(const sf::Texture& texture, const sf::Vector2f& size) noexcept
        : m_Texture{&texture},
          m_Size{size}
    {
        updateSizes();
        updateVertices();
    }

    inline void setTexture(const sf::Texture& texture)
    {
        m_Texture = &texture;
        m_UpdateTexCoords = true;
        updateSizes();
        updateVertices();
    }

    inline void setSize(const sf::Vector2f& size) noexcept
    {
        auto width(size.x), height(size.y);
        if (width < m_MinSize.x) width = m_MinSize.x;
        if (height < m_MinSize.y) height = m_MinSize.y;
        m_Size = {width, height};

        m_UpdatePos = true;
        updateVerticesPos();
    }

private:
    static constexpr int vertexCount{36}; // 9 * 4

    inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        if (m_Texture == nullptr) return;
        states.transform *= getTransform();
        states.texture = m_Texture;
        target.draw(m_Vertices, vertexCount, sf::Quads, states);
    }

    inline void updateSizes()
    {
        auto texSize(m_Texture->getSize());
        m_PatchSize = {texSize.x/3.f, texSize.y/3.f};
        m_MinSize = {m_PatchSize.x*3.f, m_PatchSize.y*3.f};
    }

    inline void updateVertices()
    {
        if (m_UpdatePos)
        {
            updateVerticesPos();
            m_UpdatePos = false;
        }
        if (m_UpdateTexCoords)
        {
            updateVerticesTexCoord();
            m_UpdateTexCoords = false;
        }
    }

    inline void updateVerticesPos()
    {
        const auto px(m_PatchSize.x), py(m_PatchSize.y);
        const auto sx(m_Size.x), sy(m_Size.y);

        // Top left (fixed)
        m_Vertices[0].position  = {0.f,     0.f};
        m_Vertices[1].position  = {px,      0.f};
        m_Vertices[2].position  = {px,      py};
        m_Vertices[3].position  = {0.f,     py};

        // Top (stretched horizontally)
        m_Vertices[4].position  = {px,      0.f};
        m_Vertices[5].position  = {sx - px, 0.f};
        m_Vertices[6].position  = {sx - px, py};
        m_Vertices[7].position  = {px,      py};

        // Top right (fixed)
        m_Vertices[8].position  = {sx - px, 0.f};
        m_Vertices[9].position  = {sx,      0.f};
        m_Vertices[10].position = {sx,      px};
        m_Vertices[11].position = {sx - px, py};

        // Right (stretched vertically)
        m_Vertices[12].position = {sx - px, py};
        m_Vertices[13].position = {sx,      px};
        m_Vertices[14].position = {sx,      sy - py};
        m_Vertices[15].position = {sx - px, sy - py};

        // Bottom right (fixed)
        m_Vertices[16].position = {sx - px, sy - py};
        m_Vertices[17].position = {sx,      sy - py};
        m_Vertices[18].position = {sx,      sy};
        m_Vertices[19].position = {sx - px, sy};

        // Bottom (stretched horizontally)
        m_Vertices[20].position = {px,      sy - py};
        m_Vertices[21].position = {sx - px, sy - py};
        m_Vertices[22].position = {sx - px, sy};
        m_Vertices[23].position = {px,      sy};

        // Bottom left (fixed)
        m_Vertices[24].position = {0.f,     sy - py};
        m_Vertices[25].position = {px,      sy - py};
        m_Vertices[26].position = {px,      sy};
        m_Vertices[27].position = {0.f,     sy};

        // Left (stretched vertically)
        m_Vertices[28].position = {0.f,     py};
        m_Vertices[29].position = {px,      py};
        m_Vertices[30].position = {px,      sy - py};
        m_Vertices[31].position = {0.f,     sy - py};

        // Center (stretched vertically and horizontally)
        m_Vertices[32].position = {px,      py};
        m_Vertices[33].position = {sx - px, py};
        m_Vertices[34].position = {sx - px, sy - py};
        m_Vertices[35].position = {px,      sy - py};
    }

    inline void updateVerticesTexCoord()
    {       
        const auto px(m_PatchSize.x), py(m_PatchSize.y);

        // Top left (fixed)
        m_Vertices[0].texCoords  = {0.f,    0.f};
        m_Vertices[1].texCoords  = {px,     0.f};
        m_Vertices[2].texCoords  = {px,     py};
        m_Vertices[3].texCoords  = {0.f,    py};

        // Top (stretched horizontally)
        m_Vertices[4].texCoords  = {px,     0.f};
        m_Vertices[5].texCoords  = {px*2.f, 0.f};
        m_Vertices[6].texCoords  = {px*2.f, py};
        m_Vertices[7].texCoords  = {px,     py};

        // Top right (fixed)
        m_Vertices[8].texCoords  = {px*2.f, 0.f};
        m_Vertices[9].texCoords  = {px*3.f, 0.f};
        m_Vertices[10].texCoords = {px*3.f, py};
        m_Vertices[11].texCoords = {px*2.f, py};

        // Right (stretched vertically)
        m_Vertices[12].texCoords = {px*2.f, py};
        m_Vertices[13].texCoords = {px*3.f, py};
        m_Vertices[14].texCoords = {px*3.f, py*2.f};
        m_Vertices[15].texCoords = {px*2.f, py*2.f};

        // Bottom right (fixed)
        m_Vertices[16].texCoords = {px*2.f, py*2.f};
        m_Vertices[17].texCoords = {px*3.f, py*2.f};
        m_Vertices[18].texCoords = {px*3.f, py*3.f};
        m_Vertices[19].texCoords = {px*2.f, py*3.f};

        // Bottom (stretched horizontally)
        m_Vertices[20].texCoords = {px,     py*2.f};
        m_Vertices[21].texCoords = {px*2.f, py*2.f};
        m_Vertices[22].texCoords = {px*2.f, py*3.f};
        m_Vertices[23].texCoords = {px,     py*3.f};

        // Bottom left (fixed)
        m_Vertices[24].texCoords = {0.f,    py*2.f};
        m_Vertices[25].texCoords = {px,     py*2.f};
        m_Vertices[26].texCoords = {px,     py*3.f};
        m_Vertices[27].texCoords = {0.f,    py*3.f};

        // Left (stretched vertically)
        m_Vertices[28].texCoords = {0.f,    py};
        m_Vertices[29].texCoords = {px,     py};
        m_Vertices[30].texCoords = {px,     py*2.f};
        m_Vertices[31].texCoords = {0.f,    py*2.f};

        // Center (stretched vertically and horizontally)
        m_Vertices[32].texCoords = {px,     py};
        m_Vertices[33].texCoords = {px*2.f, py};
        m_Vertices[34].texCoords = {px*2.f, py*2.f};
        m_Vertices[35].texCoords = {px,     py*2.f};
    }

    sf::Vertex m_Vertices[vertexCount];
    const sf::Texture* m_Texture{nullptr};
    bool m_UpdateTexCoords{true}, m_UpdatePos{true};
    sf::Vector2f m_Size, m_MinSize, m_PatchSize;

};