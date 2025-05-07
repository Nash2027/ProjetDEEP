/*
 * compteur.h
 *
 *  Created on: Mar 28, 2025
 *      Author: mguil
 */

#ifndef COMPTEUR_H_
#define COMPTEUR_H_

#include <stdint.h>  // Pour utiliser les types uint16_t, etc.

extern int Vitesse_max;  // Déclaration de la variable globale

void Dessine_un_compteur_de_vitesse();
void Dessine_Aiguille(int vitesse);
void MetAJourCompteur(int nouvelleVitesse);

void ADC1_Init(void);
int Lire_Potentiometre(void);

#endif /* COMPTEUR_H_ */
