/**
 ******************************************************************************
 * @file 	main.c
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de votre projet sur carte Nucleo STM32G431KB
 *
 * PA7 MPU6050
 * PA1 MoteurDC
 * PA0 Potentiometre
 *
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
#include "compteurNumerique.h"

#include <stdio.h>

#define VITESSE_MAX 200
#define ACCIDENT_MPU 3500

int accident_detected = 0;
extern UART_HandleTypeDef huart2;

void write_LED(bool b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

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


	Dessine_compteur_vitesse(); // Dessin initial du compteur




	// Initialisation ADC pour le potentiomètre
	BSP_ADC_init();

	// Initialisation du moteur DC sur PA1 (PWM) et PB0 (Direction ou GND)
	static motor_id_e moteur1;
	moteur1 = BSP_MOTOR_add(GPIOA, GPIO_PIN_1, GPIOB, GPIO_PIN_0);

	// Initialisation MPU6050
	MPU6050_t mpu;
	if (MPU6050_Init(&mpu, NULL, 0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok) {
	    printf("Erreur d'initialisation MPU6050\n");
	    while(1);
	}
	while(1)
	{
		// Lire la valeur brute du potentiomètre (PA0)
		uint16_t raw_value = BSP_ADC_getValue(ADC_1);
		// Calcul de la vitesse en fonction de la valeur ADC
		int vitesse = (raw_value * VITESSE_MAX) / 4095;

		//static int vitesse = 0;
		//vitesse += 5;
		//if (vitesse > VITESSE_MAX) vitesse = 0;

		// Affichage UART
		printf("\nVitesse: %3d km/h ", vitesse);

		// Mise à jour graphique de l'aiguille du compteur
		MetAJourCompteur(vitesse);


		float ratio = (float)vitesse / VITESSE_MAX;
		float duty_f = 0.5f + ratio * 0.5f;
		uint8_t duty = (uint8_t)(duty_f * 100.0f);

		// Affichage du PWM
		printf(" | PWM moteur : %d%%\n", duty);

		BSP_MOTOR_set_duty(moteur1, duty);

		// Lecture MPU6050
		MPU6050_ReadAll(&mpu);
		int16_t accelX = mpu.Accelerometer_X;

		if ((accelX >= ACCIDENT_MPU || accelX <= -ACCIDENT_MPU) && accident_detected == 0) {
			printf("ACCIDENT DETECTED\n");
			accident_detected = 1;
			envoyerSMS("+33662037899", "ALERTE ACCIDENT: La moto a subi un choc important.");
		}
		else if (accelX < ACCIDENT_MPU && accelX > -ACCIDENT_MPU) {
			accident_detected = 0;
		}

		HAL_Delay(200);
	}
}
