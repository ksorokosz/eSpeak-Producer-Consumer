/*
 * Stream.h
 *
 *  Created on: 05-03-2016
 *      Author: Kamil Sorokosz
 */

#ifndef STREAM_H_
#define STREAM_H_

class Stream
{
	public:
	
		Stream() {}
		virtual ~Stream() {};
		
		virtual void init(void* configuration) = 0;
		virtual void push(void* data, int length) = 0;
		virtual void close() = 0;
		virtual bool isopen() = 0;
};

#endif /* STREAM_H */