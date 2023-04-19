// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSCCB.h for the primary calling header

#include "VSCCB.h"             // For This
#include "VSCCB__Syms.h"


//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(VSCCB) {
    VSCCB__Syms* __restrict vlSymsp = __VlSymsp = new VSCCB__Syms(this, name());
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VSCCB::__Vconfigure(VSCCB__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

VSCCB::~VSCCB() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void VSCCB::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VSCCB::eval\n"); );
    VSCCB__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
	VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
	vlSymsp->__Vm_activity = true;
	_eval(vlSymsp);
	if (VL_UNLIKELY(++__VclockLoop > 100)) {
	    // About to fail, so enable debug to see what's not settling.
	    // Note you must run make with OPT=-DVL_DEBUG for debug prints.
	    int __Vsaved_debug = Verilated::debug();
	    Verilated::debug(1);
	    __Vchange = _change_request(vlSymsp);
	    Verilated::debug(__Vsaved_debug);
	    VL_FATAL_MT(__FILE__,__LINE__,__FILE__,"Verilated model didn't converge");
	} else {
	    __Vchange = _change_request(vlSymsp);
	}
    } while (VL_UNLIKELY(__Vchange));
}

void VSCCB::_eval_initial_loop(VSCCB__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
	_eval_settle(vlSymsp);
	_eval(vlSymsp);
	if (VL_UNLIKELY(++__VclockLoop > 100)) {
	    // About to fail, so enable debug to see what's not settling.
	    // Note you must run make with OPT=-DVL_DEBUG for debug prints.
	    int __Vsaved_debug = Verilated::debug();
	    Verilated::debug(1);
	    __Vchange = _change_request(vlSymsp);
	    Verilated::debug(__Vsaved_debug);
	    VL_FATAL_MT(__FILE__,__LINE__,__FILE__,"Verilated model didn't DC converge");
	} else {
	    __Vchange = _change_request(vlSymsp);
	}
    } while (VL_UNLIKELY(__Vchange));
}

//--------------------
// Internal Methods

void VSCCB::_initial__TOP__1(VSCCB__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_initial__TOP__1\n"); );
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // INITIAL at SCCB.sv:61
    vlTOPp->SIOC_oe = 0U;
    vlTOPp->SIOD_oe = 0U;
    vlTOPp->ready = 1U;
    // INITIAL at SCCB.sv:67
    vlTOPp->SCCB__DOT__FSM_state = 0U;
    // INITIAL at SCCB.sv:68
    vlTOPp->SCCB__DOT__FSM_return_state = 0U;
    // INITIAL at SCCB.sv:69
    vlTOPp->SCCB__DOT__timer = 0U;
    // INITIAL at SCCB.sv:72
    vlTOPp->SCCB__DOT__byte_counter = 0U;
    // INITIAL at SCCB.sv:73
    vlTOPp->SCCB__DOT__tx_byte = 0U;
    // INITIAL at SCCB.sv:74
    vlTOPp->SCCB__DOT__byte_index = 0U;
}

