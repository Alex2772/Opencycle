//
// Created by Alex2772 on 6/10/2022.
//

#include "Lights.h"
#include "Bicycle/Menu/CommonActions.h"

Lights::Lights():
    ListMenu({
        {
            "Front light intensity",
            Setting<&Settings::frontLightIntensity>(),
        },
        {
            "Back light intensity",
            Setting<&Settings::backLightIntensity>(),
        },
    })
{
}
