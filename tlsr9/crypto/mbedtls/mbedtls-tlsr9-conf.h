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

#ifndef MBEDTLS_TSLR9_CONF_H
#define MBEDTLS_TSLR9_CONF_H

#ifdef MBEDTLS_PLATFORM_MEMORY
#undef MBEDTLS_PLATFORM_MEMORY
#endif

#ifdef MBEDTLS_MEMORY_BUFFER_ALLOC_C
#undef MBEDTLS_MEMORY_BUFFER_ALLOC_C
#endif

#define MBEDTLS_AES_ALT
#define MBEDTLS_ECP_ALT

#endif /* MBEDTLS_TSLR9_CONF_H */
