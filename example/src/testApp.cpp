#include "testApp.h"

testApp::testApp()
{
}

void testApp::setup()
{
	ofRegisterURLNotification(this);
    ofSetVerticalSync(true);
    
    ipAddress="10.0.1.3";
    username="newdeveloper"; 
    apiMethod="lights";

	activeBulbId=1;

    ofLoadURLAsync("http://" + ipAddress + "/api/" + username + "/" + apiMethod);
}

void testApp::update()
{
}

void testApp::draw()
{        
    string currentUrl=ipAddress+"/api/"+username+"/"+apiMethod;
    stringstream ss;
    ss<<"Current API Call : "<<currentUrl<<endl;
    
    ss<<endl<<endl;
    ss<<"last HTTP response : "<<endl; 
    ss<<lastHttpResponse.data.getText()<<endl;
    
    if (lightbulbs.size()>0)
    {
        float x=175;
        float yStart=370;
        float ySpacing=100;
        
		for (int i=0; i<lightbulbs.size(); i++)
            lightbulbs[i]->draw(x, ySpacing*i + yStart);
    }

    ss<<endl<<endl;
    
	ss<<"KEY CONTROLS"<<endl<<endl;
	ss<<"q,w		Shift hue by 1000"<<endl;
	ss<<"a,s		Shift saturation by 10"<<endl;
	ss<<"z,x		Shift brightness by 10"<<endl;
	ss<<"o,p		Turn bulb on and off"<<endl;
	ss<<"r		Apply a random hue, saturation, and brighness to all available bulbs"<<endl;
	ss<<"1,2,3,4		Target a specific bulb"<<endl;
	
	ss<<endl<<endl<<endl;

	ss<<"Active Bulb : "<<ofToString(activeBulbId)<<endl;

	ofDrawBitmapString(ss.str(), 15, 15);
}

void testApp::urlResponse(ofHttpResponse &httpResponse)
{
	ofUnregisterURLNotification(this);

    stringstream ss ;

    cout<<"urlResponse status code : "<<httpResponse.status<<endl;
	cout<<"urlResponse name : "<<httpResponse.request.name<<endl;

    if (httpResponse.status==200)
    {
		Json::Value json;
		Json::Reader reader;

		cout<<"httpResponse.data=="<<httpResponse.data.getText()<<endl;
		
        if (reader.parse(httpResponse.data.getText(), json))
        {
			if (!lightbulbs.empty())
				lightbulbs.clear();
                
            for (int j=1 ; j<=json.size() ; j++)
            {
				cout<<"Light "<<ofToString(j)<<" is on:: "<<json[ofToString(j)]["state"]["on"].asBool()<<endl;
				
				string str=json[ofToString(j)]["name"].asString();
                cout << " name : '" << str << "'" << endl ;
                
				if (str.find( "null" )==-1)
                {
					lightbulbs.push_back((ofPtr< ofxHueLightbulb >)new ofxHueLightbulb(ipAddress, username, j));

					lightbulbs[ (lightbulbs.size() -1 ) ]->begin().setIsOn(true).commit();
                }
            }    
        }
    }

    //ofLog( OF_LOG_WARNING ,  httpResponse.data.getText() );
    lastHttpResponse = httpResponse;
    ofLog( OF_LOG_WARNING , ss.str() ) ;
}

void testApp::exit()
{
	cout<<"testApp::exit"<<endl;

    for (int j=0; j<lightbulbs.size(); j++)
		lightbulbs[ j ]->begin().setIsOn(false).commit();
}

void testApp::keyPressed(int key)
{
	int indx=activeBulbId-1;
	switch(key) 
    {
		// ON/OFF
		case 'o': 
			lightbulbs[indx]->begin().setIsOn(true).commit();
			break;
		case 'p': 
			lightbulbs[indx]->begin().setIsOn(false).commit();
			break;

		// HUE
        case 'w': 
			lightbulbs[indx]->begin().shiftHue(1000).commit();
			break;
        case 'q': 
			lightbulbs[indx]->begin().shiftHue(-1000).commit();
			break;

		// SATURATION
        case 's': 
			lightbulbs[indx]->begin().shiftSaturation(10).commit();
			break;
        case 'a': 
			lightbulbs[indx]->begin().shiftSaturation(-10).commit();
			break;

		// BRIGHTNESS
        case 'x': 
			lightbulbs[indx]->begin().shiftBrightness(10).commit();
			break;
        case 'z': 
			lightbulbs[indx]->begin().shiftBrightness(-10).commit();
			break;
		
		// RANDOM
		case 'r': 
		{
			int h=(int)ofRandom(MAX_HUE);
			int s=(int)ofRandom(255);
			int b=(int)ofRandom(255);

			for (int j=0; j<lightbulbs.size(); j++)
			{
				lightbulbs[j]->begin()
				.setHue(h)
				.setSaturation(s)
				.setBrightness(b)
				.commit();
			}
			break;
		}

		// SELECT INDIVIDUAL BULB
		case '1': 
			activeBulbId=1;
			cout<<"BULB 1 SELECTED"<<endl;
			break;
		case '2': 
			activeBulbId=2;
			cout<<"BULB 2 SELECTED"<<endl;
			break;
		case '3': 
			activeBulbId=3;
			cout<<"BULB 3 SELECTED"<<endl;
			break;
		case '4': 
			activeBulbId=4;
			cout<<"BULB 4 SELECTED"<<endl;
			break;
    }
}

void testApp::keyReleased(int key)
{
}

void testApp::gotMessage(ofMessage msg)
{
}