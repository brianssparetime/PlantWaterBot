#pragma once
#include "Relay.h"
#include "Arduino.h"

void Relay::turn_on() {
    digitalWrite(Relay::pin, LOW);
}

void Relay::turn_off() {
    digitalWrite(Relay::pin, HIGH);
}