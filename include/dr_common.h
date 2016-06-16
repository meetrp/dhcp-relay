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
 * @file 				dr_common.h
 *
 * @brief 				File contains all the common definitions
 * The error codes, the macros are all defined in this. This file will be included by
 * all the other files to utilize the definitions, the macros, the error codes defined
 * here.
 *
 * @date 				07-Jun-2016 2:42:21 pm
 * @author			rp (rp@meetrp.com)
 */

#ifndef DR_COMMON_H_
#define DR_COMMON_H_

#define PRIVATE 		static	///< PRIVATE functions will be 'static' to that file
#define PUBLIC						///< PUBLIC functions will be available from other files to be called

#define IN					const	///< IN function arguments cannot be modified; so 'const'
#define OUT							///< OUT function arguments are the return values from the function
#define INOUT						///< INOUT function arguments are both input & output arguments

/**
 * @brief the error codes
 *
 * Every error code used within this entire project will be defined in here.
 */
typedef enum _dr_err {
	DR_SUCCESS, 										///< 0

	DR_GENERIC_FAILURE,						///< 1
	DR_SOCKET_FAILURE,						///< 2
	DR_IOCTL_FAILED,								///< 3
	DR_BIND_FAILED,								///< 4
	DR_SET_SOCK_OPT_FAILED				///< 5
} dr_err_t;

#endif /* DR_COMMON_H_ */
