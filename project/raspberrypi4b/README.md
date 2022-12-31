### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(mcp9600 REQUIRED)
```

### 3. MCP9600

#### 3.1 Command Instruction

1. Show mcp9600 chip and driver information.

   ```shell
   mcp9600 (-i | --information)
   ```

2. Show mcp9600 help.

   ```shell
   mcp9600 (-h | --help)
   ```

3. Show mcp9600 pin connections of the current board.

   ```shell
   mcp9600 (-p | --port)
   ```

4. Run mcp9600 register test.

   ```shell
   mcp9600 (-t reg | --test=reg) [--addr=<0 | 1>]
   ```

5. Run mcp9600 read test, num means test times.

   ```shell
   mcp9600 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
   ```

6. Run mcp9600 interrupt test.

   ```shell
   mcp9600 (-t int | --test=int) [--addr=<0 | 1>] [--type=<k | j | t | n | s | e | b | r>]
   ```

7. Run mcp9600 basic read function, num means read times.

   ```shell
   mcp9600 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
   ```

8. Run mcp9600 shot read function, num means read times.

   ```shell
   mcp9600 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
   ```

9. Run mcp9600 interrupt read function, num means read times.

   ```shell
   mcp9600 (-e int | --example=int) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
   ```

#### 3.2 Command Example

```shell
./mcp9600 -i

mcp9600: chip is Microchip MCP9600.
mcp9600: manufacturer is Microchip.
mcp9600: interface is IIC.
mcp9600: driver version is 1.0.
mcp9600: min supply voltage is 2.7V.
mcp9600: max supply voltage is 5.5V.
mcp9600: max current is 2.50mA.
mcp9600: max temperature is 125.0C.
mcp9600: min temperature is -40.0C.
```

```shell
./mcp9600 -p

mcp9600: SCL connected to GPIO3(BCM).
mcp9600: SDA connected to GPIO2(BCM).
mcp9600: INT connected to GPIO17(BCM).
```

```shell
./mcp9600 -t reg --addr=0

