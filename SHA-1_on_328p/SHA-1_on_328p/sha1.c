/*
 * sha1.c
 *
 * Created: 25.01.2026 21:38:51
 *  Author: BigMac
 */ 

#include "sha1.h"
#include <string.h>


static inline uint32_t rotl32(uint32_t x, uint8_t n)
{
	return (uint32_t)((x << n) | (x >> (32 - n)));
}

void sha1_transform(SHA1_CTX *context, const uint8_t data[]) {
	//uint32_t a, b, c, d, e, i, j, t, w[80];
	uint32_t a, b, c, d, e, i, t;
	static uint32_t w[80];

	// Copy the block into W[0..15]
	for (i = 0; i < 16; i++) {
		w[i] = ((uint32_t)data[i*4] << 24) | ((uint32_t)data[i*4 + 1] << 16) | ((uint32_t)data[i*4 + 2] <<  8) | ((uint32_t)data[i*4 + 3]);
	}

	// Extend the 16 words into 80 words
	for (i = 16; i < 80; i++) {
		//w[i] = ROTL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
		w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
	}

	// Initialize working variables
	a = context->state[0];
	b = context->state[1];
	c = context->state[2];
	d = context->state[3];
	e = context->state[4];

	// Main loop
	for (i = 0; i < 80; i++) {
		if (i < 20)
		t = ((b & c) | (~b & d)) + 0x5A827999;
		else if (i < 40)
		t = (b ^ c ^ d) + 0x6ED9EBA1;
		else if (i < 60)
		t = ((b & c) | (b & d) | (c & d)) + 0x8F1BBCDC;
		else
		t = (b ^ c ^ d) + 0xCA62C1D6;

		//t += ROTL(a, 5) + e + w[i];
		t += rotl32(a, 5) + e + w[i];
		e = d;
		d = c;
		//c = ROTL(b, 30);
		c = rotl32(b, 30);
		b = a;
		a = t;
	}

	// Add this chunk's hash to the result
	context->state[0] += a;
	context->state[1] += b;
	context->state[2] += c;
	context->state[3] += d;
	context->state[4] += e;
}

	//BIGMAC
void sha1_init(SHA1_CTX *context) {
	memset(context,0,sizeof(*context));
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
	context->state[4] = 0xC3D2E1F0;
}

void sha1_update(SHA1_CTX *context, const uint8_t* data, uint32_t len) {
	uint32_t i, j;

	j = (context->count[0] >> 3) & 63;
	if ((context->count[0]+=len<<3) < (len << 3))
	context->count[1]++;
	context->count[1] += (len >> 29);

	for (i = 0; i < len; i++) {
		context->buffer[j++] = data[i];
		if (j == 64) {
			sha1_transform(context, context->buffer);
			j = 0;
		}
	}
}

void sha1_final(SHA1_CTX *context, uint8_t digest[SHA1_BLOCK_SIZE]) {
	uint8_t finalcount[8];
	uint32_t i;

	for (i = 0; i < 8; i++)
	finalcount[i] = (uint8_t)((context->count[(i >= 4 ? 0 : 1)] >>
	((3 - (i & 3)) * 8)) &
	255);

	sha1_update(context, (uint8_t *)"\x80", 1);
	while ((context->count[0] & 504) != 448)
	sha1_update(context, (uint8_t *)"\x00", 1);

	sha1_update(context, finalcount, 8);

	for (i = 0; i < 20; i++)
	digest[i] = (uint8_t)((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
}

//
//void sha1_hash_one_preprocessed_block(const uint8_t block[64], uint8_t digest[SHA1_BLOCK_SIZE])
//{
	//SHA1_CTX ctx;
	//sha1_init(&ctx);
//
	//// WICHTIG: block ist bereits gepaddet -> KEIN sha1_update/sha1_final verwenden!
	//sha1_transform(&ctx, block);
//
	//// state -> digest (big endian)
	//for (uint8_t i = 0; i < 20; i++) {
		//digest[i] = (uint8_t)((ctx.state[i >> 2] >> ((3 - (i & 3)) * 8)) & 0xFF);
	//}
//}