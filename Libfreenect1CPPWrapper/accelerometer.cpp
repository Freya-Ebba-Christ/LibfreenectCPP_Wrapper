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
#include <accelerometer.h>

Accelerometer::Accelerometer(freenect_device* ptr) {
    devicePtr = ptr;
}

Accelerometer::~Accelerometer() {
}

double Accelerometer::getX() {
    return x;
}

double Accelerometer::getY() {
    return y;
}

double Accelerometer::getZ() {
    return z;
}

void Accelerometer::update() {
    m_state = freenect_get_tilt_state(devicePtr);
    freenect_get_mks_accel(m_state, &this->x, &this->y, &this->z);
}

