#include "app_main_defines.h"
/* Wi-Fi Provisioning Manager Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <Arduino.h>
#include <WiFiProv.h>
#include <WiFi.h>

#include <esp_wifi.h>

void app_wifi_prov_main(bool reset_settings);
void wifi_prov_network_reset();
void wifi_prov_init();
bool wifi_prov_Active();