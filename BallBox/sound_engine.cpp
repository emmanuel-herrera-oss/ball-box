#include "sound_engine.hpp"
#include <SDL.h>
#include <chrono>

void callback(void* userdata, Uint8* stream, int len)
{
	SoundEngine* soundEngine = (SoundEngine*)userdata;
	short* snd = (short*)stream;
	len /= sizeof(*snd);
	long long now = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();


	for (int i = 0;i < len;i++) {
		snd[i] = 0;
	}
	if (soundEngine->paused) {
		return;
	}
	for (int j = 0;j < 16;j++) {
		auto& sound = soundEngine->sounds[j];
		for (int i = 0;i < len;i++) {
			double time = (now - sound.startTime) / 1000000.0;
			sound.sample += sound.frequency * 2 * M_PI / 48000.0;
			if (sound.timeTotal < 10) {
				sound.timeTotal += 1.0 / 48000.0;
			}
			if (sound.sample >= 2 * M_PI) {
				sound.sample -= 2 * M_PI;
			}
			if (sound.noDecay) {
				bool finished = now > (sound.startTime + sound.duration);
				snd[i] += (finished ? 0 : 1000) * sin(sound.sample);
			}
			else {
				snd[i] += (1 - exp(-1 * sound.timeTotal)) * (32768 * sin(sound.sample) * exp(-5 * sound.timeTotal));
			}
		}
	}
}

SoundEngine::SoundEngine(std::shared_ptr<EventQueue> event_queue)
{
	event_queue->subscribe(EVENT_COLLISION, this);
	event_queue->subscribe(EVENT_PAUSE_GAME, this);
	event_queue->subscribe(EVENT_EQUIP_NEXT, this);

	SDL_AudioSpec spec, aspec; // the specs of our piece of "music"
	SDL_zero(spec);
	spec.freq = 48000; //declare specs
	spec.format = AUDIO_S16LSB;
	spec.channels = 1;
	spec.samples = 4096;
	spec.callback = callback;
	spec.userdata = this;

	int sound_id = -1;
	//Open audio, if error, print
	if ((sound_id = SDL_OpenAudioDevice(NULL, 0, &spec, &aspec, 0)) <= 0)
	{
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	SDL_PauseAudioDevice(sound_id, 0);

	this->event_queue = event_queue;
}

void SoundEngine::HandleEvent(const Event* event)
{
	if (event->type == EVENT_COLLISION) {
		auto collisionEvent = dynamic_cast<const CollisionEvent*>(event);
		if (collisionEvent->velocity < 20) return;
		long long startTime = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
		auto& sound = this->sounds[this->sound_buffer_index];
		if (++this->sound_buffer_index >= 16) this->sound_buffer_index = 0;
		sound.startTime = startTime;
		sound.noDecay = false;
		sound.frequency = 440 - 4 * collisionEvent->size_difference;
		sound.timeTotal = 0;
	}
	else if (event->type == EVENT_PAUSE_GAME) {
		this->paused = !this->paused;
	}
	else if (event->type == EVENT_EQUIP_NEXT) {
		long long startTime = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
		long long endTime = startTime + 1000000;
		auto& sound = this->sounds[this->sound_buffer_index];
		if (++this->sound_buffer_index >= 16) this->sound_buffer_index = 0;
		sound.startTime = startTime;
		sound.frequency = 1600;
		sound.noDecay = true;
		sound.duration = 100000;
		sound.timeTotal = 0;
	}
}