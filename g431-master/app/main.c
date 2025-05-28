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


#include <stdio.h>


#define BLINK_DELAY 100	//ms
#define VITESSE_MAX 200
#define ACCIDENT_MPU 3500

int accident_detected = 0;
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

	// Initialisation moteur DC
	static motor_id_e moteur1;
	moteur1 = BSP_MOTOR_add(GPIOA, GPIO_PIN_1, GPIOB, GPIO_PIN_0);

	MPU6050_t mpu;
	// Initialisation MPU6050, supposons que Vcc est sur PA0 (utilisé dans ton config)
	if (MPU6050_Init(&mpu, NULL, 0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok) {
	    printf("Erreur d'initialisation MPU6050\n");
	    while(1); // boucle infinie en cas d'erreur
	}


	while (1)
	{
		// Lire valeur brute du potentiomètre (PA4 = ADC_17)
		uint16_t raw_value = BSP_ADC_getValue(ADC_1);

		// Convertir la valeur ADC en vitesse (0 à VITESSE_MAX)
		int vitesse = (raw_value * VITESSE_MAX) / 4095;

		// Affichage dans le terminal UART
		//printf("Vitesse: %3d km/h | raw ADC: %4d", vitesse, raw_value);

		// Mettre à jour le compteur graphique
		MetAJourCompteur(vitesse);

		// Calcul du PWM avec une réponse progressive
		float duty_f = 0.0f;
		if (vitesse > 0) {
			float ratio = (float)vitesse / VITESSE_MAX;     // 0.0 à 1.0
			duty_f = 0.6f + powf(ratio, 1.5f) * 0.4f;          // entre 60% et 100%
		}
		uint8_t duty = (uint8_t)(duty_f * 100.0f);

		// Afficher le PWM dans le terminal UART
		//printf(" | PWM: %3d %%\r\n", duty);

		// Appliquer le PWM au moteur
		BSP_MOTOR_set_duty(moteur1, duty);


		//Traite les donnees du MPU
		MPU6050_ReadAll(&mpu);
		printf("Accel X=%d\n", mpu.Accelerometer_X);
		int16_t ax_raw = mpu.Accelerometer_X;  // valeur brute

		if ((ax_raw >= 3500 || ax_raw <= -3500) && accident_detected == 0) {
		        printf("ACCIDENT DETECTED\n");
		        accident_detected = 1;

		        // Fonction d'envoi SMS (à définir)
		        envoyerSMS("+33652959374", "ALERTE ACCIDENT: La moto a subi un choc important.");
		    }
		    else if (ax_raw < 3500 && ax_raw > -3500) {
		        accident_detected = 0;
		    }

		// Pause
		HAL_Delay(200);
	}
}
