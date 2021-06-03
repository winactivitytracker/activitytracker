/*
 * encrypt.h
 *
 *  Created on: Jun 2, 2021
 *      Author: arteeh
 */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

#include "aes.h"

// The encryption key used on all devices
const char key[64] = "u7x!A%C*F-JaNdRgUkXp2s5v8y/B?E(G+KbPeShVmYq3t6w9z$C&F)J@McQfTjWn";

struct AES_ctx ctx;

void aesInit();
void aesEncrypt(char * message);
void aesDecrypt(char * message);

#endif /* ENCRYPT_H_ */
