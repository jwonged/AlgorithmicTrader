// OurTradingHeader.h
// Project header file for trading exercise. Contains definitions of constants,
// data structures and function declarations (prototypes)
// Included at top of all project files
//Lab Group 74 - Donte and Joshua Wong

// Constants
const int MAXSIZE = 100;
const int LABGROUP = 999; // you need to change this
const int SECURITYCODE = 999; // you need to change this


// User definition of a data structure for least squares analysis
// x, y and residual are arrays of type float
class ModelData{
 public:
   int numPoints;
   float x[MAXSIZE];
   float y[MAXSIZE];
   float a;
   float b;
   float residual[MAXSIZE];
   float mean;
   float variance;
   float biggestDailyRise;
   float biggestDailyFall;
   float standarddev();
};

// User definition of a data structure for the trading accounts
class TradingAccount{
 public:
   int today;
   float price[MAXSIZE][22];
   int transaction[MAXSIZE][22];
   int volume[MAXSIZE][22];
   int stock[MAXSIZE][22];
   float balance[MAXSIZE][22];
   float compiledtotal;
   float commission;
   int datatest;
};

//UIUX functions
class UIUX {
	public:
		int w, h;
		UIUX (int width,int height); //constructor to take in window width and height
		void yellowsmile();
		void pasthundred();
};

// User-defined function declarations for part 1
void ReadDataFile(ModelData &data);
void mygraphics(int w, int h);
void CalStats ();
void leastSquares();
void Smiley(int button, int state, int x, int y); // for click functions
void realtimeheaders(); // Print headers for realtime graph
void presskey(unsigned char C, int x, int y); //function for the press keys



// User-defined function declarations for part 2
void InitialiseAccount(int dataSet, int labGroup, int SecurityCode, TradingAccount &account, int dataset);
void HandleExitCode(int errorCode);
void MyTrader(TradingAccount &account, ModelData &data, int dataset);
float ConcludeAcc(TradingAccount &account, int dataset);



