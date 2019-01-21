#include "SoundSystemClass.hpp"

using namespace std;

// Source: https://cuboidzone.wordpress.com/2013/07/26/tutorial-implementing-fmod/

SoundSystemClass::SoundSystemClass()
{
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		// Report Error
		cout << "Error System_Create" << endl;
	}

	int driverCount = 0;
	m_pSystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// Report Error
		return;
	}

	// Initialize our Instance with 36 Channels
	m_pSystem->init(36, FMOD_INIT_NORMAL, nullptr);
}

void SoundSystemClass::createSound(SoundClass *pSound, const char* pFile)
{
	m_pSystem->createSound(pFile, NULL, 0, pSound);
}

void SoundSystemClass::playSound(SoundClass pSound, bool bLoop = false, bool paused = false)
{
	if (!bLoop)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	m_pSystem->playSound(pSound, FMOD_DEFAULT, paused, &channel);
}

void SoundSystemClass::releaseSound(SoundClass pSound)
{
	pSound->release();
}

void SoundSystemClass::pauseSound(bool unPause)
{
	bool isPaused;
	channel->getPaused(&isPaused);
	if (isPaused && unPause)
	{
		channel->setPaused(false);
	}
	else if (!isPaused && !unPause)
	{
		channel->setPaused(true);
	}
}