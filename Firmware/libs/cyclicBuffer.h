/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      Cyclic buffer template class

 @authors    Pawel Okas, Kubaszek Mateusz
 created on: 13-03-2013
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2013, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *//* ========================================================================================================================== */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>

template<class T>
class CyclicBuffer {
public:
    constexpr CyclicBuffer(T * const data, size_t size) :
    	dataTab(data), size(size), writePos(0), readPos(0), count(0) {
    }
    inline bool append(T data) __attribute__ ((always_inline)) {
    	if(isFull() == false){
    		count++;
    		writePos = (++writePos) % size;
    		dataTab[writePos] = data;
    		return true;
    	}
    	return false;
    }
    inline size_t write(const T *data, size_t size){
    	if(isFull() == false){
    		if(size > getFreeSpace()) size = getFreeSpace();
    		//calculate space of first part
    		size_t firstPart = this->size - writePos - 1;

    		if(firstPart >= size){
    			memcpy(dataTab + writePos + 1, data, size*sizeof(T));
    			writePos += size;
    		} else {
    			memcpy(dataTab + writePos + 1, data, firstPart * sizeof(T));
    			memcpy(dataTab, data + firstPart, (size - firstPart) * sizeof(T));
    			writePos = size - firstPart - 1;
    		}

    		count += size;
    		return size;
    	}
    	return 0;
    }
    inline size_t read(T *data, size_t size){
    	if(isEmpty() == false){
    		if(size > getLength()) size = getLength();
    		// calculate first part size
			size_t firstPart = this->size - readPos - 1;

			if (firstPart >= size) {
				memcpy(data, dataTab + readPos + 1, size * sizeof(T));
				readPos += size;
			} else {
				memcpy(data, dataTab + readPos + 1, firstPart * sizeof(T));
				memcpy(data + firstPart, dataTab, (size - firstPart) * sizeof(T));
				readPos = size - firstPart - 1;
			}

			count -= size;
			return size;
    	}
    	return 0;
    }
    inline void append_unsafe(T data) __attribute__ ((always_inline)) {
    	count++;
    	writePos = (++writePos) % size;
        dataTab[writePos] = data;
    }
    inline T get(void) __attribute__ ((always_inline)) {
    	if(isNotEmpty()){
    		count--;
    		readPos = (++readPos) % size;
    		return dataTab[readPos];
    	}
    }
    inline T get_unsafe(void) __attribute__ ((always_inline)) {
    	count--;
    	readPos = (++readPos) % size;
    	return dataTab[readPos];
    }

    inline size_t copyTo(T *data, size_t maxDataSize) {
        size_t dataToCopy;
        if(maxDataSize >= getSize()) {
            dataToCopy = getSize();
        } else {
            dataToCopy = maxDataSize;
        }
        //calculate space of first part
        size_t firstPart = this->size - ((readPos + 1) % size);

        if(firstPart >= dataToCopy){
            memcpy(data, getDataPointer(), maxDataSize * sizeof(T));
        } else {
            memcpy(data, getDataPointer(), firstPart * sizeof(T));
            memcpy(data + firstPart, dataTab, (dataToCopy - firstPart) * sizeof(T));
        }

        return dataToCopy;
    }

    inline bool isEmpty(void) const __attribute__ ((always_inline)) {
        return count == 0;
    }
    inline bool isNotEmpty(void)const __attribute__ ((always_inline)) {
        return count != 0;
    }
    inline bool isFull(void) __attribute__ ((always_inline)) {
        return count == size;
    }
    inline void flush(void) __attribute__ ((always_inline)) {
        writePos = 0;
        readPos = 0;
        count = 0;
    }
    //todo optimalize
    inline size_t getLength(void) __attribute__ ((always_inline)) {
       return count;
    }
    inline size_t getSize(void) const __attribute__ ((always_inline)) {
        return size;
    }
    inline size_t getFreeSpace() __attribute__ ((always_inline)) {
    	return getSize() - getLength();
    }
    inline size_t moveWritePointer(size_t positions) __attribute__ ((always_inline)) {
    	if(positions > getFreeSpace()){
    		positions = getFreeSpace();
    	}
    	writePos = (writePos + positions) % size;
    	count += positions;
    	return positions;
    }

private:
    T * const dataTab;
    const size_t size;
    volatile size_t writePos, readPos, count;

    T *getDataPointer(){
    	return &dataTab[(readPos + 1) % size];
    }

    T *getBufferPointer(){
    	return dataTab;
    }
};


template<class T, size_t size>
class CyclicBuffer_data : public CyclicBuffer<T> {
public:
	CyclicBuffer_data(): CyclicBuffer<T>(data, size) {
	}
private:
	T data[size];
};
#endif /* BUFFER_H_ */
