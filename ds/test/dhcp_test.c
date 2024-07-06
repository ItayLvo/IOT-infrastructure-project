#include <stdio.h>	/* printf */
#include <string.h>	/* memcmp */

#include "trie.h"
#include "dhcp.h"


int main()
{
	unsigned char base_network_id[BYTES_IN_IP] = {192,68,64,0}; /* 11000000 01000100 01000000 00000000 */
	unsigned char received_ip[BYTES_IN_IP] = {0};
	unsigned char requested_ip[BYTES_IN_IP] = {0};
	size_t count_free = 0;
	status_e status = SUCCESS;
	size_t prefix = 28;
	
	DHCP_Server_t *dhcp = DHCPCreate(base_network_id, prefix);
	if(NULL == dhcp)
	{
		printf("TestFree failed\n");
	}
	
	count_free = DHCPCountFree(dhcp);
	if(13 != count_free)
	{
		printf("TestFree 1 failed\n");
		printf("%lu = countfree\n", count_free);
	}
	
	requested_ip[0] = 192, requested_ip[1] = 68, requested_ip[2] = 64, requested_ip[3] = 0;
	status = DHCPAllocateIP(dhcp, requested_ip, received_ip);
	count_free = DHCPCountFree(dhcp);
	if(SUCCESS != status || -1 != memcmp(requested_ip, received_ip, BYTES_IN_IP) || 12 != count_free)
	{
		printf("TestFree 2 failed\n");
		printf("%lu = countfree\n", count_free);
	}
	

	DHCPDestroy(dhcp);
	return 0;
}



