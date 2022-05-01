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
 * @file      driver_mcp9600_interrupt.h
 * @brief     driver mcp9600 interrupt header file
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

#ifndef DRIVER_MCP9600_INTERRUPT_H
#define DRIVER_MCP9600_INTERRUPT_H

#include "driver_mcp9600_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup mcp9600_example_driver
 * @{
 */

/**
 * @brief mcp9600 interrupt example default definition
 */
#define MCP9600_INTERRUPT_DEFAULT_FILTER_COEFFICIENT                    MCP9600_FILTER_COEFFICIENT_1                       /**< filter coefficient 1 */
#define MCP9600_INTERRUPT_DEFAULT_COLD_JUNCTION_RESOLUTION              MCP9600_COLD_JUNCTION_RESOLUTION_0P0625            /**< cold junction resolution 0.0625C */
#define MCP9600_INTERRUPT_DEFAULT_ADC_RESOLUTION                        MCP9600_ADC_RESOLUTION_18_BIT                      /**< adc resolution 18 bit */
#define MCP9600_INTERRUPT_DEFAULT_BURST_MODE_SAMPLE                     MCP9600_BURST_MODE_SAMPLE_1                        /**< burst mode sample 1 */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_LIMIT                          30.0f                                              /**< alert1 limit */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_LIMIT                          31.0f                                              /**< alert2 limit */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_LIMIT                          32.0f                                              /**< alert3 limit */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_LIMIT                          33.0f                                              /**< alert4 limit */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_HYSTERESIS                     31.0f                                              /**< alert1 hysteresis */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_HYSTERESIS                     32.0f                                              /**< alert2 hysteresis */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_HYSTERESIS                     33.0f                                              /**< alert3 hysteresis */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_HYSTERESIS                     35.0f                                              /**< alert4 hysteresis */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_TEMPERATURE_MAINTAIN_DETECT    MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION   /**< hot junction */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_TEMPERATURE_MAINTAIN_DETECT    MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION   /**< hot junction */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_TEMPERATURE_MAINTAIN_DETECT    MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION   /**< hot junction */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_TEMPERATURE_MAINTAIN_DETECT    MCP9600_TEMPERATURE_MAINTAIN_DETECT_HOT_JUNCTION   /**< hot junction */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_EDGE                           MCP9600_DETECT_EDGE_FALLING                        /**< edge falling */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_EDGE                           MCP9600_DETECT_EDGE_FALLING                        /**< edge falling */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_EDGE                           MCP9600_DETECT_EDGE_FALLING                        /**< edge falling */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_EDGE                           MCP9600_DETECT_EDGE_FALLING                        /**< edge falling */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_ACTIVE_LEVEL                   MCP9600_ACTIVE_LEVEL_LOW                           /**< active level low */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_ACTIVE_LEVEL                   MCP9600_ACTIVE_LEVEL_LOW                           /**< active level low */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_ACTIVE_LEVEL                   MCP9600_ACTIVE_LEVEL_LOW                           /**< active level low */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_ACTIVE_LEVEL                   MCP9600_ACTIVE_LEVEL_LOW                           /**< active level low */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_INTERRUPT_MODE                 MCP9600_INTERRUPT_MODE_COMPARATOR                  /**< comparator mode */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_INTERRUPT_MODE                 MCP9600_INTERRUPT_MODE_COMPARATOR                  /**< comparator mode */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_INTERRUPT_MODE                 MCP9600_INTERRUPT_MODE_COMPARATOR                  /**< comparator mode */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_INTERRUPT_MODE                 MCP9600_INTERRUPT_MODE_COMPARATOR                  /**< comparator mode */
#define MCP9600_INTERRUPT_DEFAULT_ALERT1_OUTPUT                         MCP9600_BOOL_TRUE                                  /**< enable output */
#define MCP9600_INTERRUPT_DEFAULT_ALERT2_OUTPUT                         MCP9600_BOOL_TRUE                                  /**< enable output */
#define MCP9600_INTERRUPT_DEFAULT_ALERT3_OUTPUT                         MCP9600_BOOL_TRUE                                  /**< enable output */
#define MCP9600_INTERRUPT_DEFAULT_ALERT4_OUTPUT                         MCP9600_BOOL_TRUE                                  /**< enable output */

/**
 * @brief     interrupt example init
 * @param[in] addr_pin is the iic device address
 * @param[in] type is the thermocouple type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t mcp9600_interrupt_init(mcp9600_address_t addr_pin, mcp9600_thermocouple_type_t type);

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mcp9600_interrupt_deinit(void);

/**
 * @brief      interrupt example read
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
uint8_t mcp9600_interrupt_read(int16_t *hot_raw, float *hot_s,
                               int16_t *delta_raw, float *delta_s, int16_t *cold_raw, float *cold_s);

/**
 * @brief     interrupt example clear
 * @param[in] alert is the alert type
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 * @note      none
 */
uint8_t mcp9600_interrupt_clear(mcp9600_alert_t alert);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
