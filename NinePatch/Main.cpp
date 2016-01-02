#include "../Common/Common.hpp"

class NinePatchGame
{
public:
    inline NinePatchGame() noexcept
    {
        m_Game.onLoadContent = [this]()
        {
            loadContent();
        };
        m_Game.onUpdate = [this](float ft)
        {
            update(ft);
        };
        m_Game.onDraw = [this](sf::RenderTarget& target)
        {
            draw(target);
        };
    }

    inline void run() { m_Game.run(); }

private:
    inline void loadContent()
    {
        m_Texture.loadFromFile("Assets/ninepatch.png");
        m_NinePatch.setTexture(m_Texture);
    }

    inline void update(float ft)
    {
        auto& window(m_Game.getWindow());
        auto pos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        m_NinePatch.setSize(pos);
    }

    inline void draw(sf::RenderTarget& target)
    {
        target.draw(m_NinePatch);
    }

private:
    Game m_Game{"Nine Patch"};
    sf::Texture m_Texture;
    NinePatch m_NinePatch;
};

int main()
{
    NinePatchGame{}.run();
    return 0;
}