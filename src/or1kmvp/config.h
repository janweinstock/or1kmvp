/******************************************************************************
 *                                                                            *
 * Copyright 2018 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef OR1KMVP_CONFIG_H
#define OR1KMVP_CONFIG_H

#include "or1kmvp/common.h"

/* Default cpu clock */
#define OR1KMVP_CPU_DEFCLK (100 * vcml::MHz)

/* Memory map */
#define OR1KMVP_MEM_ADDR (0x00000000)
#define OR1KMVP_MEM_SIZE (0x08000000) // 128 MB
#define OR1KMVP_MEM_END  (OR1KMVP_MEM_ADDR + OR1KMVP_MEM_SIZE - 1)

#define OR1KMVP_UART0_ADDR (0x90000000)
#define OR1KMVP_UART0_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_UART0_END  (OR1KMVP_UART0_ADDR + OR1KMVP_UART0_SIZE - 1)

#define OR1KMVP_UART1_ADDR (0x91000000)
#define OR1KMVP_UART1_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_UART1_END  (OR1KMVP_UART1_ADDR + OR1KMVP_UART1_SIZE - 1)

#define OR1KMVP_ETHOC_ADDR (0x92000000)
#define OR1KMVP_ETHOC_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_ETHOC_END  (OR1KMVP_ETHOC_ADDR + OR1KMVP_ETHOC_SIZE - 1)

#define OR1KMVP_OCFBC_ADDR (0x93000000)
#define OR1KMVP_OCFBC_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_OCFBC_END  (OR1KMVP_OCFBC_ADDR + OR1KMVP_OCFBC_SIZE - 1)

#define OR1KMVP_OCKBD_ADDR (0x94000000)
#define OR1KMVP_OCKBD_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_OCKBD_END  (OR1KMVP_OCKBD_ADDR + OR1KMVP_OCKBD_SIZE - 1)

#define OR1KMVP_RTC_ADDR (0x95000000)
#define OR1KMVP_RTC_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_RTC_END  (OR1KMVP_RTC_ADDR + OR1KMVP_RTC_SIZE - 1)

#define OR1KMVP_OCSPI_ADDR (0x96000000)
#define OR1KMVP_OCSPI_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_OCSPI_END  (OR1KMVP_OCSPI_ADDR + OR1KMVP_OCSPI_SIZE - 1)

#define OR1KMVP_GPIO_ADDR (0x97000000)
#define OR1KMVP_GPIO_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_GPIO_END  (OR1KMVP_GPIO_ADDR + OR1KMVP_GPIO_SIZE - 1)

#define OR1KMVP_OMPIC_ADDR (0x98000000)
#define OR1KMVP_OMPIC_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_OMPIC_END  (OR1KMVP_OMPIC_ADDR + OR1KMVP_OMPIC_SIZE - 1)

#define OR1KMVP_HWRNG_ADDR (0x99000000)
#define OR1KMVP_HWRNG_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_HWRNG_END  (OR1KMVP_HWRNG_ADDR + OR1KMVP_HWRNG_SIZE - 1)

#define OR1KMVP_SDHCI_ADDR (0x9a000000)
#define OR1KMVP_SDHCI_SIZE (OR1KISS_PAGE_SIZE)
#define OR1KMVP_SDHCI_END  (OR1KMVP_SDHCI_ADDR + OR1KMVP_SDHCI_SIZE - 1)

/* Interrupt map */
#define OR1KMVP_IRQ_OMPIC (1)
#define OR1KMVP_IRQ_UART0 (2)
#define OR1KMVP_IRQ_UART1 (3)
#define OR1KMVP_IRQ_ETHOC (4)
#define OR1KMVP_IRQ_OCFBC (5)
#define OR1KMVP_IRQ_OCKBD (6)
#define OR1KMVP_IRQ_OCSPI (7)
#define OR1KMVP_IRQ_SDHCI (8)

#endif
