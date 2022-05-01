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
 * @file      driver_mcp9600_basic.c
 * @brief     driver mcp9600 basic source file
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

#include "driver_mcp9600_basic.h"

static mcp9600_handle_t gs_handle;        /**< mcp9600 handle */

/**
 * @brief     basic example init
 * @param[in] addr_pin is the iic device address
 * @param[in] type is the thermocouple type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t mcp9600_basic_init(mcp9600_address_t addr_pin, mcp9600_thermocouple_type_t type)
{
    uint8_t res;
    int16_t reg;
    uint8_t reg2;
    
    /* link interface function */
    DRIVER_MCP9600_LINK_INIT(&gs_handle, mcp9600_handle_t);
    DRIVER_MCP9600_LINK_IIC_INIT(&gs_handle, mcp9600_interface_iic_init);
    DRIVER_MCP9600_LINK_IIC_DEINIT(&gs_handle, mcp9600_interface_iic_deinit);
    DRIVER_MCP9600_LINK_IIC_READ_COMMAND(&gs_handle, mcp9600_interface_iic_read_cmd);
    DRIVER_MCP9600_LINK_IIC_WRITE_COMMAND(&gs_handle, mcp9600_interface_iic_write_cmd);
    DRIVER_MCP9600_LINK_IIC_DELAY_MS(&gs_handle, mcp9600_interface_delay_ms);
    DRIVER_MCP9600_LINK_DEBUG_PRINT(&gs_handle, mcp9600_interface_debug_print);
    
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
    
    /* set default filter coefficient */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_BASIC_DEFAULT_FILTER_COEFFICIENT);
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
    
    /* set default cold junction resolution */
    res = mcp9600_set_cold_junction_resolution(&gs_handle, MCP9600_BASIC_DEFAULT_COLD_JUNCTION_RESOLUTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default adc resolution */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_BASIC_DEFAULT_ADC_RESOLUTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default burst mode sample */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BASIC_DEFAULT_BURST_MODE_SAMPLE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* alert1 limit convert to register */
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT1_LIMIT, (int16_t *)&reg);
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
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT2_LIMIT, (int16_t *)&reg);
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
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT3_LIMIT, (int16_t *)&reg);
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
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT4_LIMIT, (int16_t *)&reg);
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
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT1_HYSTERESIS, (uint8_t *)&reg2);
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
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT2_HYSTERESIS, (uint8_t *)&reg2);
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
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT3_HYSTERESIS, (uint8_t *)&reg2);
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
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, MCP9600_BASIC_DEFAULT_ALERT4_HYSTERESIS, (uint8_t *)&reg2);
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
                                                  MCP9600_BASIC_DEFAULT_ALERT1_TEMPERATURE_MAINTAIN_DETECT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_2,
                                                  MCP9600_BASIC_DEFAULT_ALERT2_TEMPERATURE_MAINTAIN_DETECT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_3,
                                                  MCP9600_BASIC_DEFAULT_ALERT3_TEMPERATURE_MAINTAIN_DETECT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 temperature maintain detect */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_4,
                                                  MCP9600_BASIC_DEFAULT_ALERT4_TEMPERATURE_MAINTAIN_DETECT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_1, MCP9600_BASIC_DEFAULT_ALERT1_EDGE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_2, MCP9600_BASIC_DEFAULT_ALERT2_EDGE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_3, MCP9600_BASIC_DEFAULT_ALERT3_EDGE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 detect edge */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_4, MCP9600_BASIC_DEFAULT_ALERT4_EDGE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_1, MCP9600_BASIC_DEFAULT_ALERT1_ACTIVE_LEVEL);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_2, MCP9600_BASIC_DEFAULT_ALERT2_ACTIVE_LEVEL);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_3, MCP9600_BASIC_DEFAULT_ALERT3_ACTIVE_LEVEL);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 active level */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_4, MCP9600_BASIC_DEFAULT_ALERT4_ACTIVE_LEVEL);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_1, MCP9600_BASIC_DEFAULT_ALERT1_INTERRUPT_MODE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_2, MCP9600_BASIC_DEFAULT_ALERT2_INTERRUPT_MODE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_3, MCP9600_BASIC_DEFAULT_ALERT3_INTERRUPT_MODE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 interrupt mode */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_4, MCP9600_BASIC_DEFAULT_ALERT4_INTERRUPT_MODE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert1 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_1, MCP9600_BASIC_DEFAULT_ALERT1_OUTPUT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert2 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_2, MCP9600_BASIC_DEFAULT_ALERT2_OUTPUT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert3 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_3, MCP9600_BASIC_DEFAULT_ALERT3_OUTPUT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert4 output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_4, MCP9600_BASIC_DEFAULT_ALERT4_OUTPUT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start continuous read */
    res = mcp9600_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: start continuous read failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mcp9600_basic_deinit(void)
{
    uint8_t res;
    
    /* stop continuous read */
    res = mcp9600_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* deinit the chip */
    res = mcp9600_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *hot_raw points to a raw hot temperature buffer
 * @param[out] *hot_s points to a converted hot temperature buffer
 * @param[out] *delta_raw points to a raw delta temperature buffer
 * @param[out] *delta_s points to a converted delta temperature buffer
 * @param[out] *cold_raw points to a raw cold temperature buffer
 * @param[out] *cold_s points to a converted cold temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mcp9600_basic_read(int16_t *hot_raw, float *hot_s,
                           int16_t *delta_raw, float *delta_s, int16_t *cold_raw, float *cold_s)
{
    uint8_t res;
    
    /* continuous read */
    res = mcp9600_continuous_read(&gs_handle, hot_raw, hot_s, delta_raw, delta_s, cold_raw, cold_s);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
