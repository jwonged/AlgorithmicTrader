// OurTradingHeader.h

// Data for least squares analysis
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
		UIUX (int width,int height); 
		void yellowsmile();
		void pasthundred();
};

// UIUX and Modelling functions
void ReadDataFile(ModelData &data);
void mygraphics(int w, int h);
void CalStats ();
void leastSquares();
void Smiley(int button, int state, int x, int y); // for click functions
void realtimeheaders(); 
void presskey(unsigned char C, int x, int y); 



// Trade set up and intialise
void InitialiseAccount(int dataSet, int labGroup, int SecurityCode, TradingAccount &account, int dataset);
void HandleExitCode(int errorCode);
void MyTrader(TradingAccount &account, ModelData &data, int dataset);
float ConcludeAcc(TradingAccount &account, int dataset);



