/**
 ******************************************************************************
 * @file    sim808.h
 * @author  mguil
 * @date    May 25, 2025
 * @brief   Interface de communication avec le module GSM SIM808 via UART.
 *
 * Ce module fournit des fonctions pour :
 * - envoyer des commandes AT au module SIM808,
 * - envoyer des SMS � un num�ro donn�.
 *
 ******************************************************************************
 */

#ifndef SIM808_H_
#define SIM808_H_

#include <stdint.h>  /**< Pour uint32_t */
#include <stddef.h>  /**< Pour size_t (au besoin) */

/**
 * @brief Envoie une commande AT au module SIM808 via UART
 *
 * Cette fonction �crit la commande AT suivie dun retour chariot,
 * puis attend un d�lai pour laisser le module r�pondre
 *
 * @param command La commande AT � envoyer
 * @param delayMs D�lai en millisecondes � attendre apr�s l'envoi
 */
void sendAT(const char *command, uint32_t delayMs);

/**
 * @brief Envoie un SMS via le module SIM808
 *
 * Cette fonction initialise le mode texte, configure le num�ro de t�l�phone
 * et envoie le message au destinataire
 *
 * @param numero Num�ro de t�l�phone au format international
 * @param message Contenu du message SMS � envoyer
 */
void envoyerSMS(const char* numero, const char* message);

#endif /* SIM808_H_ */
