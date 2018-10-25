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

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <libfreenect.h>
#include <iostream>
#include <noncopyable.h>
#include <stdexcept>

using namespace std;

class Accelerometer : public Noncopyable {
public:
    Accelerometer(freenect_device* ptr);
    virtual ~Accelerometer();
    double getX();
    double getY();
    double getZ();
    void update();

private:
    freenect_raw_tilt_state *m_state;
    double x;
    double y;
    double z;
    freenect_device *devicePtr;
};

#endif /* ACCELEROMETER_H */


