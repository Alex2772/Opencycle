#pragma once


#include <type_traits>
#include "IMenu.h"
#include "Bicycle/App.h"
#include "Bicycle/Settings.h"
#include "ValueChangingMenu.h"

template<typename Menu>
struct ShowMenu {
    static_assert(std::is_base_of_v<IMenu, Menu>, "Menu should be a subclass of IMenu");

    void operator()() {
        App::inst().showMenu(std::make_unique<Menu>());
    }
};

template<auto f>
struct Setting {
    using ValueType = std::decay_t<decltype(Settings::inst().*f)>;
    ValueType mMin, mMax, mStep;

    Setting(ValueType min = 1, ValueType max = 100, ValueType step = 1):
            mMin(std::move(min)),
            mMax(std::move(max)),
            mStep(std::move(step))
    {

    }


    void operator()() {
        App::inst().showMenu(std::make_unique<ValueChangingMenu<ValueType>>([](ValueType&& t) {
            auto settings = Settings::inst();
            settings.*f = std::move(t);
            settings.save();
        }, Settings::inst().*f, mMin, mMax, mStep));
    }
};