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
#include <time.h>

//#include "mtwister.h"
void initb();
void setn();
void setn(int32_t pin_num, int32_t val);
int32_t readpin(int32_t pin_num);

void mysleep(int time) {
    for(int n = 0; n < time; n++);
}
void EXTI0_1_IRQHandler() {
    if ((EXTI->PR & EXTI_PR_PR0) != 0) {
        EXTI->PR |= EXTI_PR_PR0; //pb0
        mysleep(1000000);
        setn(8,1);
    }
}
void init_exti(){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
    EXTI->IMR |= EXTI_IMR_MR0; //exti_imr configures the corresponding the mask bit
    EXTI->RTSR &= ~EXTI_RTSR_TR0; //disable rising trigger selection register
    EXTI->FTSR |= EXTI_FTSR_TR0; // Trigger on falling edge (button press)

   // NVIC ->ISER[0] |= 1 <<5;
//    NVIC ->ISER[0] |= 1 <<6;
//    NVIC ->ISER[0] |= 1 <<7;
}
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
void init_tim2() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 48000 - 1;
    TIM2->ARR = 10000 - 1;
    TIM2->CR1 &= ~TIM_CR1_DIR;

}
void start_timer(){
    TIM2->CR1 |= TIM_CR1_CEN;
}
uint32_t end_timer() {
    TIM2->CR1 &= ~TIM_CR1_CEN;
    return TIM2->CNT;
}
void end_delay(int time) {
    while(TIM2->CNT <= time) {

    }
    TIM2->CR1 &= ~TIM_CR1_CEN;
}
void init_tim3(int time) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    while(TIM3->CNT != time) {
        int x = TIM3 -> CNT;
        TIM3->PSC = 48000 - 1; // sample rate = 1s/48Mhz
        TIM3->ARR = 10000 - 1; // period/rate = 10 secons/1ms
        TIM3->CR1 &= ~TIM_CR1_DIR;
        TIM3->CR1 |= TIM_CR1_CEN;
    }
    TIM3->CR1 &= ~TIM_CR1_CEN;

}
//need the external interrupt for the button

//can we have an interrupt for when the button is first pushed to turn on the green - seems unnecessary
uint32_t standard_reaction_time_test() {
//    int lower = 50000;
//    int upper = 2000000;
    //srand(time(NULL));
    //int num = (rand() % (upper - lower + 1) + lower);
    int num = rand();
//    MTRand r = seedRand(1337);
//    double num = genRand(&r) * 10;
    //create an array of 100 values from this seed?
    //if i create an array of 100 values, then can I alsways take the last value, create another array go through that, then ...
    //the only problem is that i would somhow need to keep track
    //also it would be good if num wasn't multiplied by 10, instead be in milliseconds to increase possibilities
    //what if i use the last value or the middle value or something to determine which index I will choose, then get that rand value.
    //then use another method to generate a new array and again repeat
//    int time_delay = num;
//    init_tim3(time_delay);
    while(readpin(0) == 0);//user needs to press button to start game
    setn(10, 1);              //turn the red led light on
    mysleep(5000000);             //a min amount of time
    //mysleep(num);             //a random amount of time for the green led to turn on
    //delay(5000);
    setn(8,1);                //turn the green led on
    setn(10,0);               //turn the red led off
    start_timer();
    while(readpin(0) == 0); //wait for user to press button
    setn(8,0);                //turn off green led
    setn(10,0);
    uint32_t reaction_time = end_timer();
    mysleep(1000000);
    while(readpin(0) == 0){
        setn(8,0);                //turn off green led
        setn(10,0);
    }
    mysleep(1000000);            //wait
    return reaction_time;
}
uint32_t unit_test1() {
    //press button start timer
    //press button end timer
    while(readpin(0) == 0);
    mysleep(1000000);
    setn(8,1);
    start_timer();
    while(readpin(0) == 0);
    uint32_t time = end_timer();
    setn(8,0);
    return time;
}
int main(void)
{
	initb();
	init_tim2();
//	init_exti();
//	NVIC_EnableIRQ(EXTI0_1_IRQn);
//	NVIC_SetPriority(EXTI0_1_IRQn,1);
	//red - pb10
	//green - pb08;
	while(1) {

	uint32_t reaction_time = standard_reaction_time_test();
	//uint32_t time = unit_test1();
	mysleep(1000000);
	}
	for(;;);
}

void initb() {
	//input PB0
	RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB -> MODER &= ~GPIO_MODER_MODER0;
	GPIOB-> PUPDR  &= ~GPIO_PUPDR_PUPDR0;

	GPIOB-> PUPDR |= GPIO_PUPDR_PUPDR0_1;

	//output PB8 and PB10
	GPIOB -> MODER &= ~GPIO_MODER_MODER8;
	GPIOB -> MODER &= ~GPIO_MODER_MODER10;
	GPIOB -> MODER &= ~GPIO_MODER_MODER14;

	GPIOB -> MODER |= GPIO_MODER_MODER8_0;
	GPIOB -> MODER &= ~GPIO_MODER_MODER8_1;

	GPIOB -> MODER |= GPIO_MODER_MODER10_0;
	GPIOB -> MODER &= ~GPIO_MODER_MODER10_1;

	GPIOB -> MODER |= GPIO_MODER_MODER14_0;
	GPIOB -> MODER &= ~GPIO_MODER_MODER14_1;


}

void setn(int32_t pin_num, int32_t val) {
    if(val == 0){

        GPIOB -> BRR |= 1 << pin_num;
    }
    else{
        GPIOB -> BSRR |= 1 << pin_num;

    }
}

int32_t readpin(int32_t pin_num) {
    int32_t readit = 0;
    readit = (GPIOB -> IDR);
    //readit = (GPIOB -> IDR) & 0x1;

    readit = (readit & (1 << pin_num)) >> pin_num;

    return readit;
}


