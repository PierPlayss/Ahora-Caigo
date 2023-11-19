//SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
//Headers
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "SDL2SoundEffects.h"



using namespace std;

RenderWindow window("Ahora caigo!", 1280, 720);
//RenderWindow gamesWindow("Games Window", 1000, 500);

const int FPS = 60;
int refreshRate = 1000 / FPS;
int frame = 0;
int widthres = 1280;
int widthLetra = 40;
int widthEspacio = 5;

int Usado(int cantUsado[], int& random, int& palabrasusadas, int cantLineas);
void ocultadorPalabra(int numPalabra, string palabras[], int& cantLetras);
void randomLetras(int cantLetras, int letrasOcultas[]);

int letrasOcultas[20];

class Timer
{
public:
	void start()
	{
		m_StartTime = std::chrono::system_clock::now();
		m_bRunning = true;
	}

	void stop()
	{
		m_EndTime = std::chrono::system_clock::now();
		m_bRunning = false;
	}

	double elapsedMilliseconds()
	{
		std::chrono::time_point<std::chrono::system_clock> endTime;

		if (m_bRunning)
		{
			endTime = std::chrono::system_clock::now();
		}
		else
		{
			endTime = m_EndTime;
		}

		return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
	}

	double elapsedSeconds()
	{
		return elapsedMilliseconds() / 1000.0;
	}

private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	bool                                               m_bRunning = false;
};

