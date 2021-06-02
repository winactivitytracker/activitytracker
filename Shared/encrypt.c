#include "encrypt.h"

void aesInit(void)
{
	AES_init_ctx(&ctx, (uint8_t *) key);
}

void aesEncrypt(char * message)
{
	AES_ECB_encrypt(&ctx, (uint8_t *) message);
}

void aesDecrypt(char * message)
{
	AES_ECB_decrypt(&ctx, (uint8_t *) message);
}
