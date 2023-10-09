/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdint.h>

struct CAN_Message {
	uint16_t id ;
	char data_length ;
	char data [8];
};

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(struct CAN_Message* can_msg, uint8_t mailbox_id);
uint8_t can_receive(struct CAN_Message* can_msg, uint8_t mailbox_id);

#endif /* CAN_CONTROLLER_H_ */