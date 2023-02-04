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
 * @file      driver_mcp9600.c
 * @brief     driver mcp9600 source file
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

#include "driver_mcp9600.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Microchip MCP9600"        /**< chip name */
#define MANUFACTURER_NAME         "Microchip"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.7f                       /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                       /**< chip max supply voltage */
#define MAX_CURRENT               2.5f                       /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                     /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                     /**< chip max operating temperature */
#define DRIVER_VERSION            1000                       /**< driver version */

/**
 * @brief chip register definition
 */
#define MCP9600_REG_THERMOCOUPLE_HOT_JUNCTION                0x00        /**< thermocouple hot junction register */
#define MCP9600_REG_JUNCTIONS_TEMPERATURE_DELTA              0x01        /**< junctions temperature delta register */
#define MCP9600_REG_COLD_JUNCTION_TEMPERATURE                0x02        /**< cold junction temperature register */
#define MCP9600_REG_RAW_ADC_DATA                             0x03        /**< raw adc data register */
#define MCP9600_REG_STATUS                                   0x04        /**< status register */
#define MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION        0x05        /**< thermocouple sensor configuration register */
#define MCP9600_REG_DEVICE_CONFIGURATION                     0x06        /**< device configuration register */
#define MCP9600_REG_ALERT1_CONFIGURATION                     0x08        /**< alert 1 configuration register */
#define MCP9600_REG_ALERT2_CONFIGURATION                     0x09        /**< alert 2 configuration register */
#define MCP9600_REG_ALERT3_CONFIGURATION                     0x0A        /**< alert 3 configuration register */
#define MCP9600_REG_ALERT4_CONFIGURATION                     0x0B        /**< alert 4 configuration register */
#define MCP9600_REG_ALERT1_HYSTERESIS                        0x0C        /**< alert 1 hysteresis register */
#define MCP9600_REG_ALERT2_HYSTERESIS                        0x0D        /**< alert 2 hysteresis register */
#define MCP9600_REG_ALERT3_HYSTERESIS                        0x0E        /**< alert 3 hysteresis register */
#define MCP9600_REG_ALERT4_HYSTERESIS                        0x0F        /**< alert 4 hysteresis register */
#define MCP9600_REG_TEMPERATURE_ALERT1_LIMIT                 0x10        /**< temperature alert 1 limit register */
#define MCP9600_REG_TEMPERATURE_ALERT2_LIMIT                 0x11        /**< temperature alert 2 limit register */
#define MCP9600_REG_TEMPERATURE_ALERT3_LIMIT                 0x12        /**< temperature alert 3 limit register */
#define MCP9600_REG_TEMPERATURE_ALERT4_LIMIT                 0x13        /**< temperature alert 4 limit register */
#define MCP9600_REG_DEVICE_ID_REVISON                        0x20        /**< device id/revision register */

