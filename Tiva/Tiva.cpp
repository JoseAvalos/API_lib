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
                        	a_1=_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier();

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "multiplier")
                        {
                            a_0="{\"multiplier\":\"";
                        	a_1=String(_DDS_JRO->getMultiplier());

                            httpReply.send(a_0+a_1+a_2);
                        }

                        else if (httpRequest.getResource()[1] ==  "frequencyA")
                        {
                            a_0="{\"frequencyA\":\"";
                        	a_1=_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency2());

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
                    	// String msg_status;

                    	// JsonObject&  dds_status = jsonBuffer.createObject();
                     //    dds_status["conection"] = "YES";
                     //    dds_status["clock"]= _DDS_JRO->getclock() ;
                     //    dds_status["frequencyA_hz"] =_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier();
                     //    dds_status["frequencyB_hz"] =_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier();
                     //    dds_status["multiplier"] =String(_DDS_JRO->getMultiplier());
                     //    dds_status["frequencyA"] =_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency1());
                     //    dds_status["frequencyB"] =_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency2());
                     //    dds_status["phaseA_degrees"] =_DDS_JRO->rdPhase1();
                     //    dds_status["phaseB_degrees"] =_DDS_JRO->rdPhase2();
                     //    dds_status["amplitudeI"]=_DDS_JRO->rdAmplitudeI();
                     //    dds_status["amplitudeQ"]=_DDS_JRO->rdAmplitudeQ();
                        
                        
                     //    dds_status.printTo(msg_status);
                     //    const String& msg_json = msg_status;
                     //    httpReply.send(msg_json);

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
         				// wrMultiplier(1, 0.0);
					    // wrAmplitudeI("\x0F\xC0");                //0xFC0 produces best SFDR than 0xFFF
					    // wrAmplitudeQ("\x0F\xC0");                        //0xFC0 produces best SFDR than 0xFFF    
					    // wrFrequency1("\x00\x00\x00\x00\x00\x00");        // 49.92 <> 0x3f 0xe5 0xc9 0x1d 0x14 0xe3 <> 49.92/clock*(2**48) \x3f\xe5\xc9\x1d\x14\xe3
					    // wrFrequency2("\x00\x00\x00\x00\x00\x00");
					    // wrPhase1("\x00\x00");                            //0 grados
					    // wrPhase2("\x20\x00");                            //180 grados <> 0x20 0x00 <> 180/360*(2**14)
					    // disableRF();	
                        JsonObject& jsondata = jsonBuffer.parseObject(data);
                        
                        double freq_1 = double(jsondata["frequency1"]);
                        double freq_2 = double(jsondata["frequency2"]);
                        double mult = double(jsondata["multiplier"]);
                        double clock = double(jsondata["clock"]);
                        char amplitudeI = char(jsondata["AmplitudeI"]);
                        char amplitudeQ = char(jsondata["AmplitudeQ"]);
                        char phaseA = char(jsondata["phaseA"]);
                        char phaseB = char(jsondata["phaseB"]);

                        wrMultiplier(mult, clock);
                        wrPhase1("\x00\x00");                            //0 grados
					    wrPhase2("\x20\x00");                            //180 grados <> 0x20 0x00 <> 180/360*(2**14)
					    disableRF();	



                        Serial.println(String(freq_1));
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

                        if(freq_2<=_DDS_JRO->getclock()/2)
                        {                   
                            char* fb=_DDS_JRO->freq2binary(freq_2);
                            _DDS_JRO->wrFrequency1(fb);
                        }
                        
                        else
                        {
                        	httpReply.send("{\"error\":\"Frecuency 2 set is out of range, please change value (Mhz)\"}");
                        }



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
    dds_status["frequencyA_hz"] =_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency1())*_DDS_JRO->getMultiplier();
    dds_status["frequencyB_hz"] =_DDS_JRO->binary2freq(_DDS_JRO->rdFrequency2())*_DDS_JRO->getMultiplier();
    dds_status["multiplier"] =String(_DDS_JRO->getMultiplier());
    dds_status["frequencyA"] =_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency1());
    dds_status["frequencyB"] =_DDS_JRO->binary2decimal(_DDS_JRO->rdFrequency2());
    dds_status["phaseA_degrees"] =_DDS_JRO->rdPhase1();
    dds_status["phaseB_degrees"] =_DDS_JRO->rdPhase2();
    dds_status["amplitudeI"]=_DDS_JRO->rdAmplitudeI();
    dds_status["amplitudeQ"]=_DDS_JRO->rdAmplitudeQ();
    
    
    dds_status.printTo(msg);


    return msg;
}


/*

{
  "amplitude_enabled": false, 
  "frequencyB_Mhz": 0, 
  "frequencyA": 29320310074026, 
  "phaseA_degrees": 0.0, 
  "clock": 60.0, 
  "modulation": 4, 
  "frequencyB": 0, 
  "phaseA": 0, 
  "phaseB": 0, 
  "multiplier": 4, 
  "device_id": 1, 
  "amplitudeQ": 0, 
  "phaseB_degrees": 0, 
  "amplitudeI": 0, 
  "frequencyA_Mhz": 24.99999999999943
}
*/