mcp9600: chip is Microchip MCP9600.
mcp9600: manufacturer is Microchip.
mcp9600: interface is IIC.
mcp9600: driver version is 1.0.
mcp9600: min supply voltage is 2.7V.
mcp9600: max supply voltage is 5.5V.
mcp9600: max current is 2.50mA.
mcp9600: max temperature is 125.0C.
mcp9600: min temperature is -40.0C.
mcp9600: start register test.
mcp9600: mcp9600_set_addr_pin/mcp9600_get_addr_pin test.
mcp9600: set addr pin ADDRESS_0.
mcp9600: check addr pin ok.
mcp9600: set addr pin ADDRESS_1.
mcp9600: check addr pin ok.
mcp9600: mcp9600_get_device_id_revision test.
mcp9600: device id is 0x40 revision is 0x12.
mcp9600: mcp9600_set_filter_coefficient/mcp9600_get_filter_coefficient test.
mcp9600: set filter coefficient 0.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 1.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 2.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 3.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 4.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 5.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 6.
mcp9600: check filter coefficient ok.
mcp9600: set filter coefficient 7.
mcp9600: check filter coefficient ok.
mcp9600: mcp9600_set_thermocouple_type/mcp9600_get_thermocouple_type test.
mcp9600: set thermocouple type k.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type j.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type t.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type n.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type s.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type e.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type b.
mcp9600: check thermocouple type ok.
mcp9600: set thermocouple type r.
mcp9600: check thermocouple type ok.
mcp9600: mcp9600_set_cold_junction_resolution/mcp9600_get_cold_junction_resolution test.
mcp9600: set cold junction resolution 0.0625.
mcp9600: check cold junction resolution ok.
mcp9600: set cold junction resolution 0.25.
mcp9600: check cold junction resolution ok.
mcp9600: mcp9600_set_adc_resolution/mcp9600_get_adc_resolution test.
mcp9600: set adc resolution 18 bit.
mcp9600: check adc resolution ok.
mcp9600: set adc resolution 16 bit.
mcp9600: check adc resolution ok.
mcp9600: set adc resolution 14 bit.
mcp9600: check adc resolution ok.
mcp9600: set adc resolution 12 bit.
mcp9600: check adc resolution ok.
mcp9600: mcp9600_set_burst_mode_sample/mcp9600_get_burst_mode_sample test.
mcp9600: set burst mode sample 1.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 2.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 4.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 8.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 16.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 32.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 64.
mcp9600: check burst mode sample ok.
mcp9600: set burst mode sample 128.
mcp9600: check burst mode sample ok.
mcp9600: mcp9600_set_mode/mcp9600_get_mode test.
mcp9600: set shutdown mode.
mcp9600: check mode ok.
mcp9600: set burst mode.
mcp9600: check mode ok.
mcp9600: set normal mode.
mcp9600: check mode ok.
mcp9600: mcp9600_set_temperature_maintain_detect/mcp9600_get_temperature_maintain_detect test.
mcp9600: set alert1 temperature maintain detect cold junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert1 temperature maintain detect hot junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert2 temperature maintain detect cold junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert2 temperature maintain detect hot junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert3 temperature maintain detect cold junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert3 temperature maintain detect hot junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert4 temperature maintain detect cold junction.
mcp9600: check temperature maintain detect ok.
mcp9600: set alert4 temperature maintain detect hot junction.
mcp9600: check temperature maintain detect ok.
mcp9600: mcp9600_set_detect_edge/mcp9600_get_detect_edge test.
mcp9600: set alert1 detect edge rising.
mcp9600: check detect edge ok.
mcp9600: set alert1 detect edge falling.
mcp9600: check detect edge ok.
mcp9600: set alert2 detect edge rising.
mcp9600: check detect edge ok.
mcp9600: set alert2 detect edge falling.
mcp9600: check detect edge ok.
mcp9600: set alert3 detect edge rising.
mcp9600: check detect edge ok.
mcp9600: set alert3 detect edge falling.
mcp9600: check detect edge ok.
mcp9600: set alert4 detect edge rising.
mcp9600: check detect edge ok.
mcp9600: set alert4 detect edge falling.
mcp9600: check detect edge ok.
mcp9600: mcp9600_set_active_level/mcp9600_get_active_level test.
mcp9600: set alert1 active level low.
mcp9600: check active level ok.
mcp9600: set alert1 active level high.
mcp9600: check active level ok.
mcp9600: set alert2 active level low.
mcp9600: check active level ok.
mcp9600: set alert2 active level high.
mcp9600: check active level ok.
mcp9600: set alert3 active level low.
mcp9600: check active level ok.
mcp9600: set alert3 active level high.
mcp9600: check active level ok.
mcp9600: set alert4 active level low.
mcp9600: check active level ok.
mcp9600: set alert4 active level high.
mcp9600: check active level ok.
mcp9600: mcp9600_set_interrupt_mode/mcp9600_get_interrupt_mode test.
mcp9600: set alert1 interrupt mode comparator.
mcp9600: check interrupt mode ok.
mcp9600: set alert1 interrupt mode interrupt.
mcp9600: check interrupt mode ok.
mcp9600: set alert2 interrupt mode comparator.
mcp9600: check interrupt mode ok.
mcp9600: set alert2 interrupt mode interrupt.
mcp9600: check interrupt mode ok.
mcp9600: set alert3 interrupt mode comparator.
mcp9600: check interrupt mode ok.
mcp9600: set alert3 interrupt mode interrupt.
mcp9600: check interrupt mode ok.
mcp9600: set alert4 interrupt mode comparator.
mcp9600: check interrupt mode ok.
mcp9600: set alert4 interrupt mode interrupt.
mcp9600: check interrupt mode ok.
mcp9600: mcp9600_set_alert_output/mcp9600_get_alert_output test.
mcp9600: set alert1 disable output.
mcp9600: check alert output ok.
mcp9600: set alert1 enable output.
mcp9600: check alert output ok.
mcp9600: set alert2 disable output.
mcp9600: check alert output ok.
mcp9600: set alert2 enable output.
mcp9600: check alert output ok.
mcp9600: set alert3 disable output.
mcp9600: check alert output ok.
mcp9600: set alert3 enable output.
mcp9600: check alert output ok.
mcp9600: set alert4 disable output.
mcp9600: check alert output ok.
mcp9600: set alert4 enable output.
mcp9600: check alert output ok.
mcp9600: mcp9600_alert_limit_convert_to_register/mcp9600_alert_limit_convert_to_data test.
mcp9600: alert limit 3.8300 to reg 61 and converted to data 3.8125.
mcp9600: mcp9600_set_alert_limit/mcp9600_get_alert_limit test.
mcp9600: set alert1 limit 8700.
mcp9600: check alert limit ok.
mcp9600: set alert2 limit 17800.
mcp9600: check alert limit ok.
mcp9600: set alert3 limit 11600.
mcp9600: check alert limit ok.
mcp9600: set alert4 limit 19400.
mcp9600: check alert limit error.
mcp9600: mcp9600_alert_hysteresis_convert_to_register/mcp9600_alert_hysteresis_convert_to_data test.
mcp9600: alert hysteresis 3.3500 to reg 3 and converted to data 3.0000.
mcp9600: mcp9600_set_alert_hysteresis/mcp9600_get_alert_hysteresis test.
mcp9600: set alert1 hysteresis 74.
mcp9600: check alert hysteresis ok.
mcp9600: set alert2 hysteresis 236.
mcp9600: check alert hysteresis ok.
mcp9600: set alert3 hysteresis 41.
mcp9600: check alert hysteresis ok.
mcp9600: set alert4 hysteresis 205.
mcp9600: check alert hysteresis ok.
mcp9600: mcp9600_get_interrupt/mcp9600_clear_interrupt test.
mcp9600: alert1 get interrupt 0x00.
mcp9600: check clear interrupt ok.
mcp9600: alert2 get interrupt 0x00.
mcp9600: check clear interrupt ok.
mcp9600: alert3 get interrupt 0x00.
mcp9600: check clear interrupt ok.
mcp9600: alert4 get interrupt 0x00.
mcp9600: check clear interrupt ok.
mcp9600: mcp9600_get_status_input_range test.
mcp9600: check status input range within.
mcp9600: mcp9600_get_alert_status test.
mcp9600: check alert status 1.
mcp9600: check alert status 1.
mcp9600: check alert status 1.
mcp9600: check alert status 1.
mcp9600: mcp9600_get_status_burst_complete_flag/mcp9600_clear_status_burst_complete_flag test.
mcp9600: check burst complete flag status 0.
mcp9600: mcp9600_get_status_temperature_update_flag/mcp9600_clear_status_temperature_update_flag test.
mcp9600: check temperature update flag status 1.
mcp9600: finish register test.
```

```shell
./mcp9600 -t read --addr=0 --times=3 --type=k

