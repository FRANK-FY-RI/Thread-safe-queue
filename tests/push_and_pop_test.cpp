#include "../threadsafe_queue.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

int main() {
    int n_pushth = 40, n_popth = 40, limit = 10000000;
    threadsafe_queue<int> q;
    std::atomic<int> ele = 0, consumed = 0;
    std::vector<std::atomic<int>> freq(limit+1);
    auto pushele = [&](int ind) {
        while(true) {
            int id = ++ele;
            if(id>limit) break;
            q.push(id);
            freq[id]++;
        }
    };
    auto popele = [&](int ind) {
        while(true) {
            int idx = ++consumed;
            if(idx>limit) break;
            int val;
            q.wait_and_pop(val);
            freq[val]--;
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
    else std::cout<<"queue did not work correctly\n";

    return 0;
}