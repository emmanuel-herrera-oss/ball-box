#ifndef __SOUND_HPP
#define __SOUND_HPP

struct Sound {
	double frequency;
	int lastI = 1;
	double sample;
	long long startTime;
	double time;
	bool noDecay = false;
	int duration;
	double timeTotal = 0;
};

#endif