mcp9600: chip is Microchip MCP9600.
mcp9600: manufacturer is Microchip.
mcp9600: interface is IIC.
mcp9600: driver version is 1.0.
mcp9600: min supply voltage is 2.7V.
mcp9600: max supply voltage is 5.5V.
mcp9600: max current is 2.50mA.
mcp9600: max temperature is 125.0C.
mcp9600: min temperature is -40.0C.
mcp9600: start read test.
mcp9600: start continuous read test.
mcp9600: hot 47.69 delta 22.38 cold 111.00.
mcp9600: hot 29.94 delta 1.94 cold 111.50.
mcp9600: hot 29.94 delta 1.94 cold 111.75.
mcp9600: start single read test.
mcp9600: hot 29.94 delta 1.94 cold 111.75.
mcp9600: hot 29.94 delta 1.94 cold 111.75.
mcp9600: hot 29.94 delta 1.94 cold 111.75.
mcp9600: set cold junction resolution 0.25.
mcp9600: hot 29.94 delta 1.94 cold 27.94.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: set adc resolution 18 bit.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 40 uv is 80.000000.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 40 uv is 80.000000.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 40 uv is 80.000000.
mcp9600: set adc resolution 16 bit.
mcp9600: hot 29.88 delta 1.94 cold 28.00.
mcp9600: raw is 10 uv is 80.000000.
mcp9600: hot 16.06 delta 1.94 cold 28.00.
mcp9600: raw is 10 uv is 80.000000.
mcp9600: hot 29.94 delta 1.94 cold 16.06.
mcp9600: raw is 0 uv is 0.000000.
mcp9600: set adc resolution 14 bit.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 2 uv is 64.000000.
mcp9600: hot 16.06 delta 1.94 cold 16.06.
mcp9600: raw is 2 uv is 64.000000.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 2 uv is 64.000000.
mcp9600: set adc resolution 12 bit.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 0 uv is 0.000000.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 0 uv is 0.000000.
mcp9600: hot 29.81 delta 1.94 cold 27.75.
mcp9600: raw is 0 uv is 0.000000.
mcp9600: set burst mode sample 1.
mcp9600: hot 30.50 delta 2.69 cold 27.75.
mcp9600: hot 30.75 delta 2.94 cold 27.75.
mcp9600: hot 28.81 delta 1.00 cold 27.75.
mcp9600: set burst mode sample 2.
mcp9600: hot 29.94 delta 2.19 cold 27.75.
mcp9600: hot 30.75 delta 3.00 cold 27.75.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: set burst mode sample 4.
mcp9600: hot 30.94 delta 3.06 cold 27.75.
mcp9600: hot 30.75 delta 3.00 cold 27.75.
mcp9600: hot 31.00 delta 3.00 cold 28.00.
mcp9600: set burst mode sample 8.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: set burst mode sample 16.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: hot 30.94 delta 3.06 cold 27.75.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: set burst mode sample 32.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: set burst mode sample 64.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: hot 31.19 delta 3.06 cold 28.00.
mcp9600: hot 31.06 delta 3.06 cold 28.00.
mcp9600: set burst mode sample 128.
mcp9600: hot 31.19 delta 3.06 cold 28.00.
mcp9600: hot 31.19 delta 3.06 cold 28.00.
mcp9600: hot 31.19 delta 3.06 cold 28.00.
mcp9600: set filter coefficient 0.
mcp9600: hot 31.12 delta 3.12 cold 28.00.
mcp9600: hot 31.12 delta 3.12 cold 28.00.
mcp9600: hot 31.12 delta 3.12 cold 28.00.
mcp9600: set filter coefficient 1.
mcp9600: hot 30.06 delta 2.06 cold 28.00.
mcp9600: hot 30.31 delta 2.31 cold 28.00.
mcp9600: hot 30.31 delta 2.31 cold 28.00.
mcp9600: set filter coefficient 2.
mcp9600: hot 29.25 delta 1.25 cold 28.00.
mcp9600: hot 29.56 delta 1.69 cold 27.75.
mcp9600: hot 29.81 delta 1.88 cold 28.00.
mcp9600: set filter coefficient 3.
mcp9600: hot 28.69 delta 0.69 cold 28.00.
mcp9600: hot 29.12 delta 1.12 cold 28.00.
mcp9600: hot 29.38 delta 1.38 cold 28.00.
mcp9600: set filter coefficient 4.
mcp9600: hot 28.38 delta 0.31 cold 28.00.
mcp9600: hot 28.69 delta 0.62 cold 28.00.
mcp9600: hot 28.88 delta 0.81 cold 28.00.
mcp9600: set filter coefficient 5.
mcp9600: hot 28.19 delta 0.19 cold 28.00.
mcp9600: hot 28.19 delta 0.19 cold 28.00.
mcp9600: hot 28.19 delta 0.19 cold 28.00.
mcp9600: set filter coefficient 6.
mcp9600: hot 28.06 delta 0.00 cold 28.00.
mcp9600: hot 28.06 delta 0.00 cold 28.00.
mcp9600: hot 28.06 delta 0.00 cold 28.00.
mcp9600: set filter coefficient 7.
mcp9600: hot 28.06 delta 0.00 cold 28.00.
mcp9600: hot 28.06 delta 0.00 cold 28.00.
mcp9600: hot 28.06 delta 0.00 cold 28.00.
mcp9600: finish read test.
```

```shell
./mcp9600 -t int --addr=0 --type=k

