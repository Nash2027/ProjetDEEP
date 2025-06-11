/**
 ******************************************************************************
 * @file    sim808.h
 * @author  mguil
 * @date    May 25, 2025
 * @brief   Interface de communication avec le module GSM SIM808 via UART.
 *
 * Ce module fournit des fonctions pour :
 * - envoyer des commandes AT au module SIM808,
 * - envoyer des SMS à un numéro donné.
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
 * Cette fonction écrit la commande AT suivie dun retour chariot,
 * puis attend un délai pour laisser le module répondre
 *
 * @param command La commande AT à envoyer
 * @param delayMs Délai en millisecondes à attendre après l'envoi
 */
void sendAT(const char *command, uint32_t delayMs);

/**
 * @brief Envoie un SMS via le module SIM808
 *
 * Cette fonction initialise le mode texte, configure le numéro de téléphone
 * et envoie le message au destinataire
 *
 * @param numero Numéro de téléphone au format international
 * @param message Contenu du message SMS à envoyer
 */
void envoyerSMS(const char* numero, const char* message);

#endif /* SIM808_H_ */
