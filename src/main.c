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
 * @file			 	main.c
 *
 * @brief 			File contains the main function
 * Only the main function is defined in here.
 *
 * @date 			07-Jun-2016
 * @author			rp (rp@meetrp.com)
 */

#include "dr_dhcp_relay.h"

/**
 * @brief Main function
 *
 * @param [in] argc	Number of command line inputs
 * @param [in] argv	The network interface to listen on
 * @return	Non-zero on failure.
 */
int main(int argc, char* argv[])
{
	if (1 == argc) {
		return dr_start_dhcp_relay("eth0");
	}

	return dr_start_dhcp_relay(argv[2]);
}
