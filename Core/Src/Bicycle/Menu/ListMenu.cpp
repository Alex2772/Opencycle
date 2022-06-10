#include "ListMenu.h"


#include "Bicycle/IDisplayDriver.h"

void ListMenu::paint(IDisplayDriver& display) {
    display.paintListMenu(mState);
}

void ListMenu::onKeyAction(Key key) {

}
