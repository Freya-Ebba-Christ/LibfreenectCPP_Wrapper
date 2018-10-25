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

#include <depthcamera.h>

DepthCamera::DepthCamera(freenect_device* ptr) :depthResolution(FREENECT_RESOLUTION_MEDIUM), depthFormat(FREENECT_DEPTH_11BIT){
    devicePtr = ptr;
    freenect_set_user(devicePtr, this);
    freenect_set_depth_mode(devicePtr, freenect_find_depth_mode(getDepthResolution(), getDepthFormat()));
    freenect_set_depth_callback(devicePtr, freenect_depth_callback);
}

DepthCamera::~DepthCamera() noexcept(false){
    stopDepth();
    if (freenect_close_device(devicePtr) < 0) {
        throw std::runtime_error("could not close the depth camera device");
    }
}

void DepthCamera::startDepth() {
    if (freenect_start_depth(devicePtr) < 0) throw std::runtime_error("Cannot start depth callback");
}

void DepthCamera::stopDepth() {
    if (freenect_stop_depth(devicePtr) < 0) throw std::runtime_error("Cannot stop depth callback");
}

void DepthCamera::setDepthFormat(freenect_depth_format requested_format, freenect_resolution requested_resolution) {
    if (requested_format != getDepthFormat() || requested_resolution != getDepthResolution()) {
        freenect_stop_depth(devicePtr);
        freenect_frame_mode mode = freenect_find_depth_mode(requested_resolution, requested_format);
        if (!mode.is_valid) throw std::runtime_error("Cannot set depth format: invalid mode");
        if (freenect_set_depth_mode(devicePtr, mode) < 0) throw std::runtime_error("Cannot set depth format");
        freenect_start_depth(devicePtr);
        depthFormat = requested_format;
        depthResolution = requested_resolution;
    }
}

freenect_depth_format DepthCamera::getDepthFormat() {
    return depthFormat;
}

freenect_resolution DepthCamera::getDepthResolution() {
    return depthResolution;
}

int DepthCamera::getDepthBufferSize() {
    return freenect_get_current_depth_mode(devicePtr).bytes;
}

void DepthCamera::depthCallback(void *depth, uint32_t timestamp){
    cout << "if you see this, you're either running the selftest or you forgot to use your own class" << endl;
}