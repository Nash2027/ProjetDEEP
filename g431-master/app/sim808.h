/*
 * sim808.h
 *
 *  Created on: May 25, 2025
 *      Author: mguil
 */

#ifndef SIM808_H_
#define SIM808_H_


void sendAT(const char *command, uint32_t delayMs);
void envoyerSMS(const char* numero, const char* message);


#endif /* SIM808_H_ */
