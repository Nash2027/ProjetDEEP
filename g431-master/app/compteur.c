#include "tft_ili9341/stm32g4_ili9341.h"
#include "compteur.h"
#include "math.h"
#include <string.h>

#define VITESSE_MAX 200

void Dessine_compteur_vitesse() {
    ILI9341_Fill(ILI9341_COLOR_BLACK); // Efface l'écran

    ILI9341_DrawCircle(120, 120, 100, ILI9341_COLOR_WHITE); // Cercle principal

    for (int i = 0; i <= VITESSE_MAX; i += 10) {
        float angle = ((i * 270.0f) / VITESSE_MAX - 135.0f) * (M_PI / 180.0f);

        int x1 = 120 + 80 * cosf(angle);
        int y1 = 120 + 80 * sinf(angle);
        int x2 = 120 + 90 * cosf(angle);
        int y2 = 120 + 90 * sinf(angle);
        ILI9341_DrawLine(x1, y1, x2, y2, ILI9341_COLOR_WHITE);

        char text[5];
        sprintf(text, "%d", i);
        int len = strlen(text);
        int xText = 120 + 110 * cosf(angle);
        int yText = 120 + 110 * sinf(angle);
        ILI9341_Puts(xText - (len * 7 / 2), yText - 5, text, &Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
    }
}

void Dessine_Aiguille(int vitesse) {
    static int ancienneVitesse = -1; // -1 pour forcer le premier dessin

    int x0 = 120, y0 = 120;

    // Efface l'ancienne aiguille si elle existe
    if (ancienneVitesse >= 0) {
        float ancienAngle = ((ancienneVitesse * 270.0f) / VITESSE_MAX - 135.0f) * (M_PI / 180.0f);
        int xAncien = x0 + 65 * cosf(ancienAngle);
        int yAncien = y0 + 65 * sinf(ancienAngle);
        ILI9341_DrawLine(x0, y0, xAncien, yAncien, ILI9341_COLOR_BLACK);
    }

    // Dessine la nouvelle aiguille
    float nouvelAngle = ((vitesse * 270.0f) / VITESSE_MAX - 135.0f) * (M_PI / 180.0f);
    int xNouveau = x0 + 65 * cosf(nouvelAngle);
    int yNouveau = y0 + 65 * sinf(nouvelAngle);
    ILI9341_DrawLine(x0, y0, xNouveau, yNouveau, ILI9341_COLOR_RED);

    ancienneVitesse = vitesse;
}


void MetAJourCompteur(int nouvelleVitesse) {

    Dessine_Aiguille(nouvelleVitesse);
}
