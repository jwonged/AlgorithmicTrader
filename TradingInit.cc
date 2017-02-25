
// Trading & Setup Functions
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <glue.h>
#include <iomanip>
#include <trading.h>

#include "OurTradingHeader.h"

using namespace std;

// Initialise trading account and connect to exchange
void InitialiseAccount(int dataSet, int id, int codeNumber, TradingAccount &account, int dataset) {
   int ec, day= 0;

   // Set data members of TradingAccount object for day = 0
   account.today = day;
   account.price[day][dataset] = 20.0;
   account.transaction[day][dataset] = TT_PASS;
   account.volume[day][dataset] = 0;
   account.stock[day][dataset] = 0;
   account.balance[day][dataset] = 20000.0;

   // Call trading library function and handle exit (return) code
   ec = TE_InitialiseTrading(dataSet, id, codeNumber, account.commission);
   HandleExitCode(ec);
}


void MyTrader(TradingAccount &account, ModelData &data, int dataset) {
    //Get predicted price 
	float predictedprice[100];
	for (int i=0; i<100; i++) {
		predictedprice[i] = 20 + (data.b * i);
	}
     
     //Trade for 49 days with algo
    for (int i=1; i<50; i++) {
		
		// Call library function to get today's price
		account.today++;
		HandleExitCode(TE_GetPrice(i, account.price[i][dataset]));
		
	
		// Decide on transaction and volume - only buy if price is higher than tmr's predicted
		float buyprice;
		
		if (account.price[i][dataset] > predictedprice[i+1]) {
			account.transaction[i][dataset] = TT_PASS;
			account.volume[i][dataset] = 0;
			}
			
		if (account.stock[i-1][dataset] == 0) {
			if (account.price[i][dataset] < predictedprice[i+1])  {
					account.transaction[i][dataset] = TT_BUY;
					buyprice = account.price[i][dataset];
					account.volume[i][dataset] = ((account.balance[i-1][dataset]-2*account.commission)/account.price[i][dataset]);
				}
			else {
					account.transaction[i][dataset] = TT_PASS;
					account.volume[i][dataset] = 0;
				}
				
			}
			
		if (account.stock[i-1][dataset] > 0) {
			if (account.price[i][dataset] > buyprice)  {
					account.transaction[i][dataset] = TT_SELL;
					account.volume[i][dataset] = account.stock[i-1][dataset];
				}
			else {
				account.transaction[i][dataset] = TT_PASS;	
				account.volume[i][dataset] = 0;
			}
		}

		// Make the Trade
		int ec;
		ec = TE_Trade(account.transaction[i][dataset], account.volume[i][dataset]);
		HandleExitCode(ec); 
 

		// Update accounts
		if (account.transaction[i][dataset] == TT_BUY) {
			account.stock[i][dataset] = account.stock[i-1][dataset] + account.volume[i][dataset];
			account.balance[i][dataset] = account.balance[i-1][dataset] - (account.price[i][dataset]*account.volume[i][dataset]) - account.commission;
			}
		if (account.transaction[i][dataset] == TT_SELL) {
			account.stock[i][dataset] = account.stock[i-1][dataset] - account.volume[i][dataset];
			account.balance[i][dataset] = account.balance[i-1][dataset] + (account.price[i][dataset]*account.volume[i][dataset]) - account.commission;
			}
		if (account.transaction[i][dataset] == TT_PASS) {
			account.stock[i][dataset] = account.stock[i-1][dataset];
			account.balance[i][dataset] = account.balance[i-1][dataset];
			}
		
	}
	
	//Ensure only 1000 stock for day 50 and update
	HandleExitCode(TE_GetPrice(50, account.price[50][dataset]));

	if (account.stock[49][dataset] < 1000) {
		account.volume[50][dataset] = 1000-account.stock[49][dataset];
		HandleExitCode(TE_Trade(TT_BUY,account.volume[50][dataset]));
		account.stock[50][dataset] = account.stock[49][dataset] + account.volume[50][dataset];
		account.balance[50][dataset] = account.balance[49][dataset] - (account.price[50][dataset]*account.volume[50][dataset]) - account.commission;
	}
	else if (account.stock[49][dataset] > 1000) {
		account.volume[50][dataset] = account.stock[49][dataset]-1000;
		HandleExitCode(TE_Trade(TT_SELL,account.volume[50][dataset]));
		account.stock[50][dataset] = account.stock[49][dataset] - account.volume[50][dataset];
		account.balance[50][dataset] = account.balance[49][dataset] + (account.price[50][dataset]*account.volume[50][dataset]) - account.commission;
		}
	else {
		account.volume[50][dataset] = 0;
		HandleExitCode(TE_Trade(TT_PASS,account.volume[50][dataset]));
		account.stock[50][dataset] = account.stock[49][dataset];
		account.balance[50][dataset] = account.balance[49][dataset];
	}
     
}

