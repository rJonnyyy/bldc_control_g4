/*
 * six_step_logic.c
 *
 *  Created on: Oct 22, 2025
 *      Author: jonas
 */


#include "six_step_logic.h"


/* aktuelles hallpattern einlesen */
uint8_t get_hall_pattern(void) {
	uint32_t idr = HALL_PORT->IDR;
	uint8_t h1 = (idr >> HALL_1_PIN) & 1u;
	uint8_t h2 = (idr >> HALL_2_PIN) & 1u;
	uint8_t h3 = (idr >> HALL_3_PIN) & 1u;
	return (uint8_t) ((h3<<2)|(h2<<1)|h1);
}


/* Drehrichtungsunabhängige Funktion */
uint8_t hall_2_sector(uint8_t hallpattern) {
    static const uint8_t sector_lut[8] = {
    		0, /*000-0*/
			4, /*001-1*/
			2, /*010-2*/
			3, /*011-3*/
			6, /*100-4*/
			5, /*101-5*/
 			1, /*110-6*/
			0  /*111-7*/
    }; // muss angepasst werden, das hallpattern (also z.B. die Zahl 2-010) ist hier der index ! an den jeweiligen Eintrag muss dann die entsprechende Sektornummer geschrieben werden
    hallpattern = hallpattern & 0x07;
    uint8_t sector = sector_lut[hallpattern];
    return sector;
}


/* Drehrichtungsabhängige Funktion */
uint8_t sector_2_vector_ccw(uint8_t sector) {
	uint8_t volt_vec = VEC_INVALID; // standartmäßig 0xFF - Zeichen für invalid

	switch (sector)
	{
		case 1: volt_vec = (PH_B<<4) | PH_C; break; // BC - 12
		case 2: volt_vec = (PH_B<<4) | PH_A; break; // BA - 10
		case 3: volt_vec = (PH_C<<4) | PH_A; break; // CA - 20
		case 4: volt_vec = (PH_C<<4) | PH_B; break; // CB - 21
		case 5: volt_vec = (PH_A<<4) | PH_B; break; // AB - 1
		case 6: volt_vec = (PH_A<<4) | PH_C; break; // AC - 2
		default: volt_vec = VEC_INVALID; break; // wenn Sektor = 0
	}

	return volt_vec;
}

/* Drehrichtungsabhängige Funktion */
uint8_t sector_2_vector_cw(uint8_t sector) {
	uint8_t volt_vec = VEC_INVALID; // standartmäßig 0xFF - Zeichen für invalid

	switch (sector)
	{
		case 1: volt_vec = (PH_C<<4) | PH_B; break; // CB - 21
		case 2: volt_vec = (PH_C<<4) | PH_A; break; // CA - 20
		case 3: volt_vec = (PH_B<<4) | PH_A; break; // BA - 10
		case 4: volt_vec = (PH_B<<4) | PH_C; break; // BC - 12
		case 5: volt_vec = (PH_A<<4) | PH_C; break; // AC - 02
		case 6: volt_vec = (PH_A<<4) | PH_B; break; // AB - 01
		default: volt_vec = VEC_INVALID; break; // wenn Sektor = 0
	}

	return volt_vec;
}



/* Drehrichtungsunabhängig */
/* Setzt entsprechende GPIO je nach gefordertem Spannungs - Vektor */
/* ============================================================================================
 * LOGIK DER PHASENANSTEUERUNG (BTN-Treiber)
 * --------------------------------------------------------------------------------------------
 *
 * 1) Einzelphasen-Tabelle (GPIO → Schalterzustände)
 *
 *    INH | IN | High-Side (HSS) | Low-Side (LSS)
 *   ------+----+-----------------+---------------
 *     0  | 0  | OFF              | OFF      // Phase deaktiviert
 *     0  | 1  | OFF              | OFF      // Phase deaktiviert (INH=0 dominiert)
 *     1  | 0  | OFF              | ON       // Low-Side aktiv  → negative Phase (-)
 *     1  | 1  | ON               | OFF      // High-Side aktiv → positive Phase (+)
 *
 * 2) Phasenvektoren gemäß 6-Step-Kommutierung (eine Richtung)
 *
 *    Zeiger | Aktive Phasen |  IN_A INH_A  |  IN_B INH_B  |  IN_C INH_C   |
 *   --------+---------------+-------------------------------------------------------------
 *     AB    | A+ , B−       |    1    1    |    0    1    |    x     0    |
 *     AC    | A+ , C−       |	  1    1 	|    x    0    |    0     1    |
 *     BC    | B+ , C−       |	  x    0	|    1    1    |    0     1    |
 *     BA    | B+ , A−       | 	  0    1	|    1    1    |    x     0    |
 *     CA    | C+ , A−       | 	  0    1	|    x    0    |    1     1    |
 *     CB    | C+ , B−       | 	  x    0	|	 0    1    |    1     1    |
 *
 *     bzw
 *
 *     AB    | A+ , B−       | IN_A=1, INH_A=1 ; IN_B=0, INH_B=1 ; IN_C=x, INH_C=0
 *     AC    | A+ , C−       | IN_A=1, INH_A=1 ; IN_C=0, INH_C=1 ; IN_B=x, INH_B=0
 *     BC    | B+ , C−       | IN_B=1, INH_B=1 ; IN_C=0, INH_C=1 ; IN_A=x, INH_A=0
 *     BA    | B+ , A−       | IN_B=1, INH_B=1 ; IN_A=0, INH_A=1 ; IN_C=x, INH_C=0
 *     CA    | C+ , A−       | IN_C=1, INH_C=1 ; IN_A=0, INH_A=1 ; IN_B=x, INH_B=0
 *     CB    | C+ , B−       | IN_C=1, INH_C=1 ; IN_B=0, INH_B=1 ; IN_A=x, INH_A=0
 *
 *  →  "x" bedeutet: Phase komplett aus (INH=0)
 *  →  Jede Phase hat also drei mögliche Zustände:
 *        +  (INH=1, IN=1)
 *        –  (INH=1, IN=0)
 *        OFF (INH=0)
 *
 *  Diese Zuordnung wird in vector_2_gpio() implementiert.
 * ============================================================================================ */
