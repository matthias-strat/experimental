#include "../Common/Common.hpp"
#include <iostream>
#include <cassert>

constexpr unsigned int windowWidth{1024}, windowHeight{768};

namespace easing
{
    template <typename T>
    constexpr T PI(3.14159265);

    template <typename T>
    struct In
    {
        inline static auto get() noexcept { return &T::in; }
    };

    template <typename T>
    struct Out
    {
        inline static auto get() noexcept { return &T::out; }
    };

    template <typename T>
    struct InOut
    {
        inline static auto get() noexcept { return &T::inOut; }
    };

    namespace Impl
    {
        template <typename T>
        struct Dispatcher
        {
            template <template <typename> class TEase, template <typename> class TKind>
            inline static T getMap(const T& i, const T& iMin, const T& iMax, const T& oMin, const T& oMax) noexcept
            {
                return TKind<TEase<T>>::get()(iMin + i, oMin, oMax - oMin, iMax - iMin);
            }
        };
    }

    template <typename T>
    struct Linear
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return c * t / d + b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            return in(t, b, c, d);
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            return in(t, b, c, d);
        }
    };

    template <typename T>
    struct Sine
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return -c*std::cos(t/d*(PI<T>/T(2))) + c + b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return c*std::sin(t/d * (PI<T>/T(2))) + b;
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return -c/T(2)*(std::cos(PI<T>*t/d)-T(1)) + b;
        }
    };

    template <typename T>
    struct Back
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            T s(1.70158), postFix(t/=d);
            return c*(postFix)*t*((s+T(1))*t-s)+b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            T s(1.70158);
            return c*((t=t/d-T(1))*t*((s+T(1))*t+s)+T(1))+b;
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            T s(1.70158);
            if ((t/=d/T(2)) < 1)
                return c/T(2)*(t*t*(((s*=T(1.525))+T(1))*t-s))+b;

            T postFix(t-=T(2));
            return c/T(2)*((postFix)*t*(((s*=T(1.525))+T(1))*t+s)+T(2))+b;
        }
    };

    template <typename T>
    struct Bounce
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            return c - out(d-t, T(0), c, d) + b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if ((t/=d) < (T(1)/T(2.75)))
                return c*(T(7.5625)*t*t)+b;

            if (t < T(2)/T(2.758))
            {
                T postFix(t-=(T(1.5)/T(2.75)));
                return c*(T(7.5625)*(postFix)*t + T(0.75)) +b;
            }

            if (t < (T(2.5)/T(2.75)))
            {
                T postFix(t-=(T(2.25)/T(2.75)));
                return c*(T(7.5625)*(postFix)*t+T(0.9375))+b;
            }

            T postFix(t-=(T(2.625)/T(2.75)));
            return c*(T(7.5625)*(postFix)*t+T(0.984375)) + b;
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if (t < d/T(2)) return in(t*T(2), T(0), c, d) * T(0.5) + b;
            return out(t*T(2)-d, T(0), c, d) * T(0.5) + c*T(0.5)+b;
        }
    };

    template <typename T>
    struct Circ
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return -c*(std::sqrt(T(1)-(t/=d)*t)-T(1))+b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return c*std::sqrt(T(1)-(t=t/d-T(1))*t)+b;
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if ((t/=d/T(2)) < T(1))
                return -c/T(2)*(std::sqrt(T(1)-t*t)-T(1))+b;
            return c/T(2)*(std::sqrt(T(1)-t*(t-=T(2))) + T(1))+b;
        }
    };

    template <typename T>
    struct Cubic
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return c*(t/=d)*t*t+b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            return c*((t=t/d-T(1))*t*t+T(1)) + b;
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if ((t/=d/T(2)) < T(1)) return c/T(2)*t*t*t+b;
            return c/T(2)*((t-=T(2))*t*t+T(2))+b;
        }
    };

    template <typename T>
    struct Elastic
    {
        inline static T in(T t, T b, T c, T d)
        {
            if (t==T(0)) return b;
            assert(d != 0);
            if ((t/=d)==T(1)) return b+c;

            T p(d*T(0.3)), a(c), s(p/T(4));
            T postFix(a*std::pow(T(2), T(10)*(t-=T(1))));
            return -(postFix*std::sin((t*d-s)*(T(2)*PI<T>)/p)) + b;
        }

        inline static T out(T t, T b, T c, T d)
        {
            assert(d != 0);
            if(t == T(0)) return b;
            if((t /= d) == T(1)) return b + c;

            T p(d * T(0.3)), a(c), s(p / T(4));
            assert(p != 0);

            return (a * std::pow(T(2), T(-10) * t) * std::sin((t * d - s) * (T(2) * PI<T>) / p) + c + b);
        }

        inline static T inOut(T t, T b, T c, T d)
        {
            assert(d != 0);
            if(t == T(0)) return b;
            if((t /= d / T(2)) == T(2)) return b + c;

            T p(d * T(0.3 * 1.5)), a(c), s(p / T(4));
            assert(p != 0);

            if(t < 1)
            {
                T postFix(a * std::pow(T(2), T(10) * (t -= T(1))));
                return -T(0.5) * (postFix * std::sin((t * d - s) * (T(2) * PI<T>) / p)) + b;
            }

            T postFix(a * std::pow(T(2), T(-10) * (t -= T(1))));
            return postFix * std::sin((t * d - s) * (T(2) * PI<T>) / p) * T(0.5) + c + b;
        }
    };

    template <typename T>
    struct Expo
    {
        inline static T in(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if (t == T(0)) return b;
            return c*(-std::pow(T(2), T(-10)*t/d)+T(1))+b;
        }

        inline static T out(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if (t == d) return b + c;
            return c * (-std::pow(T(2), T(-10)*t/d)+T(1))+b;
        }

        inline static T inOut(T t, T b, T c, T d) noexcept
        {
            assert(d != 0);
            if (t == T(0)) return b;
            if (t == d) return b + c;
            if ((t/=d/T(2)) < T(1))
                return c/T(2)*std::pow(T(2), T(10)*(t-T(1))) +b;
            return c/T(2)*(-std::pow(T(2), T(-10)*--t)+T(2)) +b; 
        }
    };
}

