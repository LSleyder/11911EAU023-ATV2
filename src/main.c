#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* AHB1 Base Addresses ******************************************************/
#define STM32_RCC_BASE 0x40023800 /* 0x40023800-0x40023bff: Reset and Clock control RCC */

/* GPIO Base Addresses ******************************************************/
#define STM32_GPIOA_BASE 0x40020000    /* 0x40020000-0x400203FF: GPIO PORT A */
#define STM32_GPIOC_BASE 0x40020800  /* 0x48000800-0x48000bff: GPIO Port C */

/* Register Offsets *********************************************************/
#define STM32_RCC_AHB1ENR_OFFSET 0x0030 /* AHB1 Peripheral Clock enable register */
#define STM32_GPIO_MODER_OFFSET 0x0000 /* GPIO port mode register */
#define STM32_GPIO_OTYPER_OFFSET 0x0004 /* GPIO port output type register */
#define STM32_GPIO_PUPDR_OFFSET 0x000c /* GPIO port pull-up-pull-down register */
#define STM32_GPIO_IDR_OFFSET 0x0010  /* GPIO port input data register */
#define STM32_GPIO_ODR_OFFSET 0x0014  /* GPIO port output data register */
#define STM32_GPIO_BSRR_OFFSET 0x0018 /* GPIO port bit set-reset register */

/* Register Addresses *******************************************************/
#define STM32_RCC_AHB1ENR (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)
#define STM32_GPIOA_MODER (STM32_GPIOA_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_PUPDR (STM32_GPIOA_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOA_IDR (STM32_GPIOA_BASE+STM32_GPIO_IDR_OFFSET)
#define STM32_GPIOC_MODER (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_ODR (STM32_GPIOC_BASE+STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR (STM32_GPIOC_BASE+STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */
#define RCC_AHB1ENR_GPIOAEN (1 << 0)  /* Bit 0:  IO port A clock enable */
#define RCC_AHB1ENR_GPIOCEN (1 << 2)  /* Bit 2:  IO port C clock enable */

/* GPIO port mode register */
#define GPIO_MODER_INPUT (0) /* Input */
#define GPIO_MODER_OUTPUT (1) /* General purpose output mode */
#define GPIO_MODER_ALT (2) /* Alternate mode */
#define GPIO_MODER_ANALOG (3) /* Analog mode */
#define GPIO_MODER_SHIFT(n) (n << 1)
#define GPIO_MODER_MASK(n) (3 << GPIO_MODER_SHIFT(n))

/* GPIO port output type register */
#define GPIO_OTYPER_PP (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD (1) /* 1=Output open-drain */
#define GPIO_OT_SHIFT(n) (n)
#define GPIO_OT_MASK(n) (1 << GPIO_OT_SHIFT(n))

/* GPIO port pull-up/pull-down register */
#define GPIO_PUPDR_NONE (0) /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP (1) /* Pull-up */
#define GPIO_PUPDR_PULLDOWN (2) /* Pull-down */
#define GPIO_PUPDR_SHIFT(n) (n << 1)
#define GPIO_PUPDR_MASK(n) (3 << (n << 1))

/* GPIO port input data register */
#define GPIO_IDR_MASK(n) (1 << n)

/* GPIO port bit set/reset register */
#define GPIO_BSRR_SET(n) (1 << (n))
#define GPIO_BSRR_RESET(n) (1 << ((n) + 16))

/* Variaveis para o set e reset do LED */
#define LED_DELAY_SLOW   1000000
#define LED_DELAY_FAST  100000

/* Funções */
void delay(uint32_t tick)
{
  uint32_t i;

  for (i = 0; i < tick; i++)
    {
      asm("nop");
    }
}

bool key_pressed(uint32_t *pGPIOA_IDR)
{
  uint32_t reg;
  reg = *pGPIOA_IDR;
  reg &= GPIO_IDR_MASK(0);
  return !reg;
}

/* Função principal */
int main(int argc, char *argv[])
{
    uint32_t reg;

    /* Ponteiros para registradores */
    uint32_t *pRCC_AHB1ENR  = (uint32_t *)STM32_RCC_AHB1ENR;
    uint32_t *pGPIOA_MODER  = (uint32_t *)STM32_GPIOA_MODER;
    uint32_t *pGPIOA_PUPDR  = (uint32_t *)STM32_GPIOA_PUPDR;
    uint32_t *pGPIOA_IDR    = (uint32_t *)STM32_GPIOA_IDR;
    uint32_t *pGPIOC_MODER  = (uint32_t *)STM32_GPIOC_MODER;
    uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    uint32_t *pGPIOC_PUPDR  = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOC_BSRR   = (uint32_t *)STM32_GPIOC_BSRR;

    /* Habilita clock GPIOC */
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOAEN;
    reg |= RCC_AHB1ENR_GPIOCEN;
    *pRCC_AHB1ENR = reg;

    /* Configura PC13 como saida pull-up off e pull-down off */
    reg = *pGPIOC_MODER;
    reg &= ~GPIO_MODER_MASK(13);
    reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(13));
    *pGPIOC_MODER = reg;

    reg = *pGPIOC_OTYPER;
    reg &= ~GPIO_OT_MASK(13);
    reg |= (GPIO_OTYPER_PP << GPIO_OT_SHIFT(13));
    *pGPIOC_OTYPER = reg;

    reg = *pGPIOC_PUPDR;
    reg &= ~GPIO_PUPDR_MASK(13);
    reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(13));
    *pGPIOC_PUPDR = reg;


    /* Configura PA0 como entrada */
    reg = *pGPIOA_MODER;
    reg &= ~GPIO_MODER_MASK(0);
    reg |= (GPIO_MODER_INPUT << GPIO_MODER_SHIFT(0));
    *pGPIOA_MODER = reg;

    reg = *pGPIOA_PUPDR;
    reg &= ~GPIO_PUPDR_MASK(0);
    reg |= (GPIO_PUPDR_PULLUP << GPIO_PUPDR_SHIFT(0));
    *pGPIOA_PUPDR = reg;

    /* Loop infinito */
    while(1)
    {
        /* Comando set e reset(piscar) do LED */

        *pGPIOC_BSRR = GPIO_BSRR_RESET(13);
        if(key_pressed(pGPIOA_IDR))
            delay(LED_DELAY_SLOW);
        else
            delay(LED_DELAY_FAST);

        *pGPIOC_BSRR = GPIO_BSRR_SET(13);
        if(key_pressed(pGPIOA_IDR))
            delay(LED_DELAY_SLOW);
        else
            delay(LED_DELAY_FAST);
    }

    /* Nao deveria chegar aqui */

    return EXIT_SUCCESS;
}
