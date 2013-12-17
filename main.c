#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

typedef enum{
    false=0,
    true=1
}bool;

void anti_espia(int board[10][10][2],int turno);
bool avion_espia(int board[10][10][2]);
void broken(int board[10][10][2],int flota[5][5]);
bool check_and_set(int w,char pos,int flota[5][5],int board[10][10][2]);
void erase();
void instructions(void);
bool misil_balistico(int board[10][10][2],int flota[5][5]);
bool misil_espia(int board[10][10][2]);
bool move_sub(char sub_id, char move,int flota[5][5],int board[10][10][2]);
int next_shot(int sumai, int sumaj, int a, int b);
char pcai(int espias_cont[2][2]);
int pcai_avion_espia(int a);

void reset(void);
void set_fleet_pl(int board[10][10][2],int flota[5][5]);
void set_fleet_pc(int board[10][10][2],int flota[5][5]);
void show_stats(int flota_pl[5][5],int flota_pc[5][5]);
void show_update(bool cheat);
void show_empty(void);
void transpuesta1(int matriz[7][2]);
void transpuesta2(int matriz[7][2]);
void transpuesta3(int matriz[7][2]);

/* estas matrices representan los tableros del jugador y la pc 
board[][][0] <- superficie
board[][][1] <- parte submarina
lo que reprece

 valor |  representa
-------|------------
	0  |	agua
	1  |	barco
	2  |	submarino verde
	3  |	submarino azul
       |	
	10 |	fuego a agua
	11 |	fuego a barco
	12 |	fuego a submarino verde
	13 |	fuego a submarino azul
       |	
	20 |	agua espiada
	21 |	barco espiado
	22 |	submarino verde espiado
	23 |	submarino azul espiado
       |	
	30 |	barco/submarino destruido
*/

int board_pc[10][10][2]={0};
int board_pl[10][10][2]={0};

bool ban1,ban2,espia,a_espia[2];
bool turno;
bool cheat=0;

int i,j;
int k=0;
int x=3;
int y=3;
int matriz_espia_1[7][2],matriz_espia_2[7][2];
int espias_cont[2][2];

