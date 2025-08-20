#ifndef DCF_REDUNDANCY_H
#define DCF_REDUNDANCY_H
#include "dcf_error.h"

typedef struct DCFRedundancy DCFRedundancy;

DCFError dcf_redundancy_health_check(DCFRedundancy* redundancy, const char* peer, int* rtt_out);
DCFError dcf_redundancy_measure_rtt(DCFRedundancy* redundancy, const char* peer, int* rtt_out);
DCFError dcf_redundancy_get_alternate_path(DCFRedundancy* redundancy, const char* target, char** path_out);
DCFError dcf_redundancy_init(DCFRedundancy** redundancy, const char** peers, size_t peer_count, int rtt_threshold);

#endif
