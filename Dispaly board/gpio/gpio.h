/**
* @file gpio.h
* @brief this header file will contain all required GPIO configuration
* definitions and basic utilities functions.
*
* @author amin
*
* @date 5/22/2012
*/

#include <RTL.h>
 #define analog_input    0
#define floating_input  4
#define pull_updown_input   8

#define output_pp_2m    2
#define output_od_2m    6
#define output_pp_10m   1
#define output_od_10m   5
#define output_pp_50m   3
#define output_od_50m   7

#define af_pp_2m    0x0a
#define af_od_2m    0x0e
#define af_pp_10m   9
#define af_od_10m   0x0d
#define af_pp_50m   0x0b
#define af_od_50m   0x0f

/** \brief PORTA Configuration
*
* Configure PORTA Directions.
*
* \param direction is direction type
* \param bit       is selected pin in PORTA to configure
* \return void
*/

void porta_set_direction(U8 direction,int bit);
/** \brief PORTB Configuration
*
* Configure PORTB Directions.
*
* \param direction is direction type
* \param bit       is selected pin in PORTB to configure
* \return void
*/
void portb_set_direction(U8 direction,int bit);
/** \brief PORTC Configuration
*
* Configure PORTC Directions.
*
* \param direction is direction type
* \param bit       is selected pin in PORTC to configure
* \return void
*/
void portc_set_direction(U8 direction,int bit);
/** \brief PORTD Configuration
*
* Configure PORTD Directions.
*
* \param direction is direction type
* \param bit       is selected pin in PORTD to configure
* \return void
*/
void portd_set_direction(U8 direction,int bit);
