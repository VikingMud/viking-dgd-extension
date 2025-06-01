/*
 * Viking DGD Extension - System Resource Functions
 */

#include <sys/resource.h>
#include "viking_ext.h"

/*
 * rusage() - Report on resource utilization
 * Returns an array of 14 integers with resource usage information
 */
void kf_rusage(LPC_frame f, int nargs, LPC_value retval) {
    struct rusage stat;
    LPC_dataspace data;
    LPC_array arr;
    LPC_value temp;
    
    if (getrusage(RUSAGE_SELF, &stat) == -1) {
        return; /* nil */
    }
    
    data = lpc_frame_dataspace(f);
    arr = lpc_array_new(data, 14);
    temp = lpc_value_temp(data);
    
    /* Fill array with resource usage data */
    lpc_int_putval(temp, stat.ru_utime.tv_sec * 1000 + stat.ru_utime.tv_usec / 1000);
    lpc_array_assign(data, arr, 0, temp);
    
    lpc_int_putval(temp, stat.ru_stime.tv_sec * 1000 + stat.ru_stime.tv_usec / 1000);
    lpc_array_assign(data, arr, 1, temp);
    
    lpc_int_putval(temp, stat.ru_maxrss);
    lpc_array_assign(data, arr, 2, temp);
    
    lpc_int_putval(temp, stat.ru_idrss);
    lpc_array_assign(data, arr, 3, temp);
    
    lpc_int_putval(temp, stat.ru_minflt);
    lpc_array_assign(data, arr, 4, temp);
    
    lpc_int_putval(temp, stat.ru_majflt);
    lpc_array_assign(data, arr, 5, temp);
    
    lpc_int_putval(temp, stat.ru_nswap);
    lpc_array_assign(data, arr, 6, temp);
    
    lpc_int_putval(temp, stat.ru_inblock);
    lpc_array_assign(data, arr, 7, temp);
    
    lpc_int_putval(temp, stat.ru_oublock);
    lpc_array_assign(data, arr, 8, temp);
    
    lpc_int_putval(temp, stat.ru_msgsnd);
    lpc_array_assign(data, arr, 9, temp);
    
    lpc_int_putval(temp, stat.ru_msgrcv);
    lpc_array_assign(data, arr, 10, temp);
    
    lpc_int_putval(temp, stat.ru_nsignals);
    lpc_array_assign(data, arr, 11, temp);
    
    lpc_int_putval(temp, stat.ru_nvcsw);
    lpc_array_assign(data, arr, 12, temp);
    
    lpc_int_putval(temp, stat.ru_nivcsw);
    lpc_array_assign(data, arr, 13, temp);
    
    lpc_array_putval(retval, arr);
}