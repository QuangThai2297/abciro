/*
 * ctsu_pin_info.h
 *
 *  Created on: Sep 9, 2016
 *      Author: Onkar.Raut
 */

#ifndef CTSU_PIN_INFO_H_
#define CTSU_PIN_INFO_H_

#include "r_gpio_rx_if.h"
#include "r_mpc_rx_if.h"

typedef struct st_pin_info_t
{
    uint8_t tscap_count;
    uint8_t tspin_count;
    uint8_t pin_function;
    gpio_port_pin_t const * const p_tscap;
    gpio_port_pin_t const * const p_tspin;
}ctsu_pin_info_t;

#endif /* CTSU_PIN_INFO_H_ */
