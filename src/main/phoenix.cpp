#include<iostream>


#include <spdlog/spdlog.h>
#include<phoenix/core/rng.h>
#include<tfm/tinyformat.h>


int main()
{
    phoenix::RNG r;
    for(int i=0;i<10;i++)
    {
        spdlog::info(r.nextInt());
    }
    spdlog::info(tinyformat::format("%d %d",3,2));
}