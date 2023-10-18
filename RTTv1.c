/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

void inita();
void initb();
void initc();
void setnA(int32_t pin_num, int32_t val);
void setnB(int32_t pin_num, int32_t val);
void setnC(int32_t pin_num, int32_t val);
int32_t readpinB(int32_t pin_num);
int32_t readpinA(int32_t pin_num);
void rtt();
void start_timer();
uint32_t end_timer();

void mysleep(int time) {
    for(int n = 0; n < time; n++);
}
void EXTI0_1_IRQHandler() {
    while(readpinB(1) == 0); //PB1 - VMT
    EXTI->PR |= EXTI_PR_PR1;
    //while(1) {
       // mysleep(10000);
    setnB(0,1);
    mysleep(1000000);
    //setnA(7,1);
    //rtt();
    EXTI->PR &= ~EXTI_PR_PR1;
   // }
    setnB(0,0);
}
void EXTI2_3_IRQHandler() {
    while(readpinA(3) == 0); //PA3 - RTT
    EXTI->PR |= EXTI_PR_PR3;
    setnA(0,1); //turn on green led //pa0
    setnA(1,1); //turn on red led
    start_timer();
    rtt();
    EXTI->PR &= ~EXTI_PR_PR3;
}
void EXTI4_15_IRQHandler() {
    while(readpinA(6) == 0); //PA6 - SMT
    EXTI->PR |= EXTI_PR_PR6;

    setnA(1,1); //turn on yellow led //pc0


    mysleep(1000000);

    EXTI->PR &= ~EXTI_PR_PR6;
    setnA(1,0);
}

void init_exti(){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;

    EXTI->IMR |= EXTI_IMR_MR1; //exti_imr configures the corresponding the mask bit
    EXTI->IMR |= EXTI_IMR_MR3; //exti_imr configures the corresponding the mask bit
    EXTI->IMR |= EXTI_IMR_MR6; //exti_imr configures the corresponding the mask bit
    EXTI->RTSR |= EXTI_RTSR_TR1; //enable rising trigger selection register (button press)
    EXTI->RTSR |= EXTI_RTSR_TR3; //enable rising trigger selection register (button press)
    EXTI->RTSR |= EXTI_RTSR_TR6; //enable rising trigger selection register (button press)
    EXTI->FTSR &= ~EXTI_FTSR_TR1; // disable Trigger on falling edge
    EXTI->FTSR &= ~EXTI_FTSR_TR3; // disable Trigger on falling edge
    EXTI->FTSR &= ~EXTI_FTSR_TR6; // disable Trigger on falling edge

    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn,2);
    NVIC_EnableIRQ(EXTI2_3_IRQn);
    NVIC_SetPriority(EXTI2_3_IRQn,2);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn,2);
}
void TIM2_IRQHandler(void) {
    TIM2->SR &= ~TIM_SR_UIF;
}
void init_tim2() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    //TIM2->DIER |= TIM_DIER_UIE; //enable the timer update interrupt
    TIM2->PSC = 48000 - 1;
    TIM2->ARR = 10000 - 1;
    TIM2->CR1 &= ~TIM_CR1_DIR;

    //where to set priority
}

void start_timer() {
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn,1);
}
uint32_t end_timer() {
    TIM2->CR1 &= ~TIM_CR1_CEN;

    uint32_t time = TIM2->CNT;
    NVIC_DisableIRQ(TIM2_IRQn);
    return time;
}
uint32_t standard_reaction_time_test() {


    //pa0 -green LED
    //pa1 - red LEd
    //pa2 - button
    while(readpinA(2) == 0); //user needs to press button to start game //pa2
    mysleep(1000000);
    uint32_t new_seed = end_timer();
    srand(new_seed);
    //uint32_t num = 100000+ rand() % (9999999 - 100000);
    uint32_t num = 1000+ rand() % (60000 - 1000);
    mysleep(100000);

    setnA(0,0);
    setnA(1,0);

    setnA(1,1);              //turn the red led light on

   // mysleep(3000000);             //a min amount of time
    mysleep(num*480);
    setnA(0,1);                //turn the green led on
    setnA(1,0);               //turn the red led off
    start_timer();
    while(readpinA(2) == 0); //wait for user to press button
    setnA(0,0);                //turn off green led
    setnA(1,0);
    uint32_t reaction_time = end_timer();
    mysleep(1000000);
    while(readpinA(2) == 0) {
        setnA(0,0);                //turn off green led
        setnA(1,0);
    }
    mysleep(1000000);            //wait
    return reaction_time;
}
uint32_t unit_test1() {
    //press button start timer
    //press button end timer
    while(readpinA(2) == 0);
    mysleep(1000000);
    setnA(1,1);
    start_timer();
    while(readpinA(2) == 0);
    setnA(1,0);
    uint32_t time = end_timer();

    return time;
}
void rtt()
{
    while(1) {
        uint32_t reaction_time = standard_reaction_time_test();
//        uint32_t time = unit_test1();
        mysleep(1000000);
    }
    for(;;);
}


