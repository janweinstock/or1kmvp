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

#include "or1kmvp/openrisc.h"

namespace or1kmvp {

bool openrisc::cmd_pic(const std::vector<std::string>& args,
                       std::ostream& os) {
    or1kiss::u32 picsr = m_iss->get_spr(or1kiss::SPR_PICSR, true);
    or1kiss::u32 picmr = m_iss->get_spr(or1kiss::SPR_PICMR, true);
    os << basename() << " interrupt controller status" << std::endl
       << "  PICSR: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << picsr << std::endl
       << "  PICMR: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << picmr << std::endl
       << std::endl;

    os << "  Masked: ";
    for (int i = 0; i < 32; i++) {
        if ((picmr & (1 << i)) == 0)
            os << std::dec << i << ", ";
    }

    if (picmr == 0)
        os << "none";
    os << std::endl;

    os << "  Pending: ";
    for (int i = 0; i < 32; i++) {
        if (picsr & (1 << i))
            os << std::dec << i << ", ";
    }

    if (picsr == 0)
        os << "none";
    // no newline needed

    return true;
}

bool openrisc::cmd_spr(const std::vector<std::string>& args,
                       std::ostream& os) {
    unsigned int grpid = atoi(args[0].c_str());
    if (grpid & ~0x1F) {
        os << "invalid group id: " << grpid;
        return false;
    }

    unsigned int regid = atoi(args[1].c_str());
    if (regid & ~0x7FFF) {
        os << "invalid group id: " << regid;
        return false;
    }

    unsigned int sprid = (grpid & 0x1F) << 11 | (regid & 0x7FF);
    if (args.size() > 2) {
        or1kiss::u32 val = vcml::from_string<or1kiss::u32>(args[2]);
        m_iss->set_spr(sprid, true);
        os << "SPR[" << grpid << "," << regid << "] = SPR[" << sprid
           << "] = 0x" << std::hex << std::setw(8) << std::setfill('0') << val
           << " written";
    } else {
        or1kiss::u32 val = m_iss->get_spr(sprid, true);
        os << "SPR[" << grpid << "," << regid << "] = SPR[" << sprid
           << "] = 0x" << std::hex << std::setw(8) << std::setfill('0') << val;
    }

    return true;
}

void openrisc::log_timing_info() const {
    double rt = get_run_time();
    vcml::u64 nc = cycle_count();

    log_info("clock speed   %.1f MHz", m_iss->get_clock() / 1e6);
    log_info("iss runtime   %.4f s", rt);
    log_info("iss speed     %.1f MIPS",
             rt == 0.0 ? 0.0 : m_iss->get_num_instructions() / rt * 1e-6);
    log_info("instructions  %" PRId64, m_iss->get_num_instructions());
    log_info("cycles        %" PRId64, nc);
    log_info("sleep-cycles  %" PRId64 " (%.1f%%)",
             m_iss->get_num_sleep_cycles(),
             nc == 0 ? 0.0 : m_iss->get_num_sleep_cycles() * 100.0 / nc);
    log_info("jit hit-rate  %.4f", m_iss->get_decode_cache_hit_rate());
    log_info("#lwa          %" PRId64, m_iss->get_num_lwa());
    log_info("#swa          %" PRId64, m_iss->get_num_swa());
    log_info("#swa failed   %" PRId64, m_iss->get_num_swa_failed());

    for (auto it : irq) {
        vcml::irq_stats stats;
        if (!get_irq_stats(it.first, stats) || stats.irq_count == 0)
            continue;

        std::string s;
        s += vcml::mkstr("irq %d status", stats.irq);
        s += vcml::mkstr("  %d#", stats.irq_count);
        s += vcml::mkstr(", avg %.1fus", stats.irq_uptime.to_seconds() /
                                             stats.irq_count * 1e6);
        s += vcml::mkstr(", max %.1fus", stats.irq_longest.to_seconds() * 1e6);
        log_info("%s", s.c_str());
    }
}

using vcml::VCML_ACCESS_READ;
using vcml::VCML_ACCESS_WRITE;
using vcml::VCML_ACCESS_READ_WRITE;

#define SPR(x) (32 + or1kiss::SPR_##x)

static const std::vector<vcml::debugging::cpureg> OPENRISC_CPUREGS = {
    { 0, "gpr/r0", 4, VCML_ACCESS_READ_WRITE },
    { 1, "gpr/r1", 4, VCML_ACCESS_READ_WRITE },
    { 2, "gpr/r2", 4, VCML_ACCESS_READ_WRITE },
    { 3, "gpr/r3", 4, VCML_ACCESS_READ_WRITE },
    { 4, "gpr/r4", 4, VCML_ACCESS_READ_WRITE },
    { 5, "gpr/r5", 4, VCML_ACCESS_READ_WRITE },
    { 6, "gpr/r6", 4, VCML_ACCESS_READ_WRITE },
    { 7, "gpr/r7", 4, VCML_ACCESS_READ_WRITE },
    { 8, "gpr/r8", 4, VCML_ACCESS_READ_WRITE },
    { 9, "gpr/r9", 4, VCML_ACCESS_READ_WRITE },
    { 10, "gpr/r10", 4, VCML_ACCESS_READ_WRITE },
    { 11, "gpr/r11", 4, VCML_ACCESS_READ_WRITE },
    { 12, "gpr/r12", 4, VCML_ACCESS_READ_WRITE },
    { 13, "gpr/r13", 4, VCML_ACCESS_READ_WRITE },
    { 14, "gpr/r14", 4, VCML_ACCESS_READ_WRITE },
    { 15, "gpr/r15", 4, VCML_ACCESS_READ_WRITE },
    { 16, "gpr/r16", 4, VCML_ACCESS_READ_WRITE },
    { 17, "gpr/r17", 4, VCML_ACCESS_READ_WRITE },
    { 18, "gpr/r18", 4, VCML_ACCESS_READ_WRITE },
    { 19, "gpr/r19", 4, VCML_ACCESS_READ_WRITE },
    { 20, "gpr/r20", 4, VCML_ACCESS_READ_WRITE },
    { 21, "gpr/r21", 4, VCML_ACCESS_READ_WRITE },
    { 22, "gpr/r22", 4, VCML_ACCESS_READ_WRITE },
    { 23, "gpr/r23", 4, VCML_ACCESS_READ_WRITE },
    { 24, "gpr/r24", 4, VCML_ACCESS_READ_WRITE },
    { 25, "gpr/r25", 4, VCML_ACCESS_READ_WRITE },
    { 26, "gpr/r26", 4, VCML_ACCESS_READ_WRITE },
    { 27, "gpr/r27", 4, VCML_ACCESS_READ_WRITE },
    { 28, "gpr/r28", 4, VCML_ACCESS_READ_WRITE },
    { 29, "gpr/r29", 4, VCML_ACCESS_READ_WRITE },
    { 30, "gpr/r30", 4, VCML_ACCESS_READ_WRITE },
    { 31, "gpr/r31", 4, VCML_ACCESS_READ_WRITE },

    { SPR(VR), "spr/sys/vr", 4, VCML_ACCESS_READ },
    { SPR(UPR), "spr/sys/upr", 4, VCML_ACCESS_READ },
    { SPR(CPUCFGR), "spr/sys/cpucfgr", 4, VCML_ACCESS_READ },
    { SPR(DMMUCFGR), "spr/sys/dmmucfgr", 4, VCML_ACCESS_READ },
    { SPR(IMMUCFGR), "spr/sys/immucfgr", 4, VCML_ACCESS_READ },
    { SPR(DCCFGR), "spr/sys/dccfgr", 4, VCML_ACCESS_READ },
    { SPR(ICCFGR), "spr/sys/iccfgr", 4, VCML_ACCESS_READ },
    { SPR(VR2), "spr/sys/vr2", 4, VCML_ACCESS_READ },
    { SPR(AVR), "spr/sys/avr", 4, VCML_ACCESS_READ },
    { SPR(EVBAR), "spr/sys/evbar", 4, VCML_ACCESS_READ_WRITE },
    { SPR(AECR), "spr/sys/aecr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(AESR), "spr/sys/aesr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(NPC), "spr/sys/npc", 4, VCML_ACCESS_READ_WRITE },
    { SPR(SR), "spr/sys/sr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(PPC), "spr/sys/ppc", 4, VCML_ACCESS_READ_WRITE },
    { SPR(FPCSR), "spr/sys/fpcsr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(EPCR), "spr/sys/epcr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(EEAR), "spr/sys/eear", 4, VCML_ACCESS_READ_WRITE },
    { SPR(ESR), "spr/sys/esr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(COREID), "spr/sys/coreid", 4, VCML_ACCESS_READ },
    { SPR(NUMCORES), "spr/sys/numcores", 4, VCML_ACCESS_READ },

    { SPR(DMMUCR), "spr/dmmu/dmmucr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(DTLBEIR), "spr/dmmu/tlbeir", 4, VCML_ACCESS_WRITE },

    { SPR(IMMUCR), "spr/immu/immucr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(ITLBEIR), "spr/immu/tlbeir", 4, VCML_ACCESS_WRITE },

    { SPR(DCBPR), "spr/dc/dcbpr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(DCBFR), "spr/dc/dcbfr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(DCBIR), "spr/dc/dcbir", 4, VCML_ACCESS_WRITE },
    { SPR(DCBWR), "spr/dc/dcbwr", 4, VCML_ACCESS_WRITE },
    { SPR(DCBLR), "spr/dc/dcblr", 4, VCML_ACCESS_WRITE },

    { SPR(ICBPR), "spr/ic/icbpr", 4, VCML_ACCESS_WRITE },
    { SPR(ICBIR), "spr/ic/icbir", 4, VCML_ACCESS_WRITE },
    { SPR(ICBLR), "spr/ic/icblr", 4, VCML_ACCESS_WRITE },

    { SPR(MACLO), "spr/mac/lo", 4, VCML_ACCESS_READ_WRITE },
    { SPR(MACHI), "spr/mac/hi", 4, VCML_ACCESS_READ_WRITE },

    { SPR(PMR), "spr/pmr/pm", 4, VCML_ACCESS_READ_WRITE },

    { SPR(PICMR), "spr/pic/picmr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(PICSR), "spr/pic/picsr", 4, VCML_ACCESS_READ_WRITE },

    { SPR(TTMR), "spr/tt/ttmr", 4, VCML_ACCESS_READ_WRITE },
    { SPR(TTCR), "spr/tt/ttcr", 4, VCML_ACCESS_READ_WRITE },
};
#undef SPR

openrisc::openrisc(const sc_core::sc_module_name& nm, unsigned int id):
    vcml::processor(nm, "or1k"),
    or1kiss::env(or1kiss::ENDIAN_BIG),
    m_iss(NULL),
    enable_decode_cache("enable_decode_cache", true),
    enable_sleep_mode("enable_sleep_mode", true),
    enable_insn_dmi("enable_insn_dmi", allow_dmi),
    enable_data_dmi("enable_data_dmi", allow_dmi),
    irq_ompic("irq_ompic", OR1KMVP_IRQ_OMPIC),
    irq_uart0("irq_uart0", OR1KMVP_IRQ_UART0),
    irq_uart1("irq_uart1", OR1KMVP_IRQ_UART1),
    irq_ethoc("irq_ethoc", OR1KMVP_IRQ_ETHOC),
    irq_ocfbc("irq_ocfbc", OR1KMVP_IRQ_OCFBC),
    irq_ockbd("irq_ockbd", OR1KMVP_IRQ_OCKBD),
    irq_ocspi("irq_ocspi", OR1KMVP_IRQ_OCSPI),
    irq_sdhci("irq_sdhci", OR1KMVP_IRQ_SDHCI),
    insn_trace_file("insn_trace_file", "") {
    or1kiss::decode_cache_size sz;
    sz = enable_decode_cache ? or1kiss::DECODE_CACHE_SIZE_8M
                             : or1kiss::DECODE_CACHE_OFF;

    m_iss = new or1kiss::or1k(this, sz);
    m_iss->set_core_id(id);
    m_iss->allow_sleep(enable_sleep_mode);

    if (!insn_trace_file.get().empty())
        m_iss->trace(insn_trace_file);

    register_command("pic", 0, &openrisc::cmd_pic,
                     "prints PIC status and pending interrupts");
    register_command("spr", 2, &openrisc::cmd_spr,
                     "reads or writes SPR <grpid> <regid> [value]");

    set_big_endian();
    define_cpuregs(OPENRISC_CPUREGS);
}

openrisc::~openrisc() {
    if (m_iss)
        delete m_iss;
}

void openrisc::reset() {
    memset(m_iss->gpr, 0, sizeof(m_iss->gpr));
    processor::reset();

    m_iss->reset_cycles();
    m_iss->reset_instructions();
    m_iss->reset_compiles();
    m_iss->reset_sleep_cycles();
    m_iss->set_spr(or1kiss::SPR_NPC, 0x100, true);
}

const char* openrisc::version() const {
    return OR1KISS_VERSION_STRING;
}

void openrisc::stacktrace(std::vector<vcml::debugging::stackframe>& frames,
                          size_t limit) {
    vcml::u64 pc = program_counter();
    vcml::u64 fp = frame_pointer();

    const auto& symtab = vcml::debugging::target::symbols();
    frames.clear();

    for (size_t i = 0; i < limit; i++) {
        vcml::debugging::stackframe frame;
        frame.program_counter = pc;
        frame.frame_pointer = fp;

        frame.sym = symtab.find_function(pc);
        frames.push_back(frame);

        vcml::u64 sp = fp;

        if (fp < 8) // minimum frame size
            break;

        if (!read_vmem_dbg(fp - 4, &pc, 4))
            break;

        if (!read_vmem_dbg(fp - 8, &fp, 4))
            break;

        if (fp < sp) // stack should grow down
            break;
    }
}

bool openrisc::disassemble(vcml::u8* ibuf, vcml::u64& addr,
                           std::string& code) {
    if (addr & 3) // check alignment
        return false;

    or1kiss::u32 insn = 0;
    or1kiss::memcpyswp(&insn, ibuf, sizeof(insn));
    code = or1kiss::disassemble(insn);
    addr += 4;

    return true;
}

vcml::u64 openrisc::program_counter() {
    return m_iss->get_spr(or1kiss::SPR_NPC, true);
}

vcml::u64 openrisc::link_register() {
    return m_iss->gpr[9];
}

vcml::u64 openrisc::stack_pointer() {
    return m_iss->gpr[1];
}

vcml::u64 openrisc::frame_pointer() {
    return m_iss->gpr[2];
}

vcml::u64 openrisc::core_id() {
    return m_iss->get_core_id();
}

void openrisc::set_core_id(vcml::u64 id) {
    m_iss->set_core_id(id);
}

vcml::u64 openrisc::cycle_count() const {
    return m_iss->get_num_cycles();
}

void openrisc::interrupt(unsigned int irq, bool set) {
    m_iss->interrupt(irq, set);
}

void openrisc::simulate(unsigned int n) {
    switch (m_iss->step(n)) {
    case or1kiss::STEP_EXIT:
        sc_core::sc_stop();
        wait();
        break;

    case or1kiss::STEP_BREAKPOINT:
        notify_breakpoint_hit(program_counter());
        break;

    case or1kiss::STEP_WATCHPOINT: {
        or1kiss::watchpoint_event event;
        if (m_iss->get_watchpoint_event(event)) {
            vcml::range addr(event.addr, event.addr + event.size - 1);
            if (event.iswr)
                notify_watchpoint_write(addr, event.wval);
            else
                notify_watchpoint_read(addr);
        }
        break;
    }

    case or1kiss::STEP_OK:
    default:
        break;
    }
}

void openrisc::handle_clock_update(clock_t oldclk, clock_t newclk) {
    processor::handle_clock_update(oldclk, newclk);
    m_iss->set_clock(newclk);
}

or1kiss::response openrisc::transact(const or1kiss::request& req) {
    vcml::tlm_sbi info = vcml::SBI_NONE;
    if (req.is_debug())
        info |= vcml::SBI_DEBUG;
    if (req.is_exclusive())
        info |= vcml::SBI_EXCL;

    tlm::tlm_response_status rs;
    vcml::tlm_initiator_socket& port = req.is_imem() ? insn : data;

    sc_core::sc_time now = local_time_stamp();

    unsigned int nbytes = 0;
    if (req.is_write())
        rs = port.write(req.addr, req.data, req.size, info, &nbytes);
    else
        rs = port.read(req.addr, req.data, req.size, info, &nbytes);

    // Time-keeping
    if (!req.is_debug())
        req.cycles = (local_time_stamp() - now) / clock_cycle();

    // Check bus error
    if (rs != tlm::TLM_OK_RESPONSE) {
        log_bus_error(
            port,
            req.is_read() ? vcml::VCML_ACCESS_READ : vcml::VCML_ACCESS_WRITE,
            rs, req.addr, req.size);
        return or1kiss::RESP_ERROR;
    }

    tlm::tlm_dmi dmi;
    if (req.is_dmem() && enable_data_dmi && !get_data_ptr(req.addr)) {
        if (data.dmi().lookup(req.addr, req.addr + req.size - 1,
                              tlm::TLM_READ_COMMAND, dmi)) {
            set_data_ptr(dmi.get_dmi_ptr(), dmi.get_start_address(),
                         dmi.get_end_address());
        }
    }

    if (req.is_imem() && enable_insn_dmi && !get_insn_ptr(req.addr)) {
        if (insn.dmi().lookup(req.addr, req.addr + req.size - 1,
                              tlm::TLM_READ_COMMAND, dmi)) {
            set_insn_ptr(dmi.get_dmi_ptr(), dmi.get_start_address(),
                         dmi.get_end_address());
        }
    }

    if (req.is_exclusive() && (nbytes != req.size))
        return or1kiss::RESP_FAILED;
    return or1kiss::RESP_SUCCESS;
}

bool openrisc::read_reg_dbg(vcml::u64 regno, vcml::u64& val) {
    if (regno < 32)
        val = m_iss->gpr[regno];
    else
        val = m_iss->get_spr(regno - 32, true);
    return true;
}

bool openrisc::write_reg_dbg(vcml::u64 regno, vcml::u64 val) {
    if (regno < 32)
        m_iss->gpr[regno] = val;
    else
        m_iss->set_spr(regno - 32, val, true);
    return true;
}

bool openrisc::page_size(vcml::u64& size) {
    size = OR1KISS_PAGE_SIZE;
    return m_iss->is_dmmu_active() || m_iss->is_immu_active();
}

bool openrisc::virt_to_phys(vcml::u64 va, vcml::u64& pa) {
    if (!m_iss->is_dmmu_active() && !m_iss->is_immu_active()) {
        pa = va;
        return true;
    }

    or1kiss::request req;
    req.set_imem();
    req.set_read();
    req.set_debug();
    req.addr = va;

    if (m_iss->get_dmmu()->translate(req) == or1kiss::MMU_OKAY) {
        pa = req.addr;
        return true;
    }

    if (m_iss->get_immu()->translate(req) == or1kiss::MMU_OKAY) {
        pa = req.addr;
        return true;
    }

    return false;
}

bool openrisc::insert_breakpoint(vcml::u64 addr) {
    if (addr > std::numeric_limits<or1kiss::u32>::max())
        return false;

    m_iss->insert_breakpoint((or1kiss::u32)addr);
    return true;
}

bool openrisc::remove_breakpoint(vcml::u64 addr) {
    if (addr > std::numeric_limits<or1kiss::u32>::max())
        return false;

    m_iss->remove_breakpoint((or1kiss::u32)addr);
    return true;
}

bool openrisc::insert_watchpoint(const vcml::range& mem,
                                 vcml::vcml_access prot) {
    if (mem.end > std::numeric_limits<or1kiss::u32>::max())
        return false;

    or1kiss::u32 addr = (or1kiss::u32)mem.start;
    or1kiss::u32 size = (or1kiss::u32)mem.length();

    if (vcml::is_read_allowed(prot))
        m_iss->insert_watchpoint_r(addr, size);
    if (vcml::is_write_allowed(prot))
        m_iss->insert_watchpoint_w(addr, size);

    return true;
}

bool openrisc::remove_watchpoint(const vcml::range& mem,
                                 vcml::vcml_access prot) {
    if (mem.end > std::numeric_limits<or1kiss::u32>::max())
        return false;

    or1kiss::u32 addr = (or1kiss::u32)mem.start;
    or1kiss::u32 size = (or1kiss::u32)mem.length();

    if (vcml::is_read_allowed(prot))
        m_iss->remove_watchpoint_r(addr, size);
    if (vcml::is_write_allowed(prot))
        m_iss->remove_watchpoint_w(addr, size);

    return true;
}

} // namespace or1kmvp