int main(){
	srand(time(NULL));
	int flota_pc[5][5]={0};
	int flota_pl[5][5]={0};
	char move;
	char cTecla = ' ';
	char sub_id = ' ';
	a_espia[0]=false;
	a_espia[1]=false;
	int victoria = 0;
	bool z=false;
	bool w=false;

	printf("                                BATALLA NAVAL ++");
    printf("\nREGLAS DEL JUEGO\n");
    printf("\n\tBatalla Naval++  es la secuela de  un famoso  juego de tablero para");
    printf("\n\tdos jugadores que se juega por turnos.\n");
    printf("\n\tA cada  jugador le  corresponde  un  tablero,  donde coloca toda su");
    printf("\n\tflota de barcos y submarinos en una disposicion que el otro jugador");
    printf("\n\tno puede ver. Cada uno debe destruir la flota enemiga adivinando la");
    printf("\n\tubicacion de los navios enemigos.  Para ganar el juego es necesario");
    printf("\n\tdestruir completamente la flota enemiga.\n\n");
	printf("\n\tA diferencia de Batalla Naval original, esta version se  desarrolla");
    printf("\n\ten dos tableros por  jugador,  uno representa la superficie del mar");
    printf("\n\ty el otro las profundidades. Cada jugador cuenta ademas  con  armas");
    printf("\n\ty habilidades especiales, con efectos diversos.\n\n");    
	printf("\nPresione una tecla para continuar...");
    getch();
	clrscr();
	printf("\n\n\n\n\n\n\tSaltar Instrucciones?? \n\n\tpuede ver las instrucciones oprimiendo \"i\" en su turno \n\tdurante el juego\n\n\tS/N ");
	
	do{	
		cTecla = getch();
		if(cTecla == 's')cTecla=27;
		if(cTecla == 'n'){instructions();cTecla=27;}
	}while(cTecla != 27);
	gotoxy(1,10);
	clrscr();
	printf("\n\n\n\n\n\n\n\tActivar el modo de prueba?? \n\t(usted sera capaz de ver toda la\n\tinformacion del enemigo siempre) \n\tS/N ");
	
	do{	
		cTecla = getch();
		if(cTecla == 's'){cheat=1;cTecla=27;}
		if(cTecla == 'n')cTecla=27;
	}while(cTecla != 27);

    /*las matrices flota_pc y flota_pl, contienen información acerca de cada uno de los 10 barcos del juego: la posicion de sus puntas, y su dimensión
    que se ira descontando a medida de que reciba daño*/
    /*se inicializan las dimensiones*/
	/*estas matrices se utilizan para tener conocimiento de la ubicación de los barcos/submarinos en el tablero
	El primer indice indica        El segundo indice indica
	0 -> i inicial                 0 -> Porta Aviones
	1 -> j inicial                 1 -> Acogtrazado
	2 -> i final                   2 -> Crucero
	3 -> j final                   3 -> Submarino verde
	4 -> dimension del barco       4 -> Submarino Azul*/
    flota_pc[4][0]=5;
    flota_pc[4][1]=4;
    flota_pc[4][2]=3;
    flota_pc[4][3]=3;
    flota_pc[4][4]=3;

    flota_pl[4][0]=5;
    flota_pl[4][1]=4;
    flota_pl[4][2]=3;
    flota_pl[4][3]=3;
    flota_pl[4][4]=3;	
	
	espias_cont[0][0]=2;
	espias_cont[0][1]=3;
	espias_cont[1][0]=2;
	espias_cont[1][1]=3;
	
	/*coordenadas iniciales de las matrices de espia*/
	/*posiciones iniciales de matriz_espia_1 estan dispuestas de forma que el espionaje sea el siguiente
	 
	|_|_|b|_|_|_|_|X|X|X|
	|a|_|_|B|_|_|_|X|E|X|
	|_|A|_|_|X|_|_|X|X|X|
	|_|_|X|_|_|X|_|_|_|_|
	|_|_|_|X|_|_|X|_|_|_|
	|X|X|X|_|X|_|_|X|_|_|
	|X|E|X|_|_|X|_|_|X|_|
	|X|X|X|X|X|_|X|_|_|X|
	|_|_|X|E|X|_|_|X|_|_|
	|_|_|X|X|X|_|_|_|X|_|

	posiciones iniciales de matriz_espia_2 estan dispuestas de forma que el espionaje sea el siguiente
	|a|_|_|_|X|X|X|_|_|b|
	|_|A|_|_|X|E|X|_|B|_|
	|_|_|X|_|X|X|X|X|_|_|
	|X|X|X|X|_|_|X|_|_|_|
	|X|E|X|_|X|X|_|X|X|X|
	|X|X|X|_|X|X|_|X|E|X|
	|_|_|_|X|_|_|X|X|X|X|
	|_|_|X|_|_|_|_|X|_|_|
	|_|X|_|_|_|_|_|_|X|_|
	|X|_|_|_|_|_|_|_|_|X|
	
	Referencias:
	E -> posicion donde se dipara el misil espia
	a -> posicion inicial avion espia 1
	A -> posicion final avion espia 1
	b -> posicion inicial avion espia 2
	B -> posicion final avion espia 2*/
	matriz_espia_1[0][0]=1;
	matriz_espia_1[0][1]=8;
	matriz_espia_1[1][0]=6;
	matriz_espia_1[1][1]=1;
	matriz_espia_1[2][0]=8;
	matriz_espia_1[2][1]=3;
	matriz_espia_1[3][0]=1;
	matriz_espia_1[3][1]=0;
	matriz_espia_1[4][0]=0;
	matriz_espia_1[4][1]=2;
	matriz_espia_1[5][0]=2;
	matriz_espia_1[5][1]=1;
	matriz_espia_1[6][0]=1;
	matriz_espia_1[6][1]=3;

	matriz_espia_2[0][0]=4;
	matriz_espia_2[0][1]=1;
	matriz_espia_2[1][0]=1;
	matriz_espia_2[1][1]=5;
	matriz_espia_2[2][0]=5;
	matriz_espia_2[2][1]=8;
	matriz_espia_2[3][0]=0;
	matriz_espia_2[3][1]=0;
	matriz_espia_2[4][0]=0;
	matriz_espia_2[4][1]=9;
	matriz_espia_2[5][0]=1;
	matriz_espia_2[5][1]=1;
	matriz_espia_2[6][0]=1;
	matriz_espia_2[6][1]=8;

	/*se elige una distribucion entre las matrices espia y se randomiza una traspuesta*/
	if((rand()%99999)%2){
		switch(rand()%4){
			case 0:transpuesta1(matriz_espia_1);espia=true;break;
			case 1:transpuesta2(matriz_espia_1);espia=true;break;
			case 2:transpuesta3(matriz_espia_1);espia=true;break;
			default:espia=true;break;
		}
	}else{
		switch(rand()%4){
			case 0:transpuesta1(matriz_espia_2);espia=false;break;
			case 1:transpuesta2(matriz_espia_2);espia=false;break;
			case 2:transpuesta3(matriz_espia_2);espia=false;break;
			default:espia=false;break;
		}
	}
    
    show_empty();/*muestra el tablero vacio*/
	
	gotoxy(55, 7);printf("Para ubicar los barcos");
	gotoxy(55, 8);printf("oprima\"v\"(vertrical o");
	gotoxy(55, 9);printf("\"h\"(horizontal) ");
	gotoxy(55,10);printf("luego oprimir \"s\" para");
	gotoxy(55,11);printf("ubicarlo");
	
	set_fleet_pl(board_pl,flota_pl);/*se ubican los barcos del jugador*/
	
	gotoxy(1,25);clreol();
	
	set_fleet_pc(board_pc,flota_pc);/*se ubican los barcos de la pc*/
	
	show_update(cheat);
	reset();
    
	/*se randomiza el turno*/
	turno = (rand()%99999)%2;
	gotoxy(52,10);
	if(turno){
		printf("Felicidades, el primer");gotoxy(52,11);printf("turno es suyo!");
		gotoxy(x,y);
	}else{
		printf("El primer turno es de ");gotoxy(52,11);printf("la computadora");
		_sleep(1);
	}
	cTecla=' ';
	
	/* muchas de las funciones utilizadas en este while devuelven true o false 
	segun se haya o no realizado una accion que requiera cambio de turno*/
	
	while( !victoria) 
    {
		if(turno){
			reset();
			cTecla = getch(); // sin eco
			if( cTecla == 'b'){//utilizar misil balistico
				if(x<22){//si se esta disparando dentro del tablero enemigo
					turno=(turno + misil_balistico(board_pc,flota_pc))%2;/*si la funcion devuelve true, se cambia el turno*/
					if(!turno)//si se cambio de turno
						anti_espia(board_pc,1);
					erase();
					broken(board_pc,flota_pc);/*se calcula el daño realizado*/
					show_update(cheat);
					show_stats(flota_pl,flota_pc);
					
				}
			}
			if( cTecla == 'e' && espias_cont[1][1]){//utilizar misil espia
				if(x<22){//si se esta disparando dentro del tablero enemigo
					anti_espia(board_pc,1);
					turno=(turno + misil_espia(board_pc))%2;/*si la funcion devuelve true, se cambia el turno*/
					a_espia[1]=true;
					espias_cont[1][1]--;
					show_update(cheat);
					show_stats(flota_pl,flota_pc);
				}
			}
			if( cTecla == 'a' && espias_cont[1][0]){//utilizar avion espia
				if(x<22){//si se esta disparando dentro del tablero enemigo
					erase();
					gotoxy(55, 1);printf("     Avion Espia");
					gotoxy(55, 5);printf("Lleve al cursor hasta");
					gotoxy(55, 6);printf(" la posicion inicial ");
					gotoxy(55, 7);printf(" y oprima \"s\" para ");
				    gotoxy(55, 8);printf("setearla luego lleve ");
					gotoxy(55, 9);printf("el cursor hasta la   ");
					gotoxy(55,10);printf("posicion final y vuelva");
					gotoxy(55,11);printf("a oprimir \"s\"");
					turno=(turno + avion_espia(board_pc))%2;/*si la funcion devuelve true, se cambia el turno*/
					a_espia[1]=true;
					espias_cont[1][0]--;
					show_update(cheat);
					show_stats(flota_pl,flota_pc);
				}
			}
			if( cTecla == 'm'){//mover el submarino
				if(x>27 && y>12 && board_pl[i][j][k]%10 ){//si se esta en el propio tablero en la parte submarina
					switch(board_pl[i][j][k]%10){
						case 2:{/*es el submarino verde*/
							gotoxy(55, 1);printf("    Mover Submarino");
							gotoxy(55, 5);printf("use el teclado numerico");
							gotoxy(55, 6);printf("para indicar al submarino");
							gotoxy(55, 7);printf("como moverse");
							gotoxy(x,y);
							move = getch();
							sub_id = 'v';
							turno=(turno + move_sub(sub_id,move,flota_pl,board_pl))%2;/*si la funcion devuelve true, se cambia el turno*/
							anti_espia(board_pc,1);
							break;
						}
						case 3:{/*es el submarino verde*/
							gotoxy(55, 1);printf("    Mover Submarino");
							gotoxy(55, 5);printf("use el teclado numerico");
							gotoxy(55, 6);printf("para indicar al submarino");
							gotoxy(55, 7);printf("como moverse");
							gotoxy(x,y);
							move = getch();
							sub_id = 'a';
							gotoxy(x,y);
							turno=(turno + move_sub(sub_id,move,flota_pl,board_pl))%2;/*si la funcion devuelve true, se cambia el turno*/
							anti_espia(board_pc,1);
							break;
						}
						default:break;
					   
					}
					show_update(cheat);
					show_stats(flota_pl,flota_pc);
				}
			}
			if( cTecla == 'i'){//mostrar las instrucciones
				instructions();
				show_empty();
				show_update(cheat);
				show_stats(flota_pl,flota_pc);
				gotoxy(x,y);
			}
			if( cTecla == 0  ) cTecla = getch(); // Las compuestas
			if( cTecla == 72 ){ //moverse hacia arriba
				if(y==15){
					y-=3;
					i=9;
					k=0;
				}else{
					if(y!=3){
						y--;
						i--;
					}
				}
				gotoxy(x,y);
			}
			if( cTecla == 75 ){ //moverse a la izquierda
				if(x==28){
					x-=7;
					j=9;
				}else{
					if(x>3){
						x-=2;
						j--;
					}
				}
				   
				gotoxy(x,y);
			}
			if( cTecla == 77 ){ //moverse a la derecha
				if(x==21){
					x+=7;
					j=0;
				}else{
					if(x<46){
						x+=2;
						j++;
					}
				}
				gotoxy(x,y);
			}
			if( cTecla == 80 ){ //moverse abajo
				if(y==12){
					y+=3;
					k=1;
					i=0;
				}else{
					if(y!=24){
						y++;
						i++;
					}
				}      
				gotoxy(x,y);
			}
		}else{
			cTecla = pcai(espias_cont);
			if( cTecla == 'b'){//utilizar misil balistico
				turno=(turno + misil_balistico(board_pl,flota_pl))%2;/*si la funcion devuelve true, se cambia el turno*/
				if(turno)
					anti_espia(board_pl,0);
				erase();
				broken(board_pl,flota_pl);/*se calcula el daño realizado*/
				show_update(cheat);
				show_stats(flota_pl,flota_pc);
			}
			if( cTecla == 'e' && espias_cont[0][1]){//utilizar misil espia
				anti_espia(board_pl,0);
				turno=(turno + misil_espia(board_pl))%2;/*si la funcion devuelve true, se cambia el turno*/
				a_espia[0]=true;
				espias_cont[0][1]--;
				show_update(cheat);
				show_stats(flota_pl,flota_pc);
			}
			if( cTecla == 'a' && espias_cont[0][0]){//utilizar avion espia
				turno=(turno + avion_espia(board_pl))%2;/*si la funcion devuelve true, se cambia el turno*/
				a_espia[0]=true;
				espias_cont[0][0]--;
				show_update(cheat);
				show_stats(flota_pl,flota_pc);
			}
			
		}
		
		/*se revisa si alguien gano*/
		if(flota_pl[4][0]<0 && flota_pl[4][1]<0 && flota_pl[4][2]<0 && flota_pl[4][3]<0 && flota_pl[4][4]<0)//si todos los barcos del jugador estan destruidos
			victoria = 2;
		if(flota_pc[4][0]<0 && flota_pc[4][1]<0 && flota_pc[4][2]<0 && flota_pc[4][3]<0 && flota_pc[4][4]<0)//si todos los barcos del jugador estan destruidos
			victoria = 1;
			
	}	
	clrscr();	
	if(victoria==2){
		gotoxy(10,14);printf("\tHA GANADO LA COMPUTADORA\n");
	}else{
		gotoxy(10,14);printf("\tFELICIDADES HA GANADO!!!\n");	
	}
 	printf("\n\n\n\n\tPresione cualquier tecla para terminar el programa...");
	getch();
	
	gotoxy(1,25);
	return 0;
}