long fibonacci(unsigned n)
{
	if (n < 2) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

void setupPalabra(double &timerdouble, double &timerdif, bool &firstStarted, bool &stopTimer, int &modific, bool &m1, 
	bool &m2, bool &m3,
	int cantUsado[], int &random, int &palabrasusadas, int &cantLineas,
	int &numPalabra, string palabras[], int &cantLetras, int &startingpos,bool &roundOver,
	int turno,int& widthTimer, int& postimer, int& ptimer1, int& ptimer2) {

	cantLetras = 0;

	timerdouble = 30.00;
	timerdif = 0;
	firstStarted = false;
	stopTimer = false;
	modific = 0;
	m1 = false;
	m2 = false;
	m3 = false;

	roundOver = false;

	numPalabra = Usado(cantUsado, random, palabrasusadas, cantLineas);
	ocultadorPalabra(numPalabra, palabras, cantLetras);

	startingpos = ((widthres / 2) - ((widthLetra * palabras[numPalabra].size()) + (widthEspacio * (palabras[numPalabra].size()) - 1)) / 2);

	
}

void animationIn(int &widthTimer,int &postimer,int &ptimer1,int &ptimer2, bool &animationOver) {
	//218 wT 75 255

	//cout << "llega aca" << endl;
	if (widthTimer > 218) {
		widthTimer = 218;
	}
	if (widthTimer < 218) {
		widthTimer += 30;
		
		postimer -= 15;
	
		
		
		
		
	}
	if ((ptimer2 - ptimer1) < 170) {
		ptimer1 -= 15;
		//cout << ptimer1 << endl;
		ptimer2 += 15;
		//cout << ptimer2 << endl;
	}
	
	if (widthTimer == 218 and (ptimer2 - ptimer1) < 170) {
		animationOver = true;
	}
	


}

void animationOut(int& widthTimer, int& postimer, int& ptimer1, int& ptimer2, bool& animationOver) {
	//218 wT 75 255

	if (widthTimer > 0) {
		widthTimer -= 30;
		postimer += 15;

		if (ptimer1 != ptimer2) {
			ptimer1 += 15;
			//cout << ptimer1 << endl;
			ptimer2 -= 15;
			//cout << ptimer2 << endl;
		}


	}
	if (widthTimer <= 0 and ptimer1 == ptimer2) {
		animationOver = true;
		//cout << ptimer1 << endl;
	}

	//Mover
	


}
void moveRight(int& widthTimer, int& postimer, int& ptimer1, int& ptimer2, bool& animationOver) {
	
	if (ptimer1 == ptimer2 and ptimer1 < 950) {
		ptimer1 += 40;
		ptimer2 += 40;
		postimer += 40;
		//cout << ptimer1 << endl;
		//cout << ptimer1 << endl;
	}
	if (ptimer1 >= 965) {
		ptimer1 = 965;
		ptimer2 = 965;
		animationOver = true;
	}
}

void moveLeft(int& widthTimer, int& postimer, int& ptimer1, int& ptimer2, bool& animationOver) {
	if (ptimer1 == ptimer2 and ptimer1 > 165 ) {
		ptimer1 -= 40;
		ptimer2 -= 40;
		postimer -= 40;
		//cout << ptimer1 << endl;
		//cout << ptimer1 << endl;
	}
	if (ptimer1 < 165){
		ptimer1 = 165;
		ptimer2 = 165;
		//cout << ptimer1 << endl;
		//cout << ptimer2 << endl;
		animationOver = true;
		
	}
}

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		cout << "IMG_init has failed. Error: " << SDL_GetError() << endl;

	if (TTF_Init() == -1) {
		cout << "TTF_init has failed. Error: " << SDL_GetError() << endl;
	}
	int mouseX, mouseY;
	int clicks = 0;
	int clickMenu = 0;

	//Sounds
	SDL2SoundEffects se;
	//se.addSoundEffect("res/sound/memory/pink.mp3");//0


	//Mix_Music* bgm = Mix_LoadMUS("res/sound/music.mp3");
	//se.addSoundEffect("res/sound/strike.mp3"); //0

	//SDL_Texture* brickTexture = window.loadTexture("res/gfx/brick.png");
	
	SDL_Texture* letrasTexture =  window.loadTexture("res/gfx/letras.png");
	SDL_Texture* bgTexture = window.loadTexture("res/gfx/bg.png");
	SDL_Texture* hlTexture = window.loadTexture("res/gfx/highlight.png");
	SDL_Texture* timerTexture = window.loadTexture("res/gfx/timer.png");
	SDL_Texture* logo1Tex = window.loadTexture("res/gfx/c1.png");
	SDL_Texture* logo2Tex = window.loadTexture("res/gfx/c2.png");
	SDL_Texture* notimeTex = window.loadTexture("res/gfx/notime.png");
	Entity bgEntity(0, 0, 1280, 720, bgTexture);
	Entity noTime(0, 0, 1280, 720, notimeTex);
	

	//Entity LogicE(0, 50, 1280, 720, brickTexture);

	SDL_Event event;
	SDL_Event mouseEvent{ SDL_MOUSEMOTION };

	bool gameRunning = true;

	
	string palabras[1000];
	string tempString;
	string palabraOculta;
	string definiciones[1000];
	string espaciorandom;
	int cantUsado[1000];
	int cantLetras = 0;
	int startingpos = 0;




	int random;
	srand(time(NULL));
	clock_t time;
	time = clock();


	int palabrasusadas = 0;
	int numPalabra;

	fstream words;
	stringstream tr;
	int cantLineas;
	string aux;

	//Setup

	words.open("words.txt");
	getline(words, aux);
	tr << aux;
	tr >> cantLineas;


	for (int i = 0; i < cantLineas; i++) {
		cantUsado[i] = 0;
	}

	for (int i = 0; i < cantLineas; i++) {
		getline(words, palabras[i]);
		getline(words, definiciones[i]);
		getline(words, espaciorandom);

		//cout << s[i] << endl;
	}
	words.close();
		
	numPalabra = Usado(cantUsado, random, palabrasusadas, cantLineas);
	cout << definiciones[numPalabra] << endl;
	ocultadorPalabra(numPalabra, palabras, cantLetras);


	
	startingpos =((widthres/2) - ((widthLetra * palabras[numPalabra].size()) + (widthEspacio * (palabras[numPalabra].size())-1))/2) ;

	//highlight
	int posxHighlight = -300;
	bool avanzaDerecha = true;
	
	
	//timer
	Timer timer;
	double timerdouble=29.7, timerdif;
	bool firstStarted = false;
	bool stopTimer = false;
	string timerS;
	int modific = 0;
	bool m1 = false;
	bool m2 = false;
	bool m3 = false;
	bool gameStarted = false;
	bool roundOver = false;
	Entity timerE(100, 440, 218, 63, timerTexture);



	//Animations

	bool startAnimation1 = true;
	bool startAnimation2 = false;
	bool animationL = false, animationR = false;
	int posTimer = 100;
	int widthTimer=218;
	int PosTimer1=75;
	int PosTimer2=255;
	bool animationOver=false;
	bool animationOver2 = false;
	bool animationLOver = false;
	bool animationROver = false;

	int turno = 0;

	int opacidadNT = 0;
	bool opaBool = true;
	
	//cout << palabras[numPalabra] << endl;

	//cout << cantLetras << endl;

	//setup game

	//setupPalabra(timerdouble,timerdif,firstStarted,stopTimer,modific,m1,m2,m3, cantUsado, random, palabrasusadas, cantLineas, 
		//			numPalabra, palabras, cantLetras);


	while (gameRunning)
	{
		
		if (firstStarted == true) {
			timer.stop();
		}
		
		if (roundOver == false) {
			timerdif = timer.elapsedSeconds();
			timerdouble -= timerdif;
		}
		

		if (timerdouble == 0 or timerdouble<0) {
			timerdouble = 0.00;
			stopTimer = true;
			
		}
		timerdouble= round (timerdouble*100)/100 ;

		if (firstStarted == false and stopTimer==false and gameStarted==true and roundOver==false) {
			timer.start();
		}
		

		//animation
		if (animationR == true) {
			if (animationROver == false) {
				moveRight(widthTimer, posTimer, PosTimer1, PosTimer2, animationROver);
			}
			else {
				animationR = false;
			}
		}

		if (animationL == true) {
			if (animationLOver == false) {
				moveLeft(widthTimer, posTimer, PosTimer1, PosTimer2, animationLOver);
			}
			else {
				animationL = false;
			}
		}


		if (startAnimation1 == true) {
			if (animationOver == false) {
				animationOut(widthTimer, posTimer, PosTimer1, PosTimer2, animationOver);
			}
			else {
				startAnimation1 = false;

			}
		}
		if (startAnimation2 == true) {
			if (animationOver2 == false) {
				animationIn(widthTimer, posTimer, PosTimer1, PosTimer2, animationOver2);
			}
			else {
				startAnimation1 = false;
			}
		}

		window.RenderWindow::clear();
		// Get our controls and events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				gameRunning = false;
			if (event.type == SDL_MOUSEMOTION) {

				mouseX = event.motion.x;
				mouseY = event.motion.y;
				//cout << mouseX << "," << mouseY << endl;

			}
			if (event.type == SDL_MOUSEBUTTONUP) {
			
			}

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_RETURN) {
				
						
						if (gameStarted == false) {
							
							gameStarted = true;

							setupPalabra(timerdouble, timerdif, firstStarted, stopTimer, modific, m1, m2, m3, cantUsado, random, palabrasusadas, cantLineas,
								numPalabra, palabras, cantLetras, startingpos, roundOver,turno, widthTimer, posTimer, PosTimer1, PosTimer2);

							if (animationOver == true) {
								startAnimation1 = false;
								startAnimation2 = true;
								animationOver = false;
							}

							if (turno == 0) {
								animationR = true;
								animationROver = false;
								turno = 1;
							}
							else {
								animationL = true;
								animationLOver = false;
								turno = 0;

							}
							cout << "Turno: " << turno << endl;
						}
						else {
							roundOver = true;
							gameStarted = false;

							startAnimation2 = false;
							startAnimation1 = true;

							timer.stop();

						}
						cout << gameStarted << endl;

					
						
					
				}
			}
		}

		

		window.backgroundColor(100, 100, 100, 255);
		window.render(bgEntity, 1);
		
	
		

		for (int i = 0; i < palabras[numPalabra].size();i++) {
			Entity letrasEnt(startingpos+(45*i), 610, 40, 60, letrasTexture);
			window.render(letrasEnt, 1);
				//cout << " " << palabras[numPalabra][i];
				tr.clear();
				tr << palabras[numPalabra][i];
				tr >> tempString;
				window.textCustom(definiciones[numPalabra].c_str(), "res/fonts/Reforma.ttf", 0, 0, 2, 30, 113, 180, 45, 1, 3, 200);
				window.textCustom(definiciones[numPalabra].c_str(), "res/fonts/Reforma.ttf", 0, 0, 230, 230, 230, 255, 45, 1, 0, 200);
				if (letrasOcultas[i] == 1 or roundOver==true) {
					window.textAlignTo(tempString.c_str(), letrasEnt, 13, 29, 157, 255, 50, 1, 1, startingpos + (45 * i), 615, "res/fonts/Dezen.ttf");
				}
				
				//window.textCustom("test", "res/fonts/Dezen.ttf", 0 + (50 * i), 50, 255, 255, 255, 255, 30, 0, 0, 0);
			
			
		}
		//hl
		if (posxHighlight < 1280 + 300 and avanzaDerecha==true) {
			posxHighlight += 30;
		}
		else {
			avanzaDerecha = false;
			
		}

		if (posxHighlight >0 - 300 and avanzaDerecha == false) {
			posxHighlight -= 30;
		}
		else {
			avanzaDerecha = true;
		}
		//cout << posxHighlight << endl;
		Entity hlEntity(posxHighlight, 0, 1280, 720, hlTexture);
		window.render(hlEntity, 1);

		if (timerdouble < 15) {

			if (opaBool == true) {

				opacidadNT += 10;
				//cout << opacidadNT << endl;

				if (opacidadNT >= 240) {
					opaBool = false;
				}
			}
			else {
				opacidadNT -= 10;
				if (opacidadNT <= 0) {
					opaBool = true;
				}
			}
			SDL_SetTextureAlphaMod(notimeTex, opacidadNT);

			window.render(noTime, 1);
		}


		//timer
		tr.clear();
		tr << timerdouble;
		tr >> timerS;

		Entity timerE(posTimer, 435, widthTimer, 63, timerTexture);
		window.render(timerE, 1);
		if (timerdouble < 20 and m1==false) {
			modific += 10;
			m1 = true;
		}
		if (timerdouble < 10 and m2 == false) {
			modific += 15;
			m2 = true;
		}
		if (timerdouble == 0 and m3 == false) {
			modific += 40;
			m3 = true;
		}
		if (gameStarted == true and timerdouble<29.8) {
			window.textAlignTo(timerS.c_str(), timerE, 0, 0, 0, 255, 45, 0, 0, PosTimer1 + 55 + modific, 440, "res/fonts/Peloric Bold.ttf");
		}
		

		Entity c1Entity(PosTimer1, 423, 93, 93, logo1Tex);
		Entity c2Entity(PosTimer2, 423, 93, 93, logo2Tex);
		window.render(c1Entity, 1);
		window.render(c2Entity, 1);
		//cout<<palabras[numPalabra]<<endl;
		
		
		

		window.display();
		
		
		//Sleep(refreshRate);
		if (frame == FPS) {
			frame = 0;
		}
		else frame++;


		

	}

	window.cleanUp();
	SDL_Quit();
	TTF_Quit();

	return 0;
}

int Usado(int cantUsado[], int& random, int& palabrasusadas, int cantLineas) {
	random = rand() % cantLineas;


	if (cantUsado[random] == 1) {
		Usado(cantUsado, random, palabrasusadas, cantLineas);
	}
	else {
		cantUsado[random] = 1;
		palabrasusadas++;
		return random;
	}

}

void ocultadorPalabra(int numPalabra, string palabras[], int &cantLetras) {
	char palabraChar[20];
	
	int longitudPalabra = 0;

	for (int i = 0; i < 20; i++) {
		letrasOcultas[i] = 0;
	}

	longitudPalabra = palabras[numPalabra].size();
	cantLetras = (longitudPalabra / 3) + 1;
	//cout << "cantLetras: " << cantLetras << endl;

	for (int i = 0; i < cantLetras; i++) {
		randomLetras(longitudPalabra, letrasOcultas);
	}

	for (int i = 0; i < longitudPalabra; i++) {
		palabraChar[i] = palabras[numPalabra][i];
		cout << " " << palabras[numPalabra][i];
		//if (letrasOcultas[i] == 1) {
		//	cout << " " << palabras[numPalabra][i];
		//}
		//else {
		//	cout << " -";

		//}

	}
	cout << endl;
}

void randomLetras(int longitudPalabra, int letrasOcultas[]) {
	bool encontrado = false;

	while (encontrado == false) {
		int random = rand() % longitudPalabra;
		if (letrasOcultas[random] == 0) {
			if ((letrasOcultas[random + 1]) == 0 and (letrasOcultas[random - 1] == 0)) {
				letrasOcultas[random] = 1;
				encontrado = true;
			}

		}
	}


}



		