[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9600

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9600/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The  MCP9600 is a fully integrated thermocouple  Electromotive Force (EMF) to degree Celsius converter with integrated Cold-Junction compensation. The MCP9600 supports 8 thermocouple types (K, J, T, N, S, E, B and R). The MCP9600 provides user-programmable registers, adding design flexibility for various temperature sensing applications. The registers allow user-selectable settings, such as low-power modes for battery-powered applications, an adjustable digital filter for fast transient temperatures and four individually programmable temperature alert outputs, which can be used to detect multiple temperature zones.It can be used in petrochemical thermal management, hand-held measurement equipment, industrial equipment thermal management, commercial and industrial ovens, industrial engine thermal monitor, temperature detection racks and so on.

LibDriver MCP9600 is a full function driver of MCP9600 launched by LibDriver.It provides shot reading, continuous reading, interrupt reading mode and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MCP9600 source files.

/interface includes LibDriver MCP9600 IIC platform independent template.

/test includes LibDriver MCP9600 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver MCP9600 sample code.

/doc includes LibDriver MCP9600 offline document.

/datasheet includes MCP9600 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_mcp9600_basic.h"

uint8_t res;
uint16_t i, times;

/* basic init */
res = mcp9600_basic_init(MCP9600_ADDRESS_0, MCP9600_THERMOCOUPLE_TYPE_K);
if (res != 0)
{
    return 1;
}

times = 3;
for (i = 0; i < times; i++)
{
    int16_t hot_raw;
    float hot_s;
    int16_t delta_raw;
    float delta_s;
    int16_t cold_raw;
    float cold_s;

    /* basic read */
    res = mcp9600_basic_read((int16_t *)&hot_raw, (float *)&hot_s,
                             (int16_t *)&delta_raw, (float *)&delta_s,
                             (int16_t *)&cold_raw, (float *)&cold_s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: basic read failed.\n");
        (void)mcp9600_basic_deinit();

        return 1;
    }

    /* 1000 ms */
    mcp9600_interface_delay_ms(1000);

    /* print */
    mcp9600_interface_debug_print("mcp9600: %d / %d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
}

/* deinit */
res = mcp9600_basic_deinit();
if (res != 0)
{
    return 1;
}
else
{
    return 0;
}
```

#### example shot

```C
#include "driver_mcp9600_shot.h"

uint8_t res;
uint16_t i, times;

/* shot init */
res = mcp9600_shot_init(MCP9600_ADDRESS_0, MCP9600_THERMOCOUPLE_TYPE_K);
if (res != 0)
{
    return 1;
}

times = 3;
for (i = 0; i < times; i++)
{
    int16_t hot_raw;
    float hot_s;
    int16_t delta_raw;
    float delta_s;
    int16_t cold_raw;
    float cold_s;

    /* shot read */
    res = mcp9600_shot_read((int16_t *)&hot_raw, (float *)&hot_s,
                            (int16_t *)&delta_raw, (float *)&delta_s,
                            (int16_t *)&cold_raw, (float *)&cold_s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: shot read failed.\n");
        (void)mcp9600_shot_deinit();

        return 1;
    }

    /* 1000 ms */
    mcp9600_interface_delay_ms(1000);

    /* print */
    mcp9600_interface_debug_print("mcp9600: %d / %d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
}

/* deinit */
res = mcp9600_shot_deinit();
if (res != 0)
{
    return 1;
}
else
{
    return 0;
}
```
#### example interrupt

```C
#include "driver_mcp9600_interrupt.h"

uint8_t res;
uint16_t i, times;
uint8_t g_flag;

void gpio_callback(uint16_t pin)
{
    g_flag = 1;
}

/* gpio init */
g_flag = 0;
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}

/* interrupt init */
res = mcp9600_interrupt_init(MCP9600_ADDRESS_0, MCP9600_THERMOCOUPLE_TYPE_K);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

/* clear all */
res = mcp9600_interrupt_clear(MCP9600_ALERT_1);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}
res = mcp9600_interrupt_clear(MCP9600_ALERT_2);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}
res = mcp9600_interrupt_clear(MCP9600_ALERT_3);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}
res = mcp9600_interrupt_clear(MCP9600_ALERT_4);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}

times = 3;
for (i = 0; i < times; i++)
{
    int16_t hot_raw;
    float hot_s;
    int16_t delta_raw;
    float delta_s;
    int16_t cold_raw;
    float cold_s;

    /* interrupt read */
    res = mcp9600_interrupt_read((int16_t *)&hot_raw, (float *)&hot_s,
                                 (int16_t *)&delta_raw, (float *)&delta_s,
                                 (int16_t *)&cold_raw, (float *)&cold_s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: interrupt read failed.\n");
        (void)mcp9600_interrupt_deinit();
        (void)gpio_interrupt_deinit();

        return 1;
    }

    /* 1000 ms */
    mcp9600_interface_delay_ms(1000);

    /* print */
    mcp9600_interface_debug_print("mcp9600: %d / %d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);

    if (g_flag != 0)
    {
        mcp9600_interface_debug_print("mcp9600: find interrupt.\n");

        break;
    }
}

/* deinit */
res = mcp9600_interrupt_deinit();
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}
else
{
    (void)gpio_interrupt_deinit();

    return 0;
}
```
### Document

Online documents: [https://www.libdriver.com/docs/mcp9600/index.html](https://www.libdriver.com/docs/mcp9600/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.