/* elimina todo lo espiado en el tablero que se le pasa como parametro */
void anti_espia(int board[10][10][2],int turno){
	int i,j,k;
	if(a_espia[turno]){
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				for(k=0;k<2;k++){
					if(board[i][j][k]>=20 && board[i][j][k]!=30){
						board[i][j][k]-=20;
					}
				}
			}
		}
		a_espia[turno]=false;
	}
}

bool avion_espia(int board[10][10][2]){/*funcion que representa a la herramienta Avion Espia*/
	int ini[2]={0},fin[2]={0};//posiciones inicial y final a espiar
	int ban=0;//bandera que indica que posicion se esta seteando
	int f,c;
	char cTecla;
	gotoxy(x,y);
	cTecla = ' ';
	while(ban>=0){
		if(!turno){/*se obtienen las posiciones de espionaje de la comptadora*/
			ini[0]=i;
			ini[1]=j;
			fin[0]=pcai_avion_espia(0);
			fin[1]=pcai_avion_espia(1);
			anti_espia(board_pl,0);
			cTecla=27;
		}
		/*se obtienen las posiciones de espionaje del jugador*/
		while(cTecla != 27 && ban<2){ //  cTecla != ESC y si se siguen seteando las posicoines
	        
			cTecla = getch(); // sin eco
			if( cTecla == 27)
				return false;
			
			if( cTecla == 's'){//setear posicion
				switch(ban){
					case 0:{//si es la posicion inicial
						ini[0]=i;
						ini[1]=j;
						ban++;
						break;
					}
					case 1:{//si es la posicion final
						fin[0]=i;
						fin[1]=j;
						/*si(la posicion final se encuentra en una fila, columna o diagonal con respecto a la inicial setada)*/
						if( !(ini[0]==fin[0] && ini[1]==fin[1]) && (ini[0]==fin[0] || ini[1]==fin[1] || ini[0]-ini[1]==fin[0]-fin[1] || ini[0]+ini[1]==fin[0]+fin[1])){
							ban++;
							cTecla == 27;
							anti_espia(board_pc,1);
						}else{/*si la posicion final seteada no es posible, se vuelve a leer la posicion final, hasta que coincidan*/
							erase();
							gotoxy(55,1);printf("         Avion Espia");
							gotoxy(55,5);printf("Debe Insertar por lo");
							gotoxy(55,6);printf("menos dos coordenadas");
							gotoxy(55,7);printf("en fila, columna");
							gotoxy(55,8);printf("o diagonal");
						}
						break;
					}
				}
			}
		 
			if( cTecla == 0  ) cTecla = getch(); // Las compuestas
			if( cTecla == 72 ){ //moverse hacia arriba
				if(y==15){
					y-=3;
					i=9;
					k=0;
				}else{
					if(y!=3){
						y--;
						i--;
					}
				}
				gotoxy(x,y);
			}
			if( cTecla == 75 ){ //moverse a la izquierda
				if(x==28){
					x-=7;
					j=9;
				}else{
					if(x>3){
						x-=2;
						j--;
					}
				}
					
				gotoxy(x,y);
			}
			if( cTecla == 77 ){ //moverse a la derecha
				if(x==21){
					x+=7;
					j=0;
				}else{
					if(x<46){
						x+=2;
						j++;
					}
				}
				gotoxy(x,y);
			}
			if( cTecla == 80 ){ //moverse abajo
				if(y==12){
					y+=3;
					k=1;
					i=0;
				}else{
					if(y!=24){
						y++;
						i++;
					}
				}       
				gotoxy(x,y);
			}
			
	    }
		/*if(las posiciones estan en una diagonal o en una misma fila o en una misma columna)*/
		
		
		if(ini[0]==fin[0]){//si las posiciones estan en una linea horizontal
			for(c=0;c<10;c++){//recorre la fila seteada
				if(board[ini[0]][c][0]<10)board[ini[0]][c][0]+=20;
				if(board[ini[0]][c][1]<10)board[ini[0]][c][1]+=20;
			}
		}
		if(ini[1]==fin[1]){//si las posiciones estan en una linea verticales
			for(c=0;c<10;c++){//recorre la columna seteada
				if(board[c][ini[1]][0]<10)board[c][ini[1]][0]+=20;
				if(board[c][ini[1]][1]<10)board[c][ini[1]][1]+=20;
			}
		}
		if(ini[0]-ini[1]==fin[0]-fin[1]){//si las posiciones estan en una diagonal paralela a la diagonal principal
			if(ini[0]<=ini[1]){
				for(f=0,c=ini[1]-ini[0];c<10;c++,f++){//recorre la diagonal seleccionada
					if(board[f][c][0]<10)board[f][c][0]+=20;
					if(board[f][c][1]<10)board[f][c][1]+=20;
				}
			}else{
				for(f=ini[0]-ini[1],c=0;f<10;c++,f++){//recorre la diagonal seleccionada
					if(board[f][c][0]<10)board[f][c][0]+=20;
					if(board[f][c][1]<10)board[f][c][1]+=20;
				}
			}
		}
		
		if(ini[0]+ini[1]==fin[0]+fin[1]){//si las posiciones estan en una diagonal paralela a la diagonal secundaria
			if((ini[0]+ini[1])<10){
				for(f=0,c=ini[0]+ini[1];c>=0;c--,f++){//recorre la diagonal seleccionada
					if(board[f][c][0]<10)board[f][c][0]+=20;
					if(board[f][c][1]<10)board[f][c][1]+=20;
				}
			}else{
				for(f=ini[0]-(9-ini[1]),c=9;f<10;c--,f++){//recorre la diagonal seleccionada
					if(board[f][c][0]<10)board[f][c][0]+=20;
					if(board[f][c][1]<10)board[f][c][1]+=20;
				}
			}
		}
		ban=-1;
	}
	return true;
	
}

void broken(int board[10][10][2],int flota[5][5]){/*calcula el daño realizado a cada flota, y muestra la información en pantalla en caso de que algún barco sea destruido*/
    int a,b,dmg;
	   
 	for(a=0;a<5;a++){
		dmg=0;
		if(flota[0][a] == flota[2][a]){//si el barco esta horizontal
			for(b=flota[1][a];b<=flota[3][a];b++){//se recorre cada barco
				if(board[(flota[0][a])][b][0]>=11 && board[(flota[0][a])][b][0]<20){/*por cada parte dañada se incrementa la cantidad de daño actualdel barco*/
					dmg++;
					flota[4][a]=(flota[3][a] - flota[1][a])-dmg;//se asigna la dimension menos el daño actual realizado al barco
				}
			}
			if(flota[4][a]<0){
				for(b=flota[1][a];b<=flota[3][a];b++){
					board[(flota[0][a])][b][0]=30;
				}		
			}
		}else{//el barco esta vertical
			for(b=flota[0][a];b<=flota[2][a];b++){//se recorre cada barco
				if(board[b][(flota[1][a])][0]>=11 && board[b][(flota[1][a])][0]<20){/*por cada parte dañada se incrementa la cantidad de daño actualdel barco*/
					dmg++;
					flota[4][a]=(flota[2][a] - flota[0][a])-dmg;//se asigna la dimension menos el daño actual realizado al barco
				}
			}
			if(flota[4][a]<0){//si el barco esta totalmente dañado se le asigna el codigo correspondiente
				for(b=flota[0][a];b<=flota[2][a];b++){
					board[b][(flota[1][a])][0]=30;
				}		
			}
		}
	}
}
/* w = identificador de barco ,  pos = indica si se eligio vertical u horizontal
	"i" , "j" y "k" son globales y contienen las coordenadas que se eligieron para ubicar el barco*/
