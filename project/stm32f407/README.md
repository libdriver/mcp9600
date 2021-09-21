### 1. chip

#### 1.1 chip info

chip name : STM32F407ZGT6.

extern oscillator : 8MHz.

uart pin: TX/RX PA9/PA10.

iic pin: SCL/SDA PB8/PB9.

### 2. shell

#### 2.1 shell parameter

baud rate: 115200

data bits : 8

stop bits: 1

parity: none

flow control: none

### 3. mcp9600

#### 3.1 command Instruction

​          mcp9600 is a basic command which can test all mcp9600 driver function:

​          -i        show mcp9600 chip and driver information.

​          -h       show mcp9600 help.

​          -p       show mcp9600 pin connections of the current board.

​          -t (reg -a (0 | 1) | read <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r) | interrupt -a (0 | 1) -type (k | j | t | n | s | e | b | r))           

​          -t reg -a (0 | 1)       run mcp9600 register test.

​          -t read <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)         run mcp9600 read test. times means test times.

​          -t interrupt -a (0 | 1) -type (k | j | t | n | s | e | b | r)         run mcp9600 interrupt test.

​          -c (basic <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r) | shot <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r) | interrupt <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r))

​          -c basic <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)         run mcp9600 basic read function.times means read times.

​          -c shot <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)         run mcp9600 shot read function.times means read times.

​          -c interrupt <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)         run mcp9600 interrupt read function.times means read times.

#### 3.2 command example

```shell
mcp9600 -i

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
mcp9600 -p

mcp9600: SCL connected to GPIOB PIN8.
mcp9600: SDA connected to GPIOB PIN9.
mcp9600: INT connected to GPIOB PIN0.
```

```shell
mcp9600 -t reg -a 0

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
mcp9600: alert limit 7.6800 to reg 122 and converted to data 7.6250.
mcp9600: mcp9600_set_alert_limit/mcp9600_get_alert_limit test.
mcp9600: set alert1 limit 6800.
mcp9600: check alert limit ok.
mcp9600: set alert2 limit 1000.
mcp9600: check alert limit ok.
mcp9600: set alert3 limit 16100.
mcp9600: check alert limit ok.
mcp9600: set alert4 limit 18400.
mcp9600: check alert limit ok.
mcp9600: mcp9600_alert_hysteresis_convert_to_register/mcp9600_alert_hysteresis_convert_to_data test.
mcp9600: alert hysteresis 7.4100 to reg 7 and converted to data 7.0000.
mcp9600: mcp9600_set_alert_hysteresis/mcp9600_get_alert_hysteresis test.
mcp9600: set alert1 hysteresis 200.
mcp9600: check alert hysteresis ok.
mcp9600: set alert2 hysteresis 127.
mcp9600: check alert hysteresis ok.
mcp9600: set alert3 hysteresis 19.
mcp9600: check alert hysteresis ok.
mcp9600: set alert4 hysteresis 196.
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
mcp9600: check burst complete flag status 1.
mcp9600: mcp9600_get_status_temperature_update_flag/mcp9600_clear_status_temperature_update_flag test.
mcp9600: check temperature update flag status 1.
mcp9600: finish register test.
```