mcp9600: chip is Microchip MCP9600.
mcp9600: manufacturer is Microchip.
mcp9600: interface is IIC.
mcp9600: driver version is 1.0.
mcp9600: min supply voltage is 2.7V.
mcp9600: max supply voltage is 5.5V.
mcp9600: max current is 2.50mA.
mcp9600: max temperature is 125.0C.
mcp9600: min temperature is -40.0C.
mcp9600: start interrupt test.
mcp9600: hot 28.06 delta 0.00 cold 112.00.
mcp9600: 49/50.
mcp9600: hot 29.50 delta 1.38 cold 112.25.
mcp9600: 48/50.
mcp9600: hot 29.56 delta 1.38 cold 112.50.
mcp9600: 47/50.
mcp9600: hot 29.56 delta 1.38 cold 112.50.
mcp9600: 46/50.
mcp9600: hot 29.56 delta 1.31 cold 112.50.
mcp9600: 45/50.
mcp9600: hot 29.56 delta 1.38 cold 112.50.
mcp9600: 44/50.
mcp9600: hot 29.56 delta 1.31 cold 112.50.
mcp9600: 43/50.
mcp9600: hot 29.56 delta 1.31 cold 112.50.
mcp9600: 42/50.
mcp9600: hot 29.56 delta 1.31 cold 112.50.
mcp9600: 41/50.
mcp9600: hot 29.56 delta 1.31 cold 112.50.
mcp9600: 40/50.
mcp9600: hot 29.56 delta 1.31 cold 112.50.
mcp9600: 39/50.
mcp9600: hot 29.62 delta 1.44 cold 112.50.
mcp9600: 38/50.
mcp9600: hot 29.75 delta 1.56 cold 112.50.
mcp9600: 37/50.
mcp9600: hot 33.06 delta 5.00 cold 112.50.
mcp9600: find interrupt and please check alert1 - alert4.
mcp9600: finish interrupt test.
```

```shell
./mcp9600 -e read --addr=0 --times=3 --type=k

