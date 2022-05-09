#pragma once

struct State {
    /**
     * Speed in km/h.
     */
    float currentSpeed = 0;

    /**
     * Speed of previous revolution in km/h.
     */
    float prevRevolutionSpeed = 0.f;

    /**
     * Overall distance on this session.
     */
    float distance = 0;

    float temperature = 0;
    float humidity = 0;
};