```shell
mcp9600 -t read 5 -a 0 -type k

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
mcp9600: hot 26.56 delta 0.00 cold 106.00.
mcp9600: hot 26.56 delta 0.06 cold 105.25.
mcp9600: hot 26.56 delta 0.06 cold 105.50.
mcp9600: hot 26.56 delta 0.06 cold 105.50.
mcp9600: hot 26.56 delta 0.06 cold 105.50.
mcp9600: start single read test.
mcp9600: hot 30.56 delta 4.12 cold 105.75.
mcp9600: hot 32.31 delta 6.00 cold 105.50.
mcp9600: hot 33.44 delta 7.12 cold 105.50.
mcp9600: hot 33.94 delta 7.62 cold 105.75.
mcp9600: hot 34.19 delta 7.88 cold 105.50.
mcp9600: set cold junction resolution 0.25.
mcp9600: hot 34.19 delta 7.88 cold 26.38.
mcp9600: hot 34.31 delta 8.12 cold 26.25.
mcp9600: hot 34.31 delta 8.19 cold 26.25.
mcp9600: hot 34.56 delta 8.25 cold 26.50.
mcp9600: hot 34.69 delta 8.31 cold 26.50.
mcp9600: set adc resolution 18 bit.
mcp9600: hot 34.75 delta 8.38 cold 26.50.
mcp9600: raw is 171 uv is 342.000000.
mcp9600: hot 34.62 delta 8.38 cold 26.25.
mcp9600: raw is 172 uv is 344.000000.
mcp9600: hot 34.62 delta 8.44 cold 26.25.
mcp9600: raw is 173 uv is 346.000000.
mcp9600: hot 34.69 delta 8.44 cold 26.25.
mcp9600: raw is 173 uv is 346.000000.
mcp9600: hot 34.75 delta 8.50 cold 26.25.
mcp9600: raw is 174 uv is 348.000000.
mcp9600: set adc resolution 16 bit.
mcp9600: hot 34.81 delta 8.56 cold 26.25.
mcp9600: raw is 43 uv is 344.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.50.
mcp9600: raw is 44 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 44 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 44 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 44 uv is 352.000000.
mcp9600: set adc resolution 14 bit.
mcp9600: hot 34.94 delta 8.62 cold 26.50.
mcp9600: raw is 11 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 11 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 11 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 11 uv is 352.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 11 uv is 352.000000.
mcp9600: set adc resolution 12 bit.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 2 uv is 256.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 2 uv is 256.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 2 uv is 256.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 2 uv is 256.000000.
mcp9600: hot 34.88 delta 8.62 cold 26.25.
mcp9600: raw is 2 uv is 256.000000.
mcp9600: set burst mode sample 1.
mcp9600: hot 35.31 delta 9.12 cold 26.25.
mcp9600: hot 35.44 delta 9.31 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: set burst mode sample 2.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: set burst mode sample 4.
mcp9600: hot 35.50 delta 9.38 cold 26.25.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: hot 35.62 delta 9.38 cold 26.25.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: set burst mode sample 8.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: hot 35.75 delta 9.38 cold 26.50.
mcp9600: set burst mode sample 16.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 36.00 delta 9.38 cold 26.75.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: set burst mode sample 32.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 36.00 delta 9.38 cold 26.75.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: set burst mode sample 64.
mcp9600: hot 35.88 delta 9.38 cold 26.50.
mcp9600: hot 32.81 delta 6.38 cold 26.50.
mcp9600: hot 32.94 delta 6.38 cold 26.75.
mcp9600: hot 32.81 delta 6.38 cold 26.50.
mcp9600: hot 29.69 delta 3.19 cold 26.50.
mcp9600: set burst mode sample 128.
mcp9600: hot 29.81 delta 3.19 cold 26.75.
mcp9600: hot 29.81 delta 3.19 cold 26.75.
mcp9600: hot 29.81 delta 3.19 cold 26.75.
mcp9600: hot 26.88 delta 0.06 cold 26.75.
mcp9600: hot 26.75 delta 0.06 cold 26.75.
mcp9600: set filter coefficient 0.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: set filter coefficient 1.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: hot 26.56 delta 0.00 cold 26.50.
mcp9600: set filter coefficient 2.
mcp9600: hot 26.31 delta 0.00 cold 26.25.
mcp9600: hot 26.31 delta 0.00 cold 26.25.
mcp9600: hot 28.38 delta 2.00 cold 26.50.
mcp9600: hot 31.00 delta 4.69 cold 26.25.
mcp9600: hot 32.06 delta 5.69 cold 26.50.
mcp9600: set filter coefficient 3.
mcp9600: hot 32.75 delta 6.31 cold 26.50.
mcp9600: hot 32.75 delta 6.31 cold 26.25.
mcp9600: hot 32.75 delta 6.31 cold 26.50.
mcp9600: hot 32.75 delta 6.31 cold 26.50.
mcp9600: hot 33.81 delta 7.50 cold 26.25.
mcp9600: set filter coefficient 4.
mcp9600: hot 33.06 delta 6.62 cold 26.50.
mcp9600: hot 33.62 delta 7.19 cold 26.50.
mcp9600: hot 34.00 delta 7.62 cold 26.25.
mcp9600: hot 34.25 delta 8.00 cold 26.25.
mcp9600: hot 34.00 delta 7.69 cold 26.50.
mcp9600: set filter coefficient 5.
mcp9600: hot 32.50 delta 6.31 cold 26.25.
mcp9600: hot 32.75 delta 6.62 cold 26.25.
mcp9600: hot 33.00 delta 6.88 cold 26.25.
mcp9600: hot 33.25 delta 7.12 cold 26.25.
mcp9600: hot 33.50 delta 7.38 cold 26.50.
mcp9600: set filter coefficient 6.
mcp9600: hot 35.81 delta 9.44 cold 26.50.
mcp9600: hot 35.81 delta 9.44 cold 26.50.
mcp9600: hot 35.81 delta 9.44 cold 26.25.
mcp9600: hot 35.81 delta 9.44 cold 26.25.
mcp9600: hot 35.81 delta 9.44 cold 26.25.
mcp9600: set filter coefficient 7.
mcp9600: hot 35.56 delta 9.44 cold 26.25.
mcp9600: hot 35.56 delta 9.44 cold 26.50.
mcp9600: hot 35.56 delta 9.44 cold 26.25.
mcp9600: hot 35.56 delta 9.44 cold 26.25.
mcp9600: hot 35.56 delta 9.44 cold 26.25.
mcp9600: finish read test.
```

