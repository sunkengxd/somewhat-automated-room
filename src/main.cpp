#include <Arduino.h>

#include "callbacks.h"


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    init_tasks();
}

void loop() {
    // put your main code here, to run repeatedly:
    run_tasks();
}