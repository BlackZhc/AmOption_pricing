#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

double up_factor, uptick_prob, downtick_prob, notick_prob, risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;
static double** check_call;
static double** check_put;

double max(double a, double b) {
    return (b < a) ? a : b;
}

double american_call_option(int k, int i,double current_stock_price)
{
    if (k == no_of_divisions)
        return check_call[k][i + k] = max(0.0,  (current_stock_price- strike_price));

    if (check_call[k][i + k] == -1)

        check_call[k][i + k] = max((current_stock_price-strike_price),(uptick_prob * american_call_option(k + 1, i + 1,current_stock_price*up_factor)
            + notick_prob * american_call_option(k + 1, i,current_stock_price) + downtick_prob * american_call_option(k + 1, i - 1,current_stock_price/up_factor)) / R);
    return check_call[k][i + k];
}


double american_put_option(int k, int i,double current_stock_price) {

    if (k == no_of_divisions)
        return check_put[k][i + k] = max(0.0, (strike_price-current_stock_price ));

    if (check_put[k][i + k] == -1)
        check_put[k][i + k] = max((strike_price-current_stock_price ), (uptick_prob * american_put_option(k + 1, i + 1, current_stock_price * up_factor)
            + notick_prob * american_put_option(k + 1, i, current_stock_price) + downtick_prob * american_put_option(k + 1, i - 1, current_stock_price / up_factor)) / R);
    return check_put[k][i + k];
}


int main(int argc, char* argv[])
{

    
    sscanf(argv[1], "%lf", &expiration_time);
    sscanf(argv[2], "%d", &no_of_divisions);
    sscanf(argv[3], "%lf", &risk_free_rate);
    sscanf(argv[4], "%lf", &volatility);
    sscanf(argv[5], "%lf", &initial_stock_price);
    sscanf(argv[6], "%lf", &strike_price);
    
    /*
    expiration_time = 0.5;
    no_of_divisions = 500;
    risk_free_rate = 0.08;
    volatility = 0.3;
    initial_stock_price = 60;
    strike_price = 50;
     */

    up_factor = exp(volatility * sqrt(2 * expiration_time / ((float)no_of_divisions)));
    R = exp(risk_free_rate * expiration_time / ((float)no_of_divisions));

    uptick_prob = ((sqrt(R) - 1 / sqrt(up_factor)) / (sqrt(up_factor) - 1 / sqrt(up_factor))) * ((sqrt(R) - 1 / sqrt(up_factor)) / (sqrt(up_factor) - 1 / sqrt(up_factor)));
    downtick_prob = ((sqrt(up_factor) - sqrt(R)) / (sqrt(up_factor) - 1 / sqrt(up_factor))) * ((sqrt(up_factor) - sqrt(R)) / (sqrt(up_factor) - 1 / sqrt(up_factor)));
    notick_prob = 1 - uptick_prob - downtick_prob;
    
    int size = 2 * no_of_divisions + 1;
    check_call = new double* [size];
    for (int m = 0; m < size; m++)
        check_call[m] = new double[size];
    for (int a = 0; a < size; a++)
        for (int b = 0; b < size; b++)
            check_call[a][b] = -1;

    check_put = new double* [size];
    for (int m = 0; m < size; m++)
        check_put[m] = new double[size];
    for (int a = 0; a < size; a++)
        for (int b = 0; b < size; b++)
            check_put[a][b] = -1;
    
    

    cout << "Recursive Binomial European Option Pricing" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "--------------------------------------" << endl;

    cout << "Up Factor = " << up_factor << endl;
    cout << "Uptick Probability = " << uptick_prob << endl;
    cout << "Notick Probability = " << notick_prob << endl;
    cout << "Downtick Probability = " << downtick_prob << endl;

    cout << "--------------------------------------" << endl;


    double call_price = american_call_option(0, 0, initial_stock_price);
    cout << "Trinomial Price of an American Call Option = " << call_price << endl;
    cout << "--------------------------------------" << endl;


    double put_price = american_put_option(0, 0, initial_stock_price);
    cout << "Trinomial Price of an American Put Option = " << put_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Verifying Put-Call Parity: S+P-C = Kexp(-r*T)" << endl;
    cout << initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;
    cout << initial_stock_price + put_price - call_price << "? =? " << strike_price * exp(-risk_free_rate * expiration_time) << endl;
    cout<<"Looks like Put-Call Parity NOT hold for this situation" << endl;
    cout << "--------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        delete[] check_call[i];
    }
    delete[] check_call;

    for (int i = 0; i < size; i++) {
        delete[] check_put[i];
    }
    delete[] check_put;
}


