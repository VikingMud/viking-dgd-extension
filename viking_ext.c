/*
 * Viking DGD Extension Module - Main Entry Point
 * 
 * Registers all Viking-specific kernel functions with DGD.
 */

#include <sodium.h>
#include <curl/curl.h>
#include "viking_ext.h"

/* Kfun prototypes */
static char proto_rusage[] = { LPC_TYPE_ARRAY_OF(LPC_TYPE_INT), 0 };
static char proto_argon2_hash[] = { LPC_TYPE_STRING, LPC_TYPE_STRING, LPC_TYPE_INT, LPC_TYPE_INT, 0 };
static char proto_argon2_verify[] = { LPC_TYPE_INT, LPC_TYPE_STRING, LPC_TYPE_STRING, 0 };
static char proto_http_request[] = { LPC_TYPE_MAPPING, LPC_TYPE_STRING, LPC_TYPE_STRING, 
                                     LPC_TYPE_MAPPING, LPC_TYPE_STRING, 0 };

/* Kfun table */
static LPC_ext_kfun viking_kfuns[] = {
    { "rusage", proto_rusage, &kf_rusage },
    { "argon2_hash", proto_argon2_hash, &kf_argon2_hash },
    { "argon2_verify", proto_argon2_verify, &kf_argon2_verify },
    { "http_request", proto_http_request, &kf_http_request }
};

/*
 * Module initialization
 */
int lpc_ext_init(int major, int minor, const char *config) {
    /* Initialize libsodium */
    if (sodium_init() < 0) {
        return 0;
    }
    
    /* Initialize CURL */
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    /* Register kfuns */
    lpc_ext_kfun(viking_kfuns, sizeof(viking_kfuns) / sizeof(LPC_ext_kfun));
    
    return 1;
}