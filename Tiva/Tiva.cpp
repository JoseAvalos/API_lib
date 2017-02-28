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


API::API(DDS* _DDS, IPAddress _ip, byte _mac[])
{
    _DDS_JRO=_DDS;
    IPAddress mydns(10, 10, 10, 1);
    IPAddress gateway(10, 10, 10, 1);
    IPAddress subnet(255, 255, 255, 0);
    Ethernet.begin(_mac, _ip, mydns, gateway, subnet);   
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
                		String msg_status=status_DDS();
                        const String& msg_json = msg_status;
                        httpReply.send(msg_json);
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
                    WRITE
                    *****************************************************/
                    if (httpRequest.getResource()[0] == "write")
                    {
                        JsonObject& jsondata = jsonBuffer.parseObject(data);
                        
                        double _jsonclock=_DDS_JRO->clock(), _jsonmultiplier = _DDS_JRO->multiplier();
                        double _jsonfrequency1 = _DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier();
                        double _jsonfrequency2 = _DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier();

                        float _jsonamplitudeI=_DDS_JRO->rdAmplitudeI();
    					float _jsonamplitudeQ=_DDS_JRO->rdAmplitudeQ();
    					float _jsonphaseA_degrees =_DDS_JRO->rdPhase1();
    					float _jsonphaseB_degrees =_DDS_JRO->rdPhase2();

    					int _jsonmode=_DDS_JRO->rdMode();
    					bool _jsonenable=_DDS_JRO->isRFEnabled();

                        if(jsondata.containsKey("clock"))
                            _jsonclock = double(jsondata["clock"]);

                        if(jsondata.containsKey("multiplier"))
                            _jsonmultiplier = double(jsondata["multiplier"]);

                        if(jsondata.containsKey("frequency1"))
                            _jsonfrequency1 = double(jsondata["frequency1"]);

                        if(jsondata.containsKey("frequency2"))
                            _jsonfrequency2 = double(jsondata["frequency2"]);

                        if(jsondata.containsKey("amplitudeI")
                        	_jsonamplitudeI=float(jsondata["amplitudeI"]);

                        if(jsondata.containsKey("amplitudeQ")
                        	_jsonamplitudeQ=float(jsondata["amplitudeQ"]);

                        if(jsondata.containsKey("phaseA"))
                        	_jsonphaseA_degrees=float(jsondata["phaseA"]);

                        if(jsondata.containsKey("phaseB"))
                        	_jsonphaseB_degrees=float(jsondata["phaseB"]);

                        if(jsondata.containsKey("mode"))
                        	_jsonmode=int(jsondata["mode"]);

                        if(jsondata.containsKey("enable"))
                        	_jsonenable=bool(jsondata["enable"]);


                        // if(jsondata.containsKey("clock"))
                        // {
                        //     _jsonclock = double(jsondata["clock"]);
                        // }
                        

                       

                        // unsigned int modulation= int (jsondata["modulation"]);
                        

                        // double freq_1 = double(jsondata["frequency1"]);
                        

                        // double freq_2 = double(jsondata["frequency2"]);
                        

                        // float amplitudeI = float(jsondata["AmplitudeI"]);
                        

                        // float amplitudeQ = float(jsondata["AmplitudeQ"]);
                        

                        // float phaseA = float(jsondata["phaseA"]);
                        

                        // float phaseB = float(jsondata["phaseB"]);

                        // if(clock<=300000000 && clock>=0) 
                        // {
                        //     if((mult>=4 && mult<=12) || mult=1)
                        //     {
                        //         if(modulation>=0 && modulation <=5)
                        //         {
                        //             if( freq_1 <= _DDS_JRO->getclock() * _DDS_JRO->getMultiplier())
                        //             {
                        //                 if( freq_2 <= _DDS_JRO->getclock() * _DDS_JRO->getMultiplier())
                        //                 {

                        //                 }

                        //                 else
                        //                 {
                        //                     httpReply.send("{\"error\":\"Introduce a correct frequency2 \"}");
                        //                 }
                        //             }

                        //             else
                        //             {
                        //                 httpReply.send("{\"error\":\"Introduce a correct frequency1 \"}");
                        //             }
                        //         }

                        //         else
                        //         {
                        //             httpReply.send("{\"error\":\"Introduce a modulation case \"}");
                        //         }
                        //     }

                        //     else
                        //     {
                        //         httpReply.send("{\"error\":\"Introduce a multiplier value from 4 to 12 or 1 \"}");
                        //     }
                        // }

                        // else
                        // {
                        //     httpReply.send("{\"error\":\"Introduce a clock less than 300000000 \"}");
                        // }
                        //bool enable= bool (jsondata["enable"]);

                        double freq_1 = double(jsondata["frequency1"]);
                        freq_1/=float(_DDS_JRO->getMultiplier());
                        
                        if(freq_1<=_DDS_JRO->getclock()/2)
                        {                   
                            char* fb=_DDS_JRO->freq2binary(freq_1);
                            _DDS_JRO->wrFrequency1(fb);
                        }
                        

                        else
                        {
                        	httpReply.send("{\"error\":\"Frecuency 1 set is out of range, please change value (Mhz)\"}");
                        }

         //                if(freq_2<=_DDS_JRO->getclock()/2)
         //                {                   
         //                    char* fb=_DDS_JRO->freq2binary(freq_2);
         //                    _DDS_JRO->wrFrequency1(fb);
         //                }
                        
         //                else
         //                {
         //                	httpReply.send("{\"error\":\"Frecuency 2 set is out of range, please change value (Mhz)\"}");
         //                }


                        msg=4;
                    }
                    /*******************************************************
                    START
                    *****************************************************/
                    else if (httpRequest.getResource()[0] == "start")
                    {
                        //_DDS_JRO->
                        httpReply.send("{\"start\":\"ok\"}");
                        msg=5;
                    }
                    /*******************************************************
                    STOP
                    *****************************************************/
                    else if (httpRequest.getResource()[0] == "stop")
                    {
                        //_DDS_JRO->reset();
                        _DDS_JRO->disableRF();
                        httpReply.send("{\"stop\":\"ok\"}");
                        msg=6;
                    }
                    /*******************************************************
                    POST OTHER
                    *****************************************************/
                    else
                    {
                        httpReply.send("{\"error\":\"METHOD POST DON'T IDENTIFIED\"}");
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
    
    dds_status["phaseA_degrees"] =_DDS_JRO->rdPhase1();
    dds_status["phaseB_degrees"] =_DDS_JRO->rdPhase2();
    dds_status["amplitudeI"]=_DDS_JRO->rdAmplitudeI();
    dds_status["amplitudeQ"]=_DDS_JRO->rdAmplitudeQ();
    dds_status["mode"]=String(_DDS_JRO->rdMode());
    
    dds_status.printTo(msg);


    return msg;
}