bool check_and_set(int w,char pos,int flota[5][5],int board[10][10][2]){
	/*if(  ( se_eligio_horizontal && no_se_sale_del_tablero) || ( se_eligio_vertical && no_se_sale_del_tablero  )*/
    if(  ( pos=='h' && (flota[4][w] + j)<=10 ) || ( pos=='v' && (flota[4][w] + i)<=10 )  ){
		int m;
		/*if(se_eligio_horizontal)*/
		if(pos=='h'){
			/*verifica que no haya ningún otro barco en las casillas que ocuparía el barco que se intenta ubicar*/
			for (m=0;m<flota[4][w];m++){
				if( (board[i][m+j][0]>=1) || (board[i][m+j][1]>=1) )
					return false;
			}
			/*se asignan a las casillas correspondientes los valores que indican que hay un barco o submarino*/
			for (m=0;m<flota[4][w];m++){
				if(w>2){
                    if(w==3 )
                        board[i][m+j][k]=2;
                    
                    if(w==4 )
                        board[i][m+j][k]=3;
                }else{
                    board[i][m+j][0]=1;
                }
			}
			/*a flota se le asignan las coordenadas del extremo final del barco/sumarino */
			flota[2][w]=i;
			flota[3][w]=j+flota[4][w]-1;
		}else{
			/*verifica que no haya ningún otro barco en las casillas que ocuparía el barco que se intenta ubicar*/
			for (m=0;m<flota[4][w];m++){
				if( (board[m+i][j][0]>=1) || (board[m+i][j][1]>=1) )
					return false;
			}
			/*se asignan a las casillas correspondientes los valores que indican que hay un barco o submarino*/
			for (m=0;m<flota[4][w];m++){
				if(w>2){
                    if(w==3)
                        board[m+i][j][k]=2;
                    
                    if(w==4)
                        board[m+i][j][k]=3;
                }else{
                    board[m+i][j][0]=1;
                }
			}
			/*a flota se le asignan las coordenadas del extremo final del barco/sumarino */
			flota[2][w]=i+flota[4][w]-1;
			flota[3][w]=j;
		}
		/*a flota se le asignan las coordenadas del extremo inicial del barco/sumarino*/
		flota[0][w]=i;
		flota[1][w]=j;
		return true;
	}else{
		return false;
	}
}

void erase(void){/*limpia la zona de informes/estadisticas de la interfaz*/
	int a;
	for(a=1;a<=23;a++){
		gotoxy(52,a);
		clreol();
	}
}

void instructions(void){/*muestra al usuario en la pantalla las intrucciones del juego*/
    clrscr();
	printf("\t      Instrucciones\n");
	printf("\n\n\tCOLOCAR LAS EMBARCACIONES");
	printf("\n\n\t El juego comienza cuando el usuario coloca sus barcos.");
	printf("\n\tLa manera de colocarlos tiene dos partes, elegir la orientaci%cn.",162);
	printf("\n\tdel barco (horizontal o vertical) mediante las teclas \"h\" y \"v\"");
	printf("\n\n\tLuego debe dirigirse a la posici%cn inicial de dicho barco ",162);
	printf("\n\tutilizando las flechas de direcci%cn del teclado.",162);
	printf("\n\tLa posici%cn inicial en el caso vertical ubicara el barco desde",162);
	printf("\n\tla posici%cn del cursor hacia abajo.",162);
	printf("\n\tLa posici%cn inicial en el caso horizontal lo har%c hacia la",162,160);
	printf("\n\tderecha.");
	printf("\n\n\n\n\tPresione una tecla para continuar...");
	getch();
	clrscr();
	printf("\t      Instrucciones\n");
	printf("\n\n\tMISIL BALISTICO");
	printf("\n\n\tEs la unica herramienta en el juego capaz de da%car un barco",164);
	printf("\n\n\tPara disparar un misil bal%cstico debe encontrarse en el tablero",161);
	printf("\n\tenemigo y oprimir la tecla \"b\".");
	printf("\n\n\tEl resultado ser%c que esa ubicaci%cn ser%c da%cada. Revelando la misma",160,162,160,164,162);
	printf("\n\ten pantalla");
	printf("\n\n\tSi intenta disparar en su tablero o en un lugar en el cual ya haya");
	printf("\n\tdisparado, el efecto ser%c nulo y su turno volver%c a empezar",160,160);
	printf("\n\n\tSi usted o la computadora aciertan a un barco con un disparo");
	printf("\n\tel siguiente turno vuelve a ser de quien acert%c el disparo",162);
	printf("\n\n\n\n\tPresione una tecla para continuar...");
	getch();
	clrscr();
	printf("\t      Instrucciones\n");
	printf("\n\n\tMISIL ESPIA");
	printf("\n\n\tEs una herramienta para conseguir informaci%cn del enemigo",162);
	printf("\n\n\tSe utiliza ubic%cndose en el tablero enemigo y oprimiendo \"e\"",160);
	printf("\n\tSe revelaran en pantalla la ubicaci%cn del cursor y las 8 ",162);
	printf("\n\tadyacentes durante el turno actual y el siguiente.");
	printf("\n\n\tSi intenta espiar en su propio tablero, el efecto");
	printf("\n\tno se cumple y su turno vuelve a empezar");
	printf("\n\n\tTanto usted como la computadora cuentan con 3 ");
	printf("\n\tmisiles esp%ca",161);
	printf("\n\n\tEL USO DE ESTA HERRAMIENTA SE CONSIDERA UN TURNO");
	printf("\n\n\n\n\tPresione una tecla para continuar...");
	getch();
	clrscr();
	printf("\t      Instrucciones\n");
	printf("\n\n\tAVION ESPIA");
	printf("\n\n\tEs una herramienta para conseguir informaci%cn del enemigo",162);
	printf("\n\n\tSe utiliza ubic%cndose en el tablero enemigo y oprimiendo \"a\".",160);
	printf("\n\tLuego debe elegir dos posiciones del tablero enemigo, que deben estar");
	printf("\n\ten una misma fila, columna o diagonal. Para marcar una posicion debe");
	printf("\n\toprimir \"s\".");
	printf("\n\n\tEl efecto es que se revelara la fila, columna o diagonal seleccionada");
	printf("\n\tdurante el turno actual y el siguiente.");
	printf("\n\n\tAl igual que con el misil esp%ca, no se puede activar esta herramienta",161);
	printf("\n\ten el propio tablero.");
	printf("\n\n\tTanto usted como la computadora cuentan con 2 ");
	printf("\n\taviones esp%ca",161);
	printf("\n\n\tEL USO DE ESTA HERRAMIENTA SE CONSIDERA UN TURNO");
	printf("\n\n\n\n\tPresione una tecla para continuar...");
	getch();
	clrscr();
	printf("\t      Instrucciones\n");
	printf("\n\n\tMOVER SUBMARINO");
	printf("\n\n\tUsted tiene la opcion de mover y rotar los submarinos que esten");
	printf("\n\ten el tablero submarino.");
	printf("\n\n\tPara hacerlo debe situarse sobre uno de sus submarinos y oprimir");
	printf("\n\t\"m\", luego debe elegir la direcci%cn con el num pad el taclado",162);
	printf("\n\n\t CONTROLES");
	printf("\n\t   5:rotar (cambia la orientacion del barco)");
	printf("\n\t   8:mover hacia arriba");
	printf("\n\t   6:mover hacia la derecha");
	printf("\n\t   4:mover hacia la izquierda");
	printf("\n\t   2:mover hacia abajo");
	printf("\n\n\tEL USO DE ESTA HERRAMIENTA SE CONSIDERA UN TURNO");
	printf("\n\n\n\n\tPresione una tecla para continuar...");
	getch();
	clrscr();
}

/* misil_balistico se utiliza cuando se elige disparar en la posicion "i","j","k"(variables globales) */
bool misil_balistico(int board[10][10][2],int flota[5][5]){
	if((board[i][j][k]>=10 && board[i][j][k]<20) || board[i][j][k]==30)/*si el lugar elegido ya fue disparado o esta destruido se retorna false para que el turno no se modifique*/
		return false;
		
	int id=board[i][j][k]%10;//id es el identificador de barco o submarino (0=agua, 1=barco , 2=sub verde , 3=sub azul , 4=sub verde disparado sin daño , 5=sub verde disparado sin daño)
	if(id>3)id-=2;//si el id es de un submarino que esta en la superficie pero en un lugar previamente disparado se cambia el codigo al codigo original
	int i1;
	
	if( k && id ){//si ( se esta en la parte submarina  &&  no es agua )[si es un submarino sumergido]
		/*se sube al submarino a la superficie*/
		if((flota[0][id+1])==(flota[2][id+1])){// si ( el sub esta horizontal )
			for(i1=0;i1<=2;i1++){
				board[(flota[0][id+1])][(flota[1][id+1]+i1)][1]-=id;//se borra al sub de la parte submarina
				if(board[(flota[0][id+1])][(flota[1][id+1]+i1)][0]!=10)//si lo que esta en la superficie no fue disparado
					board[(flota[0][id+1])][(flota[1][id+1]+i1)][0]+=id;//se coloca al sub en la superficie
				else
					board[(flota[0][id+1])][(flota[1][id+1]+i1)][0]+=(id-8);//se coloca al sub en la superficie asignando el codigo correspondiente
			}
		}else{// ( el sub esta vertical )
			for(i1=0;i1<=2;i1++){
				board[(flota[0][id+1]+i1)][(flota[1][id+1])][1]-=id;//se borra al sub de la parte submarina
				if(board[(flota[0][id+1]+i1)][(flota[1][id+1])][0]!=10)//si lo que esta en la superficie no fue disparado
					board[(flota[0][id+1]+i1)][(flota[1][id+1])][0]+=id;//se coloca al sub en la superficie
				else
					board[(flota[0][id+1]+i1)][(flota[1][id+1])][0]+=(id-8);//se coloca al sub en la superficie asignando el codigo correspondiente
			}
		}
		/*se marca el disparo*/
		board[i][j][1]=10;//se marca como disparo a agua en el lugar q se disparo
		board[i][j][0]=10+id;//se marca como disparo a un barco/submarino pero en la superficie
		return false;
	}else{
		if(board[i][j][k]%10){//si (no es agua)
			/*se marca el disparo y se devuelve false (no cambia de turno)*/
			board[i][j][k]=10+id;
			return false;
		}else{
			/*se marca el disparo y se devuelve true (cambia de turno)*/
			board[i][j][k]=10+id;
			return true;
		}
	}
}
/*misil espia se utiliza cuando se desea espiar alrededor de una posicion enemiga i,j (variables globales)*/
bool misil_espia(int board[10][10][2]){
	int a,b,c;
	for(c=0;c<2;c++){
		for(a=i-1;a<=i+1;a++){
			for(b=j-1;b<=j+1;b++){
				//si (el lugar esta en el tablero && es un barco/submarino sin destruir ni detectar) 
				if((a>=0 && a<10 && b>=0 && b<10) && board[a][b][c]<10){
					board[a][b][c]+=20;
				}
			}
		}
	}
	return true;
}
/*
flota[0][] xi
     [1][] yi
     [2][] xi
     [3][] yi
     [4][] ext

flota[][3] verde
     [][4] azul

*/

