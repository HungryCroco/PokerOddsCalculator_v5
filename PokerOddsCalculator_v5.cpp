#pragma once
#include <iostream>
#include <chrono>
#include "LuTables.h"
#include "ComputeHandPower.h"
#include "ComputeOdds.h"
#include "Hand.h"
#include <string>


int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    
    std::unique_ptr<LuTables> luTables = std::make_unique<LuTables>(); // move luTables to heap;
  
    std::string h0s{ "AsAhKsKdQsJd3d" };
    Hand h0{ h0s };

    std::string h1s{ "2c2d" }; //5c4c3c9c
    Hand h1{ h1s };
    std::string h2s{ "AhKd" };
    Hand h2{ h2s };
    std::string h3s{ "9s7s" };
    Hand h3{ h3s };


     
    //Odds o1 = computeOdds(h2, h3, *luTables);

    Odds o1 = computeOdds(h1, h2, h3, *luTables);
    

    //// Use for performance optimizations;
    //for (size_t i = 0; i < 10000000; i++)
    //{
    //    //Hand h0{ h0s };
    //    //h0.CalculateHandMasks();
    //    //h0.Reset();

    //    //ComputePairs(h0);

    //    //ComputeStraight(h0);
    //    //ComputeFlushAndStraightFlushPlus(h0);

    //}

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start)*1e-6;


    std::cout << " " << h1s << "      " << h2s << "      " << h3s << std::endl;
    std::cout << o1.hand_1 << "   " << o1.hand_2 << "   " << o1.hand_3 << std::endl;
    std::cout << duration.count() << " sec" << std::endl;
    std::cout << std::endl;

    return 0;
}