// Function definition to display error messages from trading exchange
// The program is terminated with exit(-1) if there is an error
void HandleExitCode(int errorCode)
{
   switch(errorCode)
   {
      case TE_OKAY:
         break;

      case TE_FAIL:
         cout << "Trading error: bad parameter." << endl;
         exit(-1);

      case TE_ALREADY_CONNECTED:
         cout << "Trading error: already connected to server." << endl;
         exit(-1);

      case TE_NOT_CONNECTED:
         cout << "Trading error: not connected to server." << endl;
         exit(-1);

      case TE_AUTH_FAILED:
         cout << "Trading error: authorisation failed." << endl;
         exit(-1);

      case TE_TOO_EARLY:
         cout << "Trading error: must agree price every day." << endl;
         exit(-1);

      case TE_TOO_LATE:
         cout << "Trading error: transaction already made for today." << endl;
         exit(-1);

      case TE_NO_FUNDS:
         cout << "Trading error: insufficient funds for trading." << endl;
         exit(-1);

      case TE_NO_STOCK:
         cout << "Trading error: insufficient stock for sale." << endl;
         exit(-1);

      case TE_SECURITY_PROBLEM:
         cout << "Wrong code number used for TE_InitialiseTrading." << endl;
         exit(-1);
 
      default:
         cout << "Trading error: trading system failure." << endl;
         exit(-1);
   }
}


float ConcludeAcc(TradingAccount &account, int dataset) {
	//float final_balance, final_stock;
	float final_balance;
	int final_stock;
	int ec = TE_CloseTrading(final_balance, final_stock); 
	HandleExitCode(ec);
	
	ofstream fileOut;
	fileOut.open("Daily Results", ios::app);
	
	//Consistency Check 
	if ((final_balance - account.balance[50][dataset])<1 and final_stock == account.stock[50][dataset]) {
		cout<<"\nThe results coincide with the trading\n";
	}
	else { 
		cout<<"final balance deos not coincide!"<<endl; 
	}
	
	if (not fileOut.good()) {
     cout  << "Error trying to open the file"  << endl;
	}
  
	//print to file
     fileOut <<"Dataset : "<<dataset<<endl;
     fileOut <<"Day "<<setfill (' ') << setw (10)<<" Price "<< setfill (' ') << std::setw (10)<<" transaction "<< std::setfill (' ') << std::setw (10)<<" volume "<< std::setfill (' ') << std::setw (10)<<" stock  "<< std::setfill (' ') << std::setw (10)<<" balance " << endl; 
     cout << " Day  Price transaction volume stock  balance " << endl; 
     for (int i=0; i<51; i++) {
			fileOut<<i<< std::setfill (' ') << std::setw (10)<<account.price[i][dataset]<< std::setfill (' ') << std::setw (10)<<account.transaction[i][dataset]<< std::setfill (' ') << std::setw (10)<<account.volume[i][dataset]<< std::setfill (' ') << std::setw (10)<<account.stock[i][dataset]<< std::setfill (' ') << std::setw (10)<<account.balance[i][dataset]<<endl;
			cout<<i<<"  "<<account.price[i][dataset]<<"  "<<account.transaction[i][dataset]<<"  "<<account.volume[i][dataset]<<"  "<<account.stock[i][dataset]<<"  "<<account.balance[i][dataset]<<endl;
		}
				
   fileOut.close();

	
	
	return final_balance;
}
