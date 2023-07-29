#include <map>
#include "absl/container/btree_map.h"
#include <benchmark/benchmark.h>
#include "benchmark/include/benchmark/benchmark.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/pool/pool_alloc.hpp>

using namespace std;

class BMFixture : public ::benchmark::Fixture
{
public:
    std::map<double, int> std_map;
    absl::btree_map<double, int> btree_map;
    typedef std::map<double, int, std::less<int>, boost::fast_pool_allocator<std::pair<const double, int>>> boostmap;
    boostmap std_pmap;
    std::vector<pair<double, int>> updates;

    pair<double, int> parseLine(const string &line)
    {
        stringstream ss(line);
        string priceStr, quantityStr;
        getline(ss, priceStr, ',');
        getline(ss, quantityStr, ',');
        double price = atof(priceStr.c_str());
        int quantity = atoi(quantityStr.c_str());
        return make_pair(price, quantity);
    }

    void SetUp(const ::benchmark::State &state)
    {
        ifstream inputFile("snapshot.csv");
        if (!inputFile)
        {
            cout << "Error opening snapshot.csv" << endl;
        }
        string line;
        while (getline(inputFile, line))
        {
            pair<double, int> data = parseLine(line);
            double price = data.first;
            int quantity = data.second;
            std_map[price] = quantity;
            btree_map[price] = quantity;
            std_pmap[price] = quantity;
        }
        inputFile.close();
        ifstream inputFile2("updates.csv");
        if (!inputFile2)
        {
            cout << "Error opening file updates.csv" << endl;
        }
        while (getline(inputFile2, line))
        {
            pair<double, int> data = parseLine(line);
            updates.push_back(data);
        }
        inputFile2.close();
    }

    void TearDown(const ::benchmark::State &)
    {
        std_map.clear();
        btree_map.clear();
        std_pmap.clear();
    }
};

BENCHMARK_DEFINE_F(BMFixture, BM_StdMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {

        for (auto i : updates)
        {
            auto it = std_map.find(i.first);
            if (it == std_map.end())
            {
                if (std_map.size() == 20)
                    std_map.erase(std_map.begin());
                std_map.insert(i);
            }
            else
            {
                if (it->second == i.second)
                    std_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}
BENCHMARK_DEFINE_F(BMFixture, BM_BTreeMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {

        for (auto i : updates)
        {
            auto it = btree_map.find(i.first);
            if (it == btree_map.end())
            {
                if (btree_map.size() == 20)
                {
                    btree_map.erase(btree_map.begin());
                }
                btree_map.insert(i);
            }
            else
            {
                if (it->second == i.second)
                    btree_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_StdPMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {

        for (auto i : updates)
        {
            auto it = std_pmap.find(i.first);
            if (it == std_pmap.end())
            {
                if (std_pmap.size() == 20)
                    std_pmap.erase(std_pmap.begin());
                std_pmap.insert(i);
            }
            else
            {
                if (it->second == i.second)
                    std_pmap.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}
BENCHMARK_REGISTER_F(BMFixture, BM_StdMap);
BENCHMARK_REGISTER_F(BMFixture, BM_BTreeMap);
BENCHMARK_REGISTER_F(BMFixture, BM_StdPMap);

BENCHMARK_MAIN();
