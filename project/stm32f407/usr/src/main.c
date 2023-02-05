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
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */
volatile uint8_t g_flag;   /**< interrupt flag */

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
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t mcp9600(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"times", required_argument, NULL, 2},
        {"type", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    mcp9600_address_t addr = MCP9600_ADDRESS_0;
    mcp9600_thermocouple_type_t chip_type = MCP9600_THERMOCOUPLE_TYPE_K;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = MCP9600_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = MCP9600_ADDRESS_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }

            /* running times */
            case 2 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* type */
            case 3 :
            {
                /* set the type */
                if (strcmp("k", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_K;
                }
                else if (strcmp("j", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_J;
                }
                else if (strcmp("t", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_T;
                }
                else if (strcmp("n", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_N;
                }
                else if (strcmp("s", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_S;
                }
                else if (strcmp("e", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_E;
                }
                else if (strcmp("b", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_B;
                }
                else if (strcmp("r", optarg) == 0)
                {
                    chip_type = MCP9600_THERMOCOUPLE_TYPE_R;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (mcp9600_register_test(addr) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (mcp9600_read_test(addr, chip_type, times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_int", type) == 0)
    {
        /* run int test */
        if (mcp9600_interrupt_test(addr, chip_type) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* basic init */
        res = mcp9600_basic_init(addr, chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
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
            
            /* output */
            mcp9600_interface_debug_print("mcp9600: %d/%d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
        }
        
        /* deinit */
        (void)mcp9600_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;

        /* shot init */
        res = mcp9600_shot_init(addr, chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
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
            
            /* output */
            mcp9600_interface_debug_print("mcp9600: %d/%d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
        }
        
        /* deinit */
        (void)mcp9600_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;

        /* gpio init */
        g_flag = 0;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* interrupt init */
        res = mcp9600_interrupt_init(addr, chip_type);
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
        
        /* loop */
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
            
            /* output */
            mcp9600_interface_debug_print("mcp9600: %d/%d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
            
            /* check the flag */
            if (g_flag != 0)
            {
                mcp9600_interface_debug_print("mcp9600: find interrupt.\n");
                
                break;
            }
        }
        
        /* deinit */
        (void)mcp9600_interrupt_deinit();
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        mcp9600_interface_debug_print("Usage:\n");
        mcp9600_interface_debug_print("  mcp9600 (-i | --information)\n");
        mcp9600_interface_debug_print("  mcp9600 (-h | --help)\n");
        mcp9600_interface_debug_print("  mcp9600 (-p | --port)\n");
        mcp9600_interface_debug_print("  mcp9600 (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        mcp9600_interface_debug_print("  mcp9600 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]\n");
        mcp9600_interface_debug_print("  mcp9600 (-t int | --test=int) [--addr=<0 | 1>] [--type=<k | j | t | n | s | e | b | r>]\n");
        mcp9600_interface_debug_print("  mcp9600 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]\n");
        mcp9600_interface_debug_print("  mcp9600 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]\n");
        mcp9600_interface_debug_print("  mcp9600 (-e int | --example=int) [--addr=<0 | 1>] [--times=<num>] [--type=<k | j | t | n | s | e | b | r>]\n");
        mcp9600_interface_debug_print("\n");
        mcp9600_interface_debug_print("Options:\n");
        mcp9600_interface_debug_print("      --addr=<0 | 1>               Set the addr pin.([default: 0])\n");
        mcp9600_interface_debug_print("  -e <read | shot | int>, --example=<read | shot | int>\n");
        mcp9600_interface_debug_print("                                   Run the driver example.\n");
        mcp9600_interface_debug_print("  -h, --help                       Show the help.\n");
        mcp9600_interface_debug_print("  -i, --information                Show the chip information.\n");
        mcp9600_interface_debug_print("  -p, --port                       Display the pin connections of the current board.\n");
        mcp9600_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        mcp9600_interface_debug_print("                                   Run the driver test.\n");
        mcp9600_interface_debug_print("      --times=<num>                Set the running times.([default: 3])\n");
        mcp9600_interface_debug_print("      --type=<k | j | t | n | s | e | b | r>\n");
        mcp9600_interface_debug_print("                                   Set the thermocouple type.([default: k])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
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
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        mcp9600_interface_debug_print("mcp9600: SCL connected to GPIOB PIN8.\n");
        mcp9600_interface_debug_print("mcp9600: SDA connected to GPIOB PIN9.\n");
        mcp9600_interface_debug_print("mcp9600: INT connected to GPIOB PIN0.\n");
        
        return 0;
    }
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
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register mcp9600 function */
    shell_init();
    shell_register("mcp9600", mcp9600);
    uart_print("mcp9600: welcome to libdriver mcp9600.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("mcp9600: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("mcp9600: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("mcp9600: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("mcp9600: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("mcp9600: param is invalid.\n");
            }
            else
            {
                uart_print("mcp9600: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