void inita() {
    //inputs PA3 (RTT select), PA6 (SMT select), PA2 (RTT button)
    //outputs (PA0, PA1)
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA -> MODER &= ~GPIO_MODER_MODER3; //pa3
    GPIOA-> PUPDR  &= ~GPIO_PUPDR_PUPDR3;
    GPIOA-> PUPDR |= GPIO_PUPDR_PUPDR3_1;

    GPIOA -> MODER &= ~GPIO_MODER_MODER6; //pa6
    GPIOA-> PUPDR  &= ~GPIO_PUPDR_PUPDR6;
    GPIOA-> PUPDR |= GPIO_PUPDR_PUPDR6_1;

    GPIOA -> MODER &= ~GPIO_MODER_MODER2; //pa2
    GPIOA-> PUPDR  &= ~GPIO_PUPDR_PUPDR2;
    GPIOA-> PUPDR |= GPIO_PUPDR_PUPDR2_1;

    //outputs
    GPIOA -> MODER &= ~GPIO_MODER_MODER0; //pa0
    GPIOA -> MODER |= GPIO_MODER_MODER0_0;
    GPIOA -> MODER &= ~GPIO_MODER_MODER0_1;

    GPIOA -> MODER &= ~GPIO_MODER_MODER1; //pa1
    GPIOA -> MODER |= GPIO_MODER_MODER1_0;
    GPIOA -> MODER &= ~GPIO_MODER_MODER1_1;
}
void initb() {
   RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
   GPIOB -> MODER &= ~GPIO_MODER_MODER1;
   GPIOB-> PUPDR  &= ~GPIO_PUPDR_PUPDR1;
   GPIOB-> PUPDR |= GPIO_PUPDR_PUPDR1_1;


   //output PB0 - Green LED
   GPIOB -> MODER &= ~GPIO_MODER_MODER0;
   GPIOB -> MODER |= GPIO_MODER_MODER0_0;
   GPIOB -> MODER &= ~GPIO_MODER_MODER0_1;
}
void initc() {
   RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
   //output PC0 - Yellow LED
   GPIOC -> MODER &= ~GPIO_MODER_MODER0;
   GPIOC -> MODER |= GPIO_MODER_MODER0_0;
   GPIOC -> MODER &= ~GPIO_MODER_MODER0_1;
}

void setnA(int32_t pin_num, int32_t val) {
    if(val == 0){

        GPIOA -> BRR |= 1 << pin_num;
    }
    else{
        GPIOA -> BSRR |= 1 << pin_num;

    }
}

void setnB(int32_t pin_num, int32_t val) {
    if(val == 0){

        GPIOB -> BRR |= 1 << pin_num;
    }
    else{
        GPIOB -> BSRR |= 1 << pin_num;

    }
}
void setnC(int32_t pin_num, int32_t val) {
    if(val == 0){

        GPIOC -> BRR |= 1 << pin_num;
    }
    else{
        GPIOC -> BSRR |= 1 << pin_num;

    }
}

int32_t readpinA(int32_t pin_num) {
    int32_t readit = 0;
    readit = (GPIOA -> IDR);
    //readit = (GPIOB -> IDR) & 0x1;

    readit = (readit & (1 << pin_num)) >> pin_num;

    return readit;
}

int32_t readpinB(int32_t pin_num) {
    int32_t readit = 0;
    readit = (GPIOB -> IDR);
    //readit = (GPIOB -> IDR) & 0x1;

    readit = (readit & (1 << pin_num)) >> pin_num;

    return readit;
}

int main(void)
{
    inita();
    initb();
    initc();
    init_exti();
    init_tim2();
    while(1) {
        //setn(8,1);

    }
    for(;;);
}


