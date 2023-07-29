#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <math.h>

using namespace std;

int generateRandomQuantity()
{
    return rand() % 3 + 1;
}

double generateRandomPrice()
{
    double price = (rand() % 2 + 1000) * 0.5;
    return floor(price * 2) / 2;
}

int generateOrderID()
{
    return rand() % 10000 + 1;
}

void generateMarketSnapshotCSV(const string &filename, int numberInitialOrders)
{
    ofstream outputFile(filename);
    if (!outputFile)
    {
        cout << "Error opening file " << filename << endl;
        return;
    }
    for (int i = 0; i < numberInitialOrders; ++i)
    {
        int orderID = generateOrderID();
        double price = generateRandomPrice();
        int quantity = generateRandomQuantity();
        outputFile << orderID << "," << fixed << setprecision(2) << price << "," << quantity << endl;
    }
    outputFile.close();
    cout << "Market snapshot CSV file generated successfully." << endl;
}

void generateMarketUpdatesCSV(const string &filename, int numUpdates)
{
    ofstream outputFile(filename);
    if (!outputFile)
    {
        cout << "Error opening file " << filename << endl;
        return;
    }
    for (int i = 0; i < numUpdates; ++i)
    {
        int orderID = generateOrderID();
        double price = generateRandomPrice();
        int quantity = generateRandomQuantity();
        outputFile << orderID << "," << fixed << setprecision(2) << price << "," << quantity << endl;
    }
    outputFile.close();

    cout << "Market updates CSV file generated successfully." << endl;
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    generateMarketSnapshotCSV("snapshot1.csv", 5000);
    generateMarketUpdatesCSV("updates1.csv", 10000);

    return 0;
}
