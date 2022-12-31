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
 * @file      driver_mcp9600.h
 * @brief     driver mcp9600 header file
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

#ifndef DRIVER_MCP9600_H
#define DRIVER_MCP9600_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup mcp9600_driver mcp9600 driver function
 * @brief    mcp9600 driver modules
 * @{
 */

/**
 * @addtogroup mcp9600_base_driver
 * @{
 */

/**
 * @brief mcp9600 address enumeration definition
 */
typedef enum
{
    MCP9600_ADDRESS_0 = 0xC0,        /**< addr pin connected to the GND */
    MCP9600_ADDRESS_1 = 0xC2,        /**< addr pin connected to the VCC */
} mcp9600_address_t;

/**
 * @brief mcp9600 cold junction resolution enumeration definition
 */
typedef enum
{
    MCP9600_COLD_JUNCTION_RESOLUTION_0P0625 = 0x00,        /**< 0.0625C */
    MCP9600_COLD_JUNCTION_RESOLUTION_0P25   = 0x01,        /**< 0.25C */
} mcp9600_cold_junction_resolution_t;

/**
 * @brief mcp9600 adc resolution enumeration definition
 */
typedef enum
{
    MCP9600_ADC_RESOLUTION_18_BIT = 0x00,        /**< 18 bit resolution */
    MCP9600_ADC_RESOLUTION_16_BIT = 0x01,        /**< 16 bit resolution */
    MCP9600_ADC_RESOLUTION_14_BIT = 0x02,        /**< 14 bit resolution */
    MCP9600_ADC_RESOLUTION_12_BIT = 0x03,        /**< 12 bit resolution */
} mcp9600_adc_resolution_t;

/**
 * @brief mcp9600 burst mode sample enumeration definition
 */
typedef enum
{
    MCP9600_BURST_MODE_SAMPLE_1   = 0x00,        /**< 1 sample */
    MCP9600_BURST_MODE_SAMPLE_2   = 0x01,        /**< 2 sample */
    MCP9600_BURST_MODE_SAMPLE_4   = 0x02,        /**< 4 sample */
    MCP9600_BURST_MODE_SAMPLE_8   = 0x03,        /**< 8 sample */
    MCP9600_BURST_MODE_SAMPLE_16  = 0x04,        /**< 16 sample */
    MCP9600_BURST_MODE_SAMPLE_32  = 0x05,        /**< 32 sample */
    MCP9600_BURST_MODE_SAMPLE_64  = 0x06,        /**< 64 sample */
    MCP9600_BURST_MODE_SAMPLE_128 = 0x07,        /**< 128 sample */
} mcp9600_burst_mode_sample_t;

/**
 * @brief mcp9600 mode enumeration definition
 */
typedef enum
{
    MCP9600_MODE_NORMAL   = 0x00,        /**< normal mode */
    MCP9600_MODE_SHUTDOWN = 0x01,        /**< shutdown mode */
    MCP9600_MODE_BURST    = 0x02,        /**< burst mode */
} mcp9600_mode_t;

/**
 * @brief mcp9600 thermocouple type enumeration definition
 */
typedef enum
{
    MCP9600_THERMOCOUPLE_TYPE_K = 0x00,        /**< type k */
    MCP9600_THERMOCOUPLE_TYPE_J = 0x01,        /**< type j */
    MCP9600_THERMOCOUPLE_TYPE_T = 0x02,        /**< type t */
    MCP9600_THERMOCOUPLE_TYPE_N = 0x03,        /**< type n */
    MCP9600_THERMOCOUPLE_TYPE_S = 0x04,        /**< type s */
    MCP9600_THERMOCOUPLE_TYPE_E = 0x05,        /**< type e */
    MCP9600_THERMOCOUPLE_TYPE_B = 0x06,        /**< type b */
    MCP9600_THERMOCOUPLE_TYPE_R = 0x07,        /**< type r */
} mcp9600_thermocouple_type_t;

/**
 * @brief mcp9600 filter coefficient enumeration definition
 */
typedef enum
{
    MCP9600_FILTER_COEFFICIENT_0 = 0x00,        /**< filter off */
    MCP9600_FILTER_COEFFICIENT_1 = 0x01,        /**< filter 1 */
    MCP9600_FILTER_COEFFICIENT_2 = 0x02,        /**< filter 2 */
    MCP9600_FILTER_COEFFICIENT_3 = 0x03,        /**< filter 3 */
    MCP9600_FILTER_COEFFICIENT_4 = 0x04,        /**< filter 4 */
    MCP9600_FILTER_COEFFICIENT_5 = 0x05,        /**< filter 5 */
    MCP9600_FILTER_COEFFICIENT_6 = 0x06,        /**< filter 6 */
    MCP9600_FILTER_COEFFICIENT_7 = 0x07,        /**< filter 7 */
} mcp9600_filter_coefficient_t;

/**
 * @brief mcp9600 alert enumeration definition
 */
typedef enum
{
    MCP9600_ALERT_1 = 0x00,        /**< alert 1 */
    MCP9600_ALERT_2 = 0x01,        /**< alert 2 */
    MCP9600_ALERT_3 = 0x02,        /**< alert 3 */
    MCP9600_ALERT_4 = 0x03,        /**< alert 4 */
} mcp9600_alert_t;

/**
 * @brief mcp9600 alert status enumeration definition
 */
typedef enum
{
    MCP9600_ALERT_STATUS_LESS = 0x00,        /**< less than threshold */
    MCP9600_ALERT_STATUS_OVER = 0x01,        /**< over threshold */
} mcp9600_alert_status_t;

/**
 * @brief mcp9600 input range enumeration definition
 */
typedef enum
{
    MCP9600_INPUT_RANGE_EXCEEDS = 0x01,        /**< exceeds */
    MCP9600_INPUT_RANGE_WITHIN  = 0x00,        /**< within */
} mcp9600_input_range_t;

/**
 * @brief mcp9600 bool enumeration definition
 */
typedef enum
{
    MCP9600_BOOL_FALSE = 0x00,        /**< false */
    MCP9600_BOOL_TRUE  = 0x01,        /**< true */
} mcp9600_bool_t;

/**
 * @brief mcp9600 temperature maintain detect enumeration definition
 */
typedef enum
{
    MCP9600_TEMPERATURE_MAINTAIN_DETECT_COLD_JUNCTION = 0x01,        /**< cold junction */
    MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION  = 0x00,        /**< hot junction */
} mcp9600_temperature_maintain_detect_t;

/**
 * @brief mcp9600 detect edge enumeration definition
 */
typedef enum
{
    MCP9600_DETECT_EDGE_RISING  = 0x00,        /**< rising detect */
    MCP9600_DETECT_EDGE_FALLING = 0x01,        /**< falling detect */
} mcp9600_detect_edge_t;

/**
 * @brief mcp9600 active level enumeration definition
 */
typedef enum
{
    MCP9600_ACTIVE_LEVEL_LOW  = 0x00,        /**< active low */
    MCP9600_ACTIVE_LEVEL_HIGH = 0x01,        /**< active high */
} mcp9600_active_level_t;

/**
 * @brief mcp9600 interrupt mode enumeration definition
 */
typedef enum
{
    MCP9600_INTERRUPT_MODE_COMPARATOR = 0x00,        /**< comparator mode */
    MCP9600_INTERRUPT_MODE_INTERRUPT  = 0x01,        /**< interrupt mode */
} mcp9600_interrupt_mode_t;

/**
 * @brief mcp9600 handle structure definition
 */
typedef struct mcp9600_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t inited;                                                            /**< inited flag */
    uint8_t iic_addr;                                                          /**< iic address */
} mcp9600_handle_t;

