#include "../threadsafe_queue.hpp"
#include "Runtime_monitor.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>

int main() {
    int n_pushth = 40, n_popth = 40, limit = 1000000;
    threadsafe_queue<int> q;
    std::vector<std::atomic<int>> freq(limit+1);
    auto pushele = [&q, &limit, &freq](int ind) {
        thread_local int ele = 1; 
        while(ele<=limit) {
            q.push(ele);
            freq[ele]++;
            ele++;
            // sleep(1);
        }
    };
    auto popele = [&q, &limit, &freq](int ind) {
        int i = 0; 
        while(i<limit) {
            int ele;
            q.wait_and_pop(ele);
            freq[ele]--;
            i++;
            // sleep(1);
        }
    };

    std::vector<std::thread> threads;

    for(int ind = 1; ind<=(n_popth+n_pushth); ind++) {
        if(ind<=n_pushth) threads.push_back(std::thread(pushele, ind));
        else threads.push_back(std::thread(popele, ind));
    }

    for(auto &it:threads) it.join();

    bool ch = true;
    for(auto &it:freq) {
        if(it != 0) {
            ch = false;
            break;
        }
    } 

    if(ch) std::cout<<"queue worked correctly\n";
    else std::cout<<"queue did not worl correctly\n";

    return 0;
}