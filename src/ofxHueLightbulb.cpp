//
//  Created by Ben McChesney on 7/13/13.
//	Expanded upon by Dr. Seuss on 2/18/14.
//

#include "ofxHueLightbulb.h"

HttpHelper::HttpHelper(const std::string& path)
{
	uri=Poco::URI(path);
}

ResponseData HttpHelper::doRequest(const std::string& method, const std::string& action, const std::string& jsonString)
{
	ResponseData responseData;
	try
	{
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		path+=("/"+action);

		Poco::Net::HTTPRequest request(method, path, Poco::Net::HTTPMessage::HTTP_1_1);

		Poco::Net::HTTPResponse response;
		ofPtr<Poco::Net::HTTPSession> session;
		std::istream* rs;

		Poco::Net::HTTPClientSession* httpSession=new Poco::Net::HTTPClientSession(uri.getHost(), uri.getPort());
		httpSession->setTimeout(Poco::Timespan(2,0));

		if (method==Poco::Net::HTTPRequest::HTTP_PUT)
		{
			request.setContentType("application/json");
			//request.setKeepAlive(true);
			request.setContentLength((int)jsonString.length());

			httpSession->sendRequest(request) << jsonString;
		}

		if (method==Poco::Net::HTTPRequest::HTTP_GET)
		{
			httpSession->sendRequest(request);
		}

		// TODO: HTTP_PUT

		rs=&httpSession->receiveResponse(response);
		session=ofPtr<Poco::Net::HTTPSession>(httpSession);

		responseData=ResponseData(response, *rs, path);

	}catch (Poco::Exception& exc)
	{
		ofLogError("HttpHelper::doRequest:") << exc.displayText();
        responseData.status=-1;
        responseData.reason=exc.displayText();
	}

	ofNotifyEvent( httpResponseEvent, responseData, this );

	return responseData;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


ofxHueLightbulb::ofxHueLightbulb(string ip, 
								 string usr,
								 int id)
: ipAddress(ip)
, username(usr)
, id(id)
{
	apiMethod="lights";
	drawDebug=YES;

	httpHelper=new HttpHelper("http://" + ipAddress + "/api/" + username + "/" + apiMethod + "/" + ofToString(id));
	
	//ofAddListener(httpHelper->httpResponseEvent, this, &ofxHueLightbulb::handleResponse);
	
	setup();
}

ofxHueLightbulb::~ofxHueLightbulb()
{
}

void ofxHueLightbulb::setup()
{
	cout<<"ofxHueLightbulb::setup::"<<id<<endl;

	ResponseData res=httpHelper->doRequest(Poco::Net::HTTPRequest::HTTP_GET);

	//cout<<res.responseBody.getText()<<endl;

	if (res.status==200)
		handleResponse(res.responseBody.getText());
	/*
	Json::FastWriter writer;
	Json::Value root;
	root["on"]=state.isOn=false;
	root["bri"]=state.brightness=255;
	root["sat"]=state.saturation=255;
	root["hue"]=state.hue=0;
	

	string jsonStr = writer.write(root);

	httpHelper->doRequest(Poco::Net::HTTPRequest::HTTP_PUT, "state", jsonStr);
	*/
}

void ofxHueLightbulb::draw( float x , float y )
{
	if (!drawDebug)
		return;

	ofSetColor( getAsOfColor() ) ;
    float radius = 20 ;
    ofCircle( x , y , radius ) ;

    ofSetColor( ofColor::black ) ;
    stringstream ss ;
	ss << state.isOn << " hue=="<< state.hue << " bri==" << state.brightness << " sat==" << state.saturation << endl ; 
    ofDrawBitmapString( ss.str() , x , y - radius - 15 );//, ofColor::black , getOfxColor() ) ;
	
}

void ofxHueLightbulb::handleResponse(ResponseData& evt)
{

}

//void ofxHueLightbulb::urlResponse(ofHttpResponse &httpResponse)
void ofxHueLightbulb::handleResponse(const std::string& rs)
{
	Json::Value json;
	Json::Reader reader;
			
    if (reader.parse(rs, json))
    {
		cout<<"STATE"<<endl;

		if (!json["state"].isNull())
		{
			state.isOn=json["state"][STATE_ON].asBool();
			state.brightness=json["state"][STATE_BRIGHTNESS].asInt();
			state.hue=json["state"][STATE_HUE].asInt();
			state.saturation=json["state"][STATE_SATURATION].asInt();
			
			const Json::Value arr=json["state"][STATE_CIE_COORDINATES];
			state.cieCoordinates=ofPoint(arr[0.0].asInt(), arr[1].asInt());

			state.colorTemperature=json["state"][STATE_TEMPERATURE].asInt();
			
			state.alert=json["state"][STATE_ALERT].asString();
			state.effect=json["state"][STATE_EFFECT].asString();
			state.colorMode=json["state"][STATE_COLORMODE].asString();
			state.isReachable=json["state"][STATE_REACHABLE].asBool();
		}

		if (!json[ATTRIBUTE_TYPE].isNull())
			attributes.type=json[ATTRIBUTE_TYPE].asString();

		if (!json[ATTRIBUTE_NAME].isNull())
			attributes.name=json[ATTRIBUTE_NAME].asString();

		if (!json[ATTRIBUTE_MODEL].isNull())
			attributes.modelID=json[ATTRIBUTE_MODEL].asString();

		if (!json[ATTRIBUTE_SOFTWARE].isNull())
			attributes.softwareVersion=json[ATTRIBUTE_SOFTWARE].asString();
	}
}

ofxHueLightbulb& ofxHueLightbulb::begin()
{
	root.clear();
	return *this;
}

// WARNING: State and attribute objects are updated on call... if the request fails they will be out of sync with the bulbs

ofxHueLightbulb& ofxHueLightbulb::swapOnOff()
{
	root[STATE_ON]=!state.isOn;
	state.isOn=!state.isOn;
	
	return *this;
}


ofxHueLightbulb& ofxHueLightbulb::setIsOn(bool b)
{
	root[STATE_ON]=b;
	state.isOn=b;
	
	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setHue(int val)
{
	root[STATE_HUE]=val;
	state.hue=val;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setSaturation(int val)
{
	root[STATE_SATURATION]=val;
	state.saturation=val;
	
	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setBrightness(int val)
{
	root[STATE_BRIGHTNESS]=val;
	state.brightness=val;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::shiftHue(int val)
{
	int tmp=ofClamp(state.hue+val, 0, MAX_HUE);
	root[STATE_HUE]=tmp;
	state.hue=tmp;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::shiftSaturation(int val)
{
	int tmp=ofClamp(state.saturation+val, 0, 255);
	root[STATE_SATURATION]=tmp;
	state.saturation=tmp;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::shiftBrightness(int val)
{
	int tmp=ofClamp(state.brightness+val, 0, 255);
	root[STATE_BRIGHTNESS]=tmp;
	state.brightness=tmp;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setCieCoordinates(ofPoint pnt)
{
	int arr[2]={pnt.x, pnt.y};

	root[STATE_CIE_COORDINATES]=arr;
	state.cieCoordinates=pnt;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setTemperature(int val)
{
	root[STATE_TEMPERATURE]=val;
	state.colorTemperature=val;

	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setAlert(string str)
{
	root[STATE_ALERT]=str;
	state.alert=str;
	
	return *this;
}

ofxHueLightbulb& ofxHueLightbulb::setEffect(string str)
{
	root[STATE_EFFECT]=str;
	state.effect=str;
	
	return *this;
}

void ofxHueLightbulb::commit()
{
	std::string jsonStr=writer.write(root);
	
	cout<<"ofxHueLightbulb::commit:"<<id<<"=="<<jsonStr<<endl;

	httpHelper->doRequest(Poco::Net::HTTPRequest::HTTP_PUT, "state", jsonStr);

	// TODO: UPDATE STATE AND ATTRIBUTE OBJECT
	// WARNING: Updating in setters... if request fails object data will be out of sync with bulbs
}

void ofxHueLightbulb::setName(const std::string& str)
{
	root.clear();

	root[ATTRIBUTE_NAME]=str;
	attributes.name=str;

	std::string jsonStr=writer.write(root);

	httpHelper->doRequest(Poco::Net::HTTPRequest::HTTP_PUT, "", jsonStr);
}

ofColor ofxHueLightbulb::getAsOfColor()
{
	float h=ofMap(state.hue, 0.0f, 65535.0f, 0.0f, 255.0f, true);
	ofColor c=ofColor::fromHsb(h, state.saturation, state.brightness);

    return c ; 
}
