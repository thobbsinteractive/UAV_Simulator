#ifndef INC_SSIMRESULT_H
#define INC_SSIMRESULT_H

struct SimResults
{
	int UAVIndex;
	bool crashed;
	bool missionComplete;
	float distanceToTarget;
	float timeToReachTarget;

	SimResults()
	{
		crashed = false;
		missionComplete = false;
		distanceToTarget = 0.0f;
		timeToReachTarget = 0.0f;
	}
};

#endif //INC_SSIMRESULT_H