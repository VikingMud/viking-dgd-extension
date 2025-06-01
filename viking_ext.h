/*
 * Viking DGD Extension - Common Header
 */

#ifndef VIKING_EXT_H
#define VIKING_EXT_H

#include "lpc_ext.h"

/* Kfun declarations */
void kf_rusage(LPC_frame f, int nargs, LPC_value retval);
void kf_argon2_hash(LPC_frame f, int nargs, LPC_value retval);
void kf_argon2_verify(LPC_frame f, int nargs, LPC_value retval);
void kf_http_request(LPC_frame f, int nargs, LPC_value retval);

#endif /* VIKING_EXT_H */