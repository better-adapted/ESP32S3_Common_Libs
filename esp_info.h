#include "app_main_defines.h"

#include <Arduino.h>
#include <WiFi.h>

#include "esp_chip_info.h"
#include <inttypes.h>

#include "esp_heap_caps.h"
#include "esp_chip_info.h"
#include "esp_partition.h"
#include "esp_mac.h"
#include "esp_log.h"

#include "soc/rtc.h"
#include "soc/clk_tree_defs.h"

#ifdef APP_SUPPORT_SHA256_INFO
void print_sha256(Stream *pStream,const uint8_t *image_hash, const char *label);
void get_sha256_of_partitions(Stream *pStream);
#endif

String Get_Version_Info_String();
void Version_Info(Stream *pStream);
void esp_print_info(void);
void print_chip_MACS();
