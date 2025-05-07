#include "tft_ili9341/stm32g4_ili9341.h"
#include "compteur.h"
#include "math.h"
//#include "stm32g4_adc.h"

#define VITESSE_MAX 200
extern ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc1;

void Dessine_compteur_vitesse(){
    ILI9341_Fill(ILI9341_COLOR_BLACK); // Efface l'écran

    // Décale le cercle vers le haut en réduisant la valeur de y (ex: 120 au lieu de 160)
    ILI9341_DrawCircle(120, 120, 100, ILI9341_COLOR_WHITE);

    for (int i = 0; i <= VITESSE_MAX; i += 10) {
        float angle = ((i * 270) / VITESSE_MAX - 135) * (M_PI / 180);

        int x1 = 120 + 80 * cos(angle);
        int y1 = 120 + 80 * sin(angle);
        int x2 = 120 + 90 * cos(angle);
        int y2 = 120 + 90 * sin(angle);

        ILI9341_DrawLine(x1, y1, x2, y2, ILI9341_COLOR_WHITE);

        char text[4];
        sprintf(text, "%d", i);
        int xText = 120 + 110 * cos(angle);
        int yText = 120 + 110 * sin(angle);
        ILI9341_Puts(xText - 8, yText - 8, text, &Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
    }
}

void Dessine_Aiguille(int vitesse) {
    static int ancienneVitesse = 0;

    float ancienAngle = ((ancienneVitesse * 270) / VITESSE_MAX - 135) * (M_PI / 180);
    float nouvelAngle = ((vitesse * 270) / VITESSE_MAX - 135) * (M_PI / 180);

    int x1 = 120, y1 = 120; // Décale l'origine de l'aiguille

    // Efface l'ancienne aiguille
    int xAncien = 120 + 65 * cos(ancienAngle);
    int yAncien = 120 + 65 * sin(ancienAngle);
    ILI9341_DrawLine(x1, y1, xAncien, yAncien, ILI9341_COLOR_BLACK);

    // Dessine la nouvelle aiguille
    int xNouveau = 120 + 65 * cos(nouvelAngle);
    int yNouveau = 120 + 65 * sin(nouvelAngle);
    ILI9341_DrawLine(x1, y1, xNouveau, yNouveau, ILI9341_COLOR_RED);

    ancienneVitesse = vitesse;
}



void MetAJourCompteur(int nouvelleVitesse) {
    static int ancienneVitesse = 0;

    // Efface l'ancienne aiguille
    Dessine_Aiguille(ancienneVitesse);

    // Dessine la nouvelle aiguille
    Dessine_Aiguille(nouvelleVitesse);

    ancienneVitesse = nouvelleVitesse;
}

void ADC1_Init(void)
{
    __HAL_RCC_ADC12_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_1; // PA0 = ADC1_IN1
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

int Lire_Potentiometre(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t valeur = HAL_ADC_GetValue(&hadc1); // 0 -> 4095

    printf("Valeur du potentiomètre: %d\r\n", valeur); // Afficher la valeur dans Docklight

    // Normaliser la valeur entre 0 et VITESSE_MAX
    return (valeur * VITESSE_MAX) / 4095; // 0 -> VITESSE_MAX
}
