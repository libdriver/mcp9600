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
 * @file      driver_mcp9600_register_test.c
 * @brief     driver mcp9600 register test source file
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
#include <stdlib.h>

static mcp9600_handle_t gs_handle;        /**< mcp9600 handle */

/**
 * @brief     register test
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mcp9600_register_test(mcp9600_address_t addr_pin)
{
    uint8_t res;
    uint8_t id;
    uint8_t revision;
    uint8_t status;
    float c, c_check;
    int16_t reg, reg_check;
    uint8_t reg2, reg2_check;
    mcp9600_info_t info;
    mcp9600_address_t addr;
    mcp9600_filter_coefficient_t coefficient;
    mcp9600_thermocouple_type_t type;
    mcp9600_cold_junction_resolution_t resolution;
    mcp9600_adc_resolution_t adc_resolution;
    mcp9600_burst_mode_sample_t sample;
    mcp9600_mode_t mode;
    mcp9600_temperature_maintain_detect_t maintain_detect;
    mcp9600_detect_edge_t edge;
    mcp9600_active_level_t level;
    mcp9600_interrupt_mode_t interrupt_mode;
    mcp9600_bool_t enable;
    mcp9600_input_range_t range;
    mcp9600_alert_status_t s;
    
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
    
    /* start register test */
    mcp9600_interface_debug_print("mcp9600: start register test.\n");
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_addr_pin/mcp9600_get_addr_pin test.\n");
    
    /* set ADDRESS_0 */
    res = mcp9600_set_addr_pin(&gs_handle, MCP9600_ADDRESS_0);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set addr pin failed.\n");
       
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set addr pin ADDRESS_0.\n");
    res = mcp9600_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get addr pin failed.\n");
       
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check addr pin %s.\n", addr == MCP9600_ADDRESS_0 ? "ok" : "error");
    
    /* set ADDRESS_1 */
    res = mcp9600_set_addr_pin(&gs_handle, MCP9600_ADDRESS_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set addr pin failed.\n");
       
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set addr pin ADDRESS_1.\n");
    res = mcp9600_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get addr pin failed.\n");
       
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check addr pin %s.\n", addr == MCP9600_ADDRESS_1 ? "ok" : "error");
    
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
    
    /* mcp9600_get_device_id_revision test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_get_device_id_revision test.\n");
    
    /* get device id revision */
    res = mcp9600_get_device_id_revision(&gs_handle, (uint8_t *)&id, (uint8_t *)&revision);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get device id revision failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: device id is 0x%02X revision is 0x%02X.\n", id, revision);
    
    /* mcp9600_set_filter_coefficient/mcp9600_get_filter_coefficient test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_filter_coefficient/mcp9600_get_filter_coefficient test.\n");
    
    /* set filter coefficient 0 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_0);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 0.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_0 ? "ok" : "error");
    
    /* set filter coefficient 1 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 1.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_1 ? "ok" : "error");
    
    /* set filter coefficient 2 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 2.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_2 ? "ok" : "error");
    
    /* set filter coefficient 3 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_3);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 3.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_3 ? "ok" : "error");
    
    /* set filter coefficient 4 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_4);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 4.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_4 ? "ok" : "error");
    
    /* set filter coefficient 5 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_5);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 5.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_5 ? "ok" : "error");
    
    /* set filter coefficient 6 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_6);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 6.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_6 ? "ok" : "error");
    
    /* set filter coefficient 7 */
    res = mcp9600_set_filter_coefficient(&gs_handle, MCP9600_FILTER_COEFFICIENT_7);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set filter coefficient 7.\n");
    res = mcp9600_get_filter_coefficient(&gs_handle, &coefficient);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get filter coefficient failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check filter coefficient %s.\n", coefficient == MCP9600_FILTER_COEFFICIENT_7 ? "ok" : "error");
    
    /* mcp9600_set_thermocouple_type/mcp9600_get_thermocouple_type test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_thermocouple_type/mcp9600_get_thermocouple_type test.\n");
    
    /* set type k */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_K);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type k.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_K ? "ok" : "error");
    
    /* set type j */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_J);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type j.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_J ? "ok" : "error");
    
    /* set type t */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_T);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type t.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_T ? "ok" : "error");
    
    /* set type n */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_N);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type n.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_N ? "ok" : "error");
    
    /* set type s */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_S);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type s.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_S ? "ok" : "error");
    
    /* set type e */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_E);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type e.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_E ? "ok" : "error");
    
    /* set type b */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_B);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type b.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_B ? "ok" : "error");
    
    /* set type r */
    res = mcp9600_set_thermocouple_type(&gs_handle, MCP9600_THERMOCOUPLE_TYPE_R);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set thermocouple type r.\n");
    res = mcp9600_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get thermocouple type failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check thermocouple type %s.\n", type == MCP9600_THERMOCOUPLE_TYPE_R ? "ok" : "error");
    
    /* mcp9600_set_cold_junction_resolution/mcp9600_get_cold_junction_resolution test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_cold_junction_resolution/mcp9600_get_cold_junction_resolution test.\n");
    
    /* set cold junction resolution 0.0625 */
    res = mcp9600_set_cold_junction_resolution(&gs_handle, MCP9600_COLD_JUNCTION_RESOLUTION_0P0625);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set cold junction resolution 0.0625.\n");
    res = mcp9600_get_cold_junction_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check cold junction resolution %s.\n", resolution == MCP9600_COLD_JUNCTION_RESOLUTION_0P0625 ? "ok" : "error");
    
    /* set cold junction resolution 0.25 */
    res = mcp9600_set_cold_junction_resolution(&gs_handle, MCP9600_COLD_JUNCTION_RESOLUTION_0P25);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set cold junction resolution 0.25.\n");
    res = mcp9600_get_cold_junction_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get cold junction resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check cold junction resolution %s.\n", resolution == MCP9600_COLD_JUNCTION_RESOLUTION_0P25 ? "ok" : "error");
    
    /* mcp9600_set_adc_resolution/mcp9600_get_adc_resolution test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_adc_resolution/mcp9600_get_adc_resolution test.\n");
    
    /* set adc resolution 18 bit */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_18_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set adc resolution 18 bit.\n");
    res = mcp9600_get_adc_resolution(&gs_handle, &adc_resolution);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check adc resolution %s.\n", adc_resolution == MCP9600_ADC_RESOLUTION_18_BIT ? "ok" : "error");
    
    /* set adc resolution 16 bit */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_16_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set adc resolution 16 bit.\n");
    res = mcp9600_get_adc_resolution(&gs_handle, &adc_resolution);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check adc resolution %s.\n", adc_resolution == MCP9600_ADC_RESOLUTION_16_BIT ? "ok" : "error");
    
    /* set adc resolution 14 bit */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_14_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set adc resolution 14 bit.\n");
    res = mcp9600_get_adc_resolution(&gs_handle, &adc_resolution);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check adc resolution %s.\n", adc_resolution == MCP9600_ADC_RESOLUTION_14_BIT ? "ok" : "error");
    
    /* set adc resolution 12 bit */
    res = mcp9600_set_adc_resolution(&gs_handle, MCP9600_ADC_RESOLUTION_12_BIT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set adc resolution 12 bit.\n");
    res = mcp9600_get_adc_resolution(&gs_handle, &adc_resolution);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get adc resolution failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check adc resolution %s.\n", adc_resolution == MCP9600_ADC_RESOLUTION_12_BIT ? "ok" : "error");
    
    /* mcp9600_set_burst_mode_sample/mcp9600_get_burst_mode_sample test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_burst_mode_sample/mcp9600_get_burst_mode_sample test.\n");
    
    /* set sample 1 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 1.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_1 ? "ok" : "error");
    
    /* set sample 2 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 2.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_2 ? "ok" : "error");
    
    /* set sample 4 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_4);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 4.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_4 ? "ok" : "error");
    
    /* set sample 8 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_8);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 8.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_8 ? "ok" : "error");
    
    /* set sample 16 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_16);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 16.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_16 ? "ok" : "error");
    
    /* set sample 32 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_32);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 32.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_32 ? "ok" : "error");
    
    /* set sample 64 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_64);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 64.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_64 ? "ok" : "error");
    
    /* set sample 128 */
    res = mcp9600_set_burst_mode_sample(&gs_handle, MCP9600_BURST_MODE_SAMPLE_128);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode sample 128.\n");
    res = mcp9600_get_burst_mode_sample(&gs_handle, &sample);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get burst mode sample failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst mode sample %s.\n", sample == MCP9600_BURST_MODE_SAMPLE_128 ? "ok" : "error");
    
    /* mcp9600_set_mode/mcp9600_get_mode test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_mode/mcp9600_get_mode test.\n");
    
    /* set shutdown mode */
    res = mcp9600_set_mode(&gs_handle, MCP9600_MODE_SHUTDOWN);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set shutdown mode.\n");
    res = mcp9600_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check mode %s.\n", mode == MCP9600_MODE_SHUTDOWN ? "ok" : "error");
    
    /* set burst mode */
    res = mcp9600_set_mode(&gs_handle, MCP9600_MODE_BURST);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set burst mode.\n");
    res = mcp9600_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check mode %s.\n", mode == MCP9600_MODE_BURST ? "ok" : "error");
    
    /* set normal mode */
    res = mcp9600_set_mode(&gs_handle, MCP9600_MODE_NORMAL);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set normal mode.\n");
    res = mcp9600_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check mode %s.\n", mode == MCP9600_MODE_NORMAL ? "ok" : "error");
    
    /* mcp9600_set_temperature_maintain_detect/mcp9600_get_temperature_maintain_detect test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_temperature_maintain_detect/mcp9600_get_temperature_maintain_detect test.\n");
    
    /* set alert1 cold junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_1,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 temperature maintain detect cold junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_1, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION ? "ok" : "error");
    
    /* set alert1 hot junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_1,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 temperature maintain detect hot junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_1, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION ? "ok" : "error");
    
    /* set alert2 cold junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_2,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 temperature maintain detect cold junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_2, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION ? "ok" : "error");
    
    /* set alert2 hot junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_2,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 temperature maintain detect hot junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_2, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION ? "ok" : "error");
    
    /* set alert3 cold junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_3,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 temperature maintain detect cold junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_3, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION ? "ok" : "error");
    
    /* set alert3 hot junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_3,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 temperature maintain detect hot junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_3, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION ? "ok" : "error");
    
    /* set alert4 cold junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_4,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 temperature maintain detect cold junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_4, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION ? "ok" : "error");
    
    /* set alert4 hot junction */
    res = mcp9600_set_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_4,
                                                  MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 temperature maintain detect hot junction.\n");
    res = mcp9600_get_temperature_maintain_detect(&gs_handle, MCP9600_ALERT_4, &maintain_detect);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get temperature maintain detect failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature maintain detect %s.\n",
                                  maintain_detect == MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION ? "ok" : "error");
    
    /* mcp9600_set_detect_edge/mcp9600_get_detect_edge test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_detect_edge/mcp9600_get_detect_edge test.\n");
    
    /* set alert1 rising */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_1, MCP9600_DETECT_EDGE_RISING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 detect edge rising.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_1, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_RISING ? "ok" : "error");
    
    /* set alert1 falling */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_1, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 detect edge falling.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_1, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_FALLING ? "ok" : "error");
    
    /* set alert2 rising */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_2, MCP9600_DETECT_EDGE_RISING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 detect edge rising.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_2, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_RISING ? "ok" : "error");
    
    /* set alert2 falling */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_2, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 detect edge falling.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_2, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_FALLING ? "ok" : "error");
    
    /* set alert3 rising */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_3, MCP9600_DETECT_EDGE_RISING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 detect edge rising.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_3, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_RISING ? "ok" : "error");
    
    /* set alert3 falling */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_3, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 detect edge falling.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_3, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_FALLING ? "ok" : "error");
    
    /* set alert4 rising */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_4, MCP9600_DETECT_EDGE_RISING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 detect edge rising.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_4, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_RISING ? "ok" : "error");
    
    /* set alert4 falling */
    res = mcp9600_set_detect_edge(&gs_handle, MCP9600_ALERT_4, MCP9600_DETECT_EDGE_FALLING);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 detect edge falling.\n");
    res = mcp9600_get_detect_edge(&gs_handle, MCP9600_ALERT_4, &edge);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get detect edge failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check detect edge %s.\n", edge == MCP9600_DETECT_EDGE_FALLING ? "ok" : "error");
    
    /* mcp9600_set_active_level/mcp9600_get_active_level test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_active_level/mcp9600_get_active_level test.\n");
    
    /* set alert1 active level low */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_1, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 active level low.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_1, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_LOW ? "ok" : "error");
    
    /* set alert1 active level high */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_1, MCP9600_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 active level high.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_1, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_HIGH ? "ok" : "error");
    
    /* set alert2 active level low */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_2, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 active level low.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_2, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_LOW ? "ok" : "error");
    
    /* set alert2 active level high */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_2, MCP9600_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 active level high.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_2, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_HIGH ? "ok" : "error");
    
    /* set alert3 active level low */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_3, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 active level low.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_3, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_LOW ? "ok" : "error");
    
    /* set alert3 active level high */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_3, MCP9600_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 active level high.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_3, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_HIGH ? "ok" : "error");
    
    /* set alert4 active level low */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_4, MCP9600_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 active level low.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_4, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_LOW ? "ok" : "error");
    
    /* set alert4 active level high */
    res = mcp9600_set_active_level(&gs_handle, MCP9600_ALERT_4, MCP9600_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 active level high.\n");
    res = mcp9600_get_active_level(&gs_handle, MCP9600_ALERT_4, &level);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get active level failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check active level %s.\n", level == MCP9600_ACTIVE_LEVEL_HIGH ? "ok" : "error");
    
    /* mcp9600_set_interrupt_mode/mcp9600_get_interrupt_mode test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_interrupt_mode/mcp9600_get_interrupt_mode test.\n");
    
    /* set alert1 interrupt mode comparator */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_1, MCP9600_INTERRUPT_MODE_COMPARATOR);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 interrupt mode comparator.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_1, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_COMPARATOR ? "ok" : "error");
    
    /* set alert1 interrupt mode interrupt */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_1, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 interrupt mode interrupt.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_1, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_INTERRUPT ? "ok" : "error");
    
    /* set alert2 interrupt mode comparator */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_2, MCP9600_INTERRUPT_MODE_COMPARATOR);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 interrupt mode comparator.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_2, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_COMPARATOR ? "ok" : "error");
    
    /* set alert2 interrupt mode interrupt */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_2, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 interrupt mode interrupt.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_2, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_INTERRUPT ? "ok" : "error");
    
    /* set alert3 interrupt mode comparator */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_3, MCP9600_INTERRUPT_MODE_COMPARATOR);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 interrupt mode comparator.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_3, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_COMPARATOR ? "ok" : "error");
    
    /* set alert3 interrupt mode interrupt */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_3, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 interrupt mode interrupt.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_3, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_INTERRUPT ? "ok" : "error");
    
    /* set alert4 interrupt mode comparator */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_4, MCP9600_INTERRUPT_MODE_COMPARATOR);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 interrupt mode comparator.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_4, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_COMPARATOR ? "ok" : "error");
    
    /* set alert4 interrupt mode interrupt */
    res = mcp9600_set_interrupt_mode(&gs_handle, MCP9600_ALERT_4, MCP9600_INTERRUPT_MODE_INTERRUPT);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 interrupt mode interrupt.\n");
    res = mcp9600_get_interrupt_mode(&gs_handle, MCP9600_ALERT_4, &interrupt_mode);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt mode failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check interrupt mode %s.\n", interrupt_mode == MCP9600_INTERRUPT_MODE_INTERRUPT ? "ok" : "error");
    
    /* mcp9600_set_alert_output/mcp9600_get_alert_output test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_alert_output/mcp9600_get_alert_output test.\n");
    
    /* set alert1 disable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_1, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 disable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_1, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_FALSE ? "ok" : "error");
    
    /* set alert1 enable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_1, MCP9600_BOOL_TRUE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 enable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_1, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_TRUE ? "ok" : "error");
    
    /* set alert2 disable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_2, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 disable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_2, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_FALSE ? "ok" : "error");
    
    /* set alert2 enable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_2, MCP9600_BOOL_TRUE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 enable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_2, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_TRUE ? "ok" : "error");
    
    /* set alert3 disable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_3, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 disable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_3, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_FALSE ? "ok" : "error");
    
    /* set alert3 enable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_3, MCP9600_BOOL_TRUE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 enable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_3, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_TRUE ? "ok" : "error");
    
    /* set alert4 disable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_4, MCP9600_BOOL_FALSE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 disable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_4, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_FALSE ? "ok" : "error");
    
    /* set alert4 enable output */
    res = mcp9600_set_alert_output(&gs_handle, MCP9600_ALERT_4, MCP9600_BOOL_TRUE);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 enable output.\n");
    res = mcp9600_get_alert_output(&gs_handle, MCP9600_ALERT_4, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert output failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert output %s.\n", enable == MCP9600_BOOL_TRUE ? "ok" : "error");
    
    /* mcp9600_alert_limit_convert_to_register/mcp9600_alert_limit_convert_to_data test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_alert_limit_convert_to_register/mcp9600_alert_limit_convert_to_data test.\n");
    
    c = (rand() % 1000) / 100.0f;
    res = mcp9600_alert_limit_convert_to_register(&gs_handle, c, (int16_t *)&reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert limit convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    res = mcp9600_alert_limit_convert_to_data(&gs_handle, reg, (float *)&c_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert limit convert to data failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: alert limit %0.4f to reg %d and converted to data %0.4f.\n", c, reg, c_check);
    
    /* mcp9600_set_alert_limit/mcp9600_get_alert_limit test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_alert_limit/mcp9600_get_alert_limit test.\n");
    
    /* set alert1 limit */
     reg = (rand() % 200) * 100 + 100;
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_1, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 limit %d.\n", reg);
    res = mcp9600_get_alert_limit(&gs_handle, MCP9600_ALERT_1, (int16_t *)&reg_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert limit %s.\n", reg_check == reg ? "ok" : "error");
    
    /* set alert2 limit */
    reg = (rand() % 200) * 100 + 100;
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_2, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 limit %d.\n", reg);
    res = mcp9600_get_alert_limit(&gs_handle, MCP9600_ALERT_2, (int16_t *)&reg_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert limit %s.\n", reg_check == reg ? "ok" : "error");
    
    /* set alert3 limit */
    reg = (rand() % 200) * 100 + 100;
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_3, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 limit %d.\n", reg);
    res = mcp9600_get_alert_limit(&gs_handle, MCP9600_ALERT_3, (int16_t *)&reg_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert limit %s.\n", reg_check == reg ? "ok" : "error");
    
    /* set alert4 limit */
    reg = (rand() % 200) * 100 + 100;
    res = mcp9600_set_alert_limit(&gs_handle, MCP9600_ALERT_4, reg);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 limit %d.\n", reg);
    res = mcp9600_get_alert_limit(&gs_handle, MCP9600_ALERT_4, (int16_t *)&reg_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert limit failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert limit %s.\n", reg_check == reg ? "ok" : "error");
    
    /* mcp9600_alert_hysteresis_convert_to_register/mcp9600_alert_hysteresis_convert_to_data test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_alert_hysteresis_convert_to_register/mcp9600_alert_hysteresis_convert_to_data test.\n");
    
    c = (rand() % 1000) / 100.0f;
    res = mcp9600_alert_hysteresis_convert_to_register(&gs_handle, c, (uint8_t *)&reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert hysteresis convert to register failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    res = mcp9600_alert_hysteresis_convert_to_data(&gs_handle, reg2, (float *)&c_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: alert hysteresis convert to data failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: alert hysteresis %0.4f to reg %d and converted to data %0.4f.\n", c, reg2, c_check);
    
    /* mcp9600_set_alert_hysteresis/mcp9600_get_alert_hysteresis test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_set_alert_hysteresis/mcp9600_get_alert_hysteresis test.\n");
    
    /* set alert1 hysteresis */
    reg2 = rand() % 256;
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_1, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert1 hysteresis %d.\n", reg2);
    res = mcp9600_get_alert_hysteresis(&gs_handle, MCP9600_ALERT_1, (uint8_t *)&reg2_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert hysteresis %s.\n", reg2_check == reg2 ? "ok" : "error");
    
    /* set alert2 hysteresis */
    reg2 = rand() % 256;
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_2, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert2 hysteresis %d.\n", reg2);
    res = mcp9600_get_alert_hysteresis(&gs_handle, MCP9600_ALERT_2, (uint8_t *)&reg2_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert hysteresis %s.\n", reg2_check == reg2 ? "ok" : "error");
    
    /* set alert3 hysteresis */
    reg2 = rand() % 256;
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_3, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert3 hysteresis %d.\n", reg2);
    res = mcp9600_get_alert_hysteresis(&gs_handle, MCP9600_ALERT_3, (uint8_t *)&reg2_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert hysteresis %s.\n", reg2_check == reg2 ? "ok" : "error");
    
    /* set alert4 hysteresis */
    reg2 = rand() % 256;
    res = mcp9600_set_alert_hysteresis(&gs_handle, MCP9600_ALERT_4, reg2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: set alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: set alert4 hysteresis %d.\n", reg2);
    res = mcp9600_get_alert_hysteresis(&gs_handle, MCP9600_ALERT_4, (uint8_t *)&reg2_check);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert hysteresis failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert hysteresis %s.\n", reg2_check == reg2 ? "ok" : "error");
    
    /* mcp9600_get_interrupt/mcp9600_clear_interrupt test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_get_interrupt/mcp9600_clear_interrupt test.\n");
    
    /* alert1 get interrupt */
    res = mcp9600_get_interrupt(&gs_handle, MCP9600_ALERT_1, (uint8_t *)&status);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: alert1 get interrupt 0x%02X.\n", status);
    res = mcp9600_clear_interrupt(&gs_handle, MCP9600_ALERT_1);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: clear interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check clear interrupt %s.\n", res == 0 ? "ok" : "error");
    
    /* alert2 get interrupt */
    res = mcp9600_get_interrupt(&gs_handle, MCP9600_ALERT_2, (uint8_t *)&status);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: alert2 get interrupt 0x%02X.\n", status);
    res = mcp9600_clear_interrupt(&gs_handle, MCP9600_ALERT_2);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: clear interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check clear interrupt %s.\n", res == 0 ? "ok" : "error");
    
    /* alert3 get interrupt */
    res = mcp9600_get_interrupt(&gs_handle, MCP9600_ALERT_3, (uint8_t *)&status);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: alert3 get interrupt 0x%02X.\n", status);
    res = mcp9600_clear_interrupt(&gs_handle, MCP9600_ALERT_3);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: clear interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check clear interrupt %s.\n", res == 0 ? "ok" : "error");
    
    /* alert4 get interrupt */
    res = mcp9600_get_interrupt(&gs_handle, MCP9600_ALERT_4, (uint8_t *)&status);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: alert4 get interrupt 0x%02X.\n", status);
    res = mcp9600_clear_interrupt(&gs_handle, MCP9600_ALERT_4);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: clear interrupt failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check clear interrupt %s.\n", res == 0 ? "ok" : "error");
    
    /* mcp9600_get_status_input_range test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_get_status_input_range test.\n");
    
    /* get status input range */
    res = mcp9600_get_status_input_range(&gs_handle, &range);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get status input range failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check status input range %s.\n", range == 1 ? "exceeds" : "within");
    
    /* mcp9600_get_alert_status test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_get_alert_status test.\n");
   
    /* get alert1 status */
    res = mcp9600_get_alert_status(&gs_handle, MCP9600_ALERT_1, &s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert status failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert status %s.\n", s == 0 ? "0" : "1");
    
    /* get alert2 status */
    res = mcp9600_get_alert_status(&gs_handle, MCP9600_ALERT_2, &s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert status failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert status %s.\n", s == 0 ? "0" : "1");
    
    /* get alert3 status */
    res = mcp9600_get_alert_status(&gs_handle, MCP9600_ALERT_3, &s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert status failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert status %s.\n", s == 0 ? "0" : "1");
    
    /* get alert4 status */
    res = mcp9600_get_alert_status(&gs_handle, MCP9600_ALERT_4, &s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get alert status failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check alert status %s.\n", s == 0 ? "0" : "1");
    
    /* mcp9600_get_status_burst_complete_flag/mcp9600_clear_status_burst_complete_flag test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_get_status_burst_complete_flag/mcp9600_clear_status_burst_complete_flag test.\n");
    
    /* get status burst complete flag */
    res = mcp9600_get_status_burst_complete_flag(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get status burst complete flag failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    res = mcp9600_clear_status_burst_complete_flag(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: clear status burst complete flag failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check burst complete flag status %s.\n", enable == 0 ? "0" : "1");
    
    /* mcp9600_get_status_temperature_update_flag/mcp9600_clear_status_temperature_update_flag test */
    mcp9600_interface_debug_print("mcp9600: mcp9600_get_status_temperature_update_flag/mcp9600_clear_status_temperature_update_flag test.\n");
    
    /* get status temperature update flag */
    res = mcp9600_get_status_temperature_update_flag(&gs_handle, &enable);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: get status temperature update flag failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    res = mcp9600_clear_status_temperature_update_flag(&gs_handle);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: clear status temperature update flag failed.\n");
        (void)mcp9600_deinit(&gs_handle);
        
        return 1;
    }
    mcp9600_interface_debug_print("mcp9600: check temperature update flag status %s.\n", enable == 0 ? "0" : "1");
    
    /* finish register test */
    mcp9600_interface_debug_print("mcp9600: finish register test.\n");
    (void)mcp9600_deinit(&gs_handle);
    
    return 0;
}
