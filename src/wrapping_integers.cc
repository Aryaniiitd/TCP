#include "wrapping_integers.hh"

#include <iostream>

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { return isn + uint32_t(n); }

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
     uint64_t base = static_cast<uint64_t>(UINT32_MAX) ;
   
     base++;
     uint64_t cmod = checkpoint % base;
    uint64_t cbase = checkpoint - cmod;
    if (cmod == 0 && cbase >= base) {
    	cbase= cbase-base;
        cmod =cmod+ base;
       
    }
    uint64_t k =n.raw_value() - isn.raw_value();
    uint64_t nmod = static_cast<uint64_t>(k);
    if (nmod > cmod) {
    uint64_t bs=base - nmod + cmod;
    uint64_t md=nmod - cmod;
      if (cbase >= base && (bs) <= (md))
            {
            uint64_t ans=cbase - base + nmod;
            return ans;}
        else
            return cbase + nmod;
    }
    uint64_t c=cmod-nmod;
    uint64_t d=nmod + base - cmod;
    if (d>=c)
        return cbase + nmod;
    else
        return cbase + base + nmod;
}
