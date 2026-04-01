/*==================================================================================================
*   Project              : RTD AUTOSAR 4.7 
*   Platform             : CORTEXM
*   Peripheral           : PORT_CI
*   Dependencies         : none
*
*   Autosar Version      : 4.7.0
*   Autosar Revision     : ASR_REL_4_7_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.0
*   Build Version        : S32K1_S32M24_AUTOSAR_4_4_-_R21-11_RTD_2_0_0_D2308_ASR_REL_4_7_REV_0000_20230804
*
*   Copyright 2020-2023 NXP Semiconductors
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v17.0
processor: S32K144
package_id: S32K144_LQFP100
mcu_data: PlatformSDK_S32K1_S32M24
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */
/*==================================================================================================*/

/**
*   @file      Port_Ci_Port_Ip_Cfg.c
*
*   @addtogroup Port_CFG
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Port_Ci_Port_Ip_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PORT_CI_PORT_IP_VENDOR_ID_CFG_C                       43
#define PORT_CI_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_C        4
#define PORT_CI_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_C        7
#define PORT_CI_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_C     0
#define PORT_CI_PORT_IP_SW_MAJOR_VERSION_CFG_C                2
#define PORT_CI_PORT_IP_SW_MINOR_VERSION_CFG_C                0
#define PORT_CI_PORT_IP_SW_PATCH_VERSION_CFG_C                0

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Port_Ci_Port_Ip_Cfg.c and Port_Ci_Port_Ip_Cfg.h are of the same vendor */
#if (PORT_CI_PORT_IP_VENDOR_ID_CFG_C != PORT_CI_PORT_IP_VENDOR_ID_CFG_H)
    #error "Port_Ci_Port_Ip_Cfg.c and Port_Ci_Port_Ip_Cfg.h have different vendor ids"
#endif
/* Check if Port_Ci_Port_Ip_Cfg.c and Port_Ci_Port_Ip_Cfg.h are of the same Autosar version */
#if ((PORT_CI_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_C    != PORT_CI_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_H) || \
     (PORT_CI_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_C    != PORT_CI_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_H) || \
     (PORT_CI_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_C != PORT_CI_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_H) \
    )
    #error "AutoSar Version Numbers of Port_Ci_Port_Ip_Cfg.c and Port_Ci_Port_Ip_Cfg.h are different"
#endif
/* Check if Port_Ci_Port_Ip_Cfg.c and Port_Ci_Port_Ip_Cfg.h are of the same Software version */
#if ((PORT_CI_PORT_IP_SW_MAJOR_VERSION_CFG_C != PORT_CI_PORT_IP_SW_MAJOR_VERSION_CFG_H) || \
     (PORT_CI_PORT_IP_SW_MINOR_VERSION_CFG_C != PORT_CI_PORT_IP_SW_MINOR_VERSION_CFG_H) || \
     (PORT_CI_PORT_IP_SW_PATCH_VERSION_CFG_C != PORT_CI_PORT_IP_SW_PATCH_VERSION_CFG_H)    \
    )
    #error "Software Version Numbers of Port_Ci_Port_Ip_Cfg.c and Port_Ci_Port_Ip_Cfg.h are different"
#endif

/*==================================================================================================
                             LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                             LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                            LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                           LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                           GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                           GLOBAL VARIABLES
==================================================================================================*/


