#include "../threadsafe_queue.hpp"
#include <vector>
#include <thread>
#include <iostream>
#include <queue>
#include <algorithm>
#include "Runtime_monitor.hpp"

int main() {
    const int iterations = 10; 
    const int limit = 1'000'000;
    
    //sequential
    std::queue<int> q;
    double seq_time = 0;
    {
        timer t;
        for(int iter = 0; iter<iterations; iter++) {
            for(int i = 1; i<=limit; i++) {
                q.push(i);
            }
        }
        seq_time = t.elapsed_ms();
    }

    //parallel
    threadsafe_queue<int> tsq;
    double par_time = 0;
    {
        int n_threads = 24;
        int block_size = limit/n_threads;
        double thread_overhead = 0;
        auto block_push = [&tsq] (int start, int end) {
            for(int i = start; i<end; i++) tsq.push(i);
        };
        timer t;
        for(int iter = 0; iter<iterations; iter++) {
            std::vector<std::thread> threads;
            for(int i = 1; i<=limit; i+=block_size) {
                threads.push_back(
                    std::thread(block_push, i, std::min(i+block_size, limit+1))
                );
            }
            for(auto &it:threads) it.join(); 
        }
        par_time = t.elapsed_ms();
    }

    unsigned int seq_sum = 0, par_sum = 0;
    while(!q.empty()) {
        seq_sum += q.front();
        q.pop();
    }
    int count = 0;
    while(true) {
        int a;
        bool ch = tsq.try_pop(a);
        if(ch) par_sum += a;
        else break;
        count++;
    } 

    std::cout<<seq_sum<<'\n'<<par_sum<<'\n';

    std::cout<<"count==limit: "<<(count==iterations*limit)<<'\n';

    std::cout<<"seq time: "<<seq_time<<'\n'<<"par time: "<<par_time<<'\n';

    return 0;
}