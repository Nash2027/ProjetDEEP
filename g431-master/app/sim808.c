#include <string.h>
#include <stdio.h>
#include "stm32g4_uart.h"


#define SIM808_UART_ID UART2_ID  // ou UART1_ID si tu veux utiliser UART1

void sendAT(const char *command, uint32_t delayMs) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%s\r\n", command);
    BSP_UART_puts(SIM808_UART_ID, (uint8_t*)buffer, strlen(buffer));
    HAL_Delay(delayMs);
}

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