```shell
mcp9600 -t interrupt -a 0 -type k

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
mcp9600: hot 26.31 delta 0.00 cold 105.00.
mcp9600: 49/50.
mcp9600: hot 27.56 delta 0.94 cold 106.00.
mcp9600: 48/50.
mcp9600: hot 27.44 delta 0.88 cold 105.75.
mcp9600: 47/50.
mcp9600: hot 27.50 delta 0.88 cold 106.00.
mcp9600: 46/50.
mcp9600: hot 31.12 delta 4.62 cold 106.00.
mcp9600: find interrupt and please check alert1 - alert4.
mcp9600: finish interrupt test.
```

```shell
mcp9600 -c basic 3 -a 0 -type k

mcp9600: 3 / 3 hot 26.88 delta 0.19 cold 106.25.
mcp9600: 2 / 3 hot 26.81 delta 0.19 cold 106.00.
mcp9600: 1 / 3 hot 26.81 delta 0.19 cold 106.25.
```

```shell
mcp9600 -c shot 3 -a 0 -type k

mcp9600: 3 / 3 hot 26.81 delta 0.19 cold 106.00.
mcp9600: 2 / 3 hot 26.81 delta 0.19 cold 106.00.
mcp9600: 1 / 3 hot 26.81 delta 0.19 cold 106.00.
```

```shell
mcp9600 -c interrupt 50 -a 0 -type k

mcp9600: 50 / 50 hot 28.81 delta 1.75 cold 107.75.
mcp9600: find interrupt.
```

```shell
mcp9600 -h

mcp9600 -i
	show mcp9600 chip and driver information.
mcp9600 -h
	show mcp9600 help.
mcp9600 -p
	show mcp9600 pin connections of the current board.
mcp9600 -t reg -a (0|1)
	run mcp9600 register test.
mcp9600 -t read <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)
	run mcp9600 read test.times means test times..
mcp9600 -t interrupt -a (0 | 1) -type (k | j | t | n | s | e | b | r)
	run mcp9600 interrupt test.
mcp9600 -c basic <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)
	run mcp9600 basic read function.times means read times.
mcp9600 -c shot <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)
	run mcp9600 shot read function.times means read times.
mcp9600 -c interrupt <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)
	run mcp9600 interrupt read function.times means read times.
```

