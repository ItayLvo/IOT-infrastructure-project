#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include <math.h>	/* pow */
#include <stdio.h>	/* printf (for testing) */

#include "trie.h" /* trie_t, TrieCreate, TrieDestroy, TrieInsert, TrieRemove */
#include "dhcp.h" /* DHCP declerations */

#define BITS_IN_BYTE 8
#define NETWORK_ADDRESS 0
#define NUM_PREDEFINED_ADDRESSES 3
#define SERVER_ADDRESS (~(0) - 1)
#define BROADCAST_ADDRESS ~(0)


struct DHCP_Server
{
	trie_t *trie;
	unsigned char base_network_id[BYTES_IN_IP];
	unsigned int subnet_mask;
};

unsigned int predefined_dhcp_addresses_g[] = {NETWORK_ADDRESS, SERVER_ADDRESS, BROADCAST_ADDRESS};

static status_e InitDHCPNetwork(trie_t *trie);
static unsigned int SetSubnetMask(size_t subnet_prefix);
static unsigned int ConvertIPToUnsignedInt(unsigned char ip_address[BYTES_IN_IP]);
static void ConvertUnsignedIntToIP(unsigned char out_param[BYTES_IN_IP], unsigned int id);
static int CheckSubnetMatch(unsigned char requested_ip[BYTES_IN_IP], DHCP_Server_t *dhcp);



DHCP_Server_t *DHCPCreate(unsigned char base_network_id[BYTES_IN_IP], size_t subnet_prefix)
{
	DHCP_Server_t *dhcp = NULL;
	status_e init_status = SUCCESS;
	size_t num_bits_host_id = 0;
	size_t i = 0;
	
	assert(NULL != base_network_id);
	assert(subnet_prefix < 32);
	
	num_bits_host_id = BYTES_IN_IP * BITS_IN_BYTE - subnet_prefix;
	
	dhcp = (DHCP_Server_t *)malloc(sizeof(DHCP_Server_t));
	if (NULL == dhcp)
	{
		return NULL;
	}
	
	dhcp->trie = TrieCreate(num_bits_host_id);
	if (NULL == dhcp->trie)
	{
		free(dhcp);
		return NULL;
	}
	
	init_status = InitDHCPNetwork(dhcp->trie);
	if (SUCCESS != init_status)
	{
		TrieDestroy(dhcp->trie);
		free(dhcp);
		return NULL;
	}
	
	dhcp->subnet_mask = SetSubnetMask(subnet_prefix);
	
	for (i = 0; i < BYTES_IN_IP; ++i)
	{
		dhcp->base_network_id[i] = base_network_id[i];
	}
	
	return dhcp;
}



void DHCPDestroy(DHCP_Server_t *dhcp)
{
	assert (dhcp);
	assert(dhcp->trie);
	
	TrieDestroy(dhcp->trie);
	free(dhcp);
}



status_e DHCPAllocateIP(DHCP_Server_t *dhcp, unsigned char requested_ip[BYTES_IN_IP], unsigned char received_ip[BYTES_IN_IP])
{
	unsigned int host_id = 0;
	e_trie_status insert_status = TRIE_SUCCESS;
	
	assert(dhcp);
	assert(requested_ip);
	assert(received_ip);
	
	if(CheckSubnetMatch(requested_ip, dhcp) != 1)
	{
		return BAD_SUBNET;
	}
	
	host_id = ConvertIPToUnsignedInt(requested_ip);
	
	insert_status = TrieInsert(dhcp->trie, host_id, &host_id);
	if(insert_status == TRIE_FULL)
	{
		return DHCP_FULL;
	}
	
	if(insert_status == TRIE_MEMORY_FALIURE)
	{
		return MEMORY_FALIURE;
	}
	
	ConvertUnsignedIntToIP(received_ip, host_id);
	
	return SUCCESS;
}



status_e DHCPFreeIP(DHCP_Server_t *dhcp, unsigned char requested_address[BYTES_IN_IP])
{
	unsigned int host_id = 0;
	
	assert(NULL != dhcp);
	assert(NULL != dhcp->trie);
	assert(NULL != requested_address);
	
	host_id = ConvertIPToUnsignedInt(requested_address);
	
	if(CheckSubnetMatch(requested_address, dhcp) != 1)
	{
		return BAD_SUBNET;
	}
	
	TrieRemove(dhcp->trie, host_id);	/* TODO: TrieRemove should return status */
	
	return SUCCESS;
}


size_t DHCPCountFree(const DHCP_Server_t *dhcp)
{
	size_t max_ips = 0;
	
	assert(NULL != dhcp);
	assert(NULL != dhcp->trie);
	
	/* convert subnet mask back from 1111...000 to 0000...1111 where the 1's are possible host ids */
	max_ips = ~(dhcp->subnet_mask) + 1;
/*	printf("max_ips = %lu\n", max_ips);*/
	return (max_ips - TrieSize(dhcp->trie));
}


/****************************** HELPER FUNCTIONS ******************************/

static status_e InitDHCPNetwork(trie_t *trie)
{
	/* TODO: fix third insert. for some reason, first and second counts, but trie doesnt count the third one. */
	unsigned int dummy = 0;
	size_t i = 0;
	e_trie_status insert_status = TRIE_SUCCESS;
	size_t count_free = 0;
	
	for (i = 0; i < NUM_PREDEFINED_ADDRESSES; ++i)
	{
		
		printf("%lu\n", i);
		insert_status = TrieInsert(trie, predefined_dhcp_addresses_g[i], &dummy);
		if (TRIE_SUCCESS != insert_status)
		{
			printf("fail?\n");
			return (status_e)insert_status;
		}
		
		count_free = TrieSize(trie);
		printf("%lu = countfree\n", count_free);
	}
/*	count_free = TrieSize(trie);*/
/*		printf("%lu = countfree\n", count_free);*/
	return (status_e)insert_status;
}



static unsigned int SetSubnetMask(size_t subnet_prefix)
{
	unsigned int base = ~(0);
	
	return (base << (BYTES_IN_IP * BITS_IN_BYTE - subnet_prefix));
}



static unsigned int ConvertIPToUnsignedInt(unsigned char ip_address[BYTES_IN_IP])
{
	unsigned int result = 0;
	size_t i = 0;
	
	for (i = 0; i < BYTES_IN_IP; ++i)
	{
		result = (result << 8) | ip_address[i];
	}
	
	return result;
}



static void ConvertUnsignedIntToIP(unsigned char out_param[BYTES_IN_IP], unsigned int host_id)
{
	unsigned int mask = 0xff;
	int i = 0;
	
	for (i = BYTES_IN_IP - 1; i >= 0; --i)
	{
		out_param[i] = (unsigned char)(host_id & mask);
		host_id >>= 8;
	}
}



static int CheckSubnetMatch(unsigned char requested_ip[BYTES_IN_IP], DHCP_Server_t *dhcp)
{
	/* convert requested IP and DHCP base network ID to unsigned int */
	unsigned int requested_subnet_id = ConvertIPToUnsignedInt(requested_ip);
	unsigned int subnet_id = ConvertIPToUnsignedInt(dhcp->base_network_id);

	/* apply the subnet mask to both addresses */
	unsigned int subnet_masked_requested = requested_subnet_id & ~(dhcp->subnet_mask);
	unsigned int subnet_masked_base = subnet_id & ~(dhcp->subnet_mask);

	int match = (subnet_masked_requested == subnet_masked_base);

	return match;
}

