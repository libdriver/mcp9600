[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9600

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9600/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9600 是一款完全集成的热电偶电动势 (EMF) 到摄氏度转换器，具有集成的冷端补偿功能。MCP9600 支持 8 种热电偶类型（K、J、T、N、S、E、B 和 R）。MCP9600 提供用户可编程寄存器，为各种温度传感应用增加了设计灵活性。这些寄存器允许用户选择设置，例如电池供电应用的低功耗模式、用于快速瞬态温度的可调数字滤波器和四个可单独编程的温度警报输出，可用于检测多个温度区域。MCP9600可用于石油化工热管理、手持测量设备、工业设备热管理、商用和工业烘箱、工业发动机热监测器、温度检测架等。

LibDriver MCP9600是LibDriver推出的MCP9600全功能驱动，该驱动提供单次读取、连续读取、中断模式等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver MCP9600的源文件。

/interface目录包含了LibDriver MCP9600与平台无关的IIC总线模板。

/test目录包含了LibDriver MCP9600驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver MCP9600编程范例。

/doc目录包含了LibDriver MCP9600离线文档。

/datasheet目录包含了MCP9600数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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
### 文档

在线文档: [https://www.libdriver.com/docs/mcp9600/index.html](https://www.libdriver.com/docs/mcp9600/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。