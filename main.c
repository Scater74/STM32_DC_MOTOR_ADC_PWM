#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM


uint16_t buttonState=0;
uint16_t flag=0;
uint16_t potValue=0;
uint16_t mapValue=0;
uint16_t read_pwm=0;
void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	//base pin + leds
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	//button
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
  GPIO_Init(GPIOC,&GPIOInitStructure);
	
	
	//pwm pin
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//pot pin
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
  GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void timerConfig(){
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
  TIMERInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Prescaler=10;
	TIMERInitStructure.TIM_Period=2399;
	TIMERInitStructure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM2,&TIMERInitStructure);
	TIM_Cmd(TIM2,ENABLE);

}


void pwmConfig(uint32_t timPulse){
	
	TIM_OCInitTypeDef TIMER_OCInitStructure;
	TIMER_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMER_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIMER_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMER_OCInitStructure.TIM_Pulse=timPulse;

  TIM_OC1Init(TIM2,&TIMER_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

}



void adcConfig(){
   ADC_InitTypeDef ADCInitStructure;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
   
   ADCInitStructure.ADC_ContinuousConvMode=ENABLE;
   ADCInitStructure.ADC_DataAlign=ADC_DataAlign_Right;
   ADCInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
   ADCInitStructure.ADC_Mode=ADC_Mode_Independent;
   ADCInitStructure.ADC_NbrOfChannel=1;
   ADCInitStructure.ADC_ScanConvMode=DISABLE;
   
   ADC_Init(ADC1,&ADCInitStructure);
   ADC_Cmd(ADC1,ENABLE);

}

uint16_t readADC(){
   
   ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
   ADC_SoftwareStartConvCmd(ADC1,ENABLE);
   
   while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
   
   return ADC_GetConversionValue(ADC1);

}


float map(float adcValue, float max, float min, float conMax, float conMin){
   
   return adcValue*((conMax-conMin)/(max-min));

}

void delay(uint32_t time){
	while(time--);
}




int main(){
	gpioConfig();
	timerConfig();
	adcConfig();
	
	while(1){	
		
		
		potValue=readADC();
		mapValue=map(potValue,4030,0,2399,0);
		pwmConfig(mapValue);
		
		buttonState=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
//		
		if(buttonState==1){
		
			if(flag==1){
				flag=0;
				
			}else{
				flag=1;
			
			}
//			
//		}
//		if(flag==1){
//			potValue=readADC();
//			mapValue=map(potValue,4030,0,2399,0);
//			pwmConfig(mapValue);
//			
//			
//			delay(7200000);
//		
//		
//		}else{
//			GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
//		
//		
//		}

		}



}