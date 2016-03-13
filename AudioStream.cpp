#include "AudioStream.h"

AudioStream::AudioStream(string filepath) : filepath(filepath)
{
	this->pfile = NULL;
	this->samplestotal = 0;
}

void AudioStream::init(void* configuration) throw(exception)
{
		int samplerate = *((int*)(configuration));
		this->init_wave( (char*)filepath.c_str(), samplerate );
		this->samplestotal = 0;
}

void AudioStream::push(void* data, int length) throw(exception)
{
	this->samplestotal += length;
	fwrite(data, length * 2, 1, this->pfile);  // data is short array	
}

void AudioStream::close() throw(exception)
{
	this->close_wave();
}

bool AudioStream::isopen()
{
	return this->pfile != NULL;
}

void AudioStream::init_wave(char *path, int rate) throw (AudioStreamException)
{
	static unsigned char wave_header[44] = {
		'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',
		0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,
		2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};

	if(path == NULL)
		throw AudioStreamException("Unknown Path");

	this->pfile = fopen(path,"wb");

	if(this->pfile == NULL)
		throw AudioStreamException("Can't write to: " + string(path));

	fwrite(wave_header,1,24,this->pfile);
	write2wave(rate);
	write2wave(rate * 2);
	fwrite(&wave_header[32],1,12,this->pfile);
}

void AudioStream::write2wave(int value) throw (AudioStreamException)
{
	int ix;

	if(this->pfile == NULL)
		throw AudioStreamException("Unknown file pointer");
	
	for(ix=0; ix<4; ix++)
	{
		fputc(value & 0xff,this->pfile);
		value = value >> 8;
	}
}

void AudioStream::close_wave() throw (AudioStreamException)
{
	unsigned int pos = 0;
	
	if(this->pfile == NULL)
		throw AudioStreamException("Unknown file pointer");

	fflush(this->pfile);
	pos = ftell(this->pfile);

	fseek(this->pfile,4,SEEK_SET);
	write2wave(pos - 8);

	fseek(this->pfile,40,SEEK_SET);
	write2wave(pos - 44);

	fclose(this->pfile);
	this->pfile = NULL;
}