/**
 * @brief      read bytes
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_mcp9600_iic_read(mcp9600_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[1];
    
    buf[0] = reg;                                                               /* set reg */
    if (handle->iic_write_cmd(handle->iic_addr, (uint8_t *)buf, 1) != 0)        /* write command */
    {   
        return 1;                                                               /* return error */
    }
    if (handle->iic_read_cmd(handle->iic_addr, data, len) != 0)                 /* read data */
    {   
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_mcp9600_iic_write(mcp9600_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 1) > 16)                                                             /* check length */
    {
        return 1;                                                                   /* return error */
    }
    buf[0] = reg;                                                                   /* set MSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[1 + i] = data[i];                                                       /* copy write data */
    }
    if (handle->iic_write_cmd(handle->iic_addr, (uint8_t *)buf, len + 1) != 0)      /* write iic command */
    {   
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an mcp9600 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mcp9600_set_addr_pin(mcp9600_handle_t *handle, mcp9600_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set pin */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an mcp9600 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mcp9600_get_addr_pin(mcp9600_handle_t *handle, mcp9600_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (mcp9600_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t mcp9600_init(mcp9600_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                                /* check debug_print */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->iic_init == NULL)                                                   /* check iic_init */
    {
        handle->debug_print("mcp9600: iic_init is null.\n");                        /* iic_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_deinit == NULL)                                                 /* check iic_deinit */
    {
        handle->debug_print("mcp9600: iic_deinit is null.\n");                      /* iic_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                               /* check iic_read_cmd */
    {
        handle->debug_print("mcp9600: iic_read_cmd is null.\n");                    /* iic_read_cmd is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                              /* check iic_write_cmd */
    {
        handle->debug_print("mcp9600: iic_write_cmd is null.\n");                   /* iic_write_cmd is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                                   /* check delay_ms */
    {
        handle->debug_print("mcp9600: delay_ms is null.\n");                        /* delay_ms is null */
       
        return 3;                                                                   /* return error */
    }
    
    if (handle->iic_init() != 0)                                                    /* iic init */
    {
        handle->debug_print("mcp9600: iic init failed.\n");                         /* iic init failed */
       
        return 1;                                                                   /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 2);                                            /* clear the buffer */
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_ID_REVISON,
                            (uint8_t *)buf, 2);                                     /* read device id */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read device id failed.\n");                   /* read device id failed */
        (void)handle->iic_deinit();                                                 /* iic deinit */
        
        return 1;                                                                   /* return error */
    }
    if (buf[0] != 0x40)                                                             /* check id */
    {
        handle->debug_print("mcp9600: id is invalid.\n");                           /* id is invalid */
        (void)handle->iic_deinit();                                                 /* iic deinit */
        
        return 4;                                                                   /* return error */
    }
    handle->inited = 1;                                                             /* set inited */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_deinit(mcp9600_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: power down failed.\n");                                     /* power down failed */
       
        return 4;                                                                                 /* return error */
    }
    
    reg &= ~(3 << 0);                                                                             /* clear configure */
    reg |= 0x1 << 0;                                                                              /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: power down failed.\n");                                     /* power down failed */
       
        return 4;                                                                                 /* return error */
    }
    res = handle->iic_deinit();                                                                   /* iic deinit */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: iic deinit failed.\n");                                     /* iic deinit failed */
       
        return 1;                                                                                 /* return error */
    }
    handle->inited = 0;                                                                           /* clear flag */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_start_continuous_read(mcp9600_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    reg &= ~(3 << 0);                                                                             /* clear configure */
    reg |= 0x0 << 0;                                                                              /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                     /* write device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_stop_continuous_read(mcp9600_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    reg &= ~(3 << 0);                                                                             /* clear configure */
    reg |= 0x02 << 0;                                                                             /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                     /* write device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
                                int16_t *delta_raw, float *delta_s, int16_t *cold_raw, float *cold_s)
{
    uint8_t res;
    uint8_t reg;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                 /* check handle */
    {
        return 2;                                                                                       /* return error */
    }
    if (handle->inited != 1)                                                                            /* check handle initialization */
    {
        return 3;                                                                                       /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_HOT_JUNCTION, (uint8_t *)buf, 2);         /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read hot junction temperature failed.\n");                        /* read hot junction temperature failed */
       
        return 1;                                                                                       /* return error */
    }
    *hot_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                             /* get raw data */
    *hot_s = (float)(*hot_raw) / 16.0f;                                                                 /* convert the data */
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_JUNCTIONS_TEMPERATURE_DELTA, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read junction thermocouple delta failed.\n");                     /* junction thermocouple delta failed */
       
        return 1;                                                                                       /* return error */
    }
    *delta_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                           /* get raw data */
    *delta_s = (float)(*delta_raw) / 16.0f;                                                             /* convert the data */
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);             /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                            /* read device configuration failed */
       
        return 1;                                                                                       /* return error */
    }
    res = a_mcp9600_iic_read(handle, MCP9600_REG_COLD_JUNCTION_TEMPERATURE, (uint8_t *)buf, 2);         /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read cold junction temperature failed.\n");                       /* read cold junction temperature failed */
       
        return 1;                                                                                       /* return error */
    }
    *cold_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                            /* get raw data */
    if (((reg >> 7) & 0x01) != 0)                                                                       /* check the config */
    {
        *cold_s = (float)(*cold_raw) / 16.0f;                                                           /* convert the data */
    }
    else
    {
        *cold_s = (float)(*cold_raw) / 4.0f;                                                            /* convert the data */
    }
    
    return 0;                                                                                           /* success return 0 */
}

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
                            int16_t *delta_raw, float *delta_s, int16_t *cold_raw, float *cold_s)
{
    uint8_t res;
    uint8_t reg;
    uint16_t timeout;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                 /* check handle */
    {
        return 2;                                                                                       /* return error */
    }
    if (handle->inited != 1)                                                                            /* check handle initialization */
    {
        return 3;                                                                                       /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);                           /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                                          /* read status failed */
       
        return 1;                                                                                       /* return error */
    }
    reg &= ~(1 << 7);                                                                                   /* clear flag */
    reg &= ~(1 << 6);                                                                                   /* clear flag */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);                          /* write config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: write status failed.\n");                                         /* write status failed */
       
        return 1;                                                                                       /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);             /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                            /* read device configuration failed */
       
        return 1;                                                                                       /* return error */
    }
    reg &= ~(3 << 0);                                                                                   /* clear configure */
    reg |= 0x02 << 0;                                                                                   /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);            /* write config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                           /* write device configuration failed */
       
        return 1;                                                                                       /* return error */
    }
    timeout = 10000;                                                                                    /* set timeout 10000 ms */
    while (timeout != 0)                                                                                /* wait timeout */
    {
        res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);                       /* read config */
        if (res != 0)                                                                                   /* check result */
        {
            handle->debug_print("mcp9600: read status failed.\n");                                      /* read status failed */
           
            return 1;                                                                                   /* return error */
        }
        if ((reg & 0xC0) == 0xC0)                                                                       /* check flag */
        {
            break;                                                                                      /* break */
        }
        handle->delay_ms(1);                                                                            /* delay 1 ms */
        timeout--;                                                                                      /* timeout-- */
    }
    if (timeout == 0)                                                                                   /* check timeout */
    {
        handle->debug_print("mcp9600: read timeout.\n");                                                /* read timeout failed */
       
        return 4;                                                                                       /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_HOT_JUNCTION, (uint8_t *)buf, 2);         /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read hot junction temperature failed.\n");                        /* read hot junction temperature failed */
       
        return 1;                                                                                       /* return error */
    }
    *hot_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                             /* get raw data */
    *hot_s = (float)(*hot_raw) / 16.0f;                                                                 /* convert the data */
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_JUNCTIONS_TEMPERATURE_DELTA, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read junction thermocouple delta failed.\n");                     /* junction thermocouple delta failed */
       
        return 1;                                                                                       /* return error */
    }
    *delta_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                           /* get raw data */
    *delta_s = (float)(*delta_raw) / 16.0f;                                                             /* convert the data */
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);             /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                            /* read device configuration failed */
       
        return 1;                                                                                       /* return error */
    }
    res = a_mcp9600_iic_read(handle, MCP9600_REG_COLD_JUNCTION_TEMPERATURE, (uint8_t *)buf, 2);         /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read cold junction temperature failed.\n");                       /* read cold junction temperature failed */
       
        return 1;                                                                                       /* return error */
    }
    *cold_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                            /* get raw data */
    if (((reg >> 7) & 0x01) != 0)                                                                       /* check the config */
    {
        *cold_s = (float)(*cold_raw) / 16.0f;                                                           /* convert the data */
    }
    else
    {
        *cold_s = (float)(*cold_raw) / 4.0f;                                                            /* convert the data */
    }
    
    return 0;                                                                                           /* success return 0 */
}

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
uint8_t mcp9600_get_status_burst_complete_flag(mcp9600_handle_t *handle, mcp9600_bool_t *status)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                      /* read status failed */
       
        return 1;                                                                   /* return error */
    }
    
    *status = (mcp9600_bool_t)((reg >> 7) & 0x01);                                  /* get flag */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_clear_status_burst_complete_flag(mcp9600_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                      /* read status failed */
       
        return 1;                                                                   /* return error */
    }
    reg &= ~(1 << 7);                                                               /* clear flag */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: write status failed.\n");                     /* write status failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_get_status_temperature_update_flag(mcp9600_handle_t *handle, mcp9600_bool_t *status)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                      /* read status failed */
       
        return 1;                                                                   /* return error */
    }
    
    *status = (mcp9600_bool_t)((reg >> 6) & 0x01);                                  /* get flag */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_clear_status_temperature_update_flag(mcp9600_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                      /* read status failed */
       
        return 1;                                                                   /* return error */
    }
    reg &= ~(1 << 6);                                                               /* clear flag */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: write status failed.\n");                     /* write status failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_get_status_input_range(mcp9600_handle_t *handle, mcp9600_input_range_t *range)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                      /* read status failed */
       
        return 1;                                                                   /* return error */
    }
    
    *range = (mcp9600_input_range_t)((reg >> 4) & 0x01);                            /* get flag */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_get_alert_status(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_alert_status_t *status)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_STATUS, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mcp9600: read status failed.\n");                      /* read status failed */
       
        return 1;                                                                   /* return error */
    }
    
    *status = (mcp9600_alert_status_t)((reg >> alert) & 0x01);                      /* get alert status */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t mcp9600_get_hot_junction_temperature(mcp9600_handle_t *handle, int16_t *raw, float *s)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_HOT_JUNCTION, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("mcp9600: read hot junction temperature failed.\n");                      /* read hot junction temperature failed */
       
        return 1;                                                                                     /* return error */
    }
    
    *raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                               /* get raw data */
    *s = (float)(*raw) / 16.0f;                                                                       /* convert the data */
    
    return 0;                                                                                         /* success return 0 */
}

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
uint8_t mcp9600_get_junction_thermocouple_delta(mcp9600_handle_t *handle, int16_t *raw, float *s)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                 /* check handle */
    {
        return 2;                                                                                       /* return error */
    }
    if (handle->inited != 1)                                                                            /* check handle initialization */
    {
        return 3;                                                                                       /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_JUNCTIONS_TEMPERATURE_DELTA, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("mcp9600: read junction thermocouple delta failed.\n");                     /* junction thermocouple delta failed */
       
        return 1;                                                                                       /* return error */
    }
    
    *raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                 /* get raw data */
    *s = (float)(*raw) / 16.0f;                                                                         /* convert the data */
    
    return 0;                                                                                           /* success return 0 */
}

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
uint8_t mcp9600_get_cold_junction_temperature(mcp9600_handle_t *handle, int16_t *raw, float *s)
{
    uint8_t res;
    uint8_t reg;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);           /* read config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                          /* read device configuration failed */
       
        return 1;                                                                                     /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_COLD_JUNCTION_TEMPERATURE, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("mcp9600: read cold junction temperature failed.\n");                     /* read cold junction temperature failed */
       
        return 1;                                                                                     /* return error */
    }
    
    *raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                               /* get raw data */
    if (((reg >> 7) & 0x01) != 0)                                                                     /* check the config */
    {
        *s = (float)(*raw) / 16.0f;                                                                   /* convert the data */
    }
    else
    {
        *s = (float)(*raw) / 4.0f;                                                                    /* convert the data */
    }
    
    return 0;                                                                                         /* success return 0 */
}

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
uint8_t mcp9600_get_raw_adc(mcp9600_handle_t *handle, int32_t *raw, double *uv)
{
    uint8_t res;
    uint8_t reg;
    uint8_t shift;
    uint16_t mask;
    uint8_t buf[3];
    double resolution;
    
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);           /* read config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                          /* read device configuration failed */
       
        return 1;                                                                                     /* return error */
    }
    
    reg = (reg >> 5) & 0x3;                                                                           /* get type */
    if (reg == 0x0)                                                                                   /* get 18 bit */
    {
        resolution = 2.0;                                                                             /* set 2.0 */
        shift = 0;                                                                                    /* set shift 0 */
        mask = 0x0;                                                                                   /* set mask 0x0 */
    }
    else if (reg == 0x1)                                                                              /* get 16 bit */
    {
        resolution = 8.0;                                                                             /* set 8.0 */
        shift = 2;                                                                                    /* set shift 2 */
        mask = 0x03;                                                                                  /* set mask 0x03 */
    }
    else if (reg == 0x2)                                                                              /* get 14 bit */
    {
        resolution = 32.0;                                                                            /* set 32.0 */
        shift = 4;                                                                                    /* set shift 4 */
        mask = 0x0F;                                                                                  /* set mask 0x0F */
    }
    else                                                                                              /* get 12 bit */
    {
        resolution = 128.0;                                                                           /* set 128.0 */
        shift = 6;                                                                                    /* set shift 6 */
        mask = 0x3F;                                                                                  /* set mask 0x3F */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_RAW_ADC_DATA, (uint8_t *)buf, 3);                    /* read config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("mcp9600: read raw adc data failed.\n");                                  /* read raw adc data failed */
       
        return 1;                                                                                     /* return error */
    }
    
    *raw = (int32_t)(((uint32_t)buf[0] << 16) | ((uint32_t)buf[1]) << 8) | buf[2];                    /* get raw data */
    if ((buf[0] & 0x80) != 0)                                                                         /* check sign */
    {
        *raw = (int32_t)((uint32_t)(*raw) | ((uint32_t)(0xFF) << 24));                                /* set sign bits */
    }
    if (((*raw) & 0x80000000U) != 0)                                                                  /* sign*/
    {
        *raw = ((*raw) >> shift) | (uint32_t)(mask) << (32 - shift);                                  /* set raw */
    }
    else
    {
        *raw = ((*raw) >> shift);                                                                     /* set raw */
    }
    
    *uv = (double)(*raw) * resolution;                                                                /* convert the data */
    
    return 0;                                                                                         /* success return 0 */
}

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
uint8_t mcp9600_set_cold_junction_resolution(mcp9600_handle_t *handle, mcp9600_cold_junction_resolution_t resolution)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    reg &= ~(1 << 7);                                                                             /* clear configure */
    reg |= resolution << 7;                                                                       /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                     /* write device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_get_cold_junction_resolution(mcp9600_handle_t *handle, mcp9600_cold_junction_resolution_t *resolution)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    *resolution = (mcp9600_cold_junction_resolution_t)((reg >> 7) & 0x01);                        /* set cold junction resolution */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_set_adc_resolution(mcp9600_handle_t *handle, mcp9600_adc_resolution_t resolution)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    reg &= ~(3 << 5);                                                                             /* clear configure */
    reg |= resolution << 5;                                                                       /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                     /* write device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_get_adc_resolution(mcp9600_handle_t *handle, mcp9600_adc_resolution_t *resolution)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    *resolution = (mcp9600_adc_resolution_t)((reg >> 5) & 0x03);                                  /* get configuration */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_set_burst_mode_sample(mcp9600_handle_t *handle, mcp9600_burst_mode_sample_t sample)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    reg &= ~(7 << 2);                                                                             /* clear configure */
    reg |= sample << 2;                                                                           /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                     /* write device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_get_burst_mode_sample(mcp9600_handle_t *handle, mcp9600_burst_mode_sample_t *sample)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    *sample = (mcp9600_burst_mode_sample_t)((reg >> 2) & 0x7);                                    /* set sample */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_set_mode(mcp9600_handle_t *handle, mcp9600_mode_t mode)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    reg &= ~(3 << 0);                                                                             /* clear configure */
    reg |= mode << 0;                                                                             /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: write device configuration failed.\n");                     /* write device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_get_mode(mcp9600_handle_t *handle, mcp9600_mode_t *mode)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mcp9600: read device configuration failed.\n");                      /* read device configuration failed */
       
        return 1;                                                                                 /* return error */
    }
    
    *mode = (mcp9600_mode_t)((reg >> 0) & 0x3);                                                   /* get mode */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t mcp9600_set_thermocouple_type(mcp9600_handle_t *handle, mcp9600_thermocouple_type_t type)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("mcp9600: read thermocouple sensor configuration failed.\n");                      /* read thermocouple sensor configuration failed */
       
        return 1;                                                                                              /* return error */
    }
    
    reg &= ~(0x7 << 4);                                                                                        /* clear configure */
    reg |= type << 4;                                                                                          /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("mcp9600: write thermocouple sensor configuration failed.\n");                     /* write thermocouple sensor configuration failed */
       
        return 1;                                                                                              /* return error */
    }
    
    return 0;                                                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_thermocouple_type(mcp9600_handle_t *handle, mcp9600_thermocouple_type_t *type)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("mcp9600: read thermocouple sensor configuration failed.\n");                      /* read thermocouple sensor configuration failed */
       
        return 1;                                                                                              /* return error */
    }
    *type = (mcp9600_thermocouple_type_t)((reg >> 4) & 0x07);                                                  /* get type */
    
    return 0;                                                                                                  /* success return 0 */
}

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
uint8_t mcp9600_set_filter_coefficient(mcp9600_handle_t *handle, mcp9600_filter_coefficient_t coefficient)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("mcp9600: read thermocouple sensor configuration failed.\n");                      /* read thermocouple sensor configuration failed */
       
        return 1;                                                                                              /* return error */
    }
    
    reg &= ~(0x7 << 0);                                                                                        /* clear configure */
    reg |= coefficient << 0;                                                                                   /* set configure */
    res = a_mcp9600_iic_write(handle, MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION, (uint8_t *)&reg, 1);      /* write config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("mcp9600: write thermocouple sensor configuration failed.\n");                     /* write thermocouple sensor configuration failed */
       
        return 1;                                                                                              /* return error */
    }
    
    return 0;                                                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_filter_coefficient(mcp9600_handle_t *handle, mcp9600_filter_coefficient_t *coefficient)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_THERMOCOUPLE_SENSOR_CONFIGURATION, (uint8_t *)&reg, 1);       /* read config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("mcp9600: read thermocouple sensor configuration failed.\n");                      /* read thermocouple sensor configuration failed */
       
        return 1;                                                                                              /* return error */
    }
    *coefficient = (mcp9600_filter_coefficient_t)((reg >> 0) & (0x7));                                         /* get coefficient */
    
    return 0;                                                                                                  /* success return 0 */
}

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
uint8_t mcp9600_alert_limit_convert_to_register(mcp9600_handle_t *handle, float c, int16_t *reg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *reg = (int16_t)(c * 16.0f);        /* convert */
    
    return 0;                           /* success return 0 */
}

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
uint8_t mcp9600_alert_limit_convert_to_data(mcp9600_handle_t *handle, int16_t reg, float *c)
{
    if (handle == NULL)              /* check handle */
    {
        return 2;                    /* return error */
    }
    if (handle->inited != 1)         /* check handle initialization */
    {
        return 3;                    /* return error */
    }
    
    *c  = (float)reg / 16.0f;        /* convert */
    
    return 0;                        /* success return 0 */
}

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
uint8_t mcp9600_set_alert_limit(mcp9600_handle_t *handle, mcp9600_alert_t alert, int16_t reg)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    switch (alert)                                                        /* alert type */
    {
        case MCP9600_ALERT_1 :                                            /* alert 1 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT1_LIMIT;              /* set alert 1 */
            
            break;                                                        /* break */
        }
        case MCP9600_ALERT_2 :                                            /* alert 2 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT2_LIMIT;              /* set alert 2 */
            
            break;                                                        /* break */
        }
        case MCP9600_ALERT_3 :                                            /* alert 3 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT3_LIMIT;              /* set alert 3 */
            
            break;                                                        /* break */
        }
        case MCP9600_ALERT_4 :                                            /* alert 4 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT4_LIMIT;              /* set alert 4 */
            
            break;                                                        /* break */
        }
        default :
        {
            res = 1;                                                      /* set failed */
            
            break;                                                        /* break */
        }
    }
    if (res != 0)                                                         /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");              /* alert is invalid */
        
        return 4;                                                         /* return error */
    }
    buf[0] = (reg >> 8) & 0xFF;                                           /* set MSB */
    buf[1] = (reg >> 0) & 0xFF;                                           /* set LSB */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 2);       /* set alert limit */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9600: set alert limit failed.\n");        /* set alert limit failed */
       
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9600_get_alert_limit(mcp9600_handle_t *handle, mcp9600_alert_t alert, int16_t *reg)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    switch (alert)                                                        /* alert type */
    {
        case MCP9600_ALERT_1 :                                            /* alert 1 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT1_LIMIT;              /* set alert 1 */
            
            break;                                                        /* break */
        }
        case MCP9600_ALERT_2 :                                            /* alert 2 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT2_LIMIT;              /* set alert 2 */
            
            break;                                                        /* break */
        }
        case MCP9600_ALERT_3 :                                            /* alert 3 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT3_LIMIT;              /* set alert 3 */
            
            break;                                                        /* break */
        }
        case MCP9600_ALERT_4 :                                            /* alert 4 */
        {
            res = 0;                                                      /* set ok */
            reg_addr = MCP9600_REG_TEMPERATURE_ALERT4_LIMIT;              /* set alert 4 */
            
            break;                                                        /* break */
        }
        default :
        {
            res = 1;                                                      /* set failed */
            
            break;                                                        /* break */
        }
    }
    if (res != 0)                                                         /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");              /* alert is invalid */
        
        return 4;                                                         /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 2);        /* get alert limit */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mcp9600: get alert limit failed.\n");        /* get alert limit failed */
       
        return 1;                                                         /* return error */
    }
    *reg = (int16_t)(((uint16_t)(buf[0]) << 8) | buf[1]);                 /* set register */
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t mcp9600_alert_hysteresis_convert_to_register(mcp9600_handle_t *handle, float c, uint8_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = (uint8_t)(c);            /* convert */
    
    return 0;                       /* success return 0 */
}

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
uint8_t mcp9600_alert_hysteresis_convert_to_data(mcp9600_handle_t *handle, uint8_t reg, float *c)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *c  = (float)(reg);             /* convert */
    
    return 0;                       /* success return 0 */
}

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
uint8_t mcp9600_set_alert_hysteresis(mcp9600_handle_t *handle, mcp9600_alert_t alert, uint8_t reg)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_HYSTERESIS;                          /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_HYSTERESIS;                          /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_HYSTERESIS;                          /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_HYSTERESIS;                          /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    buf[0] = reg;                                                              /* set register */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert hysteresis */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert hysteresis failed.\n");        /* set alert hysteresis failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_alert_hysteresis(mcp9600_handle_t *handle, mcp9600_alert_t alert, uint8_t *reg)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_HYSTERESIS;                          /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_HYSTERESIS;                          /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_HYSTERESIS;                          /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_HYSTERESIS;                          /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert hysteresis */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert hysteresis failed.\n");        /* get alert hysteresis failed */
       
        return 1;                                                              /* return error */
    }
    *reg = buf[0];                                                             /* set register */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_clear_interrupt(mcp9600_handle_t *handle, mcp9600_alert_t alert)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    buf[0] &= ~(1 << 7);                                                       /* clear reg */
    buf[0] |= 1 << 7;                                                          /* set clear */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert config failed.\n");            /* set alert config failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_interrupt(mcp9600_handle_t *handle, mcp9600_alert_t alert, uint8_t *status)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    *status = (buf[0] >> 7) & 0x01;                                            /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
                                                mcp9600_temperature_maintain_detect_t maintain_detect)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    buf[0] &= ~(1 << 4);                                                       /* clear reg */
    buf[0] |= maintain_detect << 4;                                            /* set maintain detect */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert config failed.\n");            /* set alert config failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
                                                mcp9600_temperature_maintain_detect_t *maintain_detect)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    switch (alert)                                                                           /* alert type */
    {
        case MCP9600_ALERT_1 :                                                               /* alert 1 */
        {
            res = 0;                                                                         /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                                     /* set alert 1 */
            
            break;                                                                           /* break */
        }
        case MCP9600_ALERT_2 :                                                               /* alert 2 */
        {
            res = 0;                                                                         /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                                     /* set alert 2 */
            
            break;                                                                           /* break */
        }
        case MCP9600_ALERT_3 :                                                               /* alert 3 */
        {
            res = 0;                                                                         /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                                     /* set alert 3 */
            
            break;                                                                           /* break */
        }
        case MCP9600_ALERT_4 :                                                               /* alert 4 */
        {
            res = 0;                                                                         /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                                     /* set alert 4 */
            
            break;                                                                           /* break */
        }
        default :
        {
            res = 1;                                                                         /* set failed */
            
            break;                                                                           /* break */
        }
    }
    if (res != 0)                                                                            /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                                 /* alert is invalid */
        
        return 4;                                                                            /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);                           /* get alert config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");                          /* get alert config failed */
       
        return 1;                                                                            /* return error */
    }
    *maintain_detect = (mcp9600_temperature_maintain_detect_t)((buf[0] >> 4) & 0x01);        /* get maintain detect */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t mcp9600_set_detect_edge(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_detect_edge_t edge)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    buf[0] &= ~(1 << 3);                                                       /* clear reg */
    buf[0] |= edge << 3;                                                       /* set detect edge */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert config failed.\n");            /* set alert config failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_detect_edge(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_detect_edge_t *edge)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    *edge = (mcp9600_detect_edge_t)((buf[0] >> 3) & 0x01);                     /* get detect edge */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_set_active_level(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_active_level_t level)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    buf[0] &= ~(1 << 2);                                                       /* clear reg */
    buf[0] |= level << 2;                                                      /* set active level */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert config failed.\n");            /* set alert config failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_active_level(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_active_level_t *level)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    *level = (mcp9600_active_level_t)((buf[0] >> 2) & 0x01);                   /* get level */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_set_interrupt_mode(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_interrupt_mode_t mode)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    buf[0] &= ~(1 << 1);                                                       /* clear reg */
    buf[0] |= mode << 1;                                                       /* set interrupt mode */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert config failed.\n");            /* set alert config failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_interrupt_mode(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_interrupt_mode_t *mode)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    *mode = (mcp9600_interrupt_mode_t)((buf[0] >> 1) & 0x01);                  /* get mode */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_set_alert_output(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_bool_t enable)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    buf[0] &= ~(1 << 0);                                                       /* clear reg */
    buf[0] |= enable << 0;                                                     /* set alert output */
    res = a_mcp9600_iic_write(handle, reg_addr, (uint8_t *)buf, 1);            /* set alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: set alert config failed.\n");            /* set alert config failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_alert_output(mcp9600_handle_t *handle, mcp9600_alert_t alert, mcp9600_bool_t *enable)
{
    uint8_t res;
    uint8_t reg_addr;
    uint8_t buf[1];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    switch (alert)                                                             /* alert type */
    {
        case MCP9600_ALERT_1 :                                                 /* alert 1 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT1_CONFIGURATION;                       /* set alert 1 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_2 :                                                 /* alert 2 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT2_CONFIGURATION;                       /* set alert 2 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_3 :                                                 /* alert 3 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT3_CONFIGURATION;                       /* set alert 3 */
            
            break;                                                             /* break */
        }
        case MCP9600_ALERT_4 :                                                 /* alert 4 */
        {
            res = 0;                                                           /* set ok */
            reg_addr = MCP9600_REG_ALERT4_CONFIGURATION;                       /* set alert 4 */
            
            break;                                                             /* break */
        }
        default :
        {
            res = 1;                                                           /* set failed */
            
            break;                                                             /* break */
        }
    }
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("mcp9600: alert is invalid.\n");                   /* alert is invalid */
        
        return 4;                                                              /* return error */
    }
    res = a_mcp9600_iic_read(handle, reg_addr, (uint8_t *)buf, 1);             /* get alert config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mcp9600: get alert config failed.\n");            /* get alert config failed */
       
        return 1;                                                              /* return error */
    }
    *enable = (mcp9600_bool_t)((buf[0] >> 0) & 0x01);                          /* get enable */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mcp9600_get_device_id_revision(mcp9600_handle_t *handle, uint8_t *id, uint8_t *revision)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_mcp9600_iic_read(handle, MCP9600_REG_DEVICE_ID_REVISON, (uint8_t *)buf, 2);       /* read device id */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("mcp9600: read device id failed.\n");                             /* read device id failed */
       
        return 1;                                                                             /* return error */
    }
    *id = buf[0];                                                                             /* get id */
    *revision = buf[1];                                                                       /* get revision */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t mcp9600_set_reg(mcp9600_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    }
  
    return a_mcp9600_iic_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t mcp9600_get_reg(mcp9600_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
  
    return a_mcp9600_iic_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip information
 * @param[out] *info points to an mcp9600 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mcp9600_info(mcp9600_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(mcp9600_info_t));                        /* initialize mcp9600 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
