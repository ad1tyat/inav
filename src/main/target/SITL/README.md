## SITL in Gazebo 9 with ArduCopterPlugin (running from docker)
SITL (software in the loop) simulator allows you to run betaflight (at least this branch) without any hardware.
Currently only tested on Ubuntu 18.04.5 LTS based on the docker image nvidia/cudagl:11.2.2-devel-ubuntu18.04. On top of this image, it was installed ROS Melodic Gazebo 9 and [this ArduCopter Plugin](https://github.com/SwiftGust/ardupilot_gazebo.git). You can find the image on [dockerhub](https://hub.docker.com/r/ricardodeazambuja/gazebo9_cudagl_sitl).

### Build betaflight
Before building your own version, you can change some parameters inside `src/main/target/SITL/target.h` (E.g. `SITL_UPDATE_TIMEOUT` or `SITL_TICK_INTERVAL`).  
From the base (root?) of this repository, just run `compile_sitl.sh`.

### Settings
This branch already an executable file (`betaflight_SITL.elf`) with the configuration (`eeprom.bin`) inside `obj/main`, but you can still modify it using [betaflight-configurator](https://github.com/betaflight/betaflight-configurator/releases).

Start betaflight: `cd  obj/main/` and `./betaflight_SITL.elf`.

In `Configuration` tab:
1. `ESC/Motor`: `PWM`, **enable** `Motor PWM speed Separated from PID speed`.
2. `PID loop frequency` as high as it can (8kHz?).
3. Put a new name on `Craft name` to check confirm the settings were saved.
4. Click `Save and Reboot`.

In `Modes` tab:
1. Add `ARM` on `AUX1` from 1750 to 2100.
2. Add `ANGLE` on `AUX2` from 900 to 1200.  
3. Click `Save`.

**Don't click on the `Video Transmitter` tab...**

In `Blackbox` tab:  
1. Set `Blackbox logging device` to `No logging`.  
2. Click `Save and Reboot`.

You can use the `Motors` tab to check if betaflight is communicating with the simulator.

### Start and run
1. Start docker: `run_docker_sim.sh`
2. Start gazebo: `gazebo --verbose ./iris_arducopter_demo.world`
3. Start betaflight: `cd  obj/main/` and `./betaflight_SITL.elf`.`cd  obj/main/` and `./betaflight_SITL.elf`.
3. Connect to `tcp://127.0.0.1:5761` using [YAMSPy](https://github.com/thecognifly/YAMSPy) and fly/test.

### Notes
* betaflight to gazebo	`udp://127.0.0.1:9002`
* gazebo to betaflight	`udp://127.0.0.1:9003`

You can test the communication using [netcat](https://www.digitalocean.com/community/tutorials/how-to-use-netcat-to-establish-and-test-tcp-and-udp-connections):  
1. Create a UDP server listening on port 9003: `nc -u -k -l -p 9003`
2. Connect to port 9002: `nc -u 127.0.0.1 9002`

UART1 will bind on `tcp://127.0.0.1:5761`.

`eeprom.bin`, size 8192 Byte, is for config saving.
size can be changed in `src/main/target/SITL/pg.ld` >> `__FLASH_CONFIG_Size`

#### Gazebo speed / precision
Open the file `/home/sitl_user/ardupilot_gazebo/iris_arducopter_demo.world` and change `real_time_update_rate` to a value that fits your system.  
Currently it's:  `<real_time_update_rate>400</real_time_update_rate>`
See more details [here](http://gazebosim.org/tutorials?tut=modifying_world&cat=build_world#PhysicsProperties) for detail.
`max_step_size` should NOT higher than `0.0025` as I tested. A smaller value means a more accurate simulation in the expense of higher CPU load.