/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0}
- pin_list:
  - {pin_num: '44', peripheral: ADC0, signal: adc0_se14, pin_signal: PTC16, direction: INPUT}
  - {pin_num: '79', peripheral: FXIO, signal: fxio_d2, pin_signal: PTA0, direction: OUTPUT, PE: state_1, PS: state_1}
  - {pin_num: '78', peripheral: FXIO, signal: fxio_d3, pin_signal: PTA1, direction: OUTPUT, PE: state_1, PS: state_1}
  - {pin_num: '17', peripheral: PORTE, signal: 'port, 14', pin_signal: PTE14, direction: INPUT}
  - {pin_num: '85', peripheral: FTM3, signal: ftm3_ch6, pin_signal: PTE2, direction: OUTPUT}
  - {pin_num: '39', peripheral: FTM0, signal: ftm0_ch1, pin_signal: PTC1, direction: OUTPUT}
  - {pin_num: '30', peripheral: FTM0, signal: ftm0_ch2, pin_signal: PTC2, direction: OUTPUT}
  - {pin_num: '54', peripheral: PORTB, signal: 'port, 0', pin_signal: PTB0, direction: OUTPUT}
  - {pin_num: '53', peripheral: PORTB, signal: 'port, 1', pin_signal: PTB1, direction: OUTPUT}
  - {pin_num: '58', peripheral: PORTA, signal: 'port, 6', pin_signal: PTA6, direction: OUTPUT}
  - {pin_num: '40', peripheral: PORTC, signal: 'port, 0', pin_signal: PTC0, direction: OUTPUT}
  - {pin_num: '4', peripheral: FTM2, signal: ftm2_ch0, pin_signal: PTD0, direction: OUTPUT}
  - {pin_num: '3', peripheral: PORTD, signal: 'port, 1', pin_signal: PTD1, direction: OUTPUT}
  - {pin_num: '73', peripheral: LPI2C0, signal: lpi2c0_sda, pin_signal: PTA2, direction: OUTPUT, PE: state_1, PS: state_1}
  - {pin_num: '72', peripheral: LPI2C0, signal: lpi2c0_scl, pin_signal: PTA3, direction: OUTPUT, PE: state_1, PS: state_1}
  - {pin_num: '84', peripheral: FTM3, signal: ftm3_ch7, pin_signal: PTE6, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"

/* Generate array of configured pin structures */
Port_Ci_Port_Ip_PinSettingsConfig g_pin_mux_InitConfigArr_BOARD_InitPins[NUM_OF_CONFIGURED_PINS_BOARD_InitPins] = {
    {
        .portBase        = IP_PORTA,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 0U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT4,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTA,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT4,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTA,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT3,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTA,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 3U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT3,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTA,
        .gpioBase        = IP_PTA,
        .pinPortIdx      = 6U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_AS_GPIO,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .direction       = PORT_CI_PORT_PIN_OUT,
        .digitalFilter   = (boolean)FALSE,
        .initValue       = 0U,
    },
    {
        .portBase        = IP_PORTB,
        .gpioBase        = IP_PTB,
        .pinPortIdx      = 0U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_AS_GPIO,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .direction       = PORT_CI_PORT_PIN_OUT,
        .digitalFilter   = (boolean)FALSE,
        .initValue       = 0U,
    },
    {
        .portBase        = IP_PORTB,
        .gpioBase        = IP_PTB,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_AS_GPIO,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .direction       = PORT_CI_PORT_PIN_OUT,
        .digitalFilter   = (boolean)FALSE,
        .initValue       = 0U,
    },
    {
        .portBase        = IP_PORTC,
        .gpioBase        = IP_PTC,
        .pinPortIdx      = 0U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_AS_GPIO,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .direction       = PORT_CI_PORT_PIN_OUT,
        .digitalFilter   = (boolean)FALSE,
        .initValue       = 0U,
    },
    {
        .portBase        = IP_PORTC,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT2,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTC,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 16U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT0,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTC,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT2,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTD,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 0U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT4,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTD,
        .gpioBase        = IP_PTD,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_AS_GPIO,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .direction       = PORT_CI_PORT_PIN_OUT,
        .digitalFilter   = (boolean)FALSE,
        .initValue       = 0U,
    },
    {
        .portBase        = IP_PORTE,
        .gpioBase        = IP_PTE,
        .pinPortIdx      = 14U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_AS_GPIO,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .direction       = PORT_CI_PORT_PIN_IN,
        .digitalFilter   = (boolean)FALSE,
        .initValue       = 0U,
    },
    {
        .portBase        = IP_PORTE,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT4,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
    {
        .portBase        = IP_PORTE,
        .gpioBase        = NULL_PTR,
        .pinPortIdx      = 6U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveStrength   = PORT_DRIVE_STRENGTH_LOW,
        .passiveFilter   = (boolean)FALSE,
        .mux             = PORT_MUX_ALT4,
        .lockRegister    = PORT_LOCK_REGISTER_DISABLED,
        .digitalFilter   = (boolean)FALSE,
    },
};

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"

/*==================================================================================================
                                      LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                           LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                           GLOBAL FUNCTIONS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
