#include <stm32f7xx.h>
void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
void RCC_SETUP();
void GPIO_SETUP();
void cerveza(int n);
void retorno();
int bt1=0, bt2=0, bt3=0,bt_regreso=0;

int main(){
	RCC_SETUP();
    GPIO_SETUP();
    SysTick_Init();

    while(1){
        bt_regreso = (GPIOC->IDR)&(1<<13);
        bt1 = (GPIOF->IDR)&(1<<12); 
        bt2 = (GPIOF->IDR)&(1<<13);
        bt3 = (GPIOF->IDR)&(1<<14);
        
        if(bt_regreso == (1<<13)){
            retorno();  //pone el envase en posicion inicial
        }
        if(bt1 == (1<<12)){
            cerveza(1000); //cerveza modo 1
        }
        if(bt2 == (1<<13)){
            cerveza(1500); //cerveza modo 2
        }
        if(bt3 == (1<<14)){
            cerveza(2000); //cerveza modo 3
        }


    }
}

void RCC_SETUP(){
    RCC-> AHB1ENR |= 0xFF; //Activa los puertos de la A a la F
}
void GPIO_SETUP(){
    GPIOC->MODER |= (0<<26); // PC13 es el boton de Usuario que sera el boton para el Retroceso, Input
    GPIOC->PUPDR |= (2<<26); // pull down el pin PC13

    GPIOE->MODER |= (1<<14)|(1<<16); //PE7, PE8 como salidas hacias los reles

    GPIOF->MODER |= (0<<24)|(0<<26)|(0<<28); //PF12, PF13 y PF14 como entradas para los botones de los modos
    GPIOF->PUPDR |= (2<<24)|(2<<26)|(2<<28); //PF12, PF13 y PF14 pull down

}
void retorno(){
    // La misma logica que la funcion cerveza pero con el pin del rele de giro horario
    GPIOE->ODR &=~ 0xFFFF;
    GPIOE->ODR |= (1<<8);
    SysTick_Wait1ms(3000); //Ajustar a tiempo necesario para que llegue al vaso totalmente recto
    GPIOE->ODR &=~ 0xFFFF;

}
void cerveza(int n){
    /*Primero desactiva los pines de E prendidos para evitar algun tipo de error y luego activa el rele de giro anti-horario para
    girarlo el tiempo necesario para llegar al angulo deseado
     */
    GPIOE->ODR &=~ 0xFFFF;
    GPIOE->ODR |= (1<<7);
    SysTick_Wait1ms(n);
    GPIOE->ODR &=~ 0xFFFF;

}


void SysTick_Init(void){
  SysTick->LOAD = 0x00FFFFFF;
	SysTick->CTRL = 0x00000005;
}

void SysTick_Wait(uint32_t n){
  
	SysTick->LOAD = n-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL&0x00010000)==0);
}

void SysTick_Wait1ms(uint32_t delay){
 for(uint32_t i=0; i<delay; i++){
 SysTick_Wait(16000);
 }
}