/* 
 *    Copyright [2012] Olaf - blinky0815 - christ ]
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
 */

#ifndef MOTOR_H
#define MOTOR_H
#include <libfreenect.h>
#include <iostream>
#include <noncopyable.h>
#include <stdexcept>

using namespace std;

class Motor : public Noncopyable {
public:
    Motor(freenect_device* ptr);
    virtual ~Motor() noexcept(false);
    double getTilt();
    void setTilt(double value);
    void update();
private:
    freenect_device *devicePtr;
};

#endif /* MOTOR_H */