bool move_sub(char sub_id, char move,int flota[5][5],int board[10][10][2]){
	int xi,yi,xf,yf,k1;
	/* a “k1” se le asigna el índice correspondiente al submarino seleccionado (ver las notas sobre "flota")*/
	if(sub_id=='v'){
		k1=3;
	}else{
		k1=4;
	}
	/*se asignan las coordenadas iniciales y finales guardadas en "flota"*/
	xi=flota[0][k1];//coordenada x inicial
	yi=flota[1][k1];//coordenada y inicial
	xf=flota[2][k1];//coordenada x final
	yf=flota[3][k1];//coordenada y final
	
	/*
	move = 5 <- rotar
	move = 4 <- izquierda
	move = 6 <- derecha 
	move = 8 <- arriba
	move = 2 <- abajo
	nunmeros elegidos por comodidad al usar el teclado numerico*/
	switch(move){
		case '5':/*rotar
			si( esta_horizontal && (el_movimiento_es_posible_dentro_del_tablero) && (no_hay_barcos_que_impidan_el_giro) )*/
			if( xi==xf && ( 0<xi && xi<9 ) && ( (board[xi-1][yi+1][1])%10==0 && (board[xf+1][yf-1][1])%10==0 && (board[xi-1][yi+1][0])%10==0 && (board[xf+1][yf-1][0])%10==0 )
			    && ( (board[xi-1][yi+1][1])!=30 && (board[xf+1][yf-1][1])!=30 && (board[xi-1][yi+1][0])!=30 && (board[xf+1][yf-1][0])!=30) ){
				/* se gira el submarino actualizando "board" */
				board[xi][yi][1]-=(k1-1);
				board[xf][yf][1]-=(k1-1);
				board[xi-1][yi+1][1]+=(k1-1);
				board[xf+1][yf-1][1]+=(k1-1);
				/*se actualiza "flota" con las nuevas coordenades de los extremos inicial y final*/
				flota[0][k1]=xi-1;
				flota[1][k1]=yi+1;
				flota[2][k1]=xf+1;
				flota[3][k1]=yf-1;
				return true;
			}else{
				/*si( esta_vertical && (el_movimiento_es_posible_dentro_del_tablero) && (no_hay_barcos_que_impidan_el_giro) )*/
				if(yi==yf && ( 0<yi && yi<9 ) && ((board[xi+1][yi-1][1])%10==0 && (board[xf-1][yf+1][1])%10==0 && (board[xi+1][yi-1][0])%10==0 && (board[xf-1][yf+1][0])%10==0) 
				&& ((board[xi+1][yi-1][1])!=30  && (board[xf-1][yf+1][1])!=30  && (board[xi+1][yi-1][0])!=30  && (board[xf-1][yf+1][0])!=30 )){
					/* se gira el submarino actualizando "board" */
					board[xi][yi][1]-=(k1-1);
					board[xf][yf][1]-=(k1-1);
					board[xi+1][yi-1][1]+=(k1-1);
					board[xf-1][yf+1][1]+=(k1-1);
					/*se actualiza "flota" con las nuevas coordenades de los extremos inicial y final*/
					flota[0][k1]=xi+1;
					flota[1][k1]=yi-1;
					flota[2][k1]=xf-1;
					flota[3][k1]=yf+1;
					return true;
				}else{
					return false;
				}
			}
			break;
			
		case '4'://izquierda
			/* si( esta_horizontal && (el_movimiento_es_posible_dentro_del_tablero) && no_hay_barcos_impidiendo_el_movimiento) */
			if(xi==xf && 0<yi && board[xi][yi-1][1]%10==0 && board[xi][yi-1][0]%10==0 && board[xi][yi-1][1]!=30 && board[xi][yi-1][0]!=30){
				/* se mueve el submarino a la izquierda actualizando "board" */
				board[xf][yf][1]-=(k1-1);
				board[xi][yi-1][1]+=(k1-1);
				/*se actualiza "flota" con las nuevas coordenades de los extremos inicial y final*/
				flota[1][k1]=yi-1;
				flota[3][k1]=yf-1;
				return true;
			}else{
				return false;
			}
			break;
			
		case '6'://derecha
			/* si( esta_horizontal && (el_movimiento_es_posible_dentro_del_tablero) && no_hay_barcos_impidiendo_el_movimiento) */
			if(xi==xf && yf<9 && board[xf][yf+1][1]%10==0 && board[xf][yf+1][0]%10==0 && board[xf][yf+1][1]!=30 && board[xf][yf+1][0]!=30){
				/* se mueve el submarino a la derecha actualizando "board" */
				board[xi][yi][1]-=(k1-1);
				board[xf][yf+1][1]+=(k1-1);
				/*se actualiza "flota" con las nuevas coordenades de los extremos inicial y final*/
				flota[1][k1]=yi+1;
				flota[3][k1]=yf+1;
				return true;
			}else{
				return false;
			}
			break;
			
		case '8'://arriba
			/* si( esta_vertical && (el_movimiento_es_posible_dentro_del_tablero) && (no_hay_barcos_impidiendo_el_movimiento)) */
			if(yi==yf && 0<xi && (board[xi-1][yi][1]%10==0 && board[xi-1][yi][0]%10==0) && (board[xi-1][yi][1]!=30 && board[xi-1][yi][0]!=30)){
				/* se mueve el submarino hacia arriba actualizando "board" */
				board[xf][yf][1]-=(k1-1);
				board[xi-1][yi][1]+=(k1-1);
				/*se actualiza "flota" con las nuevas coordenades de los extremos inicial y final*/
				flota[0][k1]=xi-1;
				flota[2][k1]=xf-1;
				return true;
			}else{
				return false;
			}
			break;
			
		case '2'://abajo
			/* si( esta_vertical && (el_movimiento_es_posible_dentro_del_tablero) && no_hay_barcos_impidiendo_el_movimiento) */
			if(yi==yf && xf<9 && (board[xf+1][yi][1]%10==0 && board[xf+1][yi][0]%10==0) && (board[xf+1][yi][1]!=30 && board[xf+1][yi][0]!=30)){
				/* se mueve el submarino hacia abajo actualizando "board" */
				board[xi][yi][1]-=(k1-1);
				board[xf+1][yf][1]+=(k1-1);
				/*se actualiza "flota" con las nuevas coordenades de los extremos inicial y final*/
				flota[0][k1]=xi+1;
				flota[2][k1]=xf+1;
				return true;
			}else{
				return false;
			}
			break;
		default:
			return false;
			break;
	}
}

