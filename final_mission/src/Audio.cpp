#include "Audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <stdio.h>


Audio::Audio() {
	listenerOri[0] = listenerOri[2] = listenerOri[3] = listenerOri[4] = 0.0;
	listenerOri[1] = listenerOri[5] = 1.0f;
}

void Audio::loadData() {
	this->device = alcOpenDevice(NULL); // NULL implica el dispositivo por default
	// Abre dispositivo de audio
	if (!device) {
			printf("No se pudo abrir el dispositivo de audio.\n");
			return;
	} else {
		printf("Dispositivo de audio abierto correctamente\n");
	}
	// Crea el contexto
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		printf("Error al crear al contexto\n");
	}
	checkError(alGetError(), 0);

	// Genera la fuente
	alGenSources((ALuint)1, &source);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_TRUE);
	checkError(alGetError(), 4);

	// Inicializar Listener
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);
	checkError(alGetError(), 3);


	// Genera el buffer para el audio
	buffer = alutCreateBufferFromFile("../res/Sound/jazz_piano.wav");
	if ( alutGetError() != ALUT_ERROR_NO_ERROR ) {
		printf("Error al cargar el audio\n");
	}

	checkError(alGetError(), 5);
	//Ligando buffer con source
	alSourcei(source, AL_BUFFER, buffer);
	checkError(alGetError(), 6);
}

void Audio::play() {
	alSourcePlay(source);
}

void Audio::deleteAudio() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void Audio::checkError(ALenum e, int pos) {
	if(e != AL_NO_ERROR) {
		printf("Algo salió mal %d\n", pos);
	}
}
