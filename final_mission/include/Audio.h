#ifndef FINAL_PROJECT_AUDIO_H
#define FINAL_PROJECT_AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

class Audio {
	ALCdevice  *device;
	ALCcontext *context;
	ALCenum error;
	ALfloat listenerOri[6];
	ALuint source, buffer;
	ALint source_state;
	void checkError(ALenum e, int pos);
public:
	Audio();
	void loadData();
	void play();
	void deleteAudio();
};

#endif
