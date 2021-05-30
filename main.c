#define __MAIN_C__

#include <stdint.h>
#include <float.h>
#include <math.h>

// Define the raw base address values for the i/o devices

#define AHB_KB_BASE                         0x40000000
#define AHB_LCD_BASE                        0x50000000

// Define pointers with correct type for access to 32-bit i/o devices
//
// The locations in the devices can then be accessed as:
//    KB_REGS[0]
//    KB_REGS[1]
//    LCD_REGS[0]

volatile uint32_t* KB_REGS = (volatile uint32_t*) AHB_KB_BASE;
volatile uint32_t* LCD_REGS = (volatile uint32_t*) AHB_LCD_BASE;

//////////////////////////////////////////////////////////////////
// Functions provided to access i/o devices
//////////////////////////////////////////////////////////////////

//write to LCD
void write_out_LCD(uint32_t value) {
  LCD_REGS[0] = value;
}

//read from LCD
uint32_t read_out_LCD(void) {
  return LCD_REGS[0];
}

//read from keyboard
uint32_t read_data_KB(void) {
  return KB_REGS[0];
}

//wait for someone press the buttom
void wait_for_data_KB(void) {
     while ( KB_REGS[1] == 0 ) {};
     return;
}

//////////////////////////////////////////////////////////////////
// Other Functions
//////////////////////////////////////////////////////////////////
//return the bigger number
int comp(int operator, int decimalC_valueA, int decimalC_valueB){
	int power = 0;
	if(operator==12 || operator==13){
		if(decimalC_valueA>decimalC_valueB){
			power = decimalC_valueA;
		}
		else{
			power = decimalC_valueB;
		}
	}
	else if(operator == 14){
		power = decimalC_valueA + decimalC_valueB;
	}
	else{
		power = 13;
	}
	if(power <= 13){
		power = power;
	}
	else{
		power = 13;
	}
	return power;
}
//calculation 
double calResult(double a, double b, int c, int d) {
  double results;
  if(d==1){
	  b = -b;
  }
  if(c==12)
    results = a+b; 
  else if(c == 13)
    results = a-b; 
  else if(c == 14)
    results = a*b;	
  else if(c == 15)
    results = a/b;
  //else if(c == 16)
  //  results = pow(a,b);
 // else if(c == 17){
 //   results = exp(a);
 //}
 
  else 
    results = a;
  return results;
}

//decoding the keyboard input
int decoding_KB (int a) {
	 int inputValue = 0;
     switch(a) {
          case 0x1ee:
               inputValue = 0; break;
          case 0x1de:
               inputValue = 1; break;
          case 0x1be:
               inputValue = 4; break;
          case 0x17e:
               inputValue = 7; break;
          case 0x1ed:
               inputValue = 10; break;             // .
          case 0x1dd:
               inputValue = 2; break;
          case 0x1bd:
               inputValue = 5; break;
          case 0x17d:
               inputValue = 8; break;
          case 0x1eb:
               inputValue = 11; break;             // =
          case 0x1db:
               inputValue = 3; break;
          case 0x1bb:
               inputValue = 6; break;
          case 0x17b:
               inputValue = 9; break;
          case 0x1e7:
               inputValue = 12; break;             // +
          case 0x1d7:
               inputValue = 13; break;             // -
          case 0x1b7:
               inputValue = 14; break;             // x
          case 0x177:
               inputValue = 18; break;             // esc
          case 0x0f7: 
               inputValue = 15; break;             // /
          case 0x0fb: 
               inputValue = 16; break;             // ^
          case 0x0fd: 
               inputValue = 17; break;             // sqrt			   
     } 
	 return inputValue;
}

//encoding output for LCD display
int encoding_LCD (int a) {
	 int outputValue = 0;
     switch(a) {
          case 0:
               outputValue = 0x230; break;
          case 1:
               outputValue = 0x231; break;
          case 2:
               outputValue = 0x232; break;
          case 3:
               outputValue = 0x233; break;
          case 4:
               outputValue = 0x234; break;             
          case 5:
               outputValue = 0x235; break;
          case 6:
               outputValue = 0x236; break;
          case 7:
               outputValue = 0x237; break;
          case 8:
               outputValue = 0x238; break;            
          case 9:
               outputValue = 0x239; break;
          case 10:
               outputValue = 0x22e; break;             // .
          //case 11:
          //     return 59; break;
          case 12:
               outputValue = 0x22b; break;             // +
          case 13:
               outputValue = 0x22d; break;             // -
          case 14:
               outputValue = 0x22a; break;             // x
          case 18:
               outputValue = 0x001; break;             // esc
          case 15: 
               outputValue = 0x22f; break;             // /
          case 16: 
               outputValue = 0x25e; break;             // ^
          case 17: 
               outputValue = 0x2e8; break;             // sqrt			   
     } 
	 return outputValue;
}

//decoding the result for output to LCD
float cal_power(int i) {
  float a = 1;
  if(i == 0) 
	{ a = 1;}
  else {
	for(int j = i; j>0; j--){
		a = 10 * a;
	}
  }
  return a;
}	