int next_shot(int sumai, int sumaj, int i, int j){/*funcion que devuelve la fila/columna de la posicion a disparar en el turno de la computadora*/
	if( i+sumai>=10 || j+sumaj>=10){/*si la fila o columna esta fuera de rango*/
		return 10;
	}
	
	if(board_pl[i+sumai][j+sumaj][0]<10){/*si la posicion probable es algo desconocido para la computadora*/
		if(sumai){
			ban1=true;
			return i+sumai;
		}else{
			ban2=true;
			return j+sumaj;
		}
	}else{/*si la posicion es algo conocido*/
		if(board_pl[i+sumai][j+sumaj][0]>=20 || board_pl[i+sumai][j+sumaj][0]==10 || board_pl[i+sumai][j+sumaj][0]==30){/*si la posicion es algo espiado, o agua conocida, o esta destruida*/
			if(sumai){
				ban1=false;
				return i+sumai;
			}else{
				ban2=false;
				return j+sumaj;
			}
		}
		if(sumai){/*si el siguiente espacio esta dañado, se evalua ese espacio, llamando de nuevo a la misma funcion*/
			return next_shot(1,0,i+sumai,j);
		}else{
			return next_shot(0,1,i,j+sumaj);
		}
	}
}

char pcai(int espias_cont[2][2]){
	
	/*ataca a los barcos detectados*/
	int auxi,auxj;
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			for(k=0;k<2;k++){
				if(board_pl[i][j][k]>20 && board_pl[i][j][k]!=30){/*si el barco esta detectado y no esta destruido*/
					return 'b';
				}
			}
		}
	}

	k=0;/*si se llega a esta parte se verifican solo los lugares dañados y sus alrededores, eso solo puede ocurrir en la superficie*/
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(board_pl[i][j][k]<20 && board_pl[i][j][k]>10){//si la posicion es un barco disparado
				auxi = next_shot(1,0, i, j);
				auxj = next_shot(0,1, i, j);
				
				if(auxj-j > auxi-i){//esta horizontal
					if((auxj==10 || !ban2)&& j>0 && board_pl[i][j-1][0]!=30 && board_pl[i][j-1][0]<10){//si no se puede disparar en auxj
						j--;
						return 'b';
					}
					if(board_pl[i][j-1][0]>10){
						if((auxi==10 || !ban1) && i>0 && board_pl[i-1][j][0]!=30){//si no se puede disparar en auxi
							i--;
							return 'b';
						}
						if(ban1){//si se puede disparar en auxi
						i=auxi;
						return 'b';}
					}
					
					if(ban2){//si se puede disparar en auxj
						j=auxj;
						return 'b';
					}
				}
				if(auxj-j < auxi-i){//esta vertical
					if((auxi==10 || !ban1) && i>0 && board_pl[i-1][j][0]!=30 && board_pl[i-1][j][0]<10){//si no se puede disparar en auxi
						i--;
						return 'b';
					}
					if(board_pl[i-1][j][0]<10){
						if((auxj==10 || !ban2)&& j>0 && board_pl[i][j-1][0]!=30 && board_pl[i][j-1][0]<10){//si no se puede disparar en auxj
							j--;
							return 'b';
						}
						if(ban2){//si se puede disparar en auxj
							j=auxj;
							return 'b';
						}
					}
					if(ban1){//si se puede disparar en auxi
					i=auxi;
					return 'b';}
				}
				/*si es que no se sabe si esta horizontal o vertical*/
				while(1){
					switch(rand()%4){
						case 0:
							if(auxi!=10 && ban1){/*si se puede disparar en auxi*/
								i=auxi;
								return 'b';
							}
							break;
						case 1:
							if(auxj!=10 && ban2){/*si se puede disparar en auxj*/
								j=auxj;
								return 'b';
							}
							break;
						case 2:
							if(i>0 && board_pl[i-1][j][0]!=30){/*si se puede disparar encima de la posicion*/
								i--;
								return 'b';
							}
							break;
						case 3:
							if(j>0 && board_pl[i][j-1][0]!=30){/*si se puede disparar a la izquierda de la posicion*/
								j--;
								return 'b';
							}
							break;
						default:break;
					}
				}
			}
		}
	}
	
	/*esta parte utiliza la estrategia de espionaje ya determinado*/
	if((espias_cont[0][0] + espias_cont[0][1])>0){
		
		if(espia){/*se usa matriz espia 1*/
			
			i=matriz_espia_1[(espias_cont[0][0] + espias_cont[0][1])-1][0];
			j=matriz_espia_1[(espias_cont[0][0] + espias_cont[0][1])-1][1];
			
		}else{/*se usa matriz espia 2*/
			
			i=matriz_espia_2[(espias_cont[0][0] + espias_cont[0][1])-1][0];
			j=matriz_espia_2[(espias_cont[0][0] + espias_cont[0][1])-1][1];
			
		}
		
		/*se retorna 'a' o 'e' de acuerdo a la estrategia*/
		if((espias_cont[0][0] + espias_cont[0][1]-1)<3){
			return 'e';
		}else{
			return 'a';
		}
	}
	
	/*se dispara de forma aleatoria en los lugares donde haya por lo menos 3 casillas vacias (porque los barcos mas pequeños tienen esa dimension)*/
	do{
		
		i=rand()%10;
		j=rand()%10;
		k=(rand()%99999)%2;
		
		/*calcula que este una columna de 3 vacia incluyendo la posicion randomizada, y las posiciones adyacentes*/
		if(i+1<10 && i-1>=0 && board_pl[i+1][j][k]<10 && board_pl[i][j][k]<10 && board_pl[i-1][j][k]<10)
			return 	'b';
			
		/*calcula que este una columna de 3 vacia incluyendo la posicion randomizada, y las dos posiciones debajo de la misma*/
		if(i+2<10 && board_pl[i][j][k]<10 && board_pl[i+1][j][k]<10 && board_pl[i+2][j][k]<10)
			return 	'b';
			
		/*calcula que este una columna de 3 vacia incluyendo la posicion randomizada, y las dos posiciones arriba de la misma*/
		if(i-2>=0 && board_pl[i-2][j][k]<10 && board_pl[i-1][j][k]<10 && board_pl[i][j][k]<10)
			return 	'b';
			
		/*calcula que este una fila de 3 vacia incluyendo la posicion randomizada, y las dos posiciones adyacentes*/
		if(j+1<10 && j-1>=0 && board_pl[i][j+1][k]<10 && board_pl[i][j][k]<10 && board_pl[i][j-1][k]<10)
			return 	'b';
			
		/*calcula que este una fila de 3 vacia incluyendo la posicion randomizada, y las dos posiciones a la derecha de la misma*/
		if(j+2<10 && board_pl[i][j][k]<10 && board_pl[i][j+1][k]<10 && board_pl[i][j+2][k]<10)
			return 	'b';
			
		/*calcula que este una fila de 3 vacia incluyendo la posicion randomizada, y las dos posiciones a la izquierda de la misma*/
		if(j-2>=0 && board_pl[i][j-2][k]<10 && board_pl[i][j-1][k]<10 && board_pl[i][j][k]<10)
			return 	'b';
		
	}while(1);
	
	
}

int pcai_avion_espia(int a){/*devuelve las coordenadas de la posicion final*/
	if(espia){/*se usa matriz espia 1*/
		return matriz_espia_1[(espias_cont[0][0]+espias_cont[0][1]+1)][a];
	}else{/*se usa matriz espia 2*/ 
		return matriz_espia_2[(espias_cont[0][0]+espias_cont[0][1]+1)][a];
	}
}

void reset(){/*esta funcion devuelve a las variables globales la posicion en la matriz necesaria, con respecto a donde estaba el cursor en el ultimo turno del jugador*/
	if(y<15){
		i=y-3;
		k=0;
	}else{
		i=y-15;
		k=1;
	}
	if(x<28){
		j=(x-3)/2;
	}else{
		j=(x-28)/2;
	}

}
void set_fleet_pl(int board[10][10][2],int flota[5][5]){  /*ubica los barcos en las matrices tablero*/
	char pos; //horizontal o vertical
	int w;    //identificador del barco
	bool z;	  //se usa para evaluar si la posicion es válida
	x=28;
	i=0;
	j=0;
	
	gotoxy(x,y);
	char cTecla = ' ';
	
    for(w=0;w<5;w++){
		
        gotoxy(1,25);
		cprintf("Inserte la posicion horizontal del ");
		switch(w){
			case 0: printf("Portaaviones (5 cuadros)");break;
			case 1: printf("Acorazado (4cuadros)    ");break;
			case 2: printf("Crucero (3 cuadros)     ");break; 
			case 3: printf("Submarino 1 (3 cuadros) ");break;
			case 4: printf("Submarino 2 (3 cuadros) ");break;
		}
		
		gotoxy(x,y);
		
		while( cTecla != 27 && w<5) //  != ESC
		{
			cTecla = getch(); // sin eco
			
			if( cTecla == 'h')pos = cTecla;
			if( cTecla == 'v')pos = cTecla;
			
			if( cTecla == 's'){
				erase();
				z=check_and_set(w,pos,flota,board);//se evalua que la posicion sea correcta y en caso de que lo sea se ubica el barco
				show_update(cheat);
				if (z==false){/*en caso de que el barco no pueda ubicarse, se vuelve a evaluar*/
					gotoxy(55,1);printf("     ERROR");
					gotoxy(52,3);printf("No puede ubicar un barco");
					gotoxy(52,4);printf("donde ya hay uno");
					gotoxy(52,5);printf("o si no entra en el tablero");
					
					gotoxy(x,y);
					
					w--;
				}
				break;
			}
			if( cTecla == 27) cTecla=' ';//ESC
			if( cTecla == 0 ) cTecla = getch(); // Las compuestas
			if( cTecla == 72 ){ //moverse hacia arriba
				if(y==15){
					y-=3;
					i=9;
					k=0;
				}else{
					if(y!=3){
						y--;
						i--;
					}
				}
				gotoxy(x,y);
			}
			if( cTecla == 75 ){ //moverse a la izquierda
				if(x>28){
					x-=2;
					j--;
				}
				    
				gotoxy(x,y);
			}
			if( cTecla == 77 ){ //moverse a la derecha
				
					if(x<46){
						x+=2;
						j++;
					}
							
				gotoxy(x,y);
			}
			
			if( cTecla == 80 ){ //moverse abajo
				if(y==12){
					y+=3;
					k=1;
					i=0;
				}else{
					if(y!=24){
						y++;
						i++;
					}
				}      
				gotoxy(x,y);
			}
			
		}
	}
}

