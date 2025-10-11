#include "esp32-hal.h"
#include <Arduino.h>

unsigned long TimeEpochGet(Stream* pStream);
String WiFi_macAddress_as_String();
int Tiny_gethexval(int c);
uint8_t ConvertAsciiToHex(uint8_t char1, uint8_t char2);
void Hex_bytes_to_Ascii(const uint8_t *data,int pLength,char *pDest);

uint64_t MacBytesToU64(uint8_t * pMacBytes);
void MacU64toString(uint64_t pMac, char *MacStringTemp);

#ifdef APP_SUPPORT_FILE_SYSTEM
String file_md5(File &f);
#endif
