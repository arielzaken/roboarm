#pragma once

/**
 * @file StaticVirtual.h
 * @brief CRTP-style static virtual method forwarding macros for zero-overhead interfaces
 *
 * These macros implement compile-time method forwarding for use in CRTP (Curiously Recurring Template Pattern)
 * interfaces. They simulate "static virtual" behavior without the runtime overhead of vtables or dynamic dispatch.
 *
 * This is ideal for embedded and performance-constrained systems where RAM use must be minimized.
 *
 * Macros support up to 5 parameters. Extend similarly for additional arity if needed.
 *
 * @author OpenAI + Ariel Zaken
 * @date 2025
 *
 * @par Macro Naming Convention:
 *   - SV_METHODn     : Method with return type, non-const
 *   - SV_METHODn_V   : Void return type, non-const
 *   - SV_METHODn_C   : Method with return type, const-qualified
 *   - SV_METHODn_VC  : Void return type, const-qualified
 *
 * @par Example Usage:
 * @code
 * template <typename Derived>
 * class ISensor {
 * public:
 *     SV_METHOD0(Derived, int, read)
 *     SV_METHOD2_V(Derived, configure, uint8_t, threshold, uint8_t, gain)
 * };
 *
 * class TempSensor : public ISensor<TempSensor> {
 * public:
 *     int read() { return 42; }
 *     void configure(uint8_t threshold, uint8_t gain) { ... }
 * };
 * @endcode
 */

//////////////////////////////////////////
// 0-arg methods
//////////////////////////////////////////

#define SV_METHOD0(Derived, Ret, Name) \
    Ret Name() { return static_cast<Derived*>(this)->Name(); }

#define SV_METHOD0_V(Derived, Name) \
    void Name() { static_cast<Derived*>(this)->Name(); }

#define SV_METHOD0_C(Derived, Ret, Name) \
    Ret Name() const { return static_cast<const Derived*>(this)->Name(); }

#define SV_METHOD0_VC(Derived, Name) \
    void Name() const { static_cast<const Derived*>(this)->Name(); }

//////////////////////////////////////////
// 1-arg methods
//////////////////////////////////////////

#define SV_METHOD1(Derived, Ret, Name, T1, N1) \
    Ret Name(T1 N1) { return static_cast<Derived*>(this)->Name(N1); }

#define SV_METHOD1_V(Derived, Name, T1, N1) \
    void Name(T1 N1) { static_cast<Derived*>(this)->Name(N1); }

#define SV_METHOD1_C(Derived, Ret, Name, T1, N1) \
    Ret Name(T1 N1) const { return static_cast<const Derived*>(this)->Name(N1); }

#define SV_METHOD1_VC(Derived, Name, T1, N1) \
    void Name(T1 N1) const { static_cast<const Derived*>(this)->Name(N1); }

//////////////////////////////////////////
// 2-arg methods
//////////////////////////////////////////

#define SV_METHOD2(Derived, Ret, Name, T1, N1, T2, N2) \
    Ret Name(T1 N1, T2 N2) { return static_cast<Derived*>(this)->Name(N1, N2); }

#define SV_METHOD2_V(Derived, Name, T1, N1, T2, N2) \
    void Name(T1 N1, T2 N2) { static_cast<Derived*>(this)->Name(N1, N2); }

#define SV_METHOD2_C(Derived, Ret, Name, T1, N1, T2, N2) \
    Ret Name(T1 N1, T2 N2) const { return static_cast<const Derived*>(this)->Name(N1, N2); }

#define SV_METHOD2_VC(Derived, Name, T1, N1, T2, N2) \
    void Name(T1 N1, T2 N2) const { static_cast<const Derived*>(this)->Name(N1, N2); }

//////////////////////////////////////////
// 3-arg methods
//////////////////////////////////////////

#define SV_METHOD3(Derived, Ret, Name, T1, N1, T2, N2, T3, N3) \
    Ret Name(T1 N1, T2 N2, T3 N3) { return static_cast<Derived*>(this)->Name(N1, N2, N3); }

#define SV_METHOD3_V(Derived, Name, T1, N1, T2, N2, T3, N3) \
    void Name(T1 N1, T2 N2, T3 N3) { static_cast<Derived*>(this)->Name(N1, N2, N3); }

#define SV_METHOD3_C(Derived, Ret, Name, T1, N1, T2, N2, T3, N3) \
    Ret Name(T1 N1, T2 N2, T3 N3) const { return static_cast<const Derived*>(this)->Name(N1, N2, N3); }

#define SV_METHOD3_VC(Derived, Name, T1, N1, T2, N2, T3, N3) \
    void Name(T1 N1, T2 N2, T3 N3) const { static_cast<const Derived*>(this)->Name(N1, N2, N3); }

//////////////////////////////////////////
// 4-arg methods
//////////////////////////////////////////

#define SV_METHOD4(Derived, Ret, Name, T1, N1, T2, N2, T3, N3, T4, N4) \
    Ret Name(T1 N1, T2 N2, T3 N3, T4 N4) { return static_cast<Derived*>(this)->Name(N1, N2, N3, N4); }

#define SV_METHOD4_V(Derived, Name, T1, N1, T2, N2, T3, N3, T4, N4) \
    void Name(T1 N1, T2 N2, T3 N3, T4 N4) { static_cast<Derived*>(this)->Name(N1, N2, N3, N4); }

#define SV_METHOD4_C(Derived, Ret, Name, T1, N1, T2, N2, T3, N3, T4, N4) \
    Ret Name(T1 N1, T2 N2, T3 N3, T4 N4) const { return static_cast<const Derived*>(this)->Name(N1, N2, N3, N4); }

#define SV_METHOD4_VC(Derived, Name, T1, N1, T2, N2, T3, N3, T4, N4) \
    void Name(T1 N1, T2 N2, T3 N3, T4 N4) const { static_cast<const Derived*>(this)->Name(N1, N2, N3, N4); }

//////////////////////////////////////////
// 5-arg methods
//////////////////////////////////////////

#define SV_METHOD5(Derived, Ret, Name, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5) \
    Ret Name(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5) { return static_cast<Derived*>(this)->Name(N1, N2, N3, N4, N5); }

#define SV_METHOD5_V(Derived, Name, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5) \
    void Name(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5) { static_cast<Derived*>(this)->Name(N1, N2, N3, N4, N5); }

#define SV_METHOD5_C(Derived, Ret, Name, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5) \
    Ret Name(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5) const { return static_cast<const Derived*>(this)->Name(N1, N2, N3, N4, N5); }

#define SV_METHOD5_VC(Derived, Name, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5) \
    void Name(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5) const { static_cast<const Derived*>(this)->Name(N1, N2, N3, N4, N5); }

