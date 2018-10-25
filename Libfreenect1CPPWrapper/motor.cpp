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
#include <motor.h>

Motor::Motor(freenect_device* ptr) {
    devicePtr = ptr;
}

Motor::~Motor() noexcept(false) {
    if (freenect_close_device(devicePtr) < 0) {
        throw std::runtime_error("could not close the notor device");
    }
}

double Motor::getTilt() {
    return freenect_get_tilt_degs(freenect_get_tilt_state(devicePtr));
}

void Motor::setTilt(double value) {
    if (freenect_set_tilt_degs(devicePtr, value) < 0) throw std::runtime_error("Cannot set angle in degrees");
}

void Motor::update() {
    freenect_update_tilt_state(devicePtr);
}