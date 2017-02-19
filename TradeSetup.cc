// Interface and Stats

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <glue.h>
#include <trading.h>
#include <string>
#include <sstream> 
#include <unistd.h>
#include <math.h>

#include "OurTradingHeader.h"

extern ModelData data;
extern TradingAccount account;

using namespace std;

float ModelData::standarddev() {
	return sqrt(variance);
	
}

UIUX::UIUX (int width,int height): h(height), w(width) {};

void ReadDataFile(ModelData &data) {
   char fileName[80] = "tradingData.dat";
   int number = 100;
   int day;
   float price;

   // Associate file name with fin object of class ifstream
   ifstream fin;
   fin.open(fileName);

   // Prompt for new file name if not able to open
   while(!fin.good())
   {
      cout << "Unable to open trading data file. Enter a new name: ";
      cin >> fileName;
      fin.clear();
      fin.open(fileName);
   }

   data.numPoints = number;
   for(int i=0; i< number; i++)
   {   
     fin >> day >> price;
     data.x[i] = day;
     data.y[i] = price;
   }

   fin.close();
}

void mygraphics(int w, int h) {//w and h are 1500,900 
	//Make graphics object
	UIUX GUI(w,h);

	//color it grey
	color(0.22,0.22,0.22);
	filledrectangle(0,0,w,h);
	
	//axis for given data top left
	color(BLACK);
	filledrectangle(50,525,625,350);

	color (WHITE);
	xaxis(75,600 , 10, 550, 5, "Days", -100, 50, 0);
	yaxis(550, 850, 10, 75, 5, "Price", 0, 50, 0);
	
	//plot of given data
	move(-100, data.y[1], AXES);
	for (int i=1; i<data.numPoints; i++) {
		color (RED);
		draw(data.x[i]-100, data.y[i], AXES); 
	}
	
	//graph of predicted price
	move (-100,data.b * 1 + data.a,AXES);	
	for (int i=1; i<150; i++) {
		color (BLUE);
		draw(i-100, data.b * i + data.a , AXES);
	}
	
	//print min and max
	stringstream maximum;
	stringstream minimum;
	maximum << data.biggestDailyRise;
	minimum << data.biggestDailyFall;
	color (WHITE);
	text("Max:", 480, 800);
	text(maximum.str(), 515, 800);
	text("Min:", 480, 780);
	text(minimum.str(), 515, 780);
	
	//Top right graph for past 100 days
	GUI.pasthundred();
	
	//Draw the smiley
	GUI.yellowsmile();
	
	//Realtime data bottom right graph
	color(BLACK);
	filledrectangle(825,25,625,400);
	color (WHITE);
	xaxis(850,1400 , 10, 50, 5, "Days", 0, 50, 0);
	yaxis(50, 375, 10, 850, 5, "Price", 0, 50, 0);
	move(0, 20, AXES);
	
	//Buttons to change dataset 
	keyboardfunction(presskey);
	
	//Click input
	mousefunction(Smiley);
	
	//Headers for realtime data
	realtimeheaders();

}

//Graph to print the yellow smiley face
void UIUX::yellowsmile() {
	int circlex = w/2, circley = (h/2)+25, circler = 60;
	
	color (BLACK);
	filledcircle(circlex,circley,circler+1);
	color (YELLOW);
	filledcircle(circlex,circley,circler);
	color (BLACK);
	
	filledcircle(circlex-(0.33*circler), circley+(0.33*circler), 3);
	filledcircle(circlex+(0.33*circler), circley+(0.33*circler), 3);
	move(circlex, circley-(0.5*circler), PIXELS);
	for (float i= 0; i< 5; i+=0.1) {
		draw(circlex+(i*7), circley-(0.5*circler)+(i*i), PIXELS);
	}
	move(circlex, circley-(0.5*circler), PIXELS);
	for (float i= 0; i> -5; i-=0.1) {
		draw(circlex+(i*7), circley-(0.5*circler)+(i*i), PIXELS);
	}
}

