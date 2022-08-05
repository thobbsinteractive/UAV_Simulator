// Enumerated Types utilised in the Application
#ifndef INC_ENUMERATED_H
#define INC_ENUMERATED_H

struct RESOLUTION
{
	int Width;
	int Height;
};

enum GAMEDETAIL {DETAIL_LOW,DETAIL_MEDIUM,DETAIL_HIGH};

struct SETTINGS 
{
	GAMEDETAIL GameDetail;
	RESOLUTION Resolution;
	bool Windowed;

};


#endif //INC_ENUMERATED_H