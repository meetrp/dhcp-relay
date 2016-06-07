/*************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Rp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *************************************************************/

/**
 * @file 				dr_dhcp_relay.h
 *
 * @brief 			File contains all the necessary definitions for dhcp relay to be called
 * All the function prototypes and the data structure required to utilize the dhcp
 * relay functions will be defined in this file.
 *
 * @date 			07-Jun-2016
 * @author			rp (rp@meetrp.com)
 */

#ifndef DR_DHCP_RELAY_H_
#define DR_DHCP_RELAY_H_

#include "dr_common.h"

/**
 * @brief start the DHCP relay loop
 * This function starts the DHCP relay on the given network interface ('dev')
 *
 * @param [in] dev		network interface on which relay should run
 * @return error codes
 */
dr_err_t dr_start_dhcp_relay(IN char* dev);

#endif /* DR_DHCP_RELAY_H_ */