template <template <typename> class TEase, template <typename> class TKind,
    typename T1, typename T2, typename T3>
inline Common<T1, T2, T3> getEased(const T1& i, const T2& iMin, const T3& iMax) noexcept
{
    return easing::Impl::Dispatcher<Common<T1, T2, T3>>::template getMap<TEase, TKind>(
        i, iMin, iMax, iMin, iMax);
}

template <template <typename> class TEase, template <typename> class TKind,
    typename T1, typename T2, typename T3, typename T4, typename T5>
inline Common<T1, T2, T3, T4, T5> getMapEased(const T1& i, const T2& iMin, const T3& iMax,
    const T4& oMin, const T5& oMax) noexcept
{
    return easing::Impl::Dispatcher<Common<T1, T2, T3, T4>>::template getMap<TEase, TKind>(
        i, iMin, iMax, oMin, oMax);
}

class TransGame
{
public:
    inline TransGame()
    {
        m_Game.onLoadContent = [this]()
        {
            loadContent();
        };
        m_Game.onUpdate = [this](float ft)
        {
            update(ft);
        };
        m_Game.onUpdateVariable = [this](float dt)
        {
            updateVariable(dt);
        };
        m_Game.onDraw = [this](sf::RenderTarget& target)
        {
            draw(target);
        };
        m_Game.onEvent = [this](const sf::Event& event)
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                if (m_EasingState == EasingState::None)
                {
                    if (m_State == State::Menu)
                    {
                        m_EasingState = EasingState::EaseGame;
                        m_State = State::Game;
                    }
                    else if (m_State == State::Game) 
                    {
                        m_EasingState = EasingState::EaseMenu;
                        m_State = State::Menu;
                    }
                }
            }
        };
    }

    inline void run()
    {
        m_Game.run();
    }

