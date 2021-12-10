void lcd_output_callback();     // output info to LCD
void noise_monitor_callback();  // get noise level in room
void gas_monitor_callback();    // get CO2 level in room
void volume_ctl_callback();     // handle speaker volume with gestures
void weather_btn_handler();     // handle the weather button presses
void play_pause_btn_handler();  // handle the play/pause button presses
void time_btn_handler();        // handle the time button presses

void init_tasks(); // run task setup
void run_tasks();  // loop tasks