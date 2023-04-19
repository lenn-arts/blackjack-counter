// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VSCCB__Syms.h"


//======================

void VSCCB::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addCallback(&VSCCB::traceInit, &VSCCB::traceFull, &VSCCB::traceChg, this);
}
void VSCCB::traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->open()
    VSCCB* t=(VSCCB*)userthis;
    VSCCB__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (!Verilated::calcUnusedSigs()) {
	VL_FATAL_MT(__FILE__,__LINE__,__FILE__,"Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vcdp->scopeEscape(' ');
    t->traceInitThis(vlSymsp, vcdp, code);
    vcdp->scopeEscape('.');
}
void VSCCB::traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    VSCCB* t=(VSCCB*)userthis;
    VSCCB__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    t->traceFullThis(vlSymsp, vcdp, code);
}

//======================


void VSCCB::traceInitThis(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    vcdp->module(vlSymsp->name());  // Setup signal names
    // Body
    {
	vlTOPp->traceInitThis__1(vlSymsp, vcdp, code);
    }
}

void VSCCB::traceFullThis(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	vlTOPp->traceFullThis__1(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void VSCCB::traceInitThis__1(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	vcdp->declBit  (c+9,"clk",-1);
	vcdp->declBit  (c+10,"start",-1);
	vcdp->declBus  (c+11,"address",-1,7,0);
	vcdp->declBus  (c+12,"data",-1,7,0);
	vcdp->declBit  (c+13,"ready",-1);
	vcdp->declBit  (c+14,"SIOC_oe",-1);
	vcdp->declBit  (c+15,"SIOD_oe",-1);
	vcdp->declBus  (c+16,"SCCB CLK_FREQ",-1,31,0);
	vcdp->declBus  (c+17,"SCCB SCCB_FREQ",-1,31,0);
	vcdp->declBit  (c+9,"SCCB clk",-1);
	vcdp->declBit  (c+10,"SCCB start",-1);
	vcdp->declBus  (c+11,"SCCB address",-1,7,0);
	vcdp->declBus  (c+12,"SCCB data",-1,7,0);
	vcdp->declBit  (c+13,"SCCB ready",-1);
	vcdp->declBit  (c+14,"SCCB SIOC_oe",-1);
	vcdp->declBit  (c+15,"SCCB SIOD_oe",-1);
	vcdp->declBus  (c+18,"SCCB CAMERA_ADDR",-1,7,0);
	vcdp->declBus  (c+19,"SCCB FSM_IDLE",-1,31,0);
	vcdp->declBus  (c+20,"SCCB FSM_START_SIGNAL",-1,31,0);
	vcdp->declBus  (c+21,"SCCB FSM_LOAD_BYTE",-1,31,0);
	vcdp->declBus  (c+22,"SCCB FSM_TX_BYTE_1",-1,31,0);
	vcdp->declBus  (c+23,"SCCB FSM_TX_BYTE_2",-1,31,0);
	vcdp->declBus  (c+24,"SCCB FSM_TX_BYTE_3",-1,31,0);
	vcdp->declBus  (c+25,"SCCB FSM_TX_BYTE_4",-1,31,0);
	vcdp->declBus  (c+26,"SCCB FSM_END_SIGNAL_1",-1,31,0);
	vcdp->declBus  (c+27,"SCCB FSM_END_SIGNAL_2",-1,31,0);
	vcdp->declBus  (c+28,"SCCB FSM_END_SIGNAL_3",-1,31,0);
	vcdp->declBus  (c+29,"SCCB FSM_END_SIGNAL_4",-1,31,0);
	vcdp->declBus  (c+30,"SCCB FSM_DONE",-1,31,0);
	vcdp->declBus  (c+31,"SCCB FSM_TIMER",-1,31,0);
	vcdp->declBus  (c+1,"SCCB FSM_state",-1,3,0);
	vcdp->declBus  (c+2,"SCCB FSM_return_state",-1,3,0);
	vcdp->declBus  (c+3,"SCCB timer",-1,31,0);
	vcdp->declBus  (c+7,"SCCB latched_address",-1,7,0);
	vcdp->declBus  (c+8,"SCCB latched_data",-1,7,0);
	vcdp->declBus  (c+4,"SCCB byte_counter",-1,1,0);
	vcdp->declBus  (c+5,"SCCB tx_byte",-1,7,0);
	vcdp->declBus  (c+6,"SCCB byte_index",-1,3,0);
    }
}

void VSCCB::traceFullThis__1(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	vcdp->fullBus  (c+1,(vlTOPp->SCCB__DOT__FSM_state),4);
	vcdp->fullBus  (c+2,(vlTOPp->SCCB__DOT__FSM_return_state),4);
	vcdp->fullBus  (c+3,(vlTOPp->SCCB__DOT__timer),32);
	vcdp->fullBus  (c+4,(vlTOPp->SCCB__DOT__byte_counter),2);
	vcdp->fullBus  (c+5,(vlTOPp->SCCB__DOT__tx_byte),8);
	vcdp->fullBus  (c+6,(vlTOPp->SCCB__DOT__byte_index),4);
	vcdp->fullBus  (c+7,(vlTOPp->SCCB__DOT__latched_address),8);
	vcdp->fullBus  (c+8,(vlTOPp->SCCB__DOT__latched_data),8);
	vcdp->fullBit  (c+9,(vlTOPp->clk));
	vcdp->fullBit  (c+10,(vlTOPp->start));
	vcdp->fullBus  (c+11,(vlTOPp->address),8);
	vcdp->fullBus  (c+12,(vlTOPp->data),8);
	vcdp->fullBit  (c+13,(vlTOPp->ready));
	vcdp->fullBit  (c+14,(vlTOPp->SIOC_oe));
	vcdp->fullBit  (c+15,(vlTOPp->SIOD_oe));
	vcdp->fullBus  (c+16,(0x17d7840U),32);
	vcdp->fullBus  (c+17,(0x186a0U),32);
	vcdp->fullBus  (c+18,(0x42U),8);
	vcdp->fullBus  (c+19,(0U),32);
	vcdp->fullBus  (c+20,(1U),32);
	vcdp->fullBus  (c+21,(2U),32);
	vcdp->fullBus  (c+22,(3U),32);
	vcdp->fullBus  (c+23,(4U),32);
	vcdp->fullBus  (c+24,(5U),32);
	vcdp->fullBus  (c+25,(6U),32);
	vcdp->fullBus  (c+26,(7U),32);
	vcdp->fullBus  (c+27,(8U),32);
	vcdp->fullBus  (c+28,(9U),32);
	vcdp->fullBus  (c+29,(0xaU),32);
	vcdp->fullBus  (c+30,(0xbU),32);
	vcdp->fullBus  (c+31,(0xcU),32);
    }
}