mcp9600: 3/3 hot 33.38 delta 5.31 cold 112.50.
mcp9600: 2/3 hot 30.12 delta 1.94 cold 112.50.
mcp9600: 1/3 hot 30.12 delta 1.94 cold 112.50.
```

```shell
./mcp9600 -e shot --addr=0 --times=3 --type=k

mcp9600: 3/3 hot 30.00 delta 1.75 cold 112.50.
mcp9600: 2/3 hot 29.94 delta 1.75 cold 112.50.
mcp9600: 1/3 hot 29.94 delta 1.75 cold 112.75.
```

```shell
./mcp9600 -e int --addr=0 --times=3 --type=k

mcp9600: 3/3 hot 29.94 delta 1.75 cold 112.75.
mcp9600: find interrupt.
```

```shell
./mcp9600 -h

Usage:
  mcp9600 (-i | --information)
  mcp9600 (-h | --help)
  mcp9600 (-p | --port)
  mcp9600 (-t reg | --test=reg) [--addr=<0 | 1>]
  mcp9600 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
  mcp9600 (-t int | --test=int) [--addr=<0 | 1>] [--type=<k | j | t | n | s | e | b | r>]
  mcp9600 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
  mcp9600 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]
  mcp9600 (-e int | --example=int) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]

Options:
      --addr=<0 | 1>               Set the addr pin.([default: 0])
  -e <read | shot | int>, --example=<read | shot | int>
                                   Run the driver example.
  -h, --help                       Show the help.
  -i, --information                Show the chip information.
  -p, --port                       Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                                   Run the driver test.
      --times=<num>                Set the running times.([default: 3])
      --type=<k | j | t | n | s | e | b | r>
                                   Set the thermocouple type.([default: k])
```

