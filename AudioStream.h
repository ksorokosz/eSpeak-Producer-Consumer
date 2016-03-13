/*
 * AudioStream.h
 *
 *  Created on: 05-03-2016
 *      Author: Kamil Sorokosz
 */

#ifndef AUDIO_STREAM_H_
#define AUDIO_STREAM_H_

#include "Stream.h"
#include <string>
#include <exception>
#include <cstdio>
using namespace std;

class AudioStreamException : public exception
{
	public:
		AudioStreamException(string error) : error(error) {}
		virtual ~AudioStreamException() throw() {}

		virtual const char* what() const throw()
		{
			return error.c_str();
		}

	private:
		string error;
};

class AudioStream : public Stream
{
	public:		
		AudioStream(string filepath);
		virtual ~AudioStream() {};
		
		void init(void* configuration) throw(exception);
		void push(void* data, int length) throw(exception);
		void close() throw(exception);
		bool isopen();
		
	protected:
	
		/* Wave recording */
		void init_wave(char *path, int rate) throw (AudioStreamException);
		void write2wave(int value) throw (AudioStreamException);
		void close_wave() throw (AudioStreamException);
		
	private:
		string filepath;
		int samplestotal;
		FILE* pfile;
	
};

#endif /* Audio_STREAM_H */