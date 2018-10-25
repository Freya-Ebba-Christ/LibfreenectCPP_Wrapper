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

#ifndef DEPTHCAMERA_H
#define	DEPTHCAMERA_H

#include <libfreenect.h>
#include <iostream>
#include <noncopyable.h>
#include <stdexcept>

using namespace std;

class DepthCamera : public Noncopyable {
public:
    DepthCamera(freenect_device* ptr);
    virtual ~DepthCamera() noexcept(false);
    void startDepth();
    void stopDepth();
    void setDepthFormat(freenect_depth_format requested_format, freenect_resolution requested_resolution);
    freenect_depth_format getDepthFormat();
    freenect_resolution getDepthResolution();
    int getDepthBufferSize();
    virtual void depthCallback(void *depth, uint32_t timestamp);

private:
    freenect_depth_format depthFormat;
    freenect_resolution depthResolution;
    freenect_device *devicePtr;

    static void freenect_depth_callback(freenect_device *dev, void *depth, uint32_t timestamp) {
        ((DepthCamera*)freenect_get_user(dev))->depthCallback(depth,timestamp);
    }
};

#endif	/* DEPTHCAMERA_H */

