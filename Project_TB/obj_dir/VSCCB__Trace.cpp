// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VSCCB__Syms.h"


//======================

void VSCCB::traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    VSCCB* t=(VSCCB*)userthis;
    VSCCB__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (vlSymsp->getClearActivity()) {
	t->traceChgThis(vlSymsp, vcdp, code);
    }
}

//======================


void VSCCB::traceChgThis(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	if (VL_UNLIKELY((1U & (vlTOPp->__Vm_traceActivity 
			       | (vlTOPp->__Vm_traceActivity 
				  >> 1U))))) {
	    vlTOPp->traceChgThis__2(vlSymsp, vcdp, code);
	}
	if (VL_UNLIKELY((2U & vlTOPp->__Vm_traceActivity))) {
	    vlTOPp->traceChgThis__3(vlSymsp, vcdp, code);
	}
	vlTOPp->traceChgThis__4(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void VSCCB::traceChgThis__2(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	vcdp->chgBus  (c+1,(vlTOPp->SCCB__DOT__FSM_state),4);
	vcdp->chgBus  (c+2,(vlTOPp->SCCB__DOT__FSM_return_state),4);
	vcdp->chgBus  (c+3,(vlTOPp->SCCB__DOT__timer),32);
	vcdp->chgBus  (c+4,(vlTOPp->SCCB__DOT__byte_counter),2);
	vcdp->chgBus  (c+5,(vlTOPp->SCCB__DOT__tx_byte),8);
	vcdp->chgBus  (c+6,(vlTOPp->SCCB__DOT__byte_index),4);
    }
}

void VSCCB::traceChgThis__3(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	vcdp->chgBus  (c+7,(vlTOPp->SCCB__DOT__latched_address),8);
	vcdp->chgBus  (c+8,(vlTOPp->SCCB__DOT__latched_data),8);
    }
}

void VSCCB::traceChgThis__4(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c=code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
	vcdp->chgBit  (c+9,(vlTOPp->clk));
	vcdp->chgBit  (c+10,(vlTOPp->start));
	vcdp->chgBus  (c+11,(vlTOPp->address),8);
	vcdp->chgBus  (c+12,(vlTOPp->data),8);
	vcdp->chgBit  (c+13,(vlTOPp->ready));
	vcdp->chgBit  (c+14,(vlTOPp->SIOC_oe));
	vcdp->chgBit  (c+15,(vlTOPp->SIOD_oe));
    }
}
