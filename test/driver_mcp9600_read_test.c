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
 * @file      driver_mcp9600_read_test.c
 * @brief     driver mcp9600 read test source file
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

#include "driver_mcp9600_read_test.h"

static mcp9600_handle_t gs_handle;        /**< mcp9600 handle */

/**
 * @brief     read test
 * @param[in] addr_pin is the iic device address
 * @param[in] type is the thermocouple type
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mcp9600_read_test(mcp9600_address_t addr_pin, mcp9600_thermocouple_type_t type, uint32_t times)
{
    uint8_t res;
    int16_t reg;
    uint8_t reg2;
    uint32_t i;
    mcp9600_info_t info;
    
    /* link interface function */
    DRIVER_MCP9600_LINK_INIT(&gs_handle, mcp9600_handle_t);
    DRIVER_MCP9600_LINK_IIC_INIT(&gs_handle, mcp9600_interface_iic_init);
    DRIVER_MCP9600_LINK_IIC_DEINIT(&gs_handle, mcp9600_interface_iic_deinit);
    DRIVER_MCP9600_LINK_IIC_READ_COMMAND(&gs_handle, mcp9600_interface_iic_read_cmd);
    DRIVER_MCP9600_LINK_IIC_WRITE_COMMAND(&gs_handle, mcp9600_interface_iic_write_cmd);
    DRIVER_MCP9600_LINK_IIC_DELAY_MS(&gs_handle, mcp9600_interface_delay_ms);
    DRIVER_MCP9600_LINK_DEBUG_PRINT(&gs_handle, mcp9600_interface_debug_print);
    
    /* get information */
    res = mcp9600_info(&info);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        mcp9600_interface_debug_print("mcp9600: chip is %s.\n", info.chip_name);
        mcp9600_interface_debug_print("mcp9600: manufacturer is %s.\n", info.manufacturer_name);
        mcp9600_interface_debug_print("mcp9600: interface is %s.\n", info.interface);
        mcp9600_interface_debug_print("mcp9600: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mcp9600_interface_debug_print("mcp9600: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mcp9600_interface_debug_print("mcp9600: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mcp9600_interface_debug_print("mcp9600: max current is %0.2fmA.\n", info.max_current_ma);
        mcp9600_interface_debug_print("mcp9600: max temperature is %0.1fC.\n", info.temperature_max);
        mcp9600_interface_debug_print("mcp9600: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    mcp9600_interface_debug_print("mcp9600: start read test.\n");
    
    /* set mcp9600 iic address */
    res = mcp9600_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set addr pin failed.\n");
        
        return 1;
    }
    
    /* chip init */
    res = mcp9600_init(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: init failed.\n");
        
        return 1;
    }
    
    /* set normal */
    res = mcp9600_set_mode(&gs_handle, MCP9600_MODE_NORMAL);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set filter coefficient */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set thermocouple type */
    res = mcp9600_set_thermocouple_type(&gs_handle, type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set cold junction resolution */
    res = mcp9600_set_cold_junction_resolution(&gs_handle, MCP9600_COLD_JUNCTION_RESOLUTION_0P0625);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set adc resolution */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_18_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set burst mode sample */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert1 limit convert to register */
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, 30.0f, (int16_t *)&reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert limit convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 limit */
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_1, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert2 limit convert to register */
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, 31.0f, (int16_t *)&reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert limit convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 limit */
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_2, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert3 limit convert to register */
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, 32.0f, (int16_t *)&reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert limit convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 limit */
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_3, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert4 limit convert to register */
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, 33.0f, (int16_t *)&reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert limit convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 limit */
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_4, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert1 hysteresis convert to register */
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, 50.0f, (uint8_t *)&reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert hysteresis convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 hysteresis */
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_1, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert2 hysteresis convert to register */
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, 51.0f, (uint8_t *)&reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert hysteresis convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 hysteresis */
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_2, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert3 hysteresis convert to register */
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, 52.0f, (uint8_t *)&reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert hysteresis convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 hysteresis */
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_3, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert4 hysteresis convert to register */
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, 53.0f, (uint8_t *)&reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert hysteresis convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 hysteresis */
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_4, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_1,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_2,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_3,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_4,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_1, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_2, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_3, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_4, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_1, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_2, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_3, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_4, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_1, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_2, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_3, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_4, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_1, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_2, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_3, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_4, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start continuous read test */
    mcp9600_interface_debug_print("mcp9600: start continuous read test.\n");
    
    /* start continuous read */
    res = mcp9600_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: start continuous read failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* continuous read */
        res = mcp9600_continuous_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: continuous read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* stop continuous read */
    res = mcp9600_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: stop continuous read failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start single read test */
    mcp9600_interface_debug_print("mcp9600: start single read test.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t hot_raw;
        float hot_s;
        int16_t delta_raw;
        float delta_s;
        int16_t cold_raw;
        float cold_s;
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* start continuous read */
    res = mcp9600_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: start continuous read failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set cold junction resolution 0.25 */
    mcp9600_interface_debug_print("mcp9600: set cold junction resolution 0.25.\n");
    
    /* set cold junction resolution */
    res = mcp9600_set_cold_junction_resolution(&gs_handle, MCP9600_COLD_JUNCTION_RESOLUTION_0P25);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* continuous read */
        res = mcp9600_continuous_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: continuous read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* set adc resolution 18 bit */
    mcp9600_interface_debug_print("mcp9600: set adc resolution 18 bit.\n");
    
    /* set adc resolution */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_18_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        int32_t raw;
        double uv;
        
        /* continuous read */
        res = mcp9600_continuous_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* ge raw adc */
        res = mcp9600_get_raw_adc(&gs_handle, (int32_t *)&raw, (double *)&uv);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: get raw adc failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: raw is %d uv is %f.\n", raw, uv);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* set adc resolution 16 bit */
    mcp9600_interface_debug_print("mcp9600: set adc resolution 16 bit.\n");
    
    /* set adc resolution */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_16_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        int32_t raw;
        double uv;
        
        /* continuous read */
        res = mcp9600_continuous_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* ge raw adc */
        res = mcp9600_get_raw_adc(&gs_handle, (int32_t *)&raw, (double *)&uv);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: get raw adc failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: raw is %d uv is %f.\n", raw, uv);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* stop continuous read */
    res = mcp9600_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: stop continuous read failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set adc resolution 14 bit */
    mcp9600_interface_debug_print("mcp9600: set adc resolution 14 bit.\n");
    
    /* set adc resolution */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_14_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        int32_t raw;
        double uv;
        
        /* continuous read */
        res = mcp9600_continuous_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* ge raw adc */
        res = mcp9600_get_raw_adc(&gs_handle, (int32_t *)&raw, (double *)&uv);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: get raw adc failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: raw is %d uv is %f.\n", raw, uv);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* set adc resolution 12 bit */
    mcp9600_interface_debug_print("mcp9600: set adc resolution 12 bit.\n");
    
    /* set adc resolution */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_12_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        int32_t raw;
        double uv;
        
        /* continuous read */
        res = mcp9600_continuous_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
        
        /* ge raw adc */
        res = mcp9600_get_raw_adc(&gs_handle, (int32_t *)&raw, (double *)&uv);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: get raw adc failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: raw is %d uv is %f.\n", raw, uv);
        
        /* delay 1000 ms */
        mcp9600_interface_delay_ms(1000);
    }
    
    /* stop continuous read */
    res = mcp9600_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: stop continuous read failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set burst mode sample 1 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 1.\n");
    
    /* set burst mode sample 1 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 2 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 2.\n");
    
    /* set burst mode sample 2 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 4 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 4.\n");
    
    /* set burst mode sample 4 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_4);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 8 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 8.\n");
    
    /* set burst mode sample 8 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_8);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 16 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 16.\n");
    
    /* set burst mode sample 16 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_16);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 32 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 32.\n");
    
    /* set burst mode sample 32 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_32);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 64 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 64.\n");
    
    /* set burst mode sample 64 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_64);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 128 */
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 128.\n");
    
    /* set burst mode sample 128 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_128);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set burst mode sample 2 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set filter coefficient 0 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 0.\n");
    
    /* set filter coefficient 0 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_0);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 1 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 1.\n");
    
    /* set filter coefficient 1 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 2 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 2.\n");
    
    /* set filter coefficient 2 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 3 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 3.\n");
    
    /* set filter coefficient 3 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_3);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 4 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 4.\n");
    
    /* set filter coefficient 4 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_4);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 5 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 5.\n");
    
    /* set filter coefficient 5 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_5);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 6 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 6.\n");
    
    /* set filter coefficient 6 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_6);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* set filter coefficient 7 */
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 7.\n");
    
    /* set filter coefficient 7 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_7);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
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
        
        /* single read */
        res = mcp9600_single_read(&gs_handle, (int16_t *)&hot_raw, (float *)&hot_s,
                                      (int16_t *)&delta_raw, (float *)&delta_s,
                                      (int16_t *)&cold_raw, (float *)&cold_s);
        if (res != 0)
        {
            mcp9600_interface_debug_print("mcp9600: single read failed.\n");
            (void)mcp9600_deinit(&gs_handle);
            
            return 1;
        }
        
        /* 1000 ms */
        mcp9600_interface_delay_ms(1000);
        
        /* print */
        mcp9600_interface_debug_print("mcp9600: hot %0.2f delta %0.2f cold %0.2f.\n", hot_s, delta_s, cold_s);
    }
    
    /* finish read test */
    mcp9600_interface_debug_print("mcp9600: finish read test.\n");
    (void)mcp9600_deinit(&gs_handle);
    
    return 0;
}
