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

#include "or1kmvp/system.h"

namespace or1kmvp {

system::system(const sc_core::sc_module_name& nm):
    vcml::system(nm),
    nrcpu("nrcpu", 1),
    mem("mem", vcml::range(OR1KMVP_MEM_ADDR, OR1KMVP_MEM_END)),
    uart0("uart0", vcml::range(OR1KMVP_UART0_ADDR, OR1KMVP_UART0_END)),
    uart1("uart1", vcml::range(OR1KMVP_UART1_ADDR, OR1KMVP_UART1_END)),
    rtc("rtc", vcml::range(OR1KMVP_RTC_ADDR, OR1KMVP_RTC_END)),
    gpio("gpio", vcml::range(OR1KMVP_GPIO_ADDR, OR1KMVP_GPIO_END)),
    ethoc("ethoc", vcml::range(OR1KMVP_ETHOC_ADDR, OR1KMVP_ETHOC_END)),
    ocfbc("ocfbc", vcml::range(OR1KMVP_OCFBC_ADDR, OR1KMVP_OCFBC_END)),
    ockbd("ethoc", vcml::range(OR1KMVP_OCKBD_ADDR, OR1KMVP_OCKBD_END)),
    ocspi("ocspi", vcml::range(OR1KMVP_OCSPI_ADDR, OR1KMVP_OCSPI_END)),
    ompic("ompic", vcml::range(OR1KMVP_OMPIC_ADDR, OR1KMVP_OMPIC_END)),
    hwrng("hwrng", vcml::range(OR1KMVP_HWRNG_ADDR, OR1KMVP_HWRNG_END)),
    sdhci("sdhci", vcml::range(OR1KMVP_SDHCI_ADDR, OR1KMVP_SDHCI_END)),
    m_cpus(nrcpu),
    m_clock("clock", OR1KMVP_CPU_DEFCLK),
    m_reset("reset"),
    m_bus("bus"),
    m_mem("mem", mem.get().length()),
    m_uart0("uart0"),
    m_uart1("uart1"),
    m_rtc("rtc", vcml::generic::rtc1742::NVMEM_8K),
    m_gpio("gpio"),
    m_hwrng("hwrng"),
    m_sdhci("sdhci"),
    m_ethoc("ethoc"),
    m_ocfbc("ocfbc"),
    m_ockbd("ockbd"),
    m_ocspi("ocspi"),
    m_ompic("ompic", nrcpu),
    m_spibus("spibus"),
    m_spi2sd("spi2sd"),
    m_sdcard0("sdcard0"),
    m_sdcard1("sdcard1"),
    m_sig_clock("sig_clock"),
    m_sig_reset("sig_reset"),
    m_gpio_spi0("gpio_spi0") {
    m_uart0.set_big_endian();
    m_uart1.set_big_endian();
    m_rtc.set_big_endian();
    m_gpio.set_big_endian();
    m_ethoc.set_big_endian();
    m_ocfbc.set_big_endian();
    m_ockbd.set_big_endian();
    m_ocspi.set_big_endian();
    m_ompic.set_big_endian();
    m_hwrng.set_big_endian();
    m_sdhci.set_little_endian();

    for (unsigned int cpu = 0; cpu < nrcpu; cpu++) {
        std::stringstream ss;
        ss << "cpu" << cpu;
        m_cpus[cpu] = new openrisc(ss.str().c_str(), cpu);
    }

    // Bus mapping
    for (openrisc* cpu : m_cpus) {
        m_bus.bind(cpu->insn);
        m_bus.bind(cpu->data);
    }

    m_bus.bind(m_mem.in, mem);
    m_bus.bind(m_uart0.in, uart0);
    m_bus.bind(m_uart1.in, uart1);
    m_bus.bind(m_rtc.in, rtc);
    m_bus.bind(m_gpio.in, gpio);
    m_bus.bind(m_hwrng.in, hwrng);
    m_bus.bind(m_sdhci.in, sdhci);
    m_bus.bind(m_sdhci.out);
    m_bus.bind(m_ompic.in, ompic);
    m_bus.bind(m_ethoc.in, ethoc);
    m_bus.bind(m_ethoc.out);
    m_bus.bind(m_ocfbc.in, ocfbc);
    m_bus.bind(m_ocfbc.out);
    m_bus.bind(m_ockbd.in, ockbd);
    m_bus.bind(m_ocspi.in, ocspi);

    // Clock
    m_clock.clk.bind(m_sig_clock);
    m_bus.clk.bind(m_sig_clock);
    m_mem.clk.bind(m_sig_clock);
    m_uart0.clk.bind(m_sig_clock);
    m_uart1.clk.bind(m_sig_clock);
    m_rtc.clk.bind(m_sig_clock);
    m_gpio.clk.bind(m_sig_clock);
    m_hwrng.clk.bind(m_sig_clock);
    m_sdhci.clk.bind(m_sig_clock);
    m_ethoc.clk.bind(m_sig_clock);
    m_ocfbc.clk.bind(m_sig_clock);
    m_ockbd.clk.bind(m_sig_clock);
    m_ocspi.clk.bind(m_sig_clock);
    m_ompic.clk.bind(m_sig_clock);
    m_spibus.clk.bind(m_sig_clock);
    m_spi2sd.clk.bind(m_sig_clock);
    m_sdcard0.clk.bind(m_sig_clock);
    m_sdcard1.clk.bind(m_sig_clock);

    for (auto cpu : m_cpus)
        cpu->clk.bind(m_sig_clock);

    // Reset
    m_reset.rst.bind(m_sig_reset);
    m_bus.rst.bind(m_sig_reset);
    m_mem.rst.bind(m_sig_reset);
    m_uart0.rst.bind(m_sig_reset);
    m_uart1.rst.bind(m_sig_reset);
    m_rtc.rst.bind(m_sig_reset);
    m_gpio.rst.bind(m_sig_reset);
    m_hwrng.rst.bind(m_sig_reset);
    m_sdhci.rst.bind(m_sig_reset);
    m_ethoc.rst.bind(m_sig_reset);
    m_ocfbc.rst.bind(m_sig_reset);
    m_ockbd.rst.bind(m_sig_reset);
    m_ocspi.rst.bind(m_sig_reset);
    m_ompic.rst.bind(m_sig_reset);
    m_spibus.rst.bind(m_sig_reset);
    m_spi2sd.rst.bind(m_sig_reset);
    m_sdcard0.rst.bind(m_sig_reset);
    m_sdcard1.rst.bind(m_sig_reset);

    for (auto cpu : m_cpus)
        cpu->rst.bind(m_sig_reset);

    // GPIOs
    m_gpio.ports[0].bind(m_gpio_spi0);

    // IRQ mapping
    for (auto cpu : m_cpus) {
        vcml::u64 id = cpu->core_id();

        unsigned int irq_uart0 = cpu->irq_uart0;
        unsigned int irq_uart1 = cpu->irq_uart1;
        unsigned int irq_ethoc = cpu->irq_ethoc;
        unsigned int irq_ocfbc = cpu->irq_ocfbc;
        unsigned int irq_ockbd = cpu->irq_ockbd;
        unsigned int irq_ocspi = cpu->irq_ocspi;
        unsigned int irq_ompic = cpu->irq_ompic;
        unsigned int irq_sdhci = cpu->irq_sdhci;

        cpu->irq[irq_uart0].bind(m_uart0.irq);
        cpu->irq[irq_uart1].bind(m_uart1.irq);
        cpu->irq[irq_ethoc].bind(m_ethoc.irq);
        cpu->irq[irq_ocfbc].bind(m_ocfbc.irq);
        cpu->irq[irq_ockbd].bind(m_ockbd.irq);
        cpu->irq[irq_ocspi].bind(m_ocspi.irq);
        cpu->irq[irq_sdhci].bind(m_sdhci.irq);

        cpu->irq[irq_ompic].bind(m_ompic.irq[id]);
    }

    // SPI controller -> SPI bus -> SD bus -> SD card
    m_ocspi.spi_out.bind(m_spibus.spi_in);
    m_spibus.bind(m_spi2sd.spi_in, m_gpio_spi0, false); // CS_ACTIVE_LOW

    // sdcard0 -> SDHCI, sdcard1 -> SPI
    m_sdhci.sd_out.bind(m_sdcard0.sd_in);
    m_spi2sd.sd_out.bind(m_sdcard1.sd_in);
}

system::~system() {
    for (auto cpu : m_cpus)
        SAFE_DELETE(cpu);
}

const char* system::version() const {
    return OR1KMVP_VERSION_STRING;
}

int system::run() {
    double simstart = vcml::realtime();
    int result      = vcml::system::run();
    double realtime = vcml::realtime() - simstart;
    double duration = sc_core::sc_time_stamp().to_seconds();

    log_info("duration           %.9fs", duration);
    log_info("runtime            %.4fs", realtime);
    log_info("real time ratio    %.2fs / 1s",
             duration == 0.0 ? 0.0 : realtime / duration);

    vcml::u64 ninsn = 0;
    for (auto cpu : m_cpus)
        ninsn += cpu->insn_count();
    log_info("sim speed          %.1f MIPS",
             realtime == 0.0 ? 0.0 : ninsn / realtime / 1e6);

    for (auto cpu : m_cpus)
        cpu->log_timing_info();

    return result;
}

void system::end_of_elaboration() {
    std::stringstream ss;
    m_bus.execute("show", VCML_NO_ARGS, ss);
    vcml::log_debug("%s", ss.str().c_str());
}

} // namespace or1kmvp