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
#include <microphone.h>

Microphone::Microphone(freenect_device* ptr) {
    devicePtr = ptr;

    captureState.max_samples = 256 * 60;
    captureState.current_idx = 0;
    captureState.buffers[0] = (int32_t*) malloc(captureState.max_samples * sizeof (int32_t));
    captureState.buffers[1] = (int32_t*) malloc(captureState.max_samples * sizeof (int32_t));
    captureState.buffers[2] = (int32_t*) malloc(captureState.max_samples * sizeof (int32_t));
    captureState.buffers[3] = (int32_t*) malloc(captureState.max_samples * sizeof (int32_t));
    memset(captureState.buffers[0], 0, captureState.max_samples * sizeof (int32_t));
    memset(captureState.buffers[1], 0, captureState.max_samples * sizeof (int32_t));
    memset(captureState.buffers[2], 0, captureState.max_samples * sizeof (int32_t));
    memset(captureState.buffers[3], 0, captureState.max_samples * sizeof (int32_t));
    freenect_set_user(devicePtr, &captureState);
    freenect_set_audio_in_callback(devicePtr, freenect_audio_callback);
}

Microphone::~Microphone() noexcept(false){
    if (freenect_close_device(devicePtr) < 0) {
        throw std::runtime_error("could not close the audio device");
    }
}

void Microphone::startAudio() {
    if (freenect_start_audio(devicePtr) < 0) throw std::runtime_error("Cannot start video callback");
}

void Microphone::stopAudio() {
    if (freenect_stop_audio(devicePtr) < 0) throw std::runtime_error("Cannot stop video callback");
}

void Microphone::audioCallback(int num_samples, int32_t* mic1, int32_t* mic2, int32_t* mic3, int32_t* mic4, int16_t* cancelled, void *unknown) {
    cout << "if you see this, you're either running the selftest or you forgot to use your own class" << endl;
}