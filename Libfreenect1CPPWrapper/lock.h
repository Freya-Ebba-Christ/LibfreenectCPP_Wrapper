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

#ifndef LOCK_H
#define	LOCK_H
#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

#include <mutex.h>

class Lock {
public:
    Lock(Mutex &mutex);
    virtual ~Lock();
    operator bool () const;
    void setUnlock();

private:
    Mutex &m_mutex;
    bool m_locked;
};

#endif	/* LOCK_H */

