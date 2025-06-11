/**
 ******************************************************************************
 * @file    compteur.h
 * @author  [Ton nom ou initiales]
 * @date    Mar 28, 2025
 * @brief   Interface pour le compteur de vitesse graphique sur TFT ILI9341.
 *
 * Ce module fournit des fonctions pour :
 * - dessiner un cadran circulaire avec des graduations,
 * - afficher une aiguille animée représentant la vitesse,
 * - mettre à jour dynamiquement la vitesse affichée.
 *
 ******************************************************************************
 */

#ifndef COMPTEUR_H_
#define COMPTEUR_H_

#include <stdint.h>  /**< Pour les types entiers comme int32_t, etc. */

/**
 * @brief Dessine le compteur de vitesse circulaire sur l'écran TFT.
 *
 * Affiche le cadran principal, les graduations tous les 10 km/h
 * et les chiffres correspondants.
 *
 * À appeler une seule fois à l'initialisation.
 */
void Dessine_compteur_vitesse(void);

/**
 * @brief Dessine ou met à jour l'aiguille en fonction de la vitesse actuelle.
 *
 * Efface la précédente aiguille et redessine la nouvelle à la bonne position angulaire.
 *
 * @param vitesse Vitesse actuelle à afficher (0 à VITESSE_MAX).
 */
void Dessine_Aiguille(int vitesse);

/**
 * @brief Met à jour le compteur de vitesse avec une nouvelle valeur.
 *
 * Cette fonction appelle `Dessine_Aiguille` pour gérer la mise à jour visuelle.
 *
 * @param nouvelleVitesse Nouvelle valeur de vitesse à représenter.
 */
void MetAJourCompteur(int nouvelleVitesse);

#endif /* COMPTEUR_H_ */
