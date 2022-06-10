#pragma once


#include <Bicycle/Input.h>
class IDisplayDriver;

class IMenu {
public:
    virtual void paint(IDisplayDriver& display) = 0;
    virtual void onKeyAction(Key key) = 0;

protected:
    void requestRepaint();
};