mask_vec_t vector_2_gpio(uint8_t volt_vec) {

    mask_vec_t mosfet_mask = {0};

    // Baseline: alles AUS
    mosfet_mask.bsrrA |= BSRR_RST(A_IN_PIN)  | BSRR_RST(B_IN_PIN)  | BSRR_RST(C_IN_PIN);
    mosfet_mask.bsrrA |= BSRR_RST(A_INH_PIN) | BSRR_RST(B_INH_PIN);
    mosfet_mask.bsrrB |= BSRR_RST(C_INH_PIN);

    if (volt_vec != VEC_INVALID) {
        uint8_t pos = (volt_vec >> 4) & 0x0Fu; // + Phase
        uint8_t neg =  (volt_vec) & 0x0Fu; // − Phase

        // + Phase: INH=1, IN=1
        switch (pos) {
            case PH_A:
            	mosfet_mask.bsrrA |= BSRR_SET(A_IN_PIN);
                mosfet_mask.bsrrA |= BSRR_SET(A_INH_PIN);
                break;
            case PH_B:
            	mosfet_mask.bsrrA |= BSRR_SET(B_IN_PIN);
                mosfet_mask.bsrrA |= BSRR_SET(B_INH_PIN);
                break;
            case PH_C:
            	mosfet_mask.bsrrA |= BSRR_SET(C_IN_PIN);
            	mosfet_mask.bsrrB |= BSRR_SET(C_INH_PIN);
                break;
            default: break; // OFF bleibt OFF
        }

        // − Phase: INH=1, IN=0 (IN bleibt durch Baseline=0)
        switch (neg) {
            case PH_A: mosfet_mask.bsrrA |= BSRR_SET(A_INH_PIN); break;
            case PH_B: mosfet_mask.bsrrA |= BSRR_SET(B_INH_PIN); break;
            case PH_C: mosfet_mask.bsrrB |= BSRR_SET(C_INH_PIN); break;
            default: break;
        }
    }

    return mosfet_mask;
}




/* Bitmasken ausgeben (atomar pro Port) */
void set_mosfet_output(const mask_vec_t *mosfet_mask) {
	GPIOA->BSRR = mosfet_mask->bsrrA;  // GPIOA
	GPIOB->BSRR = mosfet_mask->bsrrB;  // GPIOB
}


/* Alle Brücken sicher AUS (beide Switches offen) */
void disable_mosfets(void) {
    mask_vec_t mosfet_masks = {0};
    mosfet_masks.bsrrA = BSRR_RST(A_IN_PIN) | BSRR_RST(B_IN_PIN) | BSRR_RST(C_IN_PIN) | BSRR_RST(A_INH_PIN) | BSRR_RST(B_INH_PIN);
    mosfet_masks.bsrrB = BSRR_RST(C_INH_PIN);
    set_mosfet_output(&mosfet_masks);
}

/* CCW (=0) oder CW (=1) */
uint8_t determine_direction(void) {
	uint32_t idr = DIR_PORT->IDR;
	uint8_t dir = (idr >> DIR_PIN) & 1u;
	return dir;
}


/* Diese Funktion vereint eine vollständige Kommutierungs - Sequenz */
void six_step_sequence(void) {
	uint8_t dir = determine_direction();
	uint8_t hallpattern = get_hall_pattern();
	uint8_t sector = hall_2_sector(hallpattern);

	if (dir == CCW) { // CCW
		uint8_t vector = sector_2_vector_ccw(sector);
		mask_vec_t bitmask = vector_2_gpio(vector);
		set_mosfet_output(&bitmask);
	} else if (dir == CW) { // CW
		uint8_t vector = sector_2_vector_cw(sector);
		mask_vec_t bitmask = vector_2_gpio(vector);
		set_mosfet_output(&bitmask);
	} else {
		disable_mosfets();
	}
}


