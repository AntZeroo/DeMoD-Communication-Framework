```c
#ifndef DCF_REDUNDANCY_H
#define DCF_REDUNDANCY_H

#include "dcf_error.h"
#include <stdint.h>

typedef struct DCFRedundancy DCFRedundancy;

DCFRedundancy* dcf_redundancy_new(void);
DCFError dcf_redundancy_initialize(DCFRedundancy* redundancy, DCFConfig* config);
DCFError dcf_redundancy_start(DCFRedundancy* redundancy);
DCFError dcf_redundancy_stop(DCFRedundancy* redundancy);
DCFError dcf_redundancy_get_optimal_route(DCFRedundancy* redundancy, const char* recipient, char** route_out);
DCFError dcf_redundancy_health_check(DCFRedundancy* redundancy, const char* peer, int* rtt_out);
void dcf_redundancy_free(DCFRedundancy* redundancy);

#endif
```
