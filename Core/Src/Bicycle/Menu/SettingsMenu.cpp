//
// Created by Alex2772 on 6/10/2022.
//

#include "SettingsMenu.h"
#include "Bicycle/Menu/Settings/Lights.h"
#include "CommonActions.h"

SettingsMenu::SettingsMenu():
        ListMenu({
             {
                 "Light...",
                 ShowMenu<Lights>(),
             }
        })
{

}
