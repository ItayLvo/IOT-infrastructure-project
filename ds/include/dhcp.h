#ifndef __DHCP_H__
#define __DHCP_H__

#define BYTES_IN_IP 4

#include <stddef.h> /* size_t */

typedef enum status
{
  SUCCESS,	
  MEMORY_FALIURE,
  DHCP_FULL,
  BAD_SUBNET,
  DOUBLE_FREE
} status_e;


typedef struct DHCP_Server DHCP_Server_t;

/*
DHCPCreate
description: create the DHCP module
input: subnet id 
return: a pointer to the DHCP module, NULL if failed
time complexity: O(n)		where n is the number of bits that represent supported IPs
space complexity: O(n)
*/
DHCP_Server_t *DHCPCreate(unsigned char base_network_id[BYTES_IN_IP], size_t subnet_prefix);

/*
DHCPDestroy
description: destroy the DHCP module
input: pointer to DHCP module / can recei  ve null
return: nothing
time complexity: O(n)
space complexity: O(1)
*/
void DHCPDestroy(DHCP_Server_t *dhcp);

/*
DHCPAllocateIP
description: Allocate IP 
input: (DHCP_Server_t *dhcp)server, IPv4_t requested_ip, and output param recived_id. user can request NULL for requested_ip.
return: status 0 if success, none 0 otherwise 
(received_ip = requested ip if free. if not free then next big address that is free. 
if nothing was found that is bigger than requested then search for smaller.
return null in recived address if nothing is found)
time complexity: O(log n) (or derr-facto o(1) because it's depth has a capacity)
space complexity: O(1)
*/
status_e DHCPAllocateIP(DHCP_Server_t *dhcp,
                        unsigned char requested_ip[BYTES_IN_IP],
                        unsigned char received_ip[BYTES_IN_IP]);

/*
DHCPFreeIP
description: frees an IP from the network
input: pointer to the DHCP and the requested IP to free
return: status_e SUCCESS / BAD_SUBNET / DOUBLE_FREE
time complexity: O(log n) (or de-facto o(1) because it's depth has a capacity)
space complexity: O(1)
*/
status_e DHCPFreeIP(DHCP_Server_t *dhcp, unsigned char address[BYTES_IN_IP]);

/*
DHCPCountFree
description: count the free ip address in DHCP
input: pointer to DHCP module
return: count of the free ip address
time complexity: O(n)
space complexity: O(1)
*/
size_t DHCPCountFree(const DHCP_Server_t *dhcp);


#endif /* __DHCP_H__ */
