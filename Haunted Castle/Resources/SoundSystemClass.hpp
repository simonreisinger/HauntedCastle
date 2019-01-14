#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h" // Only if you want error checking
#include <iostream>

typedef FMOD::Sound* SoundClass;



class SoundSystemClass
{
public:
	// Pointer to the FMOD instance
	FMOD::System *m_pSystem;

	SoundSystemClass();

	void createSound(SoundClass *pSound, const char* pFile);

	void playSound(SoundClass pSound, bool bLoop);

	void releaseSound(SoundClass pSound);
};