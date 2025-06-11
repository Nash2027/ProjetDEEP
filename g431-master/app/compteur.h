/**
 ******************************************************************************
 * @file    compteur.h
 * @author  [Ton nom ou initiales]
 * @date    Mar 28, 2025
 * @brief   Interface pour le compteur de vitesse graphique sur TFT ILI9341.
 *
 * Ce module fournit des fonctions pour :
 * - dessiner un cadran circulaire avec des graduations,
 * - afficher une aiguille anim�e repr�sentant la vitesse,
 * - mettre � jour dynamiquement la vitesse affich�e.
 *
 ******************************************************************************
 */

#ifndef COMPTEUR_H_
#define COMPTEUR_H_

#include <stdint.h>  /**< Pour les types entiers comme int32_t, etc. */

/**
 * @brief Dessine le compteur de vitesse circulaire sur l'�cran TFT.
 *
 * Affiche le cadran principal, les graduations tous les 10 km/h
 * et les chiffres correspondants.
 *
 * � appeler une seule fois � l'initialisation.
 */
void Dessine_compteur_vitesse(void);

/**
 * @brief Dessine ou met � jour l'aiguille en fonction de la vitesse actuelle.
 *
 * Efface la pr�c�dente aiguille et redessine la nouvelle � la bonne position angulaire.
 *
 * @param vitesse Vitesse actuelle � afficher (0 � VITESSE_MAX).
 */
void Dessine_Aiguille(int vitesse);

/**
 * @brief Met � jour le compteur de vitesse avec une nouvelle valeur.
 *
 * Cette fonction appelle `Dessine_Aiguille` pour g�rer la mise � jour visuelle.
 *
 * @param nouvelleVitesse Nouvelle valeur de vitesse � repr�senter.
 */
void MetAJourCompteur(int nouvelleVitesse);

#endif /* COMPTEUR_H_ */
