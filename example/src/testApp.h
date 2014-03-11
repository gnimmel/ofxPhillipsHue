#pragma once

#include "ofMain.h"
#include "ofxHueLightbulb.h"  
#include "json.h"

class testApp : public ofBaseApp
{
public:
	testApp();

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void gotMessage(ofMessage msg);
		
    void urlResponse(ofHttpResponse& httpResponse);
    
    //Hue API stuff
    string ipAddress;
    string username;
    string apiMethod;
    
private:
	ofHttpResponse lastHttpResponse;
    
    std::vector< ofPtr<ofxHueLightbulb> > lightbulbs;

	int activeBulbId;
};
