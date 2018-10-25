/* 
 *    Copyright [2013] Olaf - blinky0815 - christ ]
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * 
 * Author: blinky0815
 *
 * see: http://stackoverflow.com/questions/5956759/c11-stdthread-inside-a-class-executing-a-function-member-with-thread-initia
 */

#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <atomic>
#include <thread>
#include <unistd.h>
#include <iostream>

using namespace std;

class Runnable {
public:

    Runnable() : m_thread() {
    }

    virtual ~Runnable() {
        try {
            stop();
        } catch (...) {
            cout << "Runnable says: Something's gone horribly wrong..." << endl;
        }
    }

    Runnable(Runnable const&) = delete;
    Runnable& operator=(Runnable const&) = delete;

    void stop() {
        m_thread.join();
    }

    void start() {
        m_thread = std::thread(&Runnable::run, this);
    }

protected:
    virtual void run() = 0;

private:
    std::thread m_thread;
};

#endif /* RUNNABLE_H */

