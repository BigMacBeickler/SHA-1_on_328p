/*
 * sha1.h
 *
 * Created: 25.02.2025 21:38:35
 *  Author: BigMac
 */ 


#ifndef SHA1_H_
#define SHA1_H_

#include <stdint.h>

#define SHA1_BLOCK_SIZE 20  // 160-bit output

typedef struct {
	uint32_t state[5];
	uint32_t count[2];
	uint8_t buffer[64];
} SHA1_CTX;

void sha1_init(SHA1_CTX *context);
void sha1_update(SHA1_CTX *context, const uint8_t *data, uint32_t len);
void sha1_final(SHA1_CTX *context, uint8_t digest[SHA1_BLOCK_SIZE]);



#endif /* SHA1_H_ */