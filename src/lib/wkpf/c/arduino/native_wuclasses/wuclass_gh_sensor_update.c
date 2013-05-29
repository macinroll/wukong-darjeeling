#include "debug.h"
#include "../../common/native_wuclasses/native_wuclasses.h"

#include <avr/io.h>
#include "../../../../uart/c/common/uart.h"
#include "wkcomm.h"

#ifdef ENABLE_WUCLASS_GH_SENSOR

int counter;
uint8_t state, wid, returnWid;
bool setWid, enable;

void wuclass_gh_sensor_setup(wuobject_t *wuobject) {

	uart_inituart(3, 57600);
	wid = wkcomm_get_node_id();
        DEBUG_LOG(DBG_WKPFGH, "transimit wunode id: %d\n", wid);
        uart_write_byte(3, 4);
	uart_write_byte(3, wid);//send wunode id to oct

	counter = 1000;
        setWid = false;
        while(counter > 0){
          returnWid = uart_read_byte(3);
          if( returnWid != 0){
	  	if(  wid == returnWid ){
	    		DEBUG_LOG(DBG_WKPFGH, "set wunode id success\n");
            		setWid = true;
          	}
		break;
	  } 
          counter--;
        }
	if( setWid == false)
        	DEBUG_LOG(DBG_WKPFGH, "set wunode id fail\n");
}

void wuclass_gh_sensor_update(wuobject_t *wuobject) {

    wkpf_internal_read_property_boolean(wuobject, WKPF_PROPERTY_GH_SENSOR_ENABLE, &enable);
    if(enable == true){
      uart_write_byte(3, 2);
    }
    else
      uart_write_byte(3, 3);
    
    //use get state command to check the state has been changed or not
    uart_write_byte(3, 1);
    counter = 1000;
    while(counter > 0){
      state = uart_read_byte(3);
      if (state != 0) break;
      counter--;
    }

    if(state != 0){
    	if((state == 5 && enable == true) || (state == 6 && enable != true)){
      		DEBUG_LOG(DBG_WKPFGH, "change state success\n");
    	}
    	else
      		DEBUG_LOG(DBG_WKPFGH, "change state fail\n");
    }
    else
      	DEBUG_LOG(DBG_WKPFGH, "didn't get response from GH device\n");
}

#endif
