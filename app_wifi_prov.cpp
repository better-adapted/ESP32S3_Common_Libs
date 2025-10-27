#include "app_wifi_prov.h"
#ifdef APP_SUPPORT_WIFI_PROV

#include "WiFiProv.h"
#include "WiFi.h"

const char * pop = "abcd1234"; // Proof of possession - otherwise called a PIN - string provided by the device, entered by the user in the phone app
const char * service_name = "PROV_123"; // Name of your device (the Espressif apps expects by default device name starting with "Prov_")
const char * service_key = NULL; // Password used for SofAP method (NULL = no password needed)
bool reset_provisioned = true; // When true the library will automatically delete previously provisioned data.

static uint32_t ProvActiveTimestamp;

// WARNING: SysProvEvent is called from a separate FreeRTOS task (thread)!
void SysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id)
  {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    {
		Serial.print("\nConnected IP address : ");
		Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
		break;		
	}
    
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    {
	    Serial.println("\nDisconnected. Connecting to the AP again... "); 
	    break;		
	}
    
    case ARDUINO_EVENT_PROV_START:
    {
		ProvActiveTimestamp=millis();
	    Serial.println("\nProvisioning started\nGive Credentials of your access point using smartphone app"); 
	    break;		
	}
    
    case ARDUINO_EVENT_PROV_CRED_RECV:
    {
      Serial.println("\nReceived Wi-Fi credentials");
      Serial.print("\tSSID : ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
      Serial.print("\tPassword : ");
      Serial.println((char const *)sys_event->event_info.prov_cred_recv.password);
      break;
    }
    
    case ARDUINO_EVENT_PROV_CRED_FAIL:
    {
      Serial.println("\nProvisioning failed!\nPlease reset to factory and retry provisioning\n");
      if (sys_event->event_info.prov_fail_reason == NETWORK_PROV_WIFI_STA_AUTH_ERROR)
      {
        Serial.println("\nWi-Fi AP password incorrect");
      }
      else
      {
        Serial.println("\nWi-Fi AP not found....Add API \" nvs_flash_erase() \" before beginProvision()");
      }
      break;
    }
    
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
    {
	    Serial.println("\nProvisioning Successful");
	    ProvActiveTimestamp=0;
	    break;
	}
		
    case ARDUINO_EVENT_PROV_END:
    {
        Serial.println("\nProvisioning Ends"); break;		
		ProvActiveTimestamp=0;
		break;
	}
	
    default:
    {		
	}
  }
}

void wifi_prov_network_reset() {
	wifi_config_t current_conf;
	esp_wifi_get_config((wifi_interface_t)ESP_IF_WIFI_STA, &current_conf);
	memset(current_conf.sta.ssid, 0, sizeof(current_conf.sta.ssid));
	memset(current_conf.sta.password, 0, sizeof(current_conf.sta.password));
	esp_wifi_set_config((wifi_interface_t)ESP_IF_WIFI_STA, &current_conf);
}

extern void wifi_prov_led_RGB(int pMode);
extern  bool wifi_prov_switch_get();
extern  void wifi_prov_WDT_Feed();

void app_wifi_prov_main(bool reset_settings)
{	
  WiFi.onEvent(SysProvEvent);

  Serial.println("Begin Provisioning using BLE");
  // Sample uuid that user can pass during provisioning using BLE
  uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf,
                      0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02 };
  WiFiProv.beginProvision(NETWORK_PROV_SCHEME_BLE, NETWORK_PROV_SCHEME_HANDLER_FREE_BLE, NETWORK_PROV_SECURITY_1, pop, service_name, service_key, uuid, reset_provisioned
  );
  Serial.println("ble qr");
  WiFiProv.printQR(service_name, pop, "ble");
}

uint32_t wifi_prov_Active_ms()
{
	if(ProvActiveTimestamp==0)
		return 0;
	
	return millis() - ProvActiveTimestamp;
}

void wifi_prov_init() {
	if (wifi_prov_switch_get()) {
		Serial.printf("Provision Switch Active");
		Serial.println();

		int loop_counter = 0;
		bool led_toggle = 0;

		while (wifi_prov_switch_get()) {
			delay(100);
			loop_counter++;

			if ((loop_counter > 50) && (loop_counter < 100)) {
				wifi_prov_led_RGB(led_toggle); // Config switch power up
				led_toggle = !led_toggle;
			} else {
				wifi_prov_led_RGB(1); // Config switch power up
			}
		}

		if (loop_counter > 100) {
			wifi_prov_network_reset();
		}
		app_wifi_prov_main(1);
	} else {
		wifi_prov_led_RGB(2); // Config switch power up
	}
}
#endif
