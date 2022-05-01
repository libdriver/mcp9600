/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-09-12
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/09/12  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mcp9600_register_test.h"
#include "driver_mcp9600_read_test.h"
#include "driver_mcp9600_interrupt_test.h"
#include "driver_mcp9600_basic.h"
#include "driver_mcp9600_shot.h"
#include "driver_mcp9600_interrupt.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
uint16_t g_len;            /**< uart buffer length */
uint8_t g_flag;            /**< interrupt flag */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        g_flag = 1;
    }
}

/**
 * @brief     mcp9600 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t mcp9600(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            mcp9600_info_t info;
            
            /* print mcp9600 info */
            mcp9600_info(&info);
            mcp9600_interface_debug_print("mcp9600: chip is %s.\n", info.chip_name);
            mcp9600_interface_debug_print("mcp9600: manufacturer is %s.\n", info.manufacturer_name);
            mcp9600_interface_debug_print("mcp9600: interface is %s.\n", info.interface);
            mcp9600_interface_debug_print("mcp9600: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
            mcp9600_interface_debug_print("mcp9600: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            mcp9600_interface_debug_print("mcp9600: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            mcp9600_interface_debug_print("mcp9600: max current is %0.2fmA.\n", info.max_current_ma);
            mcp9600_interface_debug_print("mcp9600: max temperature is %0.1fC.\n", info.temperature_max);
            mcp9600_interface_debug_print("mcp9600: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            mcp9600_interface_debug_print("mcp9600: SCL connected to GPIOB PIN8.\n");
            mcp9600_interface_debug_print("mcp9600: SDA connected to GPIOB PIN9.\n");
            mcp9600_interface_debug_print("mcp9600: INT connected to GPIOB PIN0.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show mcp9600 help */
            
            help:
            
            mcp9600_interface_debug_print("mcp9600 -i\n\tshow mcp9600 chip and driver information.\n");
            mcp9600_interface_debug_print("mcp9600 -h\n\tshow mcp9600 help.\n");
            mcp9600_interface_debug_print("mcp9600 -p\n\tshow mcp9600 pin connections of the current board.\n");
            mcp9600_interface_debug_print("mcp9600 -t reg -a (0|1)\n\trun mcp9600 register test.\n");
            mcp9600_interface_debug_print("mcp9600 -t read <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)\n\trun mcp9600 read test."
                                          "times means test times..\n");
            mcp9600_interface_debug_print("mcp9600 -t interrupt -a (0 | 1) -type (k | j | t | n | s | e | b | r)\n\t"
                                          "run mcp9600 interrupt test.\n");
            mcp9600_interface_debug_print("mcp9600 -c basic <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)\n\t"
                                          "run mcp9600 basic read function.times means read times.\n");
            mcp9600_interface_debug_print("mcp9600 -c shot <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)\n\t"
                                          "run mcp9600 shot read function.times means read times.\n");
            mcp9600_interface_debug_print("mcp9600 -c interrupt <times> -a (0 | 1) -type (k | j | t | n | s | e | b | r)\n\t"
                                          "run mcp9600 interrupt read function.times means read times.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-a", argv[3]) == 0)
                {
                    mcp9600_address_t addr_pin;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr_pin = MCP9600_ADDRESS_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr_pin = MCP9600_ADDRESS_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    return mcp9600_register_test(addr_pin);
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 7)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("interrupt", argv[2]) == 0)
            {
                if (strcmp("-a", argv[3]) == 0)
                {
                    if (strcmp("-type", argv[5]) == 0)
                    {
                        mcp9600_address_t addr_pin;
                        mcp9600_thermocouple_type_t type;
                        
                        if (strcmp("0", argv[4]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_0;
                        }
                        else if (strcmp("1", argv[4]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_1;
                        }
                        else
                        {
                            return 5;
                        }
                        if (strcmp("k", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_K;
                        }
                        else if (strcmp("j", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_J;
                        }
                        else if (strcmp("t", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_T;
                        }
                        else if (strcmp("n", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_N;
                        }
                        else if (strcmp("s", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_S;
                        }
                        else if (strcmp("e", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_E;
                        }
                        else if (strcmp("b", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_B;
                        }
                        else if (strcmp("r", argv[6]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_R;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        return mcp9600_interrupt_test(addr_pin, type);
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 8)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("read", argv[2]) == 0)
            {
                if (strcmp("-a", argv[4]) == 0)
                {
                    if (strcmp("-type", argv[6]) == 0)
                    {
                        mcp9600_address_t addr_pin;
                        mcp9600_thermocouple_type_t type;
                        
                        if (strcmp("0", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_0;
                        }
                        else if (strcmp("1", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_1;
                        }
                        else
                        {
                            return 5;
                        }
                        if (strcmp("k", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_K;
                        }
                        else if (strcmp("j", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_J;
                        }
                        else if (strcmp("t", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_T;
                        }
                        else if (strcmp("n", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_N;
                        }
                        else if (strcmp("s", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_S;
                        }
                        else if (strcmp("e", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_E;
                        }
                        else if (strcmp("b", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_B;
                        }
                        else if (strcmp("r", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_R;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        return mcp9600_read_test(addr_pin, type, atoi(argv[3]));
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-a", argv[4]) == 0)
                {
                    if (strcmp("-type", argv[6]) == 0)
                    {
                        uint8_t res;
                        uint16_t i, times;
                        mcp9600_address_t addr_pin;
                        mcp9600_thermocouple_type_t type;
                        
                        if (strcmp("0", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_0;
                        }
                        else if (strcmp("1", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_1;
                        }
                        else
                        {
                            return 5;
                        }
                        if (strcmp("k", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_K;
                        }
                        else if (strcmp("j", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_J;
                        }
                        else if (strcmp("t", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_T;
                        }
                        else if (strcmp("n", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_N;
                        }
                        else if (strcmp("s", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_S;
                        }
                        else if (strcmp("e", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_E;
                        }
                        else if (strcmp("b", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_B;
                        }
                        else if (strcmp("r", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_R;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        times = (uint16_t)atoi(argv[3]);
                        
                        /* basic init */
                        res = mcp9600_basic_init(addr_pin, type);
                        if (res != 0)
                        {
                            return 1;
                        }
                        
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
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("shot", argv[2]) == 0)
            {
                if (strcmp("-a", argv[4]) == 0)
                {
                    if (strcmp("-type", argv[6]) == 0)
                    {
                        uint8_t res;
                        uint16_t i, times;
                        mcp9600_address_t addr_pin;
                        mcp9600_thermocouple_type_t type;
                        
                        if (strcmp("0", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_0;
                        }
                        else if (strcmp("1", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_1;
                        }
                        else
                        {
                            return 5;
                        }
                        if (strcmp("k", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_K;
                        }
                        else if (strcmp("j", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_J;
                        }
                        else if (strcmp("t", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_T;
                        }
                        else if (strcmp("n", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_N;
                        }
                        else if (strcmp("s", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_S;
                        }
                        else if (strcmp("e", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_E;
                        }
                        else if (strcmp("b", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_B;
                        }
                        else if (strcmp("r", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_R;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        times = (uint16_t)atoi(argv[3]);
                        
                        /* shot init */
                        res = mcp9600_shot_init(addr_pin, type);
                        if (res != 0)
                        {
                            return 1;
                        }
                        
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
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("interrupt", argv[2]) == 0)
            {
                if (strcmp("-a", argv[4]) == 0)
                {
                    if (strcmp("-type", argv[6]) == 0)
                    {
                        uint8_t res;
                        uint16_t i, times;
                        mcp9600_address_t addr_pin;
                        mcp9600_thermocouple_type_t type;
                        
                        if (strcmp("0", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_0;
                        }
                        else if (strcmp("1", argv[5]) == 0)
                        {
                            addr_pin = MCP9600_ADDRESS_1;
                        }
                        else
                        {
                            return 5;
                        }
                        if (strcmp("k", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_K;
                        }
                        else if (strcmp("j", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_J;
                        }
                        else if (strcmp("t", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_T;
                        }
                        else if (strcmp("n", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_N;
                        }
                        else if (strcmp("s", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_S;
                        }
                        else if (strcmp("e", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_E;
                        }
                        else if (strcmp("b", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_B;
                        }
                        else if (strcmp("r", argv[7]) == 0)
                        {
                            type = MCP9600_THERMOCOUPLE_TYPE_R;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        times = (uint16_t)atoi(argv[3]);
                        
                        /* gpio init */
                        g_flag = 0;
                        res = gpio_interrupt_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        
                        /* interrupt init */
                        res = mcp9600_interrupt_init(addr_pin, type);
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
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register mcp9600 fuction */
    shell_init();
    shell_register("mcp9600", mcp9600);
    uart1_print("mcp9600: welcome to libdriver mcp9600.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("mcp9600: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("mcp9600: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("mcp9600: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("mcp9600: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("mcp9600: param is invalid.\n");
            }
            else
            {
                uart1_print("mcp9600: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
