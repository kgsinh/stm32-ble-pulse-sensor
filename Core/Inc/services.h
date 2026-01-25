#ifndef INC_SERVICES_H_
#define INC_SERVICES_H_

#include "bluenrg1_types.h"

tBleStatus add_services(void);
tBleStatus add_device_info_service(void);
void update_bpm_data(uint16_t *data, uint16_t length);
void update_temp_data(uint16_t *data, uint16_t length);
void APP_UserEvtRx(void *pckt);


#endif /* INC_SERVICES_H_ */
