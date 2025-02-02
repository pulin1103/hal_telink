/******************************************************************************
 * Copyright (c) 2022 Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************/

/********************************************************************************************************
 * @file	types.h
 *
 * @brief	This is the header file for B9x
 *
 * @author	Driver Group
 *
 *******************************************************************************************************/
#ifndef TYPES_H_
#define TYPES_H_

#include <stdbool.h>

#ifndef NULL
#define NULL  0
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

typedef unsigned char u8 ;
typedef unsigned short u16 ;
typedef unsigned int u32 ;
typedef unsigned long long u64 ;
typedef signed char s8 ;
typedef signed short s16 ;
typedef signed int s32 ;
typedef signed long long s64 ;

#endif /* TYPES_H_ */
