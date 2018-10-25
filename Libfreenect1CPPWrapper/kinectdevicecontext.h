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

#ifndef KINECTDEVICECONTEXT_H
#define	KINECTDEVICECONTEXT_H

#include <iostream>
#include <vector>
#include <cmath>
#include <noncopyable.h>
#include <thread.h>
#include <threadworker.h>
#include <colorcamera.h>
#include <coloranddepthcamera.h>
#include <depthcamera.h>
#include <microphone.h>
#include <motor.h>
#include <led.h>
#include <accelerometer.h>
#include <unistd.h>

class KinectDeviceContext : Noncopyable, ThreadWorker {
public:
    KinectDeviceContext(int index);
    virtual ~KinectDeviceContext();
    void execute();
    void close();
    bool isOpen();
    void startSelfTest();
    void testColorCamera();
    void testDepthCamera();
    void testDepthAndColorCamera();
    void testAccelerometer();
    void testMotor();
    void testLed();
    void testMicrophone();

    template <typename T> inline T* createColorCamera() {

        if (isOpen()) {
            if (cameraDevicePtr == NULL) {
                cout << "opening camera" << endl;
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_CAMERA);
                if (freenect_open_device(deviceContextPtr, &cameraDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open video camera");
            }
            return new T(cameraDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

    template <typename T> inline T* createDepthCamera() {
        if (isOpen()) {
            if (cameraDevicePtr == NULL) {
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_CAMERA);
                if (freenect_open_device(deviceContextPtr, &cameraDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open depth camera");
            }
            return new T(cameraDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

    template <typename T> inline T* createColorAndDepthCamera() {

        if (isOpen()) {
            if (cameraDevicePtr == NULL) {
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_CAMERA);
                if (freenect_open_device(deviceContextPtr, &cameraDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open camera");
            }
            return new T(cameraDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

    Accelerometer* createAccelerometer() {
        if (isOpen()) {
            if (motorDevicePtr == NULL) {
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_MOTOR);
                if (freenect_open_device(deviceContextPtr, &motorDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open accelerometer");
            }
            return new Accelerometer(motorDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

    Motor * createMotor() {
        if (isOpen()) {
            if (motorDevicePtr == NULL) {
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_MOTOR);
                if (freenect_open_device(deviceContextPtr, &motorDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open motor");
            }
            return new Motor(motorDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

    Led * createLed() {
        if (isOpen()) {
            if (motorDevicePtr == NULL) {
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_MOTOR);
                if (freenect_open_device(deviceContextPtr, &motorDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open led");
            }
            return new Led(motorDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

    template <typename T> inline T* createMicrophone() {
        if (isOpen()) {
            if (microphoneDevicePtr == NULL) {
                freenect_select_subdevices(deviceContextPtr, (freenect_device_flags) FREENECT_DEVICE_AUDIO);
                if (freenect_open_device(deviceContextPtr, &microphoneDevicePtr, deviceIndex) < 0) throw std::runtime_error("Cannot open microphone");
            }
            return new T(microphoneDevicePtr);
        }
        throw std::runtime_error("device not open");
        return NULL;
    }

private:
    freenect_context *deviceContextPtr;
    bool open;
    freenect_device *cameraDevicePtr;
    freenect_device *motorDevicePtr;
    freenect_device *microphoneDevicePtr;
    Thread updateThread;
    int deviceIndex;

    //these are only used during selftests
    ColorCamera* colorCamera;
    ColorAndDepthCamera* colorAndDepthCamera;
    DepthCamera* depthCamera;
    Motor* motor;
    Led* led;
    Accelerometer* accelerometer;
    Microphone* microphone;
    bool selftest;
};

#endif	/* KINECTDEVICECONTEXT_H */

