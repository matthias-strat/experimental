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
}

template <template <typename> class TEase, template <typename> class TKind,
    typename T1, typename T2, typename T3>
inline std::common_type_t<T1, T2, T3> getEased(const T1& i, const T2& iMin, const T3& iMax) noexcept
{
    return easing::Impl::Dispatcher<std::common_type_t<T1, T2, T3>>::template getMap<TEase, TKind>(
        i, iMin, iMax, iMin, iMax);
}

template <template <typename> class TEase, template <typename> class TKind,
    typename T1, typename T2, typename T3, typename T4, typename T5>
inline std::common_type_t<T1, T2, T3, T4, T5> getMapEased(const T1& i, const T2& iMin, const T3& iMax,
    const T4& oMin, const T5& oMax) noexcept
{
    return easing::Impl::Dispatcher<std::common_type_t<T1, T2, T3, T4>>::template getMap<TEase, TKind>(
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

        m_Shape.setPosition(windowWidth/2.f, windowHeight/2.f);
        m_Shape.setSize({m_Width, m_Height});
        m_Shape.setOrigin(m_Width/2.f, m_Height/2.f);
        m_Shape.setFillColor(sf::Color::Black);
    }

    inline void update(float ft)
    {
    }

    inline void updateVariable(float dt)
    {
        static constexpr float duration(1.f);
        m_Time += dt;
        if (m_Time <= duration)
        {
            auto width(getMapEased<easing::Back, easing::Out>(m_Time, 0.f, duration, m_Width, m_DestWidth));
            auto height(getMapEased<easing::Back, easing::Out>(m_Time, 0.f, duration, m_Height, m_DestHeight));

            /*auto width(easing::Bounce<float>::out(m_Time, (m_In ? m_DestWidth : m_Width), 
                (m_In ? m_Width-m_DestWidth : m_DestWidth-m_Width), duration));
            auto height(easing::Bounce<float>::out(m_Time, (m_In ? m_DestHeight : m_Height), 
                (m_In ? m_Height-m_DestHeight : m_DestHeight-m_Height), duration));*/

            m_Shape.setSize({width, height});
            m_Shape.setOrigin(width/2.f, height/2.f);
        }
        else if (m_Time > duration + 0.5f)
        {
        }
    }

    inline void draw(sf::RenderTarget& target)
    {
        target.draw(m_Shape);
    }

private:
    Game m_Game{"SFML easing", windowWidth, windowHeight};

    sf::RectangleShape m_Shape;

    float m_Width{100.f}, m_Height{100.f};
    float m_DestWidth{450.f}, m_DestHeight{450.f};
    float m_PosX{100.f}, m_PosY{300.f};
    float m_DestPosX{static_cast<float>(windowWidth)-m_Width};
    float m_DestPosY{static_cast<float>(windowHeight)-m_Height};

    float m_Time{0.f};
    bool m_In{false};
};

int main()
{
    TransGame{}.run();
    return 0;
}