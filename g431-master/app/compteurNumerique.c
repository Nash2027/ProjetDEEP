#include "tft_ili9341/stm32g4_ili9341.h"
#include "compteurNumerique.h"
#include <stdio.h>

void InitCompteurNumerique() {
    ILI9341_Fill(ILI9341_COLOR_BLACK);
    //ILI9341_Puts(40, 30, "VITESSE", &Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
}

void AfficheVitesseNumerique(int vitesse) {
    static int ancienneVitesse = -1;

    if (vitesse != ancienneVitesse) {
        // Efface l'ancienne valeur en écrivant par-dessus en noir
        char oldStr[8];
        sprintf(oldStr, "%3d km/h", ancienneVitesse);
        ILI9341_Puts(40, 100, oldStr, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);

        // Affiche la nouvelle vitesse
        char newStr[8];
        sprintf(newStr, "%3d km/h", vitesse);
        ILI9341_Puts(40, 100, newStr, &Font_16x26, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);

        ancienneVitesse = vitesse;
    }
}
