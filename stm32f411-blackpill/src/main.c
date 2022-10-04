/****************************************************************************
 * main.c
 *
 *   Copyright (C) 2022 Daniel P. Carvalho. All rights reserved.
 *   Authors: Daniel P. Carvalho <daniel.carvalho@ufu.br>
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdlib.h>

 /****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* AHB1 Base Addresses ******************************************************/

#define STM32_RCC_BASE       0x40023800     /* 0x40023800-0x40023bff: Reset 
                                               and Clock control RCC */

/* AHB2 Base Addresses ******************************************************/

#define STM32_GPIOC_BASE     0x40020800     /* 0x40020800 - 0x4002 0BFF GPIOC */

/* Register Offsets *********************************************************/

#define STM32_RCC_AHB1ENR_OFFSET  0x0030   /* AHB1 Peripheral Clock enable
                                               register */

#define STM32_GPIO_MODER_OFFSET   0x0000  /* GPIO port mode register */
#define STM32_GPIO_OTYPER_OFFSET  0x0004  /* GPIO port output type register */
#define STM32_GPIO_PUPDR_OFFSET   0x000c  /* GPIO port pull-up/pull-down register */
#define STM32_GPIO_IDR_OFFSET     0x0010  /* GPIO port input data register */
#define STM32_GPIO_ODR_OFFSET     0x0014  /* GPIO port output data register */
#define STM32_GPIO_BSRR_OFFSET    0x0018  /* GPIO port bit set/reset register */

/* Register Addresses *******************************************************/

#define STM32_RCC_AHB1ENR        (STM32_RCC_BASE + STM32_RCC_AHB1ENR_OFFSET)

#define STM32_GPIOC_MODER        (STM32_GPIOC_BASE + STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER       (STM32_GPIOC_BASE + STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR        (STM32_GPIOC_BASE + STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_IDR          (STM32_GPIOC_BASE + STM32_GPIO_IDR_OFFSET)
#define STM32_GPIOC_ODR          (STM32_GPIOC_BASE + STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR         (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */

#define RCC_AHB1ENR_GPIOCEN      (1 << 2)  /* Bit 2:  IO port C clock enable */

/* GPIO port mode register */

#define GPIO_MODER_INPUT           (0) /* Input */
#define GPIO_MODER_OUTPUT          (1) /* General purpose output mode */
#define GPIO_MODER_ALT             (2) /* Alternate mode */
#define GPIO_MODER_ANALOG          (3) /* Analog mode */

#define GPIO_MODER_SHIFT(n)        ((n) << 1)
#define GPIO_MODER_MASK(n)         (3 << GPIO_MODER_SHIFT(n))

/* GPIO port output type register */

#define GPIO_OTYPER_OD(n)          (1 << (n)) /* 1=Output open-drain */
#define GPIO_OTYPER_PP(n)          (0)        /* 0=Output push-pull */

#define GPIO_OT_MASK(n)            (1 << (n))

/* GPIO port pull-up/pull-down register */

#define GPIO_PUPDR_NONE            (0) /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP          (1) /* Pull-up */
#define GPIO_PUPDR_PULLDOWN        (2) /* Pull-down */

#define GPIO_PUPDR_SHIFT(n)        ((n) << 1)
#define GPIO_PUPDR_MASK(n)         (3 << GPIO_PUPDR_SHIFT(n))

/* GPIO port bit set/reset register */

#define GPIO_BSRR_SET(n)           (1 << (n))
#define GPIO_BSRR_RESET(n)         (1 << ((n) + 16))

#define GPIO_BSRR13_SET            (1 << 13)
#define GPIO_BSRR13_RESET          (1 << (13 + 16))

/* Configuration ************************************************************/

#define DELAY  500000

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

 /****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char *argv[])
{
  uint32_t reg;

  /* Ponteiros para registradores */

  uint32_t volatile *pRCC_AHB1ENR  = (uint32_t *)STM32_RCC_AHB1ENR;
  uint32_t volatile *pGPIOC_MODER  = (uint32_t *)STM32_GPIOC_MODER;
  uint32_t volatile *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
  uint32_t volatile *pGPIOC_PUPDR  = (uint32_t *)STM32_GPIOC_PUPDR;
  uint32_t volatile *pGPIOC_BSRR   = (uint32_t *)STM32_GPIOC_BSRR;

  /* Habilita clock GPIOA e GPIOC */

  reg  = *pRCC_AHB1ENR;
  reg |= (RCC_AHB1ENR_GPIOCEN);
  *pRCC_AHB1ENR = reg;

  /* Configura PC13 como saida pull-up off e pull-down off */

  reg = *pGPIOC_MODER;
  reg &= ~(GPIO_MODER_MASK(13));
  reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(13));
  *pGPIOC_MODER = reg;

  reg = *pGPIOC_OTYPER;
  reg &= ~(GPIO_OT_MASK(13));
  reg |= GPIO_OTYPER_PP(13);
  *pGPIOC_OTYPER = reg;

  reg = *pGPIOC_PUPDR;
  reg &= ~(GPIO_PUPDR_MASK(13));
  reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(13));
  *pGPIOC_PUPDR = reg;

  while(1)
    {
      *pGPIOC_BSRR = GPIO_BSRR13_SET;
      for (uint32_t i = 0; i < DELAY; i++){asm("nop");};
      *pGPIOC_BSRR = GPIO_BSRR13_RESET;
      for (uint32_t i = 0; i < DELAY; i++){asm("nop");};
    }

  return EXIT_SUCCESS;
}