VL_INLINE_OPT void VSCCB::_sequent__TOP__2(VSCCB__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_sequent__TOP__2\n"); );
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__SCCB__DOT__byte_index,3,0);
    VL_SIG8(__Vdly__SCCB__DOT__byte_counter,1,0);
    VL_SIG8(__Vdly__SCCB__DOT__FSM_state,3,0);
    VL_SIG8(__Vdly__SCCB__DOT__latched_address,7,0);
    VL_SIG8(__Vdly__SCCB__DOT__latched_data,7,0);
    VL_SIG8(__Vdly__SCCB__DOT__FSM_return_state,3,0);
    VL_SIG8(__Vdly__SCCB__DOT__tx_byte,7,0);
    VL_SIG(__Vdly__SCCB__DOT__timer,31,0);
    // Body
    __Vdly__SCCB__DOT__latched_address = vlTOPp->SCCB__DOT__latched_address;
    __Vdly__SCCB__DOT__latched_data = vlTOPp->SCCB__DOT__latched_data;
    __Vdly__SCCB__DOT__byte_index = vlTOPp->SCCB__DOT__byte_index;
    __Vdly__SCCB__DOT__byte_counter = vlTOPp->SCCB__DOT__byte_counter;
    __Vdly__SCCB__DOT__FSM_state = vlTOPp->SCCB__DOT__FSM_state;
    __Vdly__SCCB__DOT__FSM_return_state = vlTOPp->SCCB__DOT__FSM_return_state;
    __Vdly__SCCB__DOT__timer = vlTOPp->SCCB__DOT__timer;
    __Vdly__SCCB__DOT__tx_byte = vlTOPp->SCCB__DOT__tx_byte;
    // ALWAYS at SCCB.sv:77
    if (((((((((0U == (IData)(vlTOPp->SCCB__DOT__FSM_state)) 
	       | (1U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) 
	      | (2U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) 
	     | (3U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) 
	    | (4U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) 
	   | (5U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) 
	  | (6U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) 
	 | (7U == (IData)(vlTOPp->SCCB__DOT__FSM_state)))) {
	if ((0U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
	    __Vdly__SCCB__DOT__byte_index = 0U;
	    __Vdly__SCCB__DOT__byte_counter = 0U;
	    if (vlTOPp->start) {
		__Vdly__SCCB__DOT__FSM_state = 1U;
		__Vdly__SCCB__DOT__latched_address 
		    = vlTOPp->address;
		__Vdly__SCCB__DOT__latched_data = vlTOPp->data;
		vlTOPp->ready = 0U;
	    } else {
		vlTOPp->ready = 1U;
	    }
	} else {
	    if ((1U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
		__Vdly__SCCB__DOT__FSM_state = 0xcU;
		__Vdly__SCCB__DOT__FSM_return_state = 2U;
		__Vdly__SCCB__DOT__timer = 0x3eU;
		vlTOPp->SIOC_oe = 0U;
		vlTOPp->SIOD_oe = 1U;
	    } else {
		if ((2U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
		    __Vdly__SCCB__DOT__FSM_state = 
			(0xfU & ((3U == (IData)(vlTOPp->SCCB__DOT__byte_counter))
				  ? 7U : 3U));
		    __Vdly__SCCB__DOT__byte_index = 0U;
		    __Vdly__SCCB__DOT__byte_counter 
			= (3U & ((IData)(1U) + (IData)(vlTOPp->SCCB__DOT__byte_counter)));
		    __Vdly__SCCB__DOT__tx_byte = ((0U 
						   == (IData)(vlTOPp->SCCB__DOT__byte_counter))
						   ? 0x42U
						   : 
						  ((1U 
						    == (IData)(vlTOPp->SCCB__DOT__byte_counter))
						    ? (IData)(vlTOPp->SCCB__DOT__latched_address)
						    : (IData)(vlTOPp->SCCB__DOT__latched_data)));
		} else {
		    if ((3U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
			__Vdly__SCCB__DOT__FSM_state = 0xcU;
			__Vdly__SCCB__DOT__FSM_return_state = 4U;
			__Vdly__SCCB__DOT__timer = 0x3eU;
			vlTOPp->SIOC_oe = 1U;
		    } else {
			if ((4U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
			    __Vdly__SCCB__DOT__FSM_state = 0xcU;
			    __Vdly__SCCB__DOT__FSM_return_state = 5U;
			    __Vdly__SCCB__DOT__timer = 0x3eU;
			    vlTOPp->SIOD_oe = (1U & 
					       ((8U 
						 == (IData)(vlTOPp->SCCB__DOT__byte_index))
						 ? 0U
						 : 
						(~ 
						 (1U 
						  & ((IData)(vlTOPp->SCCB__DOT__tx_byte) 
						     >> 7U)))));
			} else {
			    if ((5U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
				__Vdly__SCCB__DOT__FSM_state = 0xcU;
				__Vdly__SCCB__DOT__FSM_return_state = 6U;
				__Vdly__SCCB__DOT__timer = 0x7dU;
				vlTOPp->SIOC_oe = 0U;
			    } else {
				if ((6U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
				    __Vdly__SCCB__DOT__tx_byte 
					= (0xffU & 
					   ((IData)(vlTOPp->SCCB__DOT__tx_byte) 
					    << 1U));
				    __Vdly__SCCB__DOT__FSM_state 
					= (0xfU & (
						   (8U 
						    == (IData)(vlTOPp->SCCB__DOT__byte_index))
						    ? 2U
						    : 3U));
				    __Vdly__SCCB__DOT__byte_index 
					= (0xfU & ((IData)(1U) 
						   + (IData)(vlTOPp->SCCB__DOT__byte_index)));
				} else {
				    __Vdly__SCCB__DOT__FSM_state = 0xcU;
				    __Vdly__SCCB__DOT__FSM_return_state = 8U;
				    __Vdly__SCCB__DOT__timer = 0x3eU;
				    vlTOPp->SIOC_oe = 1U;
				}
			    }
			}
		    }
		}
	    }
	}
    } else {
	if ((8U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
	    __Vdly__SCCB__DOT__FSM_state = 0xcU;
	    __Vdly__SCCB__DOT__FSM_return_state = 9U;
	    __Vdly__SCCB__DOT__timer = 0x3eU;
	    vlTOPp->SIOD_oe = 1U;
	} else {
	    if ((9U == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
		__Vdly__SCCB__DOT__FSM_state = 0xcU;
		__Vdly__SCCB__DOT__FSM_return_state = 0xaU;
		__Vdly__SCCB__DOT__timer = 0x3eU;
		vlTOPp->SIOC_oe = 0U;
	    } else {
		if ((0xaU == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
		    __Vdly__SCCB__DOT__FSM_state = 0xcU;
		    __Vdly__SCCB__DOT__FSM_return_state = 0xbU;
		    __Vdly__SCCB__DOT__timer = 0x3eU;
		    vlTOPp->SIOD_oe = 0U;
		} else {
		    if ((0xbU == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
			__Vdly__SCCB__DOT__FSM_state = 0xcU;
			__Vdly__SCCB__DOT__FSM_return_state = 0U;
			__Vdly__SCCB__DOT__timer = 0x1f4U;
			__Vdly__SCCB__DOT__byte_counter = 0U;
		    } else {
			if ((0xcU == (IData)(vlTOPp->SCCB__DOT__FSM_state))) {
			    __Vdly__SCCB__DOT__FSM_state 
				= (0xfU & ((0U == vlTOPp->SCCB__DOT__timer)
					    ? (IData)(vlTOPp->SCCB__DOT__FSM_return_state)
					    : 0xcU));
			    __Vdly__SCCB__DOT__timer 
				= ((0U == vlTOPp->SCCB__DOT__timer)
				    ? 0U : (vlTOPp->SCCB__DOT__timer 
					    - (IData)(1U)));
			}
		    }
		}
	    }
	}
    }
    vlTOPp->SCCB__DOT__latched_address = __Vdly__SCCB__DOT__latched_address;
    vlTOPp->SCCB__DOT__latched_data = __Vdly__SCCB__DOT__latched_data;
    vlTOPp->SCCB__DOT__byte_index = __Vdly__SCCB__DOT__byte_index;
    vlTOPp->SCCB__DOT__byte_counter = __Vdly__SCCB__DOT__byte_counter;
    vlTOPp->SCCB__DOT__FSM_state = __Vdly__SCCB__DOT__FSM_state;
    vlTOPp->SCCB__DOT__FSM_return_state = __Vdly__SCCB__DOT__FSM_return_state;
    vlTOPp->SCCB__DOT__timer = __Vdly__SCCB__DOT__timer;
    vlTOPp->SCCB__DOT__tx_byte = __Vdly__SCCB__DOT__tx_byte;
}

void VSCCB::_eval(VSCCB__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_eval\n"); );
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk)))) {
	vlTOPp->_sequent__TOP__2(vlSymsp);
	vlTOPp->__Vm_traceActivity = (2U | vlTOPp->__Vm_traceActivity);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void VSCCB::_eval_initial(VSCCB__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_eval_initial\n"); );
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
    vlTOPp->__Vm_traceActivity = (1U | vlTOPp->__Vm_traceActivity);
}

void VSCCB::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::final\n"); );
    // Variables
    VSCCB__Syms* __restrict vlSymsp = this->__VlSymsp;
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VSCCB::_eval_settle(VSCCB__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_eval_settle\n"); );
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

VL_INLINE_OPT QData VSCCB::_change_request(VSCCB__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_change_request\n"); );
    VSCCB* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VSCCB::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
	Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((start & 0xfeU))) {
	Verilated::overWidthError("start");}
}
#endif // VL_DEBUG

void VSCCB::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSCCB::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    start = VL_RAND_RESET_I(1);
    address = VL_RAND_RESET_I(8);
    data = VL_RAND_RESET_I(8);
    ready = VL_RAND_RESET_I(1);
    SIOC_oe = VL_RAND_RESET_I(1);
    SIOD_oe = VL_RAND_RESET_I(1);
    SCCB__DOT__FSM_state = VL_RAND_RESET_I(4);
    SCCB__DOT__FSM_return_state = VL_RAND_RESET_I(4);
    SCCB__DOT__timer = VL_RAND_RESET_I(32);
    SCCB__DOT__latched_address = VL_RAND_RESET_I(8);
    SCCB__DOT__latched_data = VL_RAND_RESET_I(8);
    SCCB__DOT__byte_counter = VL_RAND_RESET_I(2);
    SCCB__DOT__tx_byte = VL_RAND_RESET_I(8);
    SCCB__DOT__byte_index = VL_RAND_RESET_I(4);
    __Vclklast__TOP__clk = VL_RAND_RESET_I(1);
    __Vm_traceActivity = VL_RAND_RESET_I(32);
}
