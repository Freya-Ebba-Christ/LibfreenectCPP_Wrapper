/* 
 *    Copyright [2018] Olaf - blinky0815 - christ ]
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

#ifndef SYNCHEDCAMERA_H
#define SYNCHEDCAMERA_H

#include <libfreenect.h>
#include <libfreenect_sync.h>
#include <iostream>
#include <noncopyable.h>
#include <stdexcept>
#include <time.h>
#include <vector>
#include <iostream>

class SynchedCamera {
public:

    SynchedCamera() : videoIndex(0), depthIndex(0) {

    }

    void init() {
        setDepthFormat(FREENECT_DEPTH_REGISTERED);
        setVideoFormat(FREENECT_VIDEO_RGB);
    }

    void start() {
        this->videoVector.resize(640 * 480 * 4, 255);
        this->depthVector.resize(640 * 480, 0);
    }

    virtual ~SynchedCamera() {
    }

    const int getRGBframeWidth() {
        return this->width;
    }

    const int getRGBframeHeight() {
        return this->height;
    }

    const int getDepthFrameWidth() {
        return this->width;
    }

    const int getDepthFrameHeight() {
        return this->height;
    }

    const int getRegisteredFrameWidth() {
        return this->width;
    }

    const int getRegisteredFrameHeight() {
        return this->height;
    }

    const int getUndistortedFrameWidth() {
        return this->width;
    }

    const int getUndistortedFrameHeight() {
        return this->height;
    }

    const int getRGBvideoTimeStamp() {
        return this->videoTimestamp;
    }

    const int getDepthVideoTimeStamp() {
        return this->depthTimestamp;
    }

    void getSynchedFrames() {
        uint16_t *depth = 0;
        char *video = 0;
        getSynchedVideoAndDepth((void**) &video, (void**) &depth, &videoTimestamp, &depthTimestamp);
        std::copy(depth, depth + depthVector.size(), depthVector.begin());

        // replace with AVX code
        for (int y = 0; y < 480; ++y) {
            for (int x = 0; x < 640; ++x) {
                int p = y * 640 + x;
                videoVector[4 * p] = video[3 * p + 2];
                videoVector[4 * p + 1] = video[3 * p + 1];
                videoVector[4 * p + 2] = video[3 * p ];
                videoVector[4 * p + 3] = 255;
            }
        }
    }

    void shutDown() {
        freenect_sync_stop();
    }

    void selftest() {
        short *depth = 0;
        char *video = 0;
        uint32_t ts_video;
        uint32_t ts_depth;

        std::cout << "setting and getting video and depth format" << std::endl;
        setVideoFormat(FREENECT_VIDEO_RGB);
        setDepthFormat(FREENECT_DEPTH_REGISTERED);
        if (getVideoFormat() != FREENECT_VIDEO_RGB) {
            std::cout << "getVideoFormat() does not work" << std::endl;
        }
        if (getDepthFormat() != FREENECT_DEPTH_REGISTERED) {
            std::cout << "getDepthFormat() does not work" << std::endl;
        }

        std::cout << "setting and getting device index" << std::endl;
        setVideoIndex(1);
        setDepthIndex(1);
        if (getVideoIndex() != 1) {
            std::cout << "getting/setting video device index failed" << std::endl;
        }
        if (getVideoIndex() != 1) {
            std::cout << "getting/setting depth device index failed" << std::endl;
        }
        setVideoIndex(0);
        setDepthIndex(0);

        std::cout << "testing getSynchedVideoAndDepth()" << std::endl;
        getSynchedVideoAndDepth((void**) &video, (void**) &depth, &ts_video, &ts_depth);
        std::cout << "timestamp video: " << ts_video << std::endl;
        std::cout << "timestamp depth: " << ts_depth << std::endl;

        std::cout << "testing getSynchedVideo" << std::endl;
        getSynchedVideo((void**) &video, &ts_video);
        std::cout << "timestamp video: " << ts_video << std::endl;

        std::cout << "testing getSynchedDepth()" << std::endl;
        getSynchedDepth((void**) &depth, &ts_depth);
        std::cout << "timestamp depth: " << ts_depth << std::endl;

        static unsigned int indices[480][640];
        static unsigned int indices_[640 * 480];
        static short xyz[480][640][3];
        static short xyz_[640 * 480 * 3];
    }

    std::vector<uint8_t>& getRegisteredData() {
        return this->videoVector;
    }

    std::vector<uint16_t>& getDepthData() {
        return this->depthVector;
    }

    uint32_t getDepthTimestamp() {
        return this->depthTimestamp;
    }

    uint32_t getVideoTimestamp() {
        return this->videoTimestamp;
    }

private:
    freenect_video_format fmt_video;
    freenect_depth_format fmt_depth;
    int videoIndex;
    int depthIndex;
    uint32_t videoTimestamp;
    uint32_t depthTimestamp;
    std::vector<uint16_t> depthVector;
    std::vector<uint8_t> videoVector;
    int width = 640;
    int height = 480;
    uint32_t rgbVideoTimeStamp;
    uint32_t depthVideoTimeStamp;

    void setVideoFormat(freenect_video_format fmt) {
        fmt_video = fmt;
    }

    void setDepthFormat(freenect_depth_format fmt) {
        fmt_depth = fmt;
    }

    freenect_video_format getVideoFormat() {
        return fmt_video;
    }

    freenect_depth_format getDepthFormat() {
        return fmt_depth;
    }

    void setVideoIndex(int value) {
        videoIndex = value;
    }

    void setDepthIndex(int value) {
        depthIndex = value;
    }

    int getVideoIndex() {
        return videoIndex;
    }

    int getDepthIndex() {
        return depthIndex;
    }

    void getSynchedVideoAndDepth(void **video, void **depth, uint32_t *timestamp_video, uint32_t *timestamp_depth) {
        if (freenect_sync_get_depth(depth, timestamp_depth, depthIndex, this->fmt_depth) < 0) {
            throw std::runtime_error("error getting synched depth data. Kinect not connected?");
        }
        if (freenect_sync_get_video(video, timestamp_video, videoIndex, this->fmt_video) < 0) {
            throw std::runtime_error("error getting synched video data. Kinect not connected?");
        }
    }

    void getSynchedVideo(void **video, uint32_t *timestamp_video) {
        if (freenect_sync_get_video(video, timestamp_video, videoIndex, this->fmt_video) < 0) {
            throw std::runtime_error("error getting synched video data. Kinect not connected?");
        }
    }

    void getSynchedDepth(void **depth, uint32_t *timestamp_depth) {
        if (freenect_sync_get_depth(depth, timestamp_depth, depthIndex, this->fmt_depth) < 0) {
            throw std::runtime_error("error getting synched depth data. Kinect not connected?");
        }
    }
};

#endif /* SYNCHEDCAMERA_H */

