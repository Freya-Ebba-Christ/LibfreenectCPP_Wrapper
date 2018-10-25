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

#ifndef MICROPHONE_H
#define MICROPHONE_H
#include <stdlib.h>
#include <libfreenect.h>
#include <libfreenect_audio.h>
#include <iostream>
#include <noncopyable.h>
#include <stdexcept>
#include <string.h>
#include <signal.h>
#include <pthread.h>

typedef struct {
    int32_t * buffers[4];
    int max_samples;
    int current_idx;
    int new_data;
} capture;

static pthread_mutex_t audiobuf_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t audiobuf_cond = PTHREAD_COND_INITIALIZER;

using namespace std;

class Microphone : public Noncopyable {
public:
    Microphone(freenect_device* ptr);
    virtual ~Microphone() noexcept(false);
    void startAudio();
    void stopAudio();
    static void audioCallback(int num_samples, int32_t* mic1, int32_t* mic2, int32_t* mic3, int32_t* mic4, int16_t* cancelled, void *unknown);

private:
    freenect_device *devicePtr;
    capture captureState;

    static void freenect_audio_callback(freenect_device* dev, int num_samples, int32_t* mic1, int32_t* mic2, int32_t* mic3, int32_t* mic4, int16_t* cancelled, void *unknown) {
        cout << "if you see this, you're either running the selftest or you forgot to use your own class" << endl;
        pthread_mutex_lock(&audiobuf_mutex);
        capture* c = (capture*) freenect_get_user(dev);
        if (num_samples < c->max_samples - c->current_idx) {
            memcpy(&(c->buffers[0][c->current_idx]), mic1, num_samples * sizeof (int32_t));
            memcpy(&(c->buffers[1][c->current_idx]), mic2, num_samples * sizeof (int32_t));
            memcpy(&(c->buffers[2][c->current_idx]), mic3, num_samples * sizeof (int32_t));
            memcpy(&(c->buffers[3][c->current_idx]), mic4, num_samples * sizeof (int32_t));
        } else {
            int first = c->max_samples - c->current_idx;
            int left = num_samples - first;
            memcpy(&(c->buffers[0][c->current_idx]), mic1, first * sizeof (int32_t));
            memcpy(&(c->buffers[1][c->current_idx]), mic2, first * sizeof (int32_t));
            memcpy(&(c->buffers[2][c->current_idx]), mic3, first * sizeof (int32_t));
            memcpy(&(c->buffers[3][c->current_idx]), mic4, first * sizeof (int32_t));
            memcpy(c->buffers[0], &mic1[first], left * sizeof (int32_t));
            memcpy(c->buffers[1], &mic2[first], left * sizeof (int32_t));
            memcpy(c->buffers[2], &mic3[first], left * sizeof (int32_t));
            memcpy(c->buffers[3], &mic4[first], left * sizeof (int32_t));
        }
        c->current_idx = (c->current_idx + num_samples) % c->max_samples;
        c->new_data = 1;
        pthread_cond_signal(&audiobuf_cond);
        pthread_mutex_unlock(&audiobuf_mutex);
        audioCallback(num_samples, mic1, mic2, mic3, mic4, cancelled, unknown);
    }
};

#endif /* MICROPHONE_H */