/**
 * @brief mcp9600 information structure definition
 */
typedef struct mcp9600_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} mcp9600_info_t;

/**
 * @}
 */

/**
 * @defgroup mcp9600_link_driver mcp9600 link driver function
 * @brief    mcp9600 link driver modules
 * @ingroup  mcp9600_driver
 * @{
 */

/**
 * @brief     initialize mcp9600_handle_t structure
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] STRUCTURE is mcp9600_handle_t
 * @note      none
 */
#define DRIVER_MCP9600_LINK_INIT(HANDLE, STRUCTURE)           memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_MCP9600_LINK_IIC_INIT(HANDLE, FUC)             (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_MCP9600_LINK_IIC_DEINIT(HANDLE, FUC)           (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] FUC points to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_MCP9600_LINK_IIC_READ_COMMAND(HANDLE, FUC)     (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] FUC points to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_MCP9600_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)    (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MCP9600_LINK_IIC_DELAY_MS(HANDLE, FUC)         (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an mcp9600 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MCP9600_LINK_DEBUG_PRINT(HANDLE, FUC)          (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup mcp9600_base_driver mcp9600 base driver function
 * @brief    mcp9600 base driver modules
 * @ingroup  mcp9600_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to an mcp9600 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mcp9600_info(mcp9600_info_t *info);

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mcp9600_set_addr_pin(mcp9600_handle_t *handle, mcp9600_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mcp9600_get_addr_pin(mcp9600_handle_t *handle, mcp9600_address_t *addr_pin);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an mcp9600 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 * @note      none
 */
uint8_t mcp9600_init(mcp9600_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an mcp9600 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t mcp9600_deinit(mcp9600_handle_t *handle);

/**
 * @brief     start reading data
 * @param[in] *handle points to an mcp9600 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_start_continuous_read(mcp9600_handle_t *handle);

/**
 * @brief     stop reading data
 * @param[in] *handle points to an mcp9600 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_stop_continuous_read(mcp9600_handle_t *handle);

/**
 * @brief      read data continuously
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *hot_raw points to a raw hot temperature buffer
 * @param[out] *hot_s points to a converted hot temperature buffer
 * @param[out] *delta_raw points to a raw delta temperature buffer
 * @param[out] *delta_s points to a converted delta temperature buffer
 * @param[out] *cold_raw points to a raw cold temperature buffer
 * @param[out] *cold_s points to a converted cold temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 continuous read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       this function can be used only after run mcp9600_start_continuous_read
 *             and can be stopped by mcp9600_stop_continuous_read
 */
uint8_t mcp9600_continuous_read(mcp9600_handle_t *handle, int16_t *hot_raw, float *hot_s,
                                int16_t *delta_raw, float *delta_s, int16_t *cold_raw, float *cold_s);

/**
 * @brief      read data once
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *hot_raw points to a raw hot temperature buffer
 * @param[out] *hot_s points to a converted hot temperature buffer
 * @param[out] *delta_raw points to a raw delta temperature buffer
 * @param[out] *delta_s points to a converted delta temperature buffer
 * @param[out] *cold_raw points to a raw cold temperature buffer
 * @param[out] *cold_s points to a converted cold temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 single read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 read timeout
 * @note       none
 */
uint8_t mcp9600_single_read(mcp9600_handle_t *handle, int16_t *hot_raw, float *hot_s,
                            int16_t *delta_raw, float *delta_s, int16_t *cold_raw, float *cold_s);

/**
 * @brief      get the hot junction temperature
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hot junction temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_hot_junction_temperature(mcp9600_handle_t *handle, int16_t *raw, float *s);

/**
 * @brief      get the junction thermocouple delta
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 get junction thermocouple delta failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_junction_thermocouple_delta(mcp9600_handle_t *handle, int16_t *raw, float *s);

/**
 * @brief      get the cold junction temperature
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_cold_junction_temperature(mcp9600_handle_t *handle, int16_t *raw, float *s);

/**
 * @brief      get the raw adc
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *raw points to a raw adc buffer
 * @param[out] *uv points to a converted voltage buffer
 * @return     status code
 *             - 0 success
 *             - 1 get raw adc failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_raw_adc(mcp9600_handle_t *handle, int32_t *raw, double *uv);

/**
 * @brief     set the filter coefficient
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] coefficient is the filter coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set filter coefficient failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_filter_coefficient(mcp9600_handle_t *handle, mcp9600_filter_coefficient_t coefficient);

/**
 * @brief      get the filter coefficient
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *coefficient points to a filter coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get filter coefficient failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_filter_coefficient(mcp9600_handle_t *handle, mcp9600_filter_coefficient_t *coefficient);

/**
 * @brief     set the thermocouple type
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] type is the thermocouple type
 * @return    status code
 *            - 0 success
 *            - 1 set thermocouple type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_thermocouple_type(mcp9600_handle_t *handle, mcp9600_thermocouple_type_t type);

/**
 * @brief      get the thermocouple type
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *type points to a thermocouple type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get thermocouple type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_thermocouple_type(mcp9600_handle_t *handle, mcp9600_thermocouple_type_t *type);

/**
 * @brief      get the burst complete status flag
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status burst complete flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_status_burst_complete_flag(mcp9600_handle_t *handle, mcp9600_bool_t *status);

/**
 * @brief     clear the burst complete status flag
 * @param[in] *handle points to an mcp9600 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear status burst complete flag failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_clear_status_burst_complete_flag(mcp9600_handle_t *handle);

/**
 * @brief      get the temperature update status flag
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status temperature update flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_status_temperature_update_flag(mcp9600_handle_t *handle, mcp9600_bool_t *status);

/**
 * @brief     clear the temperature update status flag
 * @param[in] *handle points to an mcp9600 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear status temperature update flag failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_clear_status_temperature_update_flag(mcp9600_handle_t *handle);

/**
 * @brief      get the input range status
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *range points to an input range buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status input range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_status_input_range(mcp9600_handle_t *handle, mcp9600_input_range_t *range);

/**
 * @brief      get the alert status
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *status points to an alert status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_alert_status(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_alert_status_t *status);

/**
 * @brief     set the cold junction resolution
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] resolution is the cold junction resolution
 * @return    status code
 *            - 0 success
 *            - 1 set cold junction resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_cold_junction_resolution(mcp9600_handle_t *handle, mcp9600_cold_junction_resolution_t resolution);

/**
 * @brief      get the cold junction resolution
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *resolution points to a cold junction resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_cold_junction_resolution(mcp9600_handle_t *handle, mcp9600_cold_junction_resolution_t *resolution);

/**
 * @brief     set the adc resolution
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] resolution is the adc resolution
 * @return    status code
 *            - 0 success
 *            - 1 set adc resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_adc_resolution(mcp9600_handle_t *handle, mcp9600_adc_resolution_t resolution);

/**
 * @brief      get the adc resolution
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *resolution points to an adc resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get adc resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_adc_resolution(mcp9600_handle_t *handle, mcp9600_adc_resolution_t *resolution);

/**
 * @brief     set the burst mode sample
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] sample is the burst mode sample
 * @return    status code
 *            - 0 success
 *            - 1 set burst mode sample failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_burst_mode_sample(mcp9600_handle_t *handle, mcp9600_burst_mode_sample_t sample);

/**
 * @brief      get the burst mode sample
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *sample points to a burst mode sample buffer
 * @return     status code
 *             - 0 success
 *             - 1 get burst mode sample failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_burst_mode_sample(mcp9600_handle_t *handle, mcp9600_burst_mode_sample_t *sample);

/**
 * @brief     set the mode
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_mode(mcp9600_handle_t *handle, mcp9600_mode_t mode);

/**
 * @brief      get the mode
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_mode(mcp9600_handle_t *handle, mcp9600_mode_t *mode);

/**
 * @brief      convert the alert limit to the register raw data
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  c is the alert limit
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_alert_limit_convert_to_register(mcp9600_handle_t *handle, float c, int16_t *reg);

/**
 * @brief      convert the register raw data to the alert limit
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *c points to an alert limit buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_alert_limit_convert_to_data(mcp9600_handle_t *handle, int16_t reg, float *c);

/**
 * @brief     set the alert limit
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] reg is the register value
 * @return    status code
 *            - 0 success
 *            - 1 set alert limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_alert_limit(mcp9600_handle_t *handle, mcp9600_alert_t alert, int16_t reg);

/**
 * @brief      get the alert limit
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *reg points to a register value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_alert_limit(mcp9600_handle_t *handle, mcp9600_alert_t alert, int16_t *reg);

/**
 * @brief      convert the alert hysteresis to the register raw data
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  c is the alert hysteresis
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_alert_hysteresis_convert_to_register(mcp9600_handle_t *handle, float c, uint8_t *reg);

/**
 * @brief      convert the register raw data to the alert hysteresis
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *c points to an alert hysteresis buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_alert_hysteresis_convert_to_data(mcp9600_handle_t *handle, uint8_t reg, float *c);

/**
 * @brief     set the alert hysteresis
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] reg is the register value
 * @return    status code
 *            - 0 success
 *            - 1 set alert hysteresis failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_alert_hysteresis(mcp9600_handle_t *handle, mcp9600_alert_t alert, uint8_t reg);

/**
 * @brief      get the alert hysteresis
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *reg points to a register value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert hysteresis failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_alert_hysteresis(mcp9600_handle_t *handle, mcp9600_alert_t alert, uint8_t *reg);

/**
 * @brief     clear the interrupt flag
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @return    status code
 *            - 0 success
 *            - 1 clear interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_clear_interrupt(mcp9600_handle_t *handle, mcp9600_alert_t alert);

/**
 * @brief      get the interrupt flag
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_interrupt(mcp9600_handle_t *handle, mcp9600_alert_t alert, uint8_t *status);

/**
 * @brief     set the temperature maintain detect
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] maintain_detect is the temperature maintain detect
 * @return    status code
 *            - 0 success
 *            - 1 set temperature maintain detect failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_temperature_maintain_detect(mcp9600_handle_t *handle, mcp9600_alert_t alert,
                                                mcp9600_temperature_maintain_detect_t maintain_detect);

/**
 * @brief      get the temperature maintain detect
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *maintain_detect points to a temperature maintain detect buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature maintain detect failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_temperature_maintain_detect(mcp9600_handle_t *handle, mcp9600_alert_t alert,
                                                mcp9600_temperature_maintain_detect_t *maintain_detect);

/**
 * @brief     set the detect edge
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] edge is the detect edge
 * @return    status code
 *            - 0 success
 *            - 1 set detect edge failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_detect_edge(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_detect_edge_t edge);

/**
 * @brief      get the detect edge
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *edge points to a detect edge buffer
 * @return     status code
 *             - 0 success
 *             - 1 get detect edge failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_detect_edge(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_detect_edge_t *edge);

/**
 * @brief     set the active level
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] level is the active level
 * @return    status code
 *            - 0 success
 *            - 1 set active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_active_level(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_active_level_t level);

/**
 * @brief      get the active level
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *level points to an active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_active_level(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_active_level_t *level);

/**
 * @brief     set the interrupt mode
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] mode is the interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_interrupt_mode(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_interrupt_mode_t mode);

/**
 * @brief      get the interrupt mode
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *mode points to an interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_interrupt_mode(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_interrupt_mode_t *mode);

/**
 * @brief     set the alert output
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] alert is the alert number
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alert output failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 alert is invalid
 * @note      none
 */
uint8_t mcp9600_set_alert_output(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_bool_t enable);

/**
 * @brief      get the alert output
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  alert is the alert number
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 alert is invalid
 * @note       none
 */
uint8_t mcp9600_get_alert_output(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_bool_t *enable);

/**
 * @brief      get the device id and revision
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *id points to an id buffer
 * @param[out] *revision points to a revision buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device id revision failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_device_id_revision(mcp9600_handle_t *handle, uint8_t *id, uint8_t *revision);

/**
 * @}
 */

/**
 * @defgroup mcp9600_extern_driver mcp9600 extern driver function
 * @brief    mcp9600 extern driver modules
 * @ingroup  mcp9600_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mcp9600_set_reg(mcp9600_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mcp9600_get_reg(mcp9600_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
