#ifndef FULL_UDP
#define FULL_UDP

typedef struct 
{
    UINT16_T is_frag : 1;
    UINT16_T frag_id : 15;
    UINT16_T frag_offset;
} fragment_flag_t;

typedef struct 
{
    fragment_flag_t frag;
    
    char data[0];
    
} FULL_UDP_MSG_T;

#endif