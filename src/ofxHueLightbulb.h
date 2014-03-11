//
//  Created by Ben McChesney on 7/13/13.
//	Expanded upon by Dr. Seuss on 2/18/14.
//
//  We are only fiddling with the Lights API
//

#pragma once

#include <iostream>

#include "ofMain.h"
#include "json.h"
#include "ofxPhillipsHueDefs.h"

#include "Poco/URI.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/StreamCopier.h"

struct ResponseData
{
	ResponseData(Poco::Net::HTTPResponse& pocoResponse, std::istream &bodyStream, std::string _url)
	{
		status=pocoResponse.getStatus();

		try{
			timestamp=pocoResponse.getDate();
		}catch(Poco::Exception & exc){}

		reason=pocoResponse.getReasonForStatus(pocoResponse.getStatus());
		contentType=pocoResponse.getContentType();
		responseBody.set(bodyStream);

        url=_url;
	}

	ResponseData(){}

	int status;
	std::string reason;
	Poco::Timestamp timestamp;
	ofBuffer responseBody;
	std::string contentType;
	std::string url;
};

class HttpHelper
{
public:
	HttpHelper(const std::string& path);

	ResponseData doRequest(const std::string& method, 
						   const std::string& action="",
						   const std::string& jsonString="");

	ofEvent<ResponseData&> httpResponseEvent;

private:
	Poco::URI uri;
	Poco::Net::HTTPClientSession session;
	std::string path;
};

struct State
{
	bool			isOn;
	unsigned int	brightness;
    unsigned int	hue;
    unsigned int	saturation;
	ofPoint			cieCoordinates;
	unsigned int	colorTemperature;
	string			alert;
	string			effect;

	//unsigned int	transitionTime; //defaults to 4 (400ms)

	string			colorMode;
	bool			isReachable; //Always true (Good one Phillips)
};

struct Attributes
{
	string			name;
	string			type;
	string			modelID;
	string			softwareVersion;
};

class ofxHueLightbulb
{
public:
    ofxHueLightbulb(string ip, string usr, int id);
    ~ofxHueLightbulb();
    
    void setup();
    //void update();
    void draw(float x , float y);

	//void urlResponse(ofHttpResponse& httpResponse);
	void handleResponse(const std::string& rs);
	void handleResponse(ResponseData& evt);

    //Json::Value getStateJson();
	HttpHelper* httpHelper;

	int			id;
	State		state;
	Attributes	attributes;
    
	bool		drawDebug;

	// Chaining. Execute request via commit()
	ofxHueLightbulb& begin();
	void commit();

	ofxHueLightbulb& swapOnOff();
	ofxHueLightbulb& setIsOn(bool b);
	ofxHueLightbulb& setHue(int val);
	ofxHueLightbulb& setSaturation(int val);
	ofxHueLightbulb& setBrightness(int val);
	
	ofxHueLightbulb& shiftHue(int val);
	ofxHueLightbulb& shiftSaturation(int val);
	ofxHueLightbulb& shiftBrightness(int val);

	ofxHueLightbulb& setCieCoordinates(ofPoint pnt);
	ofxHueLightbulb& setTemperature(int temp);
	ofxHueLightbulb& setAlert(string str);
	ofxHueLightbulb& setEffect(string str);
	//ofxHueLightbulb& setTransitionTime(int val);

	void setName(const std::string& str); // Not chainable

    ofColor getAsOfColor( ) ;

private:
	Json::FastWriter writer;
	Json::Value root;

	string ipAddress;
    string username;
    string apiMethod;

    /*
     {"state": {"on":true,"bri":225,"hue":45489,"sat":253,"xy":[0.1859,0.0771],"ct":500,"alert":"none","effect":"none","colormode":"xy","reachable":true}, "type": "Extended color light", "name": "Hue Lamp 1", "modelid": "LCT001", "swversion": "65003148", "pointsymbol": { "1":"none", "2":"none", "3":"none", "4":"none", "5":"none", "6":"none", "7":"none", "8":"none" }}
     */
};