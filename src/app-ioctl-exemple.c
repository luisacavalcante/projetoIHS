#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc, atoi, rand... */
#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */
#include <omp.h>
#include <SDL2/SDL_mixer.h>

#define NotesNUM    4
#define DISPLAY_L   1
#define DISPLAY_R   2
#define SWITCHES    3
#define PUSHBUTTON  4
#define GREENLEDS   5
#define REDLEDS     6

#include "ioctl_cmds.h"

void LoadPiano(Mix_Chunk **Notes);
void LoadPianoStretched(Mix_Chunk **Notes);

int main(int argc, char** argv)
{
	int fd, len, retval;
	int cnt = 0;
	char buf[255];
	char cmd;

	// const data to print out in the d7 display
    const uint32_t mem_trash = 0, led_1 = 3 , led_2 = 12, led_3 = 48, led_4 = 192;;
    const uint32_t hex_b = 0xFFFFFF03, hex_e = 0xFFFFFF06, hex_d = 0xFFFFFF21, hex_empty = 0xFFFFFF7F;
    const uint32_t hex_1 = 0xFFFFFF79, hex_2 = 0xFFFFFF24, hex_3 = 0xFFFFFF30, hex_4 = 0xFFFFFF19;
    const uint32_t hex_c = 0xFFFFFF46, hex_a = 0xFFFFFF08, hex_g = 0xFFFFFF42, hex_f = 0xFFFFFF0E;

	// Quando apertar a nota, mostrar no display a nota em inglês


    	Mix_Init(MIX_INIT_MID);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk *Notes[NotesNUM];
	Mix_Chunk *NotesStretched[NotesNUM];

	LoadPiano(Notes);
	LoadPianoStretched(NotesStretched);

	//while(1) {
		//Mix_PlayChannel(2, Notes[3], 0);
	//}

	if (argc < 2) {
		printf("Syntax: %s <device file path>\n", argv[0]);
		return -EINVAL;
	}

	if ((fd = open(argv[1], O_RDWR)) < 0) {
		fprintf(stderr, "Error opening file %s\n", argv[1]);
		return -EBUSY;
	}
	unsigned int data_display_esquerda = 0xFFFFFFFF;
	    ioctl(fd, WR_L_DISPLAY);
	    retval = write(fd, &data_display_esquerda, sizeof(data_display_esquerda));

	unsigned int data_display_direita = 0xFFFFFFFF;
	    ioctl(fd, WR_R_DISPLAY);
	    retval = write(fd, &data_display_direita, sizeof(data_display_direita));

	while(1) {
        //ligar todos os leds verdes
        unsigned int data_green_led = 0;
	    ioctl(fd, WR_GREEN_LEDS);
	    write(fd, &data_green_led, sizeof(data_green_led));
	    
	    unsigned int data_red_led = 131072;
	    ioctl(fd, WR_RED_LEDS);
	    write(fd, &data_red_led, sizeof(data_red_led));
	    
	    unsigned int data_display_esquerda = 0xFFFFFFFF;
	    ioctl(fd, WR_L_DISPLAY);
	    retval = write(fd, &data_display_esquerda, sizeof(data_display_esquerda));

        //ler do switch
        unsigned int data_switch = 0;
		ioctl(fd, RD_SWITCHES);
	    read(fd, &data_switch, 1);
		if(data_switch > 0){
            unsigned int data_push = 0;
            ioctl(fd, RD_PBUTTONS);
	        read(fd, &data_push, 1);
	        unsigned int data_red_led = 131073;
	   		ioctl(fd, WR_RED_LEDS);
	    	write(fd, &data_red_led, sizeof(data_red_led));
	        
			// Notas normais
			if(data_push == 14){
                    //14 é o fá
				printf("botao 4\n");
				Mix_PlayChannel(2, Notes[3], 0);
               unsigned int data_display_direita = 0xFFFFFF8E;
	                ioctl(fd, WR_R_DISPLAY);
	                retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	                unsigned int data_green_led = 3;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar F no D7

			}
			if(data_push == 13){
                    //13 é o mi
				printf("botao 3\n");
				Mix_PlayChannel(2, Notes[2], 0);
                unsigned int data_display_direita = 0xFFFFFF86;
	                ioctl(fd, WR_R_DISPLAY);
	                retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	                unsigned int data_green_led = 12;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));

					// Printar E no D7
			}
		if(data_push == 11){
                    //11 é o ré
				printf("botao 2\n");
				Mix_PlayChannel(2, Notes[1], 0);
                unsigned int data_display_direita = 0xFFFFFFA1;
	            ioctl(fd, WR_R_DISPLAY);
	            retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	            unsigned int data_green_led = 48;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar D no D7
			}
			if(data_push == 7){
                    //7 é o dó
				printf("botao 1\n");
				Mix_PlayChannel(2, Notes[0], 0);
                unsigned int data_display_direita = 0xFFFFFFC6;
	            ioctl(fd, WR_R_DISPLAY);
	            retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	            unsigned int data_green_led = 192;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar C no D7
			}
			
			
		} else {
            unsigned int data_push = 0;
            ioctl(fd, RD_PBUTTONS);
	        read(fd, &data_push, 1);
	        
			// red_something = 0;

			// Notas alongadas
			
				if(data_push == 14){
					Mix_PlayChannel(2, NotesStretched[3], 0);
                    unsigned int data_display_direita = 0xFFFFFF8E;
	                ioctl(fd, WR_R_DISPLAY);
	                retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	                unsigned int data_green_led = 3;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar F no D7
				}
				if(data_push == 13){
					Mix_PlayChannel(2, NotesStretched[2], 0);
                    unsigned int data_display_direita = 0xFFFFFF86;
	                ioctl(fd, WR_R_DISPLAY);
	                retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	                unsigned int data_green_led = 12;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar E no D7
				}
				if(data_push == 11){
					Mix_PlayChannel(2, NotesStretched[1], 0);
                    unsigned int data_display_direita = 0xFFFFFFA1;
	                ioctl(fd, WR_R_DISPLAY);
	                retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	                unsigned int data_green_led = 48;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar D no D7
				}
				if(data_push == 7){
					Mix_PlayChannel(2, NotesStretched[0], 0);
                    unsigned int data_display_direita = 0xFFFFFFC6;
	                ioctl(fd, WR_R_DISPLAY);
	                retval = write(fd, &data_display_direita, sizeof(data_display_direita));
	                unsigned int data_green_led = 192;
	    			ioctl(fd, WR_GREEN_LEDS);
	    			write(fd, &data_green_led, sizeof(data_green_led));
					// Printar C no D7
				}
			}
			if ((cnt % 2) == 0) {
				data_red_led ^= 131072;
				ioctl(fd, WR_RED_LEDS);
				write(fd, data_red_led, sizeof(data_red_led));
			}
			cnt++;
        	sleep(1);
	}

	close(fd);

	return 0;
}

void LoadPiano(Mix_Chunk **Notes) {
	Notes[0] = Mix_LoadWAV("Samples/do.wav");
    Notes[1] = Mix_LoadWAV("Samples/re.wav");
    Notes[2] = Mix_LoadWAV("Samples/mi.wav");
    Notes[3] = Mix_LoadWAV("Samples/fa.wav");
}

void LoadPianoStretched(Mix_Chunk **Notes) {
	Notes[0] = Mix_LoadWAV("Samples/do-stretched.wav");
    Notes[1] = Mix_LoadWAV("Samples/re-stretched.wav");
    Notes[2] = Mix_LoadWAV("Samples/mi-stretched.wav");
    Notes[3] = Mix_LoadWAV("Samples/fa-stretched.wav");
}
