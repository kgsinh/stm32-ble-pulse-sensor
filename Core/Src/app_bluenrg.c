#include "app_bluenrg.h"
#include "services.h"
#include "bluenrg_conf.h"
#include "bluenrg1_types.h"
#include "bluenrg1_gap.h"
#include "bluenrg1_aci.h"
#include "bluenrg1_hci_le.h"
#include "stdio.h"
#include "stdlib.h"
#include "hci.h"

#define BDADDR_SIZE 6
#define GAP_DEV_NAME_LEN 8
#define ADV_INT_MIN 0x20 // 20 ms
#define ADV_INT_MAX 0x40 // 40 ms



uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x02};

void bluenrg_init(void)
{
	tBleStatus ret;
	uint8_t bdaddr[BDADDR_SIZE];
	const char *gap_name = "STM32BLE";
	uint8_t name_len = (uint8_t)strlen(gap_name);

	uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

	BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));

	/* Initialize the BlueNRG HCI */
	hci_init(APP_UserEvtRx, 0);

	/* Reset BlueNRG hardware */
	hci_reset();

	/* Wait until the BlueNRG is ready */
	HAL_Delay(100);

	/* Configure device address */
	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
	                                 CONFIG_DATA_PUBADDR_LEN,
	                                 bdaddr);
	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in aci_hal_write_config_data: 0x%02X\r\n", ret);
	}

	/* Initialize the GATT */
	ret = aci_gatt_init();

	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in aci_gatt_init: 0x%02X\r\n", ret);
	}

	/* Initialize the GAP */
	ret = aci_gap_init(GAP_PERIPHERAL_ROLE,
	                   0,
	                   GAP_DEV_NAME_LEN,
	                   &service_handle,
	                   &dev_name_char_handle,
	                   &appearance_char_handle);

	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in aci_gap_init: 0x%02X\r\n", ret);
	}

	/* Update device name characteristic value */
	ret = aci_gatt_update_char_value(service_handle,
	                              dev_name_char_handle,
	                              0,
	                              name_len,
	                              (uint8_t *)gap_name);

	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in aci_gatt_update_char_value: 0x%02X\r\n", ret);
	}


	/* Add custom services */
	ret = add_services();

	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in add_simple_service: 0x%02X\r\n", ret);
	}
}

void bluenrg_process(void)
{
    static uint8_t advertising_started = 0;
    tBleStatus ret;

    if (!advertising_started)
    {
        advertising_started = 1;

        uint8_t local_name[] = {
            sizeof("STM32BLE") + 1,
            AD_TYPE_COMPLETE_LOCAL_NAME,
            'S','T','M','3','2','B','L','E'
        };

        ret = aci_gap_set_discoverable(
            ADV_IND,
            ADV_INT_MIN,
            ADV_INT_MAX,
            PUBLIC_ADDR,
            NO_WHITE_LIST_USE,
            sizeof(local_name),
            local_name,
            0,
            NULL,
            0,
            0
        );

        printf("aci_gap_set_discoverable = 0x%02X\r\n", ret);
    }

    hci_user_evt_proc();
}



