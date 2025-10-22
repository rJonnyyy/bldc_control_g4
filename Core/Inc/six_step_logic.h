/*
 * six_step_logic.h
 *
 *  Created on: Oct 22, 2025
 *      Author: jonas
 */

#ifndef INC_SIX_STEP_LOGIC_H_
#define INC_SIX_STEP_LOGIC_H_

/* ============================================================================================================ */
/* Includes */

#include "main.h"   // oder main.h, falls du HAL verwenden willst
#include <stdint.h>


/* ============================================================================================================ */
/* Pinbelegungen - ANPASSEN JE NACH HARDWARE VERSCHALTUNG */


#define A_IN_PORT   	GPIOA
#define A_IN_PIN   		8
#define A_INH_PORT    	GPIOA
#define A_INH_PIN     	11

#define B_IN_PORT   	GPIOA
#define B_IN_PIN    	9
#define B_INH_PORT    	GPIOA
#define B_INH_PIN     	12

#define C_IN_PORT   	GPIOA
#define C_IN_PIN    	10
#define C_INH_PORT    	GPIOB
#define C_INH_PIN     	15

#define HALL_PORT     	GPIOC
#define HALL_1_PIN    	6
#define HALL_2_PIN    	7
#define HALL_3_PIN    	8

#define DIR_PORT		GPIOC
#define DIR_PIN			10

#define CCW 			0
#define CW 				1

/* Datentypen / Strukturen */

/* Kennung für „ungültig/alles aus“ im Vektorcode */
#define VEC_INVALID 0xFF

/* Bit Set/Reset Register BSSR Makros:
 *  Warum BSSR und nicht ODR: Mit dem BSSR kannst du gleichzeitig bestimmte Bits eines Ports setzen und andere Rücksetzen (in einem Befehl)
 *  Beim ODR ein Befehl für RÜcksetzen (AND Maske) und ein Befehl fürs Setzen (OR Maske) */
#define BSRR_SET(pin) (1u << (pin))          // Bit 0..15
#define BSRR_RST(pin) (1u << ((pin) + 16))   // Bit 16..31


/* ============================================================================================================ */
/* Datentypen / Strukturen */

/* bit set reset zeug - alternative zu direkt odr schreiben */
typedef struct {
    uint32_t bsrrA;
    uint32_t bsrrB;
} mask_vec_t;


/* -------------------------
 * Phasen-Indizes A = 0; B = 1; C = 2;
 * -------------------------
 * Vektorcode Interpretation:
 * -------------------------
 * Linke 4 Bits = positive Phase +
 * Rechte 4 Bits = negative Phase -
 * Bsp: (B+, C−) → (1<<4)|2 = 0x12
 */
enum {
	PH_A = 0,
	PH_B = 1,
	PH_C = 2
};


/* ============================================================================================================ */
/* Funktionen */

/* Hallpattern */
uint8_t get_hall_pattern(void);

/* CCW */
uint8_t hall_2_sector_ccw(uint8_t hallpattern);
uint8_t sector_2_vector_ccw(uint8_t sector);

/* CW */
uint8_t hall_2_sector_cw(uint8_t hallpattern);
uint8_t sector_2_vector_cw(uint8_t sector);

/* Richtungsunabhängig */
mask_vec_t vector_2_gpio(uint8_t vector);

void set_output_gpio(const mask_vec_t *gpio_masks);

uint8_t determine_direction(void);

void six_step_sequence(void);

void disable_mosfets(void); // Am Anfang der main einfach einmal aufrufen, dass sicher alles aus ist

/* ============================================================================================================ */
#endif /* INC_SIX_STEP_LOGIC_H_ */
