#include "Tiva.h"

#include <Energia.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <ArduinoHttpServer.h>
#include <string>
#include <AD9854.h>

using namespace std;

#define GET_NUMBER 2
#define POST_NUMBER 3

StaticJsonBuffer<40000> jsonBuffer;
String json_WRITE_ERROR;



API::API(DDS* _DDS, IPAddress _ip, byte _mac[])
{
    _DDS_JRO=_DDS;
    IPAddress mydns(10, 10, 10, 1);
    IPAddress gateway(10, 10, 10, 1);
    IPAddress subnet(255, 255, 255, 0);
    Ethernet.begin(_mac, _ip, mydns, gateway, subnet); 

     _set_ip=_ip;
     _set_dns=mydns;
     _set_subnet=subnet;
     _set_gateway=gateway; 
}

int API::readcommand( EthernetClient client)
{
	int msg=0;
    
    if (client.connected())
    {
    	/*******************************************************
            CONECTION
        *****************************************************/
        ArduinoHttpServer::StreamHttpRequest<50000> httpRequest(client);
        ArduinoHttpServer::StreamHttpReply httpReply(client, "application/json");

        if (httpRequest.readRequest())
        {
    	    /*******************************************************
                CONECTION DDS
            *****************************************************/
    	    if(_DDS_JRO->verifyconnection())
        	{    
        		char* data = (char*) httpRequest.getBody();
                int data_length = (int) httpRequest.getContentLength();
                ArduinoHttpServer::MethodEnum method( ArduinoHttpServer::MethodInvalid );
                method = httpRequest.getMethod();

                /*******************************************************
                METHOD GET
                *****************************************************/
                if ( method == ArduinoHttpServer::MethodGet)
                {
                    /*******************************************************
                    READ
                    *****************************************************/
                    if (httpRequest.getResource()[0] ==  "read")
                    {	
                    	String a_0, a_1, a_2="\"}", msg_read;

                        if (httpRequest.getResource()[1] ==  "clock")
                        {
                        	a_0="{\"clock\":\"";
                        	a_1=_DDS_JRO->getclock();

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "frequencyA_hz")
                        {
                            a_0="{\"frequencyA_hz\":\"";
                        	a_1=_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier();

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "frequencyB_hz")
                        {
                        	a_0="{\"frequencyB_hz\":\"";
                        	a_1=String(int(_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier()));

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "multiplier")
                        {
                            a_0="{\"multiplier\":\"";
                        	a_1=String(int(_DDS_JRO->getMultiplier()));

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "frequencyA")
                        {
                            a_0="{\"frequencyA\":\"";
                        	a_1=_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency1());

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "frequencyB")
                        {
                            a_0="{\"frequencyB\":\"";
                        	a_1=_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency2());

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "phaseA_degrees")
                        {
                            a_0="{\"phaseA_degrees\":\"";
                        	a_1=_DDS_JRO->rdPhase1();

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "phaseB_degrees")
                        {
                            a_0="{\"phaseB_degrees\":\"";
                        	a_1=_DDS_JRO->rdPhase2();
                        }

                        else if (httpRequest.getResource()[1] ==  "amplitudeI")
                        {
                            a_0="{\"amplitudeI\":\"";
                        	a_1=_DDS_JRO->rdAmplitudeI();
                        }

                        else if (httpRequest.getResource()[1] ==  "amplitudeQ")
                        {
                            a_0="{\"amplitudeQ\":\"";
                        	a_1=_DDS_JRO->rdAmplitudeQ();
                        }

                        else 
                        {
                            a_0="{\"MethodGet\":\"READ-No identified \"}";
                            a_1="";
                            a_2="";
                        }

                        msg_read= a_0+a_1+a_2;

                        httpReply.send(msg_read);

                    }
                    /*******************************************************
                    STATUS
                    *****************************************************/
                    else if (httpRequest.getResource()[0] == "status")
                    {
                        httpReply.send(status_DDS());
                        msg=2;
                    }
                    /*******************************************************
                	METHOD GET - OTHER
                	*****************************************************/
                    else
                    {
                        httpReply.send("{\"error\":\"METHOD GET DON'T IDENTIFIED\"}");
                        msg=3;
                    }
                }
                /*******************************************************
                METHOD POST
                *****************************************************/
                else if ( method == ArduinoHttpServer::MethodPost)
                {
                    /*******************************************************
                    CHANGE IP
                    *****************************************************/
                    if (httpRequest.getResource()[0] == "changeip")
                    {    
                        msg=99;
                        JsonObject& ip_data = jsonBuffer.parseObject(data);

                        if(ip_data.containsKey("ip") &&  ip_data.containsKey("gateway"))

                        {
                            if ((ip_data["ip"][0] == 10  && ip_data["gateway"][0] == 10) )
                            {
                                IPAddress x_ip(ip_data["ip"][0], ip_data["ip"][1], ip_data["ip"][2], ip_data["ip"][3]);
                                IPAddress x_gateway(ip_data["gateway"][0], ip_data["gateway"][1], ip_data["gateway"][2], ip_data["gateway"][3]);
                                
                                _set_ip = x_ip;
                                _set_gateway = x_gateway;

                                if(ip_data.containsKey("dns") && ip_data["dns"][0] == 10 )
                                {
                                    IPAddress x_dns(ip_data["dns"][0], ip_data["dns"][1], ip_data["dns"][2], ip_data["dns"][3]);
                                    _set_dns = x_dns;
                                }

                                else
                                {
                                    IPAddress x_dns(10, 10, 10, 1);
                                    _set_dns = x_dns;
                                }


                                if(ip_data.containsKey("subnet") )
                                {
                                    IPAddress x_subnet(ip_data["subnet"][0], ip_data["subnet"][1], ip_data["subnet"][2], ip_data["subnet"][3]);
                                    _set_subnet = x_subnet;
                                }

                                else
                                {
                                    IPAddress x_subnet(255, 255, 255, 0);
                                    _set_subnet = x_subnet;
                                }


                                httpReply.send("{\"changeip\":\"ok\"}");
                            }
                        }    
                        
                        else
                            httpReply.send("{\"error\":\"Please introduce unless a value for *ip* and *gateway* \"}");
                    }
                    /*******************************************************
                    WRITE
                    *****************************************************/
                    else if (httpRequest.getResource()[0] == "write")
                    {
                        JsonObject& jsondata = jsonBuffer.parseObject(data);
                        
                        double _jsonclock=_DDS_JRO->getclock();
                        int _jsonmultiplier = _DDS_JRO->getMultiplier();
                        double _jsonfrequency1 = _DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier();
                        double _jsonfrequency2 = _DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier();

                        float _jsonamplitudeI=_DDS_JRO->getAmplitudeI();
    					float _jsonamplitudeQ=_DDS_JRO->getAmplitudeQ();
    					float _jsonphaseA =_DDS_JRO->getPhase1();
    					float _jsonphaseB =_DDS_JRO->getPhase2();

    					int _jsonmode=_DDS_JRO->getMode();
    					bool _jsonenable=_DDS_JRO->isRFEnabled();

                        if(jsondata.containsKey("clock"))
                            _jsonclock = double(jsondata["clock"]);

                        if(jsondata.containsKey("multiplier"))
                            _jsonmultiplier = int(jsondata["multiplier"]);

                        if(jsondata.containsKey("frequencyA_hz"))
                            _jsonfrequency1 = double(jsondata["frequencyA_hz"]);

                        if(jsondata.containsKey("frequencyB_hz"))
                            _jsonfrequency2 = double(jsondata["frequencyB_hz"]);

                        if(jsondata.containsKey("amplitudeI"))
                        	_jsonamplitudeI=float(jsondata["amplitudeI"]);

                        if(jsondata.containsKey("amplitudeQ"))
                        	_jsonamplitudeQ=float(jsondata["amplitudeQ"]);

                        if(jsondata.containsKey("phaseA"))
                        	_jsonphaseA=float(jsondata["phaseA"]);

                        if(jsondata.containsKey("phaseB"))
                        	_jsonphaseB=float(jsondata["phaseB"]);

                        if(jsondata.containsKey("mode"))
                        	_jsonmode=int(jsondata["mode"]);

                        if(jsondata.containsKey("enable"))
                        	_jsonenable=bool(jsondata["enable"]);

                        
                        if(validation(_jsonclock,_jsonmultiplier, _jsonfrequency1, _jsonfrequency2,_jsonamplitudeI,_jsonamplitudeQ,_jsonphaseA,_jsonphaseB, _jsonmode))
                        {
                            _DDS_JRO->wrMultiplier(_jsonmultiplier,_jsonclock);
                            _DDS_JRO->wrPhase1(_jsonphaseA);
                            _DDS_JRO->wrPhase2(_jsonphaseB);
                            _DDS_JRO->wrAmplitudeI(_jsonamplitudeI);
                            _DDS_JRO->wrAmplitudeQ(_jsonamplitudeQ);
                            _DDS_JRO->wrMode(_jsonmode);

                            _DDS_JRO->wrFrequency1(_DDS_JRO->freq2binary(_jsonfrequency1));
                            _DDS_JRO->wrFrequency2(_DDS_JRO->freq2binary(_jsonfrequency2));

                            httpReply.send(status_DDS());

                        }

                        else{
                            httpReply.send(json_WRITE_ERROR);
                        }
                        
                        

                        msg=4;
                    }
                    /*******************************************************
                    START
                    *****************************************************/
                    else if (httpRequest.getResource()[0] == "start")
                    {
                        _DDS_JRO->enableRF();
                        httpReply.send(status_DDS());
                        msg=5;
                    }
                    /*******************************************************
                    STOP
                    *****************************************************/
                    else if (httpRequest.getResource()[0] == "stop")
                    {
                        //_DDS_JRO->reset();
                        _DDS_JRO->disableRF();
                        httpReply.send(status_DDS());
                        msg=6;
                    }
                    /*******************************************************
                    POST OTHER
                    *****************************************************/
                    else
                    {
                        httpReply.send("{\"error\":\"RESOURCE POST DON'T IDENTIFIED\"}");
                        msg=7;
                    }
                }
                /*******************************************************
                METHOD OTHER
                *****************************************************/
                else
            	{
            		httpReply.send("{\"error\":\"METHOD DON'T IDENTIFIED. USE GET OR POST\"}");
                    msg=7;
            	}
            }
            /*******************************************************
                NO CONECTION
            *****************************************************/
            else
            {
               httpReply.send("{\"error\":\"DDS no conection\"}"); 
            }
        }

        else
        {
            ArduinoHttpServer::StreamHttpErrorReply httpReply(client, httpRequest.getContentType());
            httpReply.send(httpRequest.getErrorDescrition());
        }
    
    }
    
    client.stop();
    return msg;

}

