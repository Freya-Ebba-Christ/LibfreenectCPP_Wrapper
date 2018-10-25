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

#ifndef COLORCAMERA_H
#define	COLORCAMERA_H

#include <libfreenect.h>
#include <iostream>
#include <noncopyable.h>
#include <stdexcept>

using namespace std;

class ColorCamera : public Noncopyable {
public:
    ColorCamera(freenect_device* ptr);
    virtual ~ColorCamera() noexcept(false);
    void startVideo();
    void stopVideo();
    void setVideoFormat(freenect_video_format requested_format, freenect_resolution requested_resolution);
    freenect_video_format getVideoFormat();
    freenect_resolution getVideoResolution();
    int getVideoBufferSize();
    virtual void videoCallback(void *video, uint32_t timestamp);

private:
    freenect_video_format videoFormat;
    freenect_resolution videoResolution;
    freenect_device *devicePtr;
    static void freenect_video_callback(freenect_device *dev, void *video, uint32_t timestamp) {
        ((ColorCamera*)freenect_get_user(dev))->videoCallback(video,timestamp);
    }
};

#endif	/* COLORCAMERA_H */

