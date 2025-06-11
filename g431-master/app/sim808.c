/**
 * @file sim808_sms.c
 * @brief Fonctions pour envoyer des commandes AT et des SMS via le module SIM808.
 *
 * Ce fichier contient des fonctions permettant de communiquer avec un module SIM808
 * en utilisant l'UART (via BSP_UART_*) sur une carte STM32 (ex: STM32G4).
 */

#include <string.h>
#include <stdio.h>
#include "stm32g4_uart.h"

#define SIM808_UART_ID UART2_ID  /**< ID de l'UART utilis� pour la communication avec le SIM808 */

/**
 * @brief Envoie une commande AT au module SIM808.
 *
 * Cette fonction formate et envoie une commande AT termin�e par retour chariot et saut de ligne,
 * puis attend un certain d�lai pour permettre au module de r�pondre.
 *
 * @param command La commande AT � envoyer (ex: "AT+CMGF=1").
 * @param delayMs Temps d'attente apr�s l'envoi (en millisecondes).
 */
void sendAT(const char *command, uint32_t delayMs) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%s\r\n", command);
    BSP_UART_puts(SIM808_UART_ID, (uint8_t*)buffer, strlen(buffer));
    HAL_Delay(delayMs);
}

/**
 * @brief Envoie un SMS � un num�ro donn� via le module SIM808.
 *
 * Cette fonction configure le SIM808 en mode texte, pr�pare la commande d'envoi de SMS
 * avec le num�ro et le message, puis envoie le tout. Elle termine par l'envoi du caract�re
 * Ctrl+Z pour d�clencher l'envoi r�el.
 *
 * @param numero Num�ro de t�l�phone du destinataire (ex: "+33612345678").
 * @param message Message � envoyer.
 */
void envoyerSMS(const char* numero, const char* message) {
    sendAT("AT", 1000);          // Test communication
    sendAT("AT+CMGF=1", 1000);   // Mode texte

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"", numero);
    sendAT(cmd, 1000);

    BSP_UART_puts(SIM808_UART_ID, (uint8_t*)message, strlen(message));

    uint8_t ctrlZ = 0x1A;
    BSP_UART_putc(SIM808_UART_ID, ctrlZ); // Ctrl+Z
    HAL_Delay(3000); // Attendre l'envoi
}
