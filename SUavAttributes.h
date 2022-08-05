#ifndef INC_SUAVATTRIB_H
#define INC_SUAVATTRIB_H

struct UavAttributes
{
	float startPosX,
		  startAltitude,
		  startPosZ;

	float rescuePosX,
		  rescueAltitude,
		  rescuePosZ;

	float startRotX,
		  startRotY,
		  startRotZ;

	float flightSpeed; // normal fightspeed, note this cannot not exceed the 
	                   // UAV max flight speed.
	float turnSpeed; // Speed the plane slows down to in order to turn 
					 // (not actual speed of turn that is fixed.)
	float startFuel; // Effects weight which effects min air speed

	float radarRange;    // To high and it will avoid too much, to low it won't have time to 
	                     // turn/climb
	float radarAspectWidth;
	float radarAspectHeight;
	float radarViewAngle; // To wide and it will avoid too much (ie never get around buildings)
};

#endif //INC_SUAVATTRIB_H