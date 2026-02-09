#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void setFrequency(unsigned int freq) {

	long kd;
	long base = 1193180;		// внут. частота спикера
	outp(0x43, 0xB6);			//  акал 2, режим 3, вид операции 3 
	kd = base / freq;			// определ€ем коэф. делени€
	outp(0x42, kd % 256);		// пишем в порт динамика младший бит делител€
	outp(0x42, kd / 256);		// старший бит
}

void playMelody(){

	int melody[] = { 392, 329, 329, 392, 329, 329, 392,349 ,329 ,0 };
	int i;

	for (i = 0; melody[i] != 0; i++) {
		setFrequency(melody[i]);
		outp(0x61, inp(0x61) | 3);		// устанавливаем 2 последних бита 61 порта в 11
		delay(400);						// приостанавливаем прогу на N милисек
		outp(0x61, inp(0x61) & 0xFC);	// сброс (операци€ и)
	}

	printf("Press any key to exit:\n");
}

void get_random(int number){

	int i;

	outp(0x43,0xb4);
	outp(0x42, number & 0x00ff);
	outp(0x42, (number & 0xff00) >> 8);
	outp(0x61, inp(0x61) | 1);

	printf("Press any key to generate a number, press 'esc' to exit: \n");

	while (getch() != 27){

		outp(0x43, 0x80);
		i = inp(0x42);
		i = (inp(0x42) << 8) + i;
		printf("%d\n", i);
	}

	outp(0x61, inp(0x61) & 0xFC);
}

void printKd(){

	long i, j, kdLow, kdHigh, kd, kdMax;

	for (i = 0; i < 3; i++){
		kdMax = 0;
		for (j = 0; j < 50000; j++){

			if (i == 0){
				outp(0x43, 0x0);
				kdLow = inp(0x40);
				kdHigh = inp(0x40);
			}

			if (i == 1){
				outp(0x43, 0x40);
				kdLow = inp(0x41);
				kdHigh = inp(0x41);
			}

			if (i == 2){
				outp(0x61, inp(0x61) | 1);
				outp(0x43, 0x80);
				kdLow = inp(0x42);
				kdHigh = inp(0x42);
			}

			kd = kdHigh * 256 + kdLow;

			if (kdMax < kd)
				kdMax = kd;
			outp (0x61, inp(0x61) & 0xfc);
		}
		if (i == 0) printf("KD for 0 channel: ");
		if (i == 1) printf("KD for 1 channel: ");
		if (i == 2) printf("KD for 2 channel: ");
		printf("%X\n", kdMax);
	}
	printf("Press any key to exit: \n");
}

void printWords(){

	int i, iChannel, state;
	char temp;
	for (iChannel = 0; iChannel < 3; iChannel++){

		if (iChannel == 0){
			outp(0x43, 0xe2);//11100010
			state = inp(0x40);
			printf("Word of 0 channel: ");
		}

		if (iChannel == 1){
			outp(0x43, 0xe4);
			state = inp(0x41);
			printf("Word of 1 channel: ");
		}

		if (iChannel == 2){
			outp(0x43, 0xe8);
			state = inp(0x42);
			printf("Word of 2 channel: ");
		}

		for (i = 7; i >= 0; i--){
			temp = state % 2;
			printf("%d", temp);
			state /= 2;
		}
		printf("\n");
	}
	printf("Press any key to exit:\n");
}

void Menu(){
	int i;
	long randomNumber;
	char control;

	while (1){

		system("cls");
		printf("Enter command:\n");
		printf("1: Play Music\n");
		printf("2: Print division factor\n");
		printf("3: Print condition words\n");
		printf("4: Generate random numbers\n");
		printf("5: Exit\n");

		control = getch();
		system("cls");

		switch (control){

		case '1':
			printf("1: Play Music\n");
			playMelody();
			getch();
			break;

		case '2':
			printf("2: Print division factor\n");
			printKd();
			getch();
			break;

		case '3':
			printf("3: Print condition words\n");
			printWords();
			getch();
			break;

		case '4':
			printf("4: Generate random numbers\n");
			printf("Enter the above range of random: ");
			scanf("%ld", &randomNumber);
			get_random(randomNumber);
			printf("\n");
			break;

		case '5':
			return ;

		default:
			printf("Error key! Please try again\n");
			printf("Press any key to continue: ");
			getch();
			break;
		}
	}
}

int main(){

	Menu();
	return 0;
}