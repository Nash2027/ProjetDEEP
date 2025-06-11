/**
 ******************************************************************************
 * @file    main.c
 * @author  jjo
 * @date    Mar 29, 2024
 * @brief   Fichier principal du projet STM32G431KB : Compteur de vitesse,
 *          gestion moteur, detection d'accident et envoi SMS avec SIM808.
 *
 * Ce programme :
 * - Affiche un compteur de vitesse sur écran TFT ILI9341
 * - Contrôle un moteur DC en fonction d un potentiomètre
 * - Lit un accéléromètre MPU6050 pour détecter un choc
 * - Envoie un SMS d alerte via un module SIM808 si un choc est détecté
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
#include "sim808.h"
#include "MotorDC/stm32g4_motorDC.h"
#include "MPU6050/stm32g4_mpu6050.h"
#include "constantes.h"

#include <stdio.h>



int accident_detected = 0;   /**< Indique si un accident a déjà été détecté */
extern UART_HandleTypeDef huart2;

/**
 * @brief Allume ou éteint la LED verte.
 * @param b `true` pour allumer, `false` pour éteindre.
 */
void write_LED(bool b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

/**
 * @brief Point d'entrée principal du programme.
 *
 * Initialise tous les périphériques (GPIO, UART, ADC, TFT, moteur, MPU6050) et
 * effectue dans une boucle infinie :
 * - Lecture du potentiomètre pour calculer la vitesse
 * - Affichage graphique de la vitesse sur TFT
 * - Réglage du PWM pour le moteur
 * - Lecture de l'accéléromètre pour détection d'accident
 * - Envoi d un SMS d alerte via SIM808 si accident détecté
 *
 * @return int Ne retourne jamais (boucle infinie).
 */
int main(void)
{
	// Initialisation HAL
	HAL_Init();

	// Initialisation GPIO et UART
	BSP_GPIO_enable();
	BSP_UART_init(UART2_ID, 115200);

	// Redirection printf vers UART2
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	// Configuration LED verte (sortie)
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF);

	// Initialisation de l'écran TFT
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	Dessine_compteur_vitesse();  // Dessin initial du compteur

	// Initialisation ADC pour lecture du potentiomètre
	BSP_ADC_init();

	// Initialisation moteur DC : PA1 (PWM), PB0 (Direction/GND)
	static motor_id_e moteur1;
	moteur1 = BSP_MOTOR_add(GPIOA, GPIO_PIN_1, GPIOB, GPIO_PIN_0);

	// Initialisation MPU6050
	MPU6050_t mpu;
	if (MPU6050_Init(&mpu, NULL, 0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok) {
	    printf("Erreur d'initialisation MPU6050\n");
	    while(1);  // Boucle infinie si erreur MPU
	}

	// Boucle principale
	while(1)
	{
		// Lecture du potentiomètre (canal ADC PA0)
		uint16_t raw_value = BSP_ADC_getValue(ADC_1);
		int vitesse = (raw_value * VITESSE_MAX) / 4095;

		// Affichage UART
		printf("\nVitesse: %3d km/h ", vitesse);

		// Mise à jour de l aiguille sur le compteur TFT
		MetAJourCompteur(vitesse);

		// Calcul et affichage PWM en fonction de la vitesse
		float ratio = (float)vitesse / VITESSE_MAX;
		float duty_f = 0.5f + ratio * 0.5f;
		uint8_t duty = (uint8_t)(duty_f * 100.0f);
		printf(" | PWM moteur : %d%%\n", duty);
		BSP_MOTOR_set_duty(moteur1, duty);

		// Lecture des données MPU6050
		MPU6050_ReadAll(&mpu);
		int16_t accelX = mpu.Accelerometer_X;

		// Détection de choc
		if ((accelX >= ACCIDENT_MPU || accelX <= -ACCIDENT_MPU) && accident_detected == 0) {
			printf("ACCIDENT DETECTED\n");
			accident_detected = 1;
			envoyerSMS("+33662037899", "ALERTE ACCIDENT: La moto a subi un choc important.");
		}
		else if (accelX < ACCIDENT_MPU && accelX > -ACCIDENT_MPU) {
			accident_detected = 0;
		}

		// Attente de 200 ms
		HAL_Delay(200);
	}
}