void set_fleet_pc(int board[10][10][2],int flota[5][5]){  /*ubica los barcos en las matrices tablero*/
	char pos;
	int w,a,b,c,cont;
	k=1;
	
    for(w=0;w<5;){
		cont=0;
		pos=104+(rand()%2)*14;//randomiza entre 104(ascii "h") y 118(ascii"v");
		
		if(pos=='v'){//si el barco se ubica verticalmente y entra en el tablero
			i=rand()%(10-flota[4][w]);
			j=rand()%10;
			gotoxy(1,25);
			for(c=0;c<2;c++){
				for(a=i-1;a<=flota[4][w]+i;a++){
					for(b=j-1;b<=j+1;b++){
						if(a>=0 && a<10 && b>=0 && b<10 && board[a][b][c])cont++;
					}
				}
			}
		}else{//si el barco se ubica horizontalmente
			i=rand()%10;
			j=rand()%(10-flota[4][w]);
			gotoxy(1,25);
				for(c=0;c<2;c++){
					for(a=i-1;a<=i+1;a++){
						for(b=j-1;b<=flota[4][w]+j;b++){
							if(a>=0 && a<10 && b>=0 && b<10 && board[a][b][c])cont++;
						}
					}
				}
			
		}
		if(!cont && check_and_set(w,pos,flota,board)){//se evalua que la posicion sea correcta y en caso de que lo sea se ubica el barco
			w++;show_update(cheat);
		}
	}
}
void show_stats(int flota_pl[5][5],int flota_pc[5][5]){/*imprime las estadisticas del juego*/
	int a;
	erase();
	gotoxy(55,1);printf("     ESTADISTICAS");
	gotoxy(57,3);printf("Barcos Destruidos");
	gotoxy(52,5);printf("     PC           Jugador",179);
	
	for(a=5;a<12;a++){
		gotoxy(65,a);
		printf("%c",179);
		
	}
		//se imprimen los barcos que pierde la computadora
	if(flota_pc[4][0]<0){gotoxy(52, 7);cprintf("PortaAviones");}
	if(flota_pc[4][1]<0){gotoxy(52, 8);cprintf("Acorazado");}
	if(flota_pc[4][2]<0){gotoxy(52, 9);cprintf("Crucero");}
	if(flota_pc[4][3]<0){gotoxy(52,10);cprintf("Submarino 1");}
	if(flota_pc[4][4]<0){gotoxy(52,11);cprintf("Submarino 2");}
	//se imprimen los barcos que pierde el jugador
	if(flota_pl[4][0]<0){gotoxy(67, 7);cprintf("PortaAviones");}
	if(flota_pl[4][1]<0){gotoxy(67, 8);cprintf("Acorazado");}
	if(flota_pl[4][2]<0){gotoxy(67, 9);cprintf("Crucero");}
	if(flota_pl[4][3]<0){gotoxy(67,10);cprintf("Submarino 1");}
	if(flota_pl[4][4]<0){gotoxy(67,11);cprintf("Submarino 2");}  
	
	gotoxy(57,13);printf(" ESPIAS  DISPONIBLES ");

	for(a=14;a<17;a++){
		gotoxy(65,a);
		printf("%c",179);
	}
	gotoxy(54,15);printf("A Espia: %d",espias_cont[0][0]);
	gotoxy(67,15);printf("A Espia: %d",espias_cont[1][0]);
	gotoxy(54,16);printf("M Espia: %d",espias_cont[0][1]);
	gotoxy(67,16);printf("M Espia: %d",espias_cont[1][1]);
	
	gotoxy(52,18);printf("TURNO ACTUAL");
	gotoxy(54,20);
	if(turno)
		printf("Jugador");
	else
		printf("Computadora");
	
	gotoxy(x,y);
}
void show_update(bool cheat){/*actualiza la pantalla, mostrando los aciertos, agua, espias, etc*/

/*los modos (trampa activada o desactivada)
estan dados por combinaciones de colores, 
si la trampa esta activada, el jugador
puede ver toda la información del juego de la computadora
*/
int i,j;
	if(cheat){//si hubo trampa (el usuario ve toda la información del juego)
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pc[i][j][0]){
					case  0: gotoxy(2*j+3,i+3);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case  2: gotoxy(2*j+3,i+3);textbackground(2);printf("_");break;
					case  3: gotoxy(2*j+3,i+3);textbackground(3);printf("_");break;
					case  4: gotoxy(2*j+3,i+3);textbackground(10);printf("_");break;
					case  5: gotoxy(2*j+3,i+3);textbackground(11);printf("_");break;
					case 10: gotoxy(2*j+3,i+3);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+3,i+3);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+3,i+3);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+3,i+3);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+3,i+3);textbackground(9);printf("_");break;
					case 21: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case 23: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case 30: gotoxy(2*j+3,i+3);textbackground(12);printf("_");break;
				}
				textbackground(0);
			}
		}
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pc[i][j][1]){
					case  0: gotoxy(2*j+3,i+15);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case  2: gotoxy(2*j+3,i+15);textbackground(2);printf("_");break;
					case  3: gotoxy(2*j+3,i+15);textbackground(3);printf("_");break;
					case  4: gotoxy(2*j+3,i+15);textbackground(10);printf("_");break;
					case  5: gotoxy(2*j+3,i+15);textbackground(11);printf("_");break;
					case 10: gotoxy(2*j+3,i+15);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+3,i+15);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+3,i+15);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+3,i+15);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+3,i+15);textbackground(9);printf("_");break;
					case 21: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case 23: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case 30: gotoxy(2*j+3,i+15);textbackground(12);printf("_");break;
				}
				textbackground(0);
			}
		}
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pl[i][j][0]){
					case  0: gotoxy(2*j+28,i+3);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+28,i+3);textbackground(6);printf("_");break;
					case  2: gotoxy(2*j+28,i+3);textbackground(2);printf("_");break;
					case  3: gotoxy(2*j+28,i+3);textbackground(3);printf("_");break;
					case  4: gotoxy(2*j+28,i+3);textbackground(2);printf("_");break;
					case  5: gotoxy(2*j+28,i+3);textbackground(3);printf("_");break;
					case 10: gotoxy(2*j+28,i+3);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+28,i+3);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+28,i+3);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+28,i+3);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+28,i+3);textbackground(9);printf("_");break;
					case 21: gotoxy(2*j+28,i+3);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+28,i+3);textbackground(6);printf("_");break;
					case 23: gotoxy(2*j+28,i+3);textbackground(6);printf("_");break;
					case 30: gotoxy(2*j+28,i+3);textbackground(12);printf("_");break;
				}
				textbackground(0);
			}
		}
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pl[i][j][1]){
					case  0: gotoxy(2*j+28,i+15);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+28,i+15);textbackground(6);printf("_");break;
					case  2: gotoxy(2*j+28,i+15);textbackground(2);printf("_");break;
					case  3: gotoxy(2*j+28,i+15);textbackground(3);printf("_");break;
					case  4: gotoxy(2*j+28,i+15);textbackground(2);printf("_");break;
					case  5: gotoxy(2*j+28,i+15);textbackground(3);printf("_");break;
					case 10: gotoxy(2*j+28,i+15);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+28,i+15);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+28,i+15);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+28,i+15);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+28,i+15);textbackground(9);printf("_");break;
					case 21: gotoxy(2*j+28,i+15);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+28,i+15);textbackground(6);printf("_");break;
					case 23: gotoxy(2*j+28,i+15);textbackground(6);printf("_");break;
					case 30: gotoxy(2*j+28,i+15);textbackground(12);printf("_");break;
				}
				textbackground(0);
			}
		}
		gotoxy(x,y);
	}else{//si la trampa esta desactivada
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pc[i][j][0]){
					case  0: gotoxy(2*j+3,i+3);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+3,i+3);textbackground(0);printf("_");break;
					case  4: gotoxy(2*j+3,i+3);textbackground(9);printf("_");break;
					case  5: gotoxy(2*j+3,i+3);textbackground(9);printf("_");break;
					case 10: gotoxy(2*j+3,i+3);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+3,i+3);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+3,i+3);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+3,i+3);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+3,i+3);textbackground(9);printf("_");break;
					case 21: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case 23: gotoxy(2*j+3,i+3);textbackground(6);printf("_");break;
					case 30: gotoxy(2*j+3,i+3);textbackground(12);printf("_");break;
					default: gotoxy(2*j+3,i+3);textbackground(0);printf("_");break;
				}
				textbackground(0);
			}
		}
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pc[i][j][1]){
					case  0: gotoxy(2*j+3,i+15);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+3,i+15);textbackground(0);printf("_");break;
					case  4: gotoxy(2*j+3,i+15);textbackground(9);printf("_");break;
					case  5: gotoxy(2*j+3,i+15);textbackground(9);printf("_");break;
					case 10: gotoxy(2*j+3,i+15);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+3,i+15);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+3,i+15);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+3,i+15);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+3,i+15);textbackground(9);printf("_");break;
					case 22: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case 23: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case 21: gotoxy(2*j+3,i+15);textbackground(6);printf("_");break;
					case 30: gotoxy(2*j+3,i+15);textbackground(12);printf("_");break;
					default: gotoxy(2*j+3,i+15);textbackground(0);printf("_");break;
				}
				textbackground(0);
			}
		}
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pl[i][j][0]){
					case  0: gotoxy(2*j+28,i+3);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+28,i+3);textbackground(6);printf("_");break;
					case  2: gotoxy(2*j+28,i+3);textbackground(2);printf("_");break;
					case  3: gotoxy(2*j+28,i+3);textbackground(3);printf("_");break;
					case  4: gotoxy(2*j+28,i+3);textbackground(2);printf("_");break;
					case  5: gotoxy(2*j+28,i+3);textbackground(3);printf("_");break;
					case 10: gotoxy(2*j+28,i+3);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+28,i+3);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+28,i+3);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+28,i+3);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+28,i+3);textbackground(0);printf("_");break;
					case 21: gotoxy(2*j+28,i+3);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+28,i+3);textbackground(2);printf("_");break;
					case 23: gotoxy(2*j+28,i+3);textbackground(3);printf("_");break;
					case 30: gotoxy(2*j+28,i+3);textbackground(12);printf("_");break;
				}
				textbackground(0);
			}
		}
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				switch(board_pl[i][j][1]){
					case  0: gotoxy(2*j+28,i+15);textbackground(0);printf("_");break;
					case  1: gotoxy(2*j+28,i+15);textbackground(6);printf("_");break;
					case  2: gotoxy(2*j+28,i+15);textbackground(2);printf("_");break;
					case  3: gotoxy(2*j+28,i+15);textbackground(3);printf("_");break;
					case  4: gotoxy(2*j+28,i+15);textbackground(2);printf("_");break;
					case  5: gotoxy(2*j+28,i+15);textbackground(3);printf("_");break;
					case 10: gotoxy(2*j+28,i+15);textbackground(9);printf("_");break;
					case 11: gotoxy(2*j+28,i+15);textbackground(4);printf("_");break;
					case 12: gotoxy(2*j+28,i+15);textbackground(4);printf("_");break;
					case 13: gotoxy(2*j+28,i+15);textbackground(4);printf("_");break;
					case 20: gotoxy(2*j+28,i+15);textbackground(0);printf("_");break;
					case 21: gotoxy(2*j+28,i+15);textbackground(6);printf("_");break;
					case 22: gotoxy(2*j+28,i+15);textbackground(2);printf("_");break;
					case 23: gotoxy(2*j+28,i+15);textbackground(3);printf("_");break;
					case 30: gotoxy(2*j+28,i+15);textbackground(12);printf("_");break;
				}
				textbackground(0);
			}
		}
		gotoxy(x,y);

	}
};

