#ifndef __ENCODING_H__
#define __ENCODING_H__

#define CSR_MTIME 0xB00

// From 0x50-0x100
#define CSR_CCTABLE_START 0x50

/* Configuration */
#define CSR_USER_TABLE_SIZE 0x80 
#define CSR_USER_PACKET_SIZE 0x81 /*num of 32bit-ints, valid:0,1,2,4,8,16*/
#define CSR_TRACED_PACKETS 0x82

/*Input for core*/
#define CSR_HAS_EVENT 0x90
#define CSR_RECEIVED_EVENT_COUNT 0x91

/*Output for core*/
#define CSR_PROCESSED_EVENT_COUNT 0x92
#define CSR_EVENT_TYPE 0x93

#define CSR_PRINT_ADDR 0xF0
#define CSR_PRINT_STRING_NUM 0xF1
#define CSR_PRINT_STRING_LEN 0xF2
#define CSR_TRAP 0xF3

#endif