String API::status_DDS()
{
	String msg;

	JsonObject&  dds_status = jsonBuffer.createObject();

    dds_status["conection"] = "YES";
    dds_status["clock"]= _DDS_JRO->getclock() ;
    dds_status["frequencyA_hz"] =String(_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier());
    dds_status["frequencyB_hz"] =String(_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier());
    dds_status["multiplier"] =int(_DDS_JRO->getMultiplier());
    dds_status["frequencyA"] =String(_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency1()));
    dds_status["frequencyB"] =String(_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency2()));
    
    dds_status["phaseA_degrees"] =_DDS_JRO->getPhase1();
    dds_status["phaseB_degrees"] =_DDS_JRO->getPhase2();
    dds_status["amplitudeI"]=_DDS_JRO->getAmplitudeI();
    dds_status["amplitudeQ"]=_DDS_JRO->getAmplitudeQ();
    dds_status["mode"]=String(_DDS_JRO->getMode());
    
    dds_status.printTo(msg);


    return msg;
}

int API::validation(double _vclock, int _vmultiplier, double _vfrequency1, double _vfrequency2, float _vamplitudeI, float _vamplitudeQ, float _vphaseA, float _vphaseB, int _vmode)
{
    int sucess = 0;

    if(_vclock<=300000000 && _vclock>=0) 
    {
        if((_vmultiplier>=4 && _vmultiplier<=20) || _vmultiplier==1)
        {
            if(_vmode>=0 && _vmode <=5)
            {
                if( _vfrequency1 <= _DDS_JRO->getclock() * _DDS_JRO->getMultiplier() * 0.5)
                {
                    if( _vfrequency2 <= _DDS_JRO->getclock() * _DDS_JRO->getMultiplier() * 0.5)
                    {
                        if( _vamplitudeI <= 1 && _vamplitudeI >=0)
                        {
                            if( _vamplitudeQ<= 1 && _vamplitudeQ >=0)
                            {
                               if( _vphaseA <= 360 && _vphaseA >=0)
                                {
                                    if( _vphaseB <= 360 && _vphaseB >=0)
                                    {
                                        sucess=1;
                                    }

                                    else
                                        json_WRITE_ERROR="{\"error\":\"Introduce a phaseA between 0 and 1 \"}";
                                }

                                else
                                    json_WRITE_ERROR="{\"error\":\"Introduce a phaseA between 0 and 1 \"}";
                            }

                            else
                                json_WRITE_ERROR="{\"error\":\"Introduce an amplitudeQ between 0 and 1 \"}";
                        }

                        else
                            json_WRITE_ERROR="{\"error\":\"Introduce an amplitudeI between 0 and 1 \"}";
                    }

                    else
                        json_WRITE_ERROR="{\"error\":\"Introduce a correct frequency2 \"}";
                }

                else
                    json_WRITE_ERROR="{\"error\":\"Introduce a correct frequency1 \"}";
            }

            else
                json_WRITE_ERROR="{\"error\":\"Introduce a modulation case \"}";
        }

        else
            json_WRITE_ERROR="{\"error\":\"Introduce a multiplier value from 4 to 20 or 1 \"}";
    }

    else
        json_WRITE_ERROR="{\"error\":\"Introduce a clock less than 300000000 \"}";

    return sucess;
}