int sending_results(double result, int decimalC_result){
	int output_counter = 0;
	int counter_res = 0;
	
     if(result < 0){
          write_out_LCD(0x22d);   // display "-" for negative number
          result = -result;
		  output_counter++;
	 }
	 else{
		 //write_out_LCD(0x81);		// 22 Apr  starts at 01H
		 result = result;
	 }
	// output_counter = 1;    
	 
	//get the integer and decimal part
	 double y, x;
	 x = modf(result, &y);	

	//integer part counter	 
	 float temp = y;
	 int i = 0;
     while(temp/10 >= 1){
          i++;
          temp = temp / 10;
	 }
	 
	int digial_decimal = 0;
	if(decimalC_result<6){
		digial_decimal = decimalC_result;
	}
	else{
		digial_decimal=6;
	}
	
	float yy = y;
	// output the integer part
     for(int j = i; j >= 0; j--){
		 if(output_counter == 8){
			 write_out_LCD(0xC0);   // the 9th bit start at 40H at LCD
		 }
          float z = cal_power(j);
          int a = yy / z;
		  yy = yy - z*a;
		  write_out_LCD(encoding_LCD(a));    
		  output_counter++ ;
     }	

	 // output the decimal point
	 float xx = x;
	 float db = round(xx * cal_power(digial_decimal));	
	 
	 if(db != 0){   //if decimal part is not zero
		 if(output_counter == 8){
			 write_out_LCD(0xC0);   // the 9th bit start at 40H at LCD
		 }		 
		 write_out_LCD(0x22e);
		 output_counter++;
	 }
	 
	 //output the decimal part 	 
	 for(int j = digial_decimal; j>0; j--){
		 if(db > 0){
			if(output_counter == 8){
				write_out_LCD(0xC0);   // the 9th bit start at 40H at LCD
			}				 
			int a = db/cal_power(j-1);
			write_out_LCD(encoding_LCD(a));
			output_counter++;
			counter_res++;
			db = db - a*cal_power(j-1);
		 }
	 }	
	 return counter_res;
}

//send out "Error"
void sending_Error(void){	
	write_out_LCD(0x245);		//output "Error" : E
	write_out_LCD(0x272);		// r
	write_out_LCD(0x272);		// r
	write_out_LCD(0x26f);		// o
	write_out_LCD(0x272);		// r	
}

//delay
void time_delay(int i){
	int a = 0;
	while(a < i){
		a++;
	};
	return;
}

//set LCD DDRAM to the orginal address
void set_LCD(void){
	write_out_LCD( 0x001 );	// clear all the regs, set DDRAM add to 00H
	time_delay(10);			//delay 1.7ms > 1.53ms
	write_out_LCD( 0x006 );	// set entry mode, shift off
	//time_delay(1);			//delay 230us
	write_out_LCD(0x245);
	write_out_LCD(0x245);
	write_out_LCD(0x245);
	write_out_LCD(0x245);
	write_out_LCD(0x245);

}

//////////////////////////////////////////////////////////////////
// Main Function
//////////////////////////////////////////////////////////////////

