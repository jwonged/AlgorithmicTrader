// main
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <glue.h>
#include <trading.h>

#include "OurTradingHeader.h"

ModelData data;
TradingAccount account;
float compiledtotalbalance = 0;

using namespace std;

int main() {
   
   account.datatest = 1;
   //Read trading price data from file (100 days)
   ReadDataFile(data);
	
   //Produce linear model - Method of least-squares
   leastSquares(); 
   
   // Call to function to analyse mean and variance of residual errors
   CalStats();
	
   for (int dataset=1; dataset<21; dataset++) {
		// Call to function to initialise trading account
		InitialiseAccount(dataset, 74, 2+333+15, account, dataset); 

		// Call to function to trade and update accounts over next 50 days
		MyTrader(account, data, dataset);
		
		//Call to function to close trading account and output accounts and add sum to total
		account.compiledtotal += ConcludeAcc(account, dataset);

	}
   
	// Set up graphics. 
   glueWindow(1500,900,50,50,WHITE); 
   graphicsfunction(mygraphics); 
   glueGo();
   return 0;
}


