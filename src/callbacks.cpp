#include <TaskScheduler.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "callbacks.h"
#include "pins.h"
#include "request_codes.h"

Scheduler runner;

Task lcd_output_task(3000, TASK_FOREVER, &lcd_output_callback);
Task noise_monitor_task(TASK_SECOND, TASK_FOREVER, &noise_monitor_callback);
Task gas_monitor_task(TASK_SECOND, TASK_FOREVER, &gas_monitor_callback);
Task volume_ctl_task(TASK_IMMEDIATE, TASK_FOREVER, &volume_ctl_callback);
Task weather_btn_task(TASK_IMMEDIATE, TASK_FOREVER, &weather_btn_handler);
Task play_pause_btn_task(TASK_IMMEDIATE, TASK_FOREVER, &play_pause_btn_handler);
Task time_btn_task(TASK_IMMEDIATE, TASK_FOREVER, &time_btn_handler);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int weather_degrees_c;
int time;

void init_tasks() {
    runner.init();

    runner.addTask(lcd_output_task);  
    runner.addTask(noise_monitor_task);  
    runner.addTask(gas_monitor_task);  
    runner.addTask(volume_ctl_task);  
    runner.addTask(weather_btn_task);  
    runner.addTask(play_pause_btn_task);
    runner.addTask(time_btn_task);

    lcd_output_task.enable();
    noise_monitor_task.enable();
    gas_monitor_task.enable();
    volume_ctl_task.enable();
    weather_btn_task.enable();
    play_pause_btn_task.enable();
    time_btn_task.enable();
}


void run_tasks() {
    runner.execute();
}


void lcd_output_callback() {
    if(lcd_output_task.isFirstIteration()) {
        Serial.println("LCD initializing..");
        lcd.init();
        lcd.backlight();

        lcd.setCursor(0, 0);
        lcd.print("Hello there!");
    }


}


void noise_monitor_callback() {
    if(noise_monitor_task.isFirstIteration()) {
        Serial.println("Running noise monitor..");
        pinMode(MICROPHONE_PIN, INPUT);
    }

    int mic_val = analogRead(MICROPHONE_PIN);
}


void gas_monitor_callback() {
    if(gas_monitor_task.isFirstIteration()) {
        Serial.println("Running gas monitor..");

    }
}


void volume_ctl_callback() {
    if(volume_ctl_task.isFirstIteration()) {
        Serial.println("Running volume control..");
        pinMode(ECHO_PIN, INPUT);
        pinMode(TRIGGER_PIN, OUTPUT);
    }

    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    int distance = duration * 0.034 / 2; // cm
}


void weather_btn_handler() {
    if(weather_btn_task.isFirstIteration()) {
        Serial.println("Starting weather request handler..");

        pinMode(WEATHER_BTN_PIN, INPUT_PULLUP);
    }

    bool request_weather = digitalRead(WEATHER_BTN_PIN);

    if(request_weather) {
        Serial.write(REQUEST_WEATHER);
        while (Serial.available() <= 0);
        
        weather_degrees_c = Serial.read();
    }
}

void play_pause_btn_handler() {
    if(play_pause_btn_task.isFirstIteration()) {
        Serial.println("Starting playse/pause button handler..");

        pinMode(PAUSE_PLAY_BTN_PIN, INPUT_PULLUP);
    }
    bool is_playing = false;
    bool switch_pause = digitalRead(PAUSE_PLAY_BTN_PIN);

    if(switch_pause) {
        is_playing = !is_playing;

        if(is_playing) {

        }
    }
}


void time_btn_handler() {
    if(time_btn_task.isFirstIteration()) {
        Serial.println("Starting time request handler..");
        
        pinMode(GET_TIME_BTN_PIN, INPUT_PULLUP);
    }

    bool request_time = digitalRead(GET_TIME_BTN_PIN);

    if(request_time) {
        Serial.write(REQUEST_TIME);
        while (Serial.available() <= 0);
        
        time = Serial.read();
    }
}