#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/pool/pool_alloc.hpp>
#include <unordered_map>
#include "absl/container/flat_hash_map.h"
#include <boost/unordered_map.hpp>
#include "robin_hood.h"
#include <functional>
#include "absl/hash/hash.h"

using namespace std;

class BMFixture : public ::benchmark::Fixture
{
public:
    struct ordInfo
    {
        double prc;
        int qty;
    };

    unordered_map<int, ordInfo> std_unordered_map;
    absl::flat_hash_map<int, ordInfo> flat_hash_map;
    typedef boost::unordered_map<int, ordInfo> boost_unordered_map;
    typedef boost::unordered_map<int, ordInfo, boost::hash<int>, std::equal_to<int>, boost::fast_pool_allocator<std::pair<const int, ordInfo>>> boost_pool_unordered_map;
    boost_unordered_map boost_unordered;
    boost_pool_unordered_map boost_pool_unordered;
    typedef robin_hood::unordered_node_map<int, ordInfo, std::hash<int>> robin_hood_map_std;
    typedef robin_hood::unordered_node_map<int, ordInfo, absl::Hash<int>> robin_hood_map_absl;
    robin_hood_map_std robin_hood_std_map;
    robin_hood_map_absl robin_hood_absl_map;
    robin_hood::unordered_flat_map<int, ordInfo, std::hash<int>> robin_hood_flat_std_map;
    robin_hood::unordered_flat_map<int, ordInfo, absl::Hash<int>> robin_hood_flat_absl_map;

    std::vector<pair<int, ordInfo>> updates;

    pair<int, ordInfo> parseLine(const string &line)
    {
        stringstream ss(line);
        string ordIdStr, actionStr, qtyStr, prcStr;
        getline(ss, ordIdStr, ',');
        getline(ss, prcStr, ',');
        getline(ss, qtyStr, '\n');
        int ordId = atoi(ordIdStr.c_str());
        double prc = atof(prcStr.c_str());
        int qty = atoi(qtyStr.c_str());
        return make_pair(ordId, ordInfo{prc, qty});
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
            pair<int, ordInfo> data = parseLine(line);
            int ordId = data.first;
            ordInfo ord = data.second;
            std_unordered_map[ordId] = ord;
            flat_hash_map[ordId] = ord;
            boost_unordered[ordId] = ord;
            boost_pool_unordered[ordId] = ord;
            robin_hood_std_map[ordId] = ord;
            robin_hood_absl_map[ordId] = ord;
            robin_hood_flat_std_map[ordId] = ord;
            robin_hood_flat_absl_map[ordId] = ord;
        }
        inputFile.close();

        ifstream inputFile2("updates.csv");
        if (!inputFile2)
        {
            cout << "Error opening file updates.csv" << endl;
        }
        while (getline(inputFile2, line))
        {
            pair<int, ordInfo> data = parseLine(line);
            updates.push_back(data);
        }
        inputFile2.close();
    }

    void TearDown(const ::benchmark::State &)
    {
        std_unordered_map.clear();
        flat_hash_map.clear();
        boost_unordered.clear();
        boost_pool_unordered.clear();
        robin_hood_std_map.clear();
        robin_hood_absl_map.clear();
        robin_hood_flat_std_map.clear();
        robin_hood_flat_absl_map.clear();
    }
};

BENCHMARK_DEFINE_F(BMFixture, BM_StdUnorderedMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = std_unordered_map.find(i.first);
            if (it == std_unordered_map.end())
            {
                std_unordered_map.insert(i);
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    std_unordered_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_FlatHashMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = flat_hash_map.find(i.first);
            if (it == flat_hash_map.end())
            {
                flat_hash_map.insert(i);
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    flat_hash_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_BoostUnorderedMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = boost_unordered.find(i.first);
            if (it == boost_unordered.end())
            {
                boost_unordered.insert(i);
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    boost_unordered.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_BoostPoolUnorderedMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = boost_pool_unordered.find(i.first);
            if (it == boost_pool_unordered.end())
            {
                boost_pool_unordered.insert(i);
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    boost_pool_unordered.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_RobinHoodStdMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = robin_hood_std_map.find(i.first);
            if (it == robin_hood_std_map.end())
            {
                robin_hood_std_map.insert({i.first, i.second});
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    robin_hood_std_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_RobinHoodAbseilMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = robin_hood_absl_map.find(i.first);
            if (it == robin_hood_absl_map.end())
            {
                robin_hood_absl_map.insert({i.first, i.second});
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    robin_hood_absl_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_RobinHoodFlatStdMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = robin_hood_flat_std_map.find(i.first);
            if (it == robin_hood_flat_std_map.end())
            {
                robin_hood_flat_std_map.insert({i.first, i.second});
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    robin_hood_flat_std_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_DEFINE_F(BMFixture, BM_RobinHoodFlatAbseilMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i : updates)
        {
            auto it = robin_hood_flat_absl_map.find(i.first);
            if (it == robin_hood_flat_absl_map.end())
            {
                robin_hood_flat_absl_map.insert({i.first, i.second});
            }
            else
            {
                if (it->second.prc == i.second.prc && it->second.qty == i.second.qty)
                    robin_hood_flat_absl_map.erase(it);
                else
                    it->second = i.second;
            }
        }
    }
}

BENCHMARK_REGISTER_F(BMFixture, BM_StdUnorderedMap);
BENCHMARK_REGISTER_F(BMFixture, BM_FlatHashMap);
BENCHMARK_REGISTER_F(BMFixture, BM_BoostUnorderedMap);
BENCHMARK_REGISTER_F(BMFixture, BM_BoostPoolUnorderedMap);
BENCHMARK_REGISTER_F(BMFixture, BM_RobinHoodStdMap);
BENCHMARK_REGISTER_F(BMFixture, BM_RobinHoodAbseilMap);
BENCHMARK_REGISTER_F(BMFixture, BM_RobinHoodFlatStdMap);
BENCHMARK_REGISTER_F(BMFixture, BM_RobinHoodFlatAbseilMap);

BENCHMARK_MAIN();
