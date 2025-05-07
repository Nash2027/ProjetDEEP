/**
 ******************************************************************************
 * @file 	main.c
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de votre projet sur carte Nucleo STM32G431KB
 ******************************************************************************
 */

#include "config.h"
#include "stm32g4_sys.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_adc.h"
#include "stm32g4_utils.h"
#include "tft_ili9341/stm32g4_ili9341.h"
#include "math.h"
#include "compteur.h"
#include "MotorDC/stm32g4_motorDC.h"
#include <stdio.h>


#define BLINK_DELAY		100	//ms
#define VITESSE_MAX 200

extern UART_HandleTypeDef huart2; // Handle global UART2 pour printf

// LED verte On/Off
void write_LED(bool b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

/**
  * @brief  Point d'entrée de votre application
  */
int main(void)
{
	// Initialisation HAL
	HAL_Init();

	// Init GPIO + UART2
	BSP_GPIO_enable();
	BSP_UART_init(UART2_ID, 115200);

	// Redirection des printf vers UART2
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	// Init LED verte
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF);

	// Init écran TFT
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);

	// Dessiner compteur de vitesse au démarrage
	Dessine_compteur_vitesse();

	// Initialisation ADC (potentiomètre)
	BSP_ADC_init();

	static motor_id_e moteur1;

	moteur1 = BSP_MOTOR_add(GPIOA,GPIO_PIN_1,GPIOB,GPIO_PIN_0);




	while (1)
	{
		// Lire valeur brute du potentiomètre (PA0 = ADC_1)
		uint16_t raw_value = BSP_ADC_getValue(ADC_1);

		// Convertir la valeur ADC en vitesse (0 à VITESSE_MAX)
		int vitesse = (raw_value * VITESSE_MAX) / 4095;

		// Affichage dans le terminal UART
		printf("Vitesse: %3d km/h | raw ADC: %4d\r\n", vitesse, raw_value);

		// Mettre à jour le compteur graphique
		MetAJourCompteur(vitesse);

		uint8_t duty = (vitesse * 100) / VITESSE_MAX;    // Convertir en % du PWM
		BSP_MOTOR_set_duty(moteur1, duty);



		// Petite pause pour stabiliser la lecture
		HAL_Delay(200);
	}
}