int main(void) {

  double inputValueA = 0;
  double inputValueB = 0;
  int operator = 0;			// + : 11; - : 12; * : 13; / : 14;
  int operator_n = 0;		// negative input of operand B
  int operator_1 = 0;		// 1: operand B is negative
  int inputPattern;
  int operand_a = 0;		// "0" : input for A; "1" input for B
  int operator_b = 0;		// "1" : operator was already input before; "0" : operator is input the first time 
  int decimal_c = 0;		// "1" : decimal point input;
 // int decimal_counter = 1;	// number of decimal digit, starts at 1
  int esc_counter = 0;		// counter for "esc" input
  int counter_valueB = 0;	// 0: the first number input for valueB
  int decimalC_valueA = 0;  // number of decimal digits
  int decimalC_valueB = 0; 
  int decimalC_result = 0;

  //initial LCD    clock = 10 000ns
  time_delay(270);            //delay 36ms > 30ms
  write_out_LCD( 0x038 );	// function set: 8-bit, 2 line, 5*8 dot   16 Apr
  //time_delay(1);			//delay 200us > 39us
  write_out_LCD( 0x00c );	// set display on, cursor off, blink off
  //time_delay(1);			//delay 300us > 39us
  set_LCD();
  
// repeat forever (embedded programs generally do not terminate)

  while(1){
		
       wait_for_data_KB();
       inputPattern = decoding_KB(read_data_KB());
		// detect "." input
	   if(inputPattern == 10){
		   if(decimal_c == 0){		//21 April: response to the first ".", the others are ignored
			   decimal_c = 1;
			   write_out_LCD(encoding_LCD(inputPattern));
			   if(counter_valueB == 0){			//21 April: reset value B when input the frist "." for the sencond operand
				   inputValueB = 0;
			   }
		   }
	   }
	   // input number for the first operand
       if(inputPattern < 10 && operand_a == 0){
		   if(decimal_c == 0){
               inputValueA = inputPattern + inputValueA * 10;
               inputValueB = inputPattern + inputValueB * 10;
		   }
		   else{
			   inputValueA = inputValueA + (float)inputPattern / cal_power(decimal_c); 
			   inputValueB = inputValueA;
			   decimal_c++;
			   decimalC_valueA++;
			   decimalC_valueB++;
          }
		  write_out_LCD(encoding_LCD(inputPattern));	  
	   }
	   // input number for the second operand
       else if(inputPattern < 10 && operand_a == 1) {
		   if(counter_valueB == 0){	
			   inputValueB = 0;
			   decimalC_valueB = 0;
			   //decimal_c = 0;
			   if(operator_1 == 0){    // 23 April
					set_LCD();
			   }
		   }
		   if(decimal_c == 0){
               inputValueB = inputPattern + inputValueB * 10;
		   }
		   else{
			   inputValueB = inputValueB + (float)inputPattern / cal_power(decimal_c);
			   decimal_c++;
			   decimalC_valueB++;
		   }
               write_out_LCD(encoding_LCD(inputPattern));
               operator_b = 1;
			   esc_counter = 0;
			   counter_valueB = 1;
          }
		  		  
		//input the operator
       else if(inputPattern>11 && inputPattern<18){
            operand_a = 1;
			decimal_c = 0;
			//decimal_counter = 1;
			set_LCD();	
            if(operator_b == 0){
				if(operator_n == 0){
					operator = inputPattern;					
					write_out_LCD(encoding_LCD(inputPattern));
					operator_n = 1;
				}
				else{
					if(inputPattern == 13){  //if the second operator is "-", it means operand B is a negative number
						operator_1 = 1;
						//set_LCD();
						write_out_LCD(encoding_LCD(inputPattern));
					}
					else{
						operator = inputPattern;
						write_out_LCD(encoding_LCD(inputPattern));
					}
				}
            }
            else if(operator_b == 1){						//input operator for the sencond time
				if(inputValueB == 0 && operator == 15){
					//set_LCD();
					sending_Error();
			        operand_a = 0;
					operator_b = 0;
					operator_n = 0;    // 23 April
					operator_1 = 0;
					decimal_c = 0;
					//decimal_counter = 1;
					inputPattern = 0;
					inputValueA = 0;
					inputValueB = 0;
					operator = 0;
					esc_counter = 0;
					decimalC_valueA=0;
					decimalC_valueB=0;					
				}
				else{
					inputValueA = calResult(inputValueA, inputValueB, operator, operator_1);
					//set_LCD();
					decimalC_result = comp(operator, decimalC_valueA, decimalC_valueB); 
					decimalC_valueA =sending_results(inputValueA, decimalC_result);      		//output result to lcd
				}					
				operator = inputPattern;
				operator_b = 0;  
				operator_1 = 0;	// 29 April  clear the states about operand B
            }
				//inputValueB = 0;      03-22
				counter_valueB = 0;  
	   }
        // = trigger the calculation and reset variables for the next round
      else if(inputPattern == 11){  
					set_LCD();	
					if(inputValueB == 0 && operator == 15){	 
						sending_Error();
						inputPattern = 0;
						inputValueA = 0;
						inputValueB = 0;
						operator = 0;
						esc_counter = 0;
						decimalC_valueA=0;
						decimalC_valueB=0;							
					}
					else{
						inputValueA = calResult(inputValueA, inputValueB, operator, operator_1);
						decimalC_result = comp(operator, decimalC_valueA, decimalC_valueB);
						decimalC_valueA = sending_results(inputValueA, decimalC_result);
					}
					operand_a = 0;
					operator_b = 0;
					operator_n = 0;   // 23 April
					operator_1 = 0;
					decimal_c = 0;
					//decimal_counter = 1;
					counter_valueB = 0;
					//inputValueB = 0;			keep inputValueB for the next "=" input operation: 1+2= = = =
               } 
		  // input ecs clear the input value, if input twice, clear all
       else if(inputPattern == 18){           	       // esc input once
				set_LCD();
				if(operand_a == 0){						//input esc when input value for A
					inputValueA = 0;
					decimalC_valueA=0;
					esc_counter = 0;
				}		
				else {								    // input esc when input value for B, clear valueB
					if(esc_counter == 0){
					esc_counter ++;			
				}	
					else if(esc_counter == 1){			// esc input twice consecutively, clear all
						operand_a = 0;
						operator_n = 0;  // 23 April
						operator_b = 0;
						decimal_c = 0;
						//decimal_counter = 1;
						inputPattern = 0;
						inputValueA = 0;
						operator = 0;
						esc_counter = 0;
						decimalC_valueA=0;
					}
				}
				//inputValueB = 0;
				counter_valueB = 0;	
				operator_1 = 0;       // 23 April
				//decimalC_valueB=0;
				decimal_c = 0;
			}
     }
}