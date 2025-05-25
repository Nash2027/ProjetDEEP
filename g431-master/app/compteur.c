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

