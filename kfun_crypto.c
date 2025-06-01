/*
 * Viking DGD Extension - Cryptographic Functions
 */

#include <string.h>
#include <sodium.h>
#include "viking_ext.h"

/*
 * argon2_hash(string password, int opsLimit, int memLimit)
 * Hash a password using Argon2id
 */
void kf_argon2_hash(LPC_frame f, int nargs, LPC_value retval) {
    LPC_value arg;
    LPC_string password_str, result_str;
    LPC_dataspace data;
    char *password;
    int opsLimit, memLimit;
    char hash[crypto_pwhash_STRBYTES];
    
    /* Get password */
    arg = lpc_frame_arg(f, nargs, 0);
    if (lpc_value_type(arg) != LPC_TYPE_STRING) {
        lpc_runtime_error(f, "Bad argument 1 for kfun argon2_hash");
        return;
    }
    password_str = lpc_string_getval(arg);
    password = lpc_string_text(password_str);
    
    /* Get opsLimit */
    arg = lpc_frame_arg(f, nargs, 1);
    if (lpc_value_type(arg) != LPC_TYPE_INT) {
        lpc_runtime_error(f, "Bad argument 2 for kfun argon2_hash");
        return;
    }
    opsLimit = lpc_int_getval(arg);
    
    /* Get memLimit */
    arg = lpc_frame_arg(f, nargs, 2);
    if (lpc_value_type(arg) != LPC_TYPE_INT) {
        lpc_runtime_error(f, "Bad argument 3 for kfun argon2_hash");
        return;
    }
    memLimit = lpc_int_getval(arg);
    
    /* Generate hash */
    if (crypto_pwhash_str(hash, password, strlen(password), 
                          opsLimit, memLimit) != 0) {
        return; /* nil - out of memory */
    }
    
    /* Return hash string */
    data = lpc_frame_dataspace(f);
    result_str = lpc_string_new(data, hash, strlen(hash));
    lpc_string_putval(retval, result_str);
}

/*
 * argon2_verify(string password, string hash)
 * Verify a password against an Argon2id hash
 */
void kf_argon2_verify(LPC_frame f, int nargs, LPC_value retval) {
    LPC_value arg;
    LPC_string password_str, hash_str;
    char *password, *hash;
    int result;
    
    /* Get password */
    arg = lpc_frame_arg(f, nargs, 0);
    if (lpc_value_type(arg) != LPC_TYPE_STRING) {
        lpc_runtime_error(f, "Bad argument 1 for kfun argon2_verify");
        return;
    }
    password_str = lpc_string_getval(arg);
    password = lpc_string_text(password_str);
    
    /* Get hash */
    arg = lpc_frame_arg(f, nargs, 1);
    if (lpc_value_type(arg) != LPC_TYPE_STRING) {
        lpc_runtime_error(f, "Bad argument 2 for kfun argon2_verify");
        return;
    }
    hash_str = lpc_string_getval(arg);
    hash = lpc_string_text(hash_str);
    
    /* Verify */
    result = crypto_pwhash_str_verify(hash, password, strlen(password));
    
    /* Return 1 for match, 0 for no match */
    lpc_int_putval(retval, (result == 0) ? 1 : 0);
}