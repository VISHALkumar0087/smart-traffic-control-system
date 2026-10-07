# Smart Traffic Control System

Sensor-driven traffic signal manager that adjusts green-light time based on vehicle density at a 4-way junction.

## How it works
1. Each lane has a (simulated) sensor that reports the vehicle count.
2. Priority = `vehicles + 3 * steps_waited`. The highest-priority lane gets green.
3. Green time = `10 + vehicles` seconds, limited to 10..60 s.
4. Vehicles pass at 1 per 2 s. All other lanes' waiting counters increase, so no lane starves.

## Run
```bash
g++ -std=c++17 traffic.cpp -o traffic
./traffic 12        # number of signal steps (Windows: traffic.exe 12)
```

## Moving to real hardware
The `vehicles` values can come from IR or ultrasonic sensors on an Arduino. The same `greenTime()` and `priority()` logic runs on the board, and the chosen lane drives LEDs through output pins.

## Tech
C++, embedded-systems concepts, sensor logic, scheduling algorithm