void show_empty(void){/*imprime los trableros vacíos*/
	clrscr();
	printf("\n");
	printf("  1 2 3 4 5 6 7 8 9 10 %c   1 2 3 4 5 6 7 8 9 10\n",179);
	printf("A|_|_|_|_|_|_|_|_|_|_| %c A|_|_|_|_|_|_|_|_|_|_| S\n",179);
	printf("B|_|_|_|_|_|_|_|_|_|_| %c B|_|_|_|_|_|_|_|_|_|_| U\n",179);
	printf("C|_|_|_|_|_|_|_|_|_|_| %c C|_|_|_|_|_|_|_|_|_|_| P\n",179);
	printf("D|_|_|_|_|_|_|_|_|_|_| %c D|_|_|_|_|_|_|_|_|_|_| E\n",179);
	printf("E|_|_|_|_|_|_|_|_|_|_| %c E|_|_|_|_|_|_|_|_|_|_| R\n",179);
	printf("F|_|_|_|_|_|_|_|_|_|_| %c F|_|_|_|_|_|_|_|_|_|_| F\n",179);
	printf("G|_|_|_|_|_|_|_|_|_|_| %c G|_|_|_|_|_|_|_|_|_|_| I\n",179);
	printf("H|_|_|_|_|_|_|_|_|_|_| %c H|_|_|_|_|_|_|_|_|_|_| C\n",179);
	printf("I|_|_|_|_|_|_|_|_|_|_| %c I|_|_|_|_|_|_|_|_|_|_| I\n",179);
	printf("J|_|_|_|_|_|_|_|_|_|_| %c J|_|_|_|_|_|_|_|_|_|_| E\n          PC           %c         JUGADOR\n",179,179);
	printf("  1 2 3 4 5 6 7 8 9 10 %c   1 2 3 4 5 6 7 8 9 10\n",179);
	printf("A|_|_|_|_|_|_|_|_|_|_| %c A|_|_|_|_|_|_|_|_|_|_| S\n",179);
	printf("B|_|_|_|_|_|_|_|_|_|_| %c B|_|_|_|_|_|_|_|_|_|_| U\n",179);
	printf("C|_|_|_|_|_|_|_|_|_|_| %c C|_|_|_|_|_|_|_|_|_|_| B\n",179);
	printf("D|_|_|_|_|_|_|_|_|_|_| %c D|_|_|_|_|_|_|_|_|_|_| M\n",179);
	printf("E|_|_|_|_|_|_|_|_|_|_| %c E|_|_|_|_|_|_|_|_|_|_| A\n",179);
	printf("F|_|_|_|_|_|_|_|_|_|_| %c F|_|_|_|_|_|_|_|_|_|_| R\n",179);
	printf("G|_|_|_|_|_|_|_|_|_|_| %c G|_|_|_|_|_|_|_|_|_|_| I\n",179);
	printf("H|_|_|_|_|_|_|_|_|_|_| %c H|_|_|_|_|_|_|_|_|_|_| N\n",179);
	printf("I|_|_|_|_|_|_|_|_|_|_| %c I|_|_|_|_|_|_|_|_|_|_| O\n",179);
	printf("J|_|_|_|_|_|_|_|_|_|_| %c J|_|_|_|_|_|_|_|_|_|_|\n",179);
}
/*funciones que calculan las posiciones iniciales de las traspuestas para las matrices espia*/

/*transpuesta 1*/
void transpuesta1(int matriz[7][2]){
	int aux,a;
	for(a=0;a<7;a++){
		aux = matriz[a][0];
		matriz[a][0]=matriz[a][1];
		matriz[a][1]=aux;
	}
} 
/*trasnspuesta 2*/
void transpuesta2(int matriz[7][2]){
	int a;
	for(a=0;a<7;a++){
		matriz[a][1]=9-matriz[a][1];
	}
}
/*trasnspuesta 3*/
void transpuesta3(int matriz[7][2]){
	int a;
	for(a=0;a<7;a++){
		matriz[a][0]=9-matriz[a][0];
	}
}
