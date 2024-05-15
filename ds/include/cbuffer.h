#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include <stddef.h> /* size_t */
#include <sys/types.h> /*ssize_t*/

typedef struct cbuffer cbuffer_t;
void PrintBuffer(cbuffer_t *cbuffer);


/*
Description: Create the circular buffer
Return Value: on success - pointer to created cbuffer, on failure - NULL
Time Complexity - O(1)
Space: O(n)
*/
cbuffer_t *CBufferCreate(size_t buffer_size);

/*
Description: Destroy the circular buffer
Return Value: void
Time Complexity - O(1)
Space: O(1)
*/
void CBufferDestroy(cbuffer_t *cbuffer);

/*
Description: Return the available space for writing in the buffer 
Return Value: size_t the available space in Bytes
Time Complexity - O(1)
Space: O(1)
*/
size_t CBufferAvailableSpace(const cbuffer_t *cbuffer);

/*
Description: Return the size of the buffer in bytes
Return Value: size_t
Time Complexity - O(1)
Space: O(1)
*/
size_t CBufferBufsiz(const cbuffer_t *cbuffer);

/*
Description: read from cbuffer count bytes into dest
Return Value: On success, the number of bytes read is returned. On error, -1 is returned
Time Complexity - O(n) (of size_t count and not buffer size)
Space: O(1)
*/
ssize_t CBufferRead(cbuffer_t *cbuffer, void *dest, size_t count);

/*
Description: writing from the source_buffer to cbuffer count bytes
Return Value: On success, the number of bytes written is returned.  On error, -1 is returned
Time Complexity - O(n)
Space: O(1)
*/
ssize_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t count);

/*
Description: Return if the buffer empty  
Return Value: 1 if empty, 0 if not empty
Time Complexity - O(1)
Space: O(1)
*/
int CBufferIsEmpty(const cbuffer_t *cbuffer);

/*
Description: Return if the buffer full  
Return Value: 1 if full, 0 if not full
Time Complexity - O(1)
Space: O(1)

int CBufferIsFull(const cbuffer_t *cbuffer);
*/

#endif /* __CBUFFER_H__ */
