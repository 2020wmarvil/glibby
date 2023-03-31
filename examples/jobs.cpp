#include "glibby/job/job_system.h"

#include <iostream>
#include <chrono>
#include <string>
 
using namespace std;
 
void Spin(float milliseconds)
{
    milliseconds /= 1000.0f;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    double ms = 0;
    while (ms < milliseconds)
    {
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        ms = time_span.count();
    }
}
 
struct timer
{
    string name;
    chrono::high_resolution_clock::time_point start;
 
    timer(const string& name) : name(name), start(chrono::high_resolution_clock::now()) {}
    ~timer()
    {
        auto end = chrono::high_resolution_clock::now();
        cout << name << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " milliseconds" << endl;
    }
};
 
int main()
{
    glibby::job::Initialize();
 
    // Serial test
    {
        auto t = timer("Serial test: ");
        Spin(100);
        Spin(100);
        Spin(100);
        Spin(100);
        Spin(100);
    }
 
    // Execute test
    {
        auto t = timer("Execute() test: ");
        glibby::job::Execute([] { Spin(100); });
        glibby::job::Execute([] { Spin(100); });
        glibby::job::Execute([] { Spin(100); });
        glibby::job::Execute([] { Spin(100); });
        glibby::job::Execute([] { Spin(100); });
        glibby::job::Wait();
    }

    struct Data
    {
        float m[16];
        void Compute(uint32_t value)
        {
            for (int i = 0; i < 16; ++i)
            {
                m[i] += float(value + i);
            }
        }
    };
    uint32_t dataCount = 1000000;
 
    // Loop test:
    {
        Data* dataSet = new Data[dataCount];
        {
            auto t = timer("Loop test: ");
     
            for (uint32_t i = 0; i < dataCount; ++i)
            {
                dataSet[i].Compute(i);
            }
        }
        delete[] dataSet;
    }
     
    // Dispatch test:
    {
        Data* dataSet = new Data[dataCount];
        {
            auto t = timer("Dispatch() test: ");
     
            const uint32_t groupSize = 10000;
            glibby::job::Dispatch(dataCount, groupSize, [&dataSet](glibby::job::JobDispatchArgs args) {
                dataSet[args.jobIndex].Compute(1);
            });
            glibby::job::Wait();
        }
        delete[] dataSet;
    }
 
    return 0;
}
