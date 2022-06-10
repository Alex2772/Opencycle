#pragma once

#include "IMenu.h"
#include "Bicycle/IDisplayDriver.h"

template<typename T>
class ValueChangingMenu: public IMenu {
public:
    static_assert(std::is_same_v<T, std::decay_t<T>>, "T should be decayed");
    using AcceptCallback = std::function<void(T&&)>;

    ValueChangingMenu(AcceptCallback accept, T initial, T min = 1, T max = 100, T step = 1):
            mAccept(std::move(accept)),
            mValue(std::move(initial)),
            mMin(std::move(min)),
            mMax(std::move(max)),
            mStep(std::move(step))
    {

    }

    void paint(IDisplayDriver& display) override {
        display.paintValueChangingMenu(std::to_string(mValue));
    }

    void onKeyAction(Key key) override {
        switch (key) {
            case Key::UP: {
                if (mValue >= mMax) return;
                mValue += mStep;
                break;
            }
            case Key::DOWN: {
                if (mValue <= mMin) return;
                mValue -= mStep;
                break;
            }
            default: return;
        }
        mValue = std::clamp(mValue, mMin, mMax);
        requestRepaint();
    }

private:
    AcceptCallback mAccept;
    T mValue;
    T mMin;
    T mMax;
    T mStep;
};