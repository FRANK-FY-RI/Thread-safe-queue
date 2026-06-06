#include "threadsafe_queue.hpp"
#include "../../c++/Runtime_monitor.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <unistd.h>

int main() {
    int n_pushth = 40, n_popth = 40, limit = 100;
    threadsafe_queue<int> q;
    auto pushele = [&q, &limit](int ind) {
        thread_local int ele = 1; 
        while(ele<=limit) {
            std::cout<<"Thread "<<ind<<" push: "<<ele<<'\n';
            q.push(ele);
            ele++;
            // sleep(1);
        }
    };
    auto popele = [&q, &limit](int ind) {
        int i = 0; 
        while(i<limit) {
            int ele;
            std::cout<<"Thread "<<ind<<" pop: "<<ele<<'\n';
            q.wait_and_pop(ele);
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

    std::cout<<"remaining elements\n";
    int val;
    while(q.try_pop(val)) {
        std::cout<<val<<'\n';
    }

    return 0;
}