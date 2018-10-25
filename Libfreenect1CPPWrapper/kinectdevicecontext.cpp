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

#include <kinectdevicecontext.h>
#include <stdexcept>
#include <unistd.h>

using namespace std;

KinectDeviceContext::KinectDeviceContext(int index) : open(false), cameraDevicePtr(NULL), motorDevicePtr(NULL), microphoneDevicePtr(NULL), selftest(false) {
    if (freenect_init(&deviceContextPtr, NULL) < 0) {
        throw std::runtime_error("Cannot initialize freenect library");
    } else {
        deviceIndex = index;
        open = true;
    }

    this->setActive(true);
    updateThread.setThreadWorker(this);
    updateThread.start();
    cout << "started " << endl;
}

KinectDeviceContext::~KinectDeviceContext() {
    close();
}

void KinectDeviceContext::execute() {

    if (isOpen()) {
        if (freenect_process_events(deviceContextPtr) < 0) {
            throw std::runtime_error("unable to process freenect events");
        }
    }
}

void KinectDeviceContext::close() {
    this->setActive(false);
    updateThread.stop();
    if (freenect_shutdown(deviceContextPtr) < 0) {
        throw std::runtime_error("unable to shutdown properly");
    } else {
        open = false;
    }
}

bool KinectDeviceContext::isOpen() {
    return open;
}

void KinectDeviceContext::startSelfTest() {
    selftest = true;
    cout << "checking for devicecontext" << endl;
    cout << "is Kinect open? " << this->isOpen() << endl;

    testMotor();
    testAccelerometer();
    testLed();
    testColorCamera();
    testDepthCamera();
    testDepthAndColorCamera();
    //testMicrophone();
    cout << "done!" << endl;
}

void KinectDeviceContext::testDepthAndColorCamera() {
    cout << "creating color and depth camera: " << endl;
    colorAndDepthCamera = createColorAndDepthCamera<ColorAndDepthCamera > ();
    cout << "testing color camera: " << endl;
    cout << "start color camera: " << endl;
    colorAndDepthCamera->startVideo();
    sleep(3);
    cout << "stop color camera: " << endl;
    colorAndDepthCamera->stopVideo();

    cout << "testing depth camera: " << endl;
    cout << "start depth camera: " << endl;
    colorAndDepthCamera->startDepth();
    sleep(3);
    cout << "stop depth camera: " << endl;
    colorAndDepthCamera->stopDepth();
    if (!selftest) {
        delete colorAndDepthCamera;
    }
}

void KinectDeviceContext::testColorCamera() {
    cout << "creating color camera: " << endl;
    colorCamera = createColorCamera<ColorCamera > ();
    cout << "testing color camera: " << endl;
    cout << "start color camera: " << endl;
    colorCamera->startVideo();
    sleep(3);
    cout << "stop color camera: " << endl;
    colorCamera->stopVideo();
    if (!selftest) {
        delete colorCamera;
    }
}

void KinectDeviceContext::testDepthCamera() {
    cout << "creating depth camera: " << endl;
    depthCamera = createDepthCamera<DepthCamera > ();
    cout << "testing depth camera: " << endl;
    cout << "start depth camera: " << endl;
    depthCamera->startDepth();
    sleep(3);
    cout << "stop depth camera: " << endl;
    depthCamera->stopDepth();
    if (!selftest) {
        delete depthCamera;
    }
}

void KinectDeviceContext::testAccelerometer() {
    cout << "creating accelerometer: " << endl;
    accelerometer = createAccelerometer();
    cout << "testing accelerometer: " << endl;
    accelerometer->update();
    cout << "x: " << accelerometer->getX() << " y: " << accelerometer->getY() << " z: " << accelerometer->getZ() << endl;
    if (!selftest) {
        delete accelerometer;
    }
}

void KinectDeviceContext::testMotor() {
    motor = createMotor();
    cout << "testing motor: " << endl;
    motor->setTilt(0);
    sleep(3);
    motor->update();
    cout << "tilt: " << motor->getTilt() << endl;
    motor->setTilt(20);
    sleep(3);
    motor->update();
    cout << "tilt:" << motor->getTilt() << endl;
    motor->setTilt(-20);
    sleep(3);
    motor->update();
    cout << "tilt:" << motor->getTilt() << endl;
    motor->setTilt(0);
    sleep(3);
    motor->update();
    cout << "tilt:" << motor->getTilt() << endl;
    if (!selftest) {
        delete motor;
    }
}

void KinectDeviceContext::testLed() {
    cout << "creating LED: " << endl;
    led = createLed();
    cout << "OFF " << endl;
    led->setColor(LED_OFF);
    sleep(3);
    cout << "LED_BLINK_GREEN " << endl;
    led->setColor(LED_BLINK_GREEN);
    sleep(3);
    cout << "LED_BLINK_RED_YELLOW " << endl;
    led->setColor(LED_BLINK_RED_YELLOW);
    sleep(3);
    cout << "LED_RED " << endl;
    led->setColor(LED_RED);
    sleep(3);
    cout << "LED_YELLOW " << endl;
    led->setColor(LED_YELLOW);
    sleep(3);
    cout << "LED_GREEN " << endl;
    led->setColor(LED_GREEN);
    sleep(3);
    if (!selftest) {
        delete led;
    }
}

void KinectDeviceContext::testMicrophone() {
    cout << "creating microphone array: " << endl;
    microphone = createMicrophone<Microphone > ();
    cout << "testing microphone array: " << endl;
    cout << "starting microphone array " << endl;
    microphone->startAudio();
    sleep(3);
    cout << "stop audio: " << endl;
    microphone->stopAudio();
    sleep(3);
    if (!selftest) {
        delete microphone;
    }
}