// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VSCCB_H_
#define _VSCCB_H_

#include "verilated.h"

class VSCCB__Syms;
class VerilatedVcd;

//----------

VL_MODULE(VSCCB) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(start,0,0);
    VL_IN8(address,7,0);
    VL_IN8(data,7,0);
    VL_OUT8(ready,0,0);
    VL_OUT8(SIOC_oe,0,0);
    VL_OUT8(SIOD_oe,0,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(SCCB__DOT__FSM_state,3,0);
    VL_SIG8(SCCB__DOT__FSM_return_state,3,0);
    VL_SIG8(SCCB__DOT__latched_address,7,0);
    VL_SIG8(SCCB__DOT__latched_data,7,0);
    VL_SIG8(SCCB__DOT__byte_counter,1,0);
    VL_SIG8(SCCB__DOT__tx_byte,7,0);
    VL_SIG8(SCCB__DOT__byte_index,3,0);
    VL_SIG(SCCB__DOT__timer,31,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clk,0,0);
    VL_SIG(__Vm_traceActivity,31,0);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VSCCB__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VSCCB);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VSCCB(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VSCCB();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options=0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VSCCB__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VSCCB__Syms* symsp, bool first);
  private:
    static QData _change_request(VSCCB__Syms* __restrict vlSymsp);
    void _ctor_var_reset();
  public:
    static void _eval(VSCCB__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif // VL_DEBUG
  public:
    static void _eval_initial(VSCCB__Syms* __restrict vlSymsp);
    static void _eval_settle(VSCCB__Syms* __restrict vlSymsp);
    static void _initial__TOP__1(VSCCB__Syms* __restrict vlSymsp);
    static void _sequent__TOP__2(VSCCB__Syms* __restrict vlSymsp);
    static void traceChgThis(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__2(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__3(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__4(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceFullThis(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceFullThis__1(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceInitThis(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceInitThis__1(VSCCB__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code);
    static void traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code);
    static void traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code);
} VL_ATTR_ALIGNED(128);

#endif // guard
