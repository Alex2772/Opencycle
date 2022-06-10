#pragma once

#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <functional>
#include "IMenu.h"

class ListMenu: public IMenu {
public:
    struct State {
        struct Item {
            std::string name;
            std::function<void()> onSelected;
            std::optional<std::string> value;
        };

        std::vector<Item> items;
        std::vector<Item>::iterator selectedItem;

        State(std::vector<Item> items) :
            items(std::move(items)),
            selectedItem(State::items.begin())
            {}
    };

    explicit ListMenu(std::vector<State::Item> items) : mState(std::move(items)) {}

    void paint(IDisplayDriver& display) override;
    void onKeyAction(Key key) override;

private:
    State mState;
};
