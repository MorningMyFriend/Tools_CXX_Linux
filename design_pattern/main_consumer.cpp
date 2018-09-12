//
// Created by wurui on 18-9-7.
//

#include <iostream>
#include <queue>
#include <vector>
#include <zconf.h>

#include "pthread.h"

using namespace std;

// global var
const int max_que_szie = 5;

pthread_cond_t cond_workline;
pthread_mutex_t mutex_workline;
std::queue<int> WorkLine;

struct Product {
    int threadId;
    string name;
};

void createThreads(const int numThreads, void *(*threadFun)(void *), vector<void *> params){
    pthread_t pthreads[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&pthreads[i], NULL, threadFun, params[i]);
    }
}

void *pthreadFun(void *param) {
    Product product = *(static_cast<Product*>(param));
    char buffer[255];
    for (int i = 0; i < 5; ++i) {
        sprintf(buffer, "thread %d: product is %s NO.%d", product.threadId, product.name.c_str(), i);
        cout << buffer << endl;
    }
}

void *producer(void *param){
    int threadId = *((int *)param);

    for (int i = 0; i < 20; ++i) {
        pthread_mutex_lock(&mutex_workline);  // --- lock work line

        while(WorkLine.size()>=max_que_szie){
            cout << "worline max!!!" << endl;
            pthread_cond_wait(&cond_workline, &mutex_workline); // wait cond and unclock mutex
        }
        WorkLine.push(threadId*10+i);
        pthread_mutex_unlock(&mutex_workline);  // --- unlock work line
        char c[256];
        sprintf(c, "producer thread:%d + %d   \n", threadId, threadId*10+i);
        cout << c;
        sleep(0.01);

    }
}

void *consumer(void *param){
    int threadId = *((int *)param);
    while (true) {
        pthread_mutex_lock(&mutex_workline);      // --- lock work line
        if (WorkLine.empty()){
            pthread_mutex_unlock(&mutex_workline);  // --- unlock work line
            sleep(1);
            continue;
        }
        int item = WorkLine.front();
        int len = WorkLine.size();
        WorkLine.pop();
        pthread_cond_signal(&cond_workline);
        pthread_mutex_unlock(&mutex_workline);      // --- unlock work line
        char c[256];
        sprintf(c, "consumer thread:%d - %d  workline len:%d \n", threadId, item, len);
        cout << c;
        sleep(1);
    }
}

void egProducerConumer(){
    pthread_mutex_init(&mutex_workline, NULL);
    pthread_cond_init(&cond_workline, NULL);

    // producer
    int threadIds[2] = {1,2};
    vector<void *> params = {(void *)&threadIds[0], (void *)&threadIds[1]};
    createThreads(2, producer, params);

    // consumer
    int ids[] = {5,6};
    vector<void *> consumer_params = {(void *)&ids[0], (void *)&ids[1]};
    createThreads(2, consumer, consumer_params);

    pthread_exit(NULL);
}


int main() {
    egProducerConumer();
}