private:
    inline void loadContent()
    {
        const auto windowWidth(m_Game.getWindowWidth());
        const auto windowHeight(m_Game.getWindowHeight());

        m_TxScoreboard.loadFromFile("assets/scoreboard.png");

        m_Shape.setPosition(windowWidth/2.f, windowHeight/2.f);
        m_Shape.setSize({m_Width, m_Height});
        m_Shape.setOrigin(m_Width/2.f, m_Height/2.f);
        m_Shape.setFillColor(sf::Color::Black);

        m_Scoreboard.setTexture(m_TxScoreboard);
        m_Scoreboard.setPosition(windowWidth / 2.f + 480 / 2.f - 250.f, m_ScoreY);

    }

    inline void update(float ft)
    {
    }

    inline void updateVariable(float dt)
    {
        static constexpr float duration(1.f);
        if (m_EasingState == EasingState::None) return;

        m_Time += dt;
        if (m_Time <= duration)
        {
            auto width(m_Shape.getSize().x), height(m_Shape.getSize().y);
            auto posX(m_Shape.getPosition().x), posY(m_Shape.getPosition().y);
            switch (m_EasingState)
            {
                case EasingState::EaseGame:
                {
                    width = getMapEased<easing::Bounce, easing::Out>(m_Time, 0.f, duration, m_Width, m_DestWidth);
                    height = getMapEased<easing::Bounce, easing::Out>(m_Time, 0.f, duration, m_Height, m_DestHeight);
                    break;
                }
                case EasingState::MoveGame:
                {
                    posX = getMapEased<easing::Cubic, easing::InOut>(m_Time, 0.f, duration, m_PosX, m_DestPosX);
                    posY = getMapEased<easing::Cubic, easing::InOut>(m_Time, 0.f, duration, m_PosY, m_DestPosY);

                    auto scoreX(m_Scoreboard.getPosition().x);
                    auto scoreY(getMapEased<easing::Back, easing::Out>(m_Time, 0.f, duration, m_ScoreY, m_ScoreDestY));
                    m_Scoreboard.setPosition(scoreX, scoreY);
                    break;
                }
                case EasingState::EaseMenu:
                {
                    width = getMapEased<easing::Bounce, easing::Out>(m_Time, 0.f, duration, m_DestWidth, m_Width);
                    height = getMapEased<easing::Bounce, easing::Out>(m_Time, 0.f, duration, m_DestHeight, m_Height);

                    auto scoreX(m_Scoreboard.getPosition().x);
                    auto scoreY(getMapEased<easing::Back, easing::In>(m_Time, 0.f, duration, m_ScoreDestY, m_ScoreY));
                    m_Scoreboard.setPosition(scoreX, scoreY);

                    break;
                }
                case EasingState::MoveMenu:
                {
                    posX = getMapEased<easing::Cubic, easing::InOut>(m_Time, 0.f, duration, m_DestPosX, m_PosX);
                    posY = getMapEased<easing::Cubic, easing::InOut>(m_Time, 0.f, duration, m_DestPosY, m_PosY);
                    break;
                }
            }
            
            m_Shape.setPosition(posX, posY);
            m_Shape.setSize({width, height});
            m_Shape.setOrigin(width/2.f, height/2.f);
        }
        else
        {
            m_Time = 0.f;
            if (m_EasingState == EasingState::EaseGame) m_EasingState = EasingState::MoveGame;
            else if (m_EasingState == EasingState::EaseMenu) m_EasingState = EasingState::MoveMenu;
            else
            m_EasingState = EasingState::None;
        }
    }

    inline void draw(sf::RenderTarget& target)
    {
        if (m_State == State::Game || (m_State == State::Menu && m_EasingState != EasingState::None)) 
            target.draw(m_Scoreboard);
        target.draw(m_Shape);
    }

private:
    enum class State
    {
        Menu,
        Game
    };

    enum class EasingState
    {
        None,
        EaseGame,
        MoveGame,
        EaseMenu,
        MoveMenu
    };

    Game m_Game{"SFML easing", windowWidth, windowHeight};

    sf::Texture m_TxScoreboard;

    sf::RectangleShape m_Shape;
    sf::Sprite m_Scoreboard;

    float m_Width{230.f}, m_Height{300.f};
    float m_DestWidth{480.f}, m_DestHeight{480.f};
    float m_PosX{windowWidth/2.f}, m_PosY{windowHeight/2.f};
    float m_DestPosX{m_PosX}, m_DestPosY{m_PosY+30.f};
    float m_ScoreY{-200.f}, m_ScoreDestY{45.f};

    float m_Time{0.f};

    State m_State{State::Menu};
    EasingState m_EasingState{EasingState::None};
};

int main()
{
    TransGame{}.run();
    return 0;
}