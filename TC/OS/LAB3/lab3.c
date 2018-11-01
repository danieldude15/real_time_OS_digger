// sou3.c
#include <dos.h>
#include <stdio.h>

#define ON (1)
#define OFF (0)
#define MAX_CLICKS 200
volatile int global_saved_times[MAX_CLICKS/2];
volatile int global_i=0;
volatile int global_key_counter=0;
volatile int global_timer=0;
volatile char global_scan_code = -1;
volatile int global_key_pressed=0;
volatile int global_debug=1;


void interrupt (*int8save)(void);
void interrupt (*int9save)(void);

void interrupt newint8(void);
void interrupt newint9(void);
void save_counted_time();
void setup_interrupts();
int find_max();
int find_min();
int get_total();
void sort_array();
void swap(int j);
void finilize();
void ChangeSpeaker( int status );
void setup_sound_latch( int hertz );
void NoSound( void );

int main( void ){
	if(global_debug)
		printf("setting up interrupts\n");
	
	setup_interrupts();
   
	return 1;
} /*--main( )-------*/


void interrupt newint8(void){
   (*int8save)();
   global_timer++;
} // newint8(void)

void interrupt newint9(void){
	char sc;
	if(global_debug)
			printf("got into interrupt 9\n");
	(*int9save)();
	asm {
		push ax
		in al,60h
		mov sc,al
		pop ax
		test sc,80h
		jnz Release
	}
	global_key_pressed=1;
	
	if(global_scan_code == -1){//when a button is first pressed
		if(global_debug)
			printf("got new scan code = %c\n",sc);
		global_scan_code = sc;
		global_timer=0;
	}
	if (sc==global_scan_code){//when the same key is pressed again
		if(global_debug)
			printf("key was pressed for the second time\n");
		global_saved_times[global_i]=global_timer;
		setup_sound_latch(global_saved_times[global_i++]);
		global_scan_code=-1;
	}
	global_key_counter++;
	if(global_key_counter>=MAX_CLICKS || sc==1){
		if(global_debug)
			printf("200 keys was pressed\n");
		finilize();
	}
	asm {
		jmp Skip
	}
	Release: // release
	if(global_debug)
			printf("Button was released\n");
	global_key_pressed=0;
	Skip:
	asm {
		in   al,61h
		or   al,80h
		out  61h,al
		and  al,7Fh
		out  61h,al
		mov   al,20h
		out   20h,al
		pop ax
	}
	//printf("Existing int9");
} // newint9(void)

void setup_interrupts(){
	//setup latch to 700 assuming a second is worth 1069 interrupts8
	if(global_debug)
			printf("starting asm code in setup\n");
		
        asm {
          CLI
          PUSH AX
          MOV AL,036h
          OUT 43h,AL
          MOV AX,700
          OUT 40h,AL
          MOV AL,AH
          OUT 40h,AL 
          POP AX
		  STI
        } // asm
	
	if(global_debug)
			printf("finished asm code in setup\n");
		
	int8save = getvect(8);
    setvect(8,newint8);
	
	
	if(global_debug)
			printf("done setting int 8\n");
		
	//setting up new int9 for keypress
	int9save = getvect(9);
    setvect(9,newint9);
	
	if(global_debug)
			printf("done setting int 9\nsetup Done\n");
		
		   while(1){
   
   }
}

int find_max(){
	int i,max=-1;
	for(i=0;i<global_i;i++)
		if(global_saved_times[i]>max)
			max=global_saved_times[i];
		
	return max;
}

int find_min(){
	int i,min;
	min=find_max();
	for(i=0;i<global_i;i++)
		if(global_saved_times[i]<min)
			min=global_saved_times[i];
		
	return min;
}

int get_total(){
	int i,sum=0;
	for(i=0;i<global_i;i++)
		sum+=global_saved_times[i];
	return sum;
}
		
void sort_array(){
	int i,j;
	//sorting array
	for(i=global_i;i>0;i--){
		for(j=0;j<i-1;j++){
			if(global_saved_times[j]>global_saved_times[j+1])
				swap(j);
		}
	}
}

void swap(int j){
	int temp=global_saved_times[j];
	global_saved_times[j]=global_saved_times[j+1];
	global_saved_times[j+1]=temp;
}

void finilize(){
    int i,max,min,med,total;
	setvect(8,int8save);
	setvect(9,int9save);
	printf("Timed:\n");
	for(i=0;i<global_i;i++)
		printf("%d\t",global_saved_times[i]);
	
	sort_array();
	max = find_max();
	min = find_min();
	med = global_saved_times[global_i/2];
	total = get_total();
	
	printf("\nSorted:\n");
	for(i=0;i<global_i;i++)
		printf("%d\t",global_saved_times[i]);
	
	printf("\n\nMax_Time=%d/1069 secs\nMin_Time=%d/1069 secs\nMed_Time=%d\nTotal_Time=%d/1069\n",max,min,med,total);
	NoSound();
	asm {
		mov AH,4Ch
		int 21h
	}
}

void ChangeSpeaker( int status ) {
  int portval;
//   portval = inportb( 0x61 );

      portval = 0;
   asm {
        PUSH AX
        MOV AL,61h
        MOV byte ptr portval,AL
        POP AX
       }

    if ( status==ON )
     portval |= 0x03;
      else
       portval &=~ 0x03;
        // outportb( 0x61, portval );
        asm {
		  CLI
          PUSH AX
          MOV AX,portval
          OUT 61h,AL
          POP AX
		  STI
        } // asm

	} /*--ChangeSpeaker( )----------*/

void setup_sound_latch( int hertz ){
 unsigned divisor = 1193180L / hertz;

  ChangeSpeaker( ON );

//        outportb( 0x43, 0xB6 );
	asm {
	  PUSH AX
	  MOV AL,0B6h
	  OUT 43h,AL
	} // asm


 //       outportb( 0x42, divisor & 0xFF ) ;
	asm {
	  MOV AX,divisor
	  AND AX,0FFh
	  OUT 42h,AL
	} // asm


 //        outportb( 0x42, divisor >> 8 ) ;

	asm {
	  MOV AX,divisor
	  MOV AL,AH
	  OUT 42h,AL
	  POP AX
	} // asm

} /*--Sound( )-----*/

void NoSound( void ){
	 ChangeSpeaker( OFF );
} /*--NoSound( )------*/