void realtimeheaders() {
	//realtime transaction data
	color(BLACK);
	filledrectangle(50,25,625,400);
	color(WHITE);
	text("Past 100 Days record", 1050, 850);
	text("Compiled Results", 260, 850);
	text("Real-time Data", 1060, 395);
	color(WHITE);
	int xvalue = 70, yvalue = 395+5, adder = 65;
	text("Day", xvalue, yvalue);
	text("Price", xvalue+adder, yvalue);
	text("stock", xvalue+2*adder, yvalue);
	text("balance", xvalue+3*adder, yvalue);
	
	//For final balance
	color(WHITE);
	filledrectangle(700,300,100,25);
	color(BLACK);
	text("Final Balance", 698, 330);
	
	//For realtime balance
	color(WHITE);
	filledrectangle(700,250,100,25);
	color(BLACK);
	text("Dataset Balance", 698, 280);
}

//Graph for past 100 days data
void UIUX::pasthundred() {
	color(BLACK);
	filledrectangle(825,525,625,350);
	color (WHITE);
	xaxis(850,1400 , 10, 550, 5, "Days", 0, 100, 0);
	yaxis(550, 850, 10, 850, 5, "Price", 0, 50, 0);
	move(0, data.y[1], AXES);
	for (int i=1; i<data.numPoints; i++) {
		color (RED);
		draw(data.x[i], data.y[i], AXES);
	}
}

void Smiley(int button, int state, int x, int y) {

	int dataset = 1;//choose dataset
	float sumbalance = 0;//running balance
	
	//activate function when smiley is clicked on
	if (button == GLUE_LEFT_BUTTON and state == GLUE_DOWN) {
		
		//Click Smiley
		if (x>690 and x<810 and y>415 and y<535) {
			
			//Ensure the user knows the smiley feels pain
			color(WHITE);
			text("Ouch!!", 850, 460);
			
			//alternate datasets
			dataset = account.datatest;
			
			//Clean up the axis
			realtimeheaders();
			
				move(0, account.price[1][dataset], AXES);
				for (int i=1; i<51; i++) {
					
					//Realtime graph
					if (account.price[i][dataset]<account.price[i-1][dataset]) {
							color (RED);
						}
					if (account.price[i][dataset]>account.price[i-1][dataset]) {
							color (GREEN);
						}
						
					//Draw the graph
					draw(i, account.price[i][dataset], AXES);
					
					//Realtime transactions
					color(GREEN);
					int xvalue = 70, yvalue = 395, adder = 65, Day = i, yminus = 14, resety = 0;
					
					//second header
					if (i>25) {
						color(WHITE);
						xvalue = xvalue + 4*adder+30;
						resety = 25* yminus;
						text("Day", xvalue, yvalue+5);
						text("Price", xvalue+adder, yvalue+5);
						text("stock", xvalue+2*adder, yvalue+5);
						text("balance", xvalue+3*adder, yvalue+5);
						color(GREEN);
					}
					
					//print the realtime transactions by line
					stringstream tempday;
					tempday << Day;
					text(tempday.str(), xvalue, yvalue-i*yminus + resety);
					stringstream price;
					price << account.price[i][dataset];
					text(price.str(), xvalue+adder, yvalue-i*yminus + resety);
					stringstream stock;
					stock << account.stock[i][dataset];
					text(stock.str(), xvalue+2*adder, yvalue-i*yminus + resety);
					stringstream balance;
					balance << account.balance[i][dataset];
					text(balance.str(), xvalue+3*adder, yvalue-i*yminus + resety);
					
					//realtime balance
					sumbalance = account.balance[i][dataset];
					color(WHITE);
					filledrectangle(700,250,100,25);
					stringstream sumbal;
					sumbal << sumbalance;
					color(BLACK);
					text(sumbal.str(), 708, 258);
					
					//Timed plotting
					usleep(100000);
						}
			}
		}
}

void presskey(unsigned char C, int x, int y) {	
	color(0.22,0.22,0.22);
		filledrectangle(700,220,100,25);
		color(BLACK);
	
   switch (C) {
      case '1':
        account.datatest = 1;;
		text("Dataset 1", 707, 232);
         break;
         
         case '2':
        account.datatest = 2;
		text("Dataset 2", 707, 232);
         break;
         
        case '3':
        account.datatest = 3;
		text("Dataset 3", 707, 232);
         break;
         
        case '4':
        account.datatest = 4;
		text("Dataset 4", 707, 232);
         break;
         
         case '5':
        account.datatest = 5;
		text("Dataset 5", 707, 232);
         break;
         
         case '6':
        account.datatest = 6;
		text("Dataset 6", 707, 232);
         break;
         
         case '7':
        account.datatest = 7;
		text("Dataset 7", 707, 232);
         break;
         
         case '8':
        account.datatest = 8;
		text("Dataset 8", 707, 232);
         break;
         
         case '9':
        account.datatest = 9;
		text("Dataset 9", 707, 232);
         break;
         
         case 'q':
        account.datatest = 10;
		text("Dataset 10", 707, 232);
         break;
         
         case 'w':
        account.datatest = 11;
		text("Dataset 11", 707, 232);
         break;
         
         case 'e':
        account.datatest = 12;
		text("Dataset 12", 707, 232);
         break;
         
         case 'r':
        account.datatest = 13;
		text("Dataset 13", 707, 232);
         break;
         
         case 't':
        account.datatest = 14;
		text("Dataset 14", 707, 232);
         break;
         
         case 'y':
        account.datatest = 15;
		text("Dataset 15", 707, 232);
         break;
         
         case 'u':
        account.datatest = 16;
		text("Dataset 16", 707, 232);
         break;
         
         case 'i':
        account.datatest = 17;
		text("Dataset 17", 707, 232);
         break;
         
         case 'o':
        account.datatest = 18;
		text("Dataset 18", 707, 232);
         break;
         
         case 'p':
        account.datatest = 19;
		text("Dataset 19", 707, 232);
         break;
         
         case 'a':
        account.datatest = 20;
		text("Dataset 20", 707, 232);
         break;
         
         case 'b':
			color(WHITE);
			filledrectangle(700,300,100,25);
			stringstream compiledtot;
			compiledtot << account.compiledtotal;
			color(BLACK);
			text(compiledtot.str(), 708, 308);
			break;

	}
}

void leastSquares () {
	
	//find a and b
	float sx = 0, sy=0, sxx=0, sxy=0;
	
	for (int z=0; z < data.numPoints; z++) 
	{
			sx = sx + data.x[z];
			sy = sy + data.y[z];
			sxx = sxx + data.x[z]*data.x[z];
			sxy = sxy + data.y[z]*data.x[z];
			
		}	
	//a and b for y= bx + a
	data.a = ( sxx*sy - sx*sxy ) / ( data.numPoints*sxx - sx*sx );	
	data.b = ( data.numPoints*sxy - sx*sy ) / ( data.numPoints*sxx - sx*sx );

}
	
void CalStats () {
	
		float maxresidual=0, minresidual=1000;
		int maxday, minday;
		
		for (int t=0; t<data.numPoints;t++) {
			data.residual[t] = data.y[t] - (data.a + (data.b*data.x[t]));
			
			//find max and min
			if (data.residual[t]<minresidual) {
				minresidual = data.residual[t];
				minday = t;
			}
			
			if (data.residual[t]>maxresidual) {
				maxresidual = data.residual[t];
				maxday = t;
			}
			
			data.mean = data.mean + data.residual[t];
		}
		
		data.mean = data.mean/data.numPoints;
		
		for (int t=0; t<data.numPoints;t++) {
			data.variance = data.variance + (((data.residual[t]-data.mean)*(data.residual[t]-data.mean))/(data.numPoints-1));
		}	
		
		//biggest and smallest deviation from best fit line on minday and maxday
		data.biggestDailyRise = maxresidual;
		data.biggestDailyFall = minresidual;
		
}
	
