

#ifndef        AD9854_ISP_DRIVER
#define        AD9854_ISP_DRIVER

#include "Energia.h"
#include <SPI.h>
#include <PinTiva.h>

#define SPI_BITS 8
#define SPI_MODE 0
#define SPI_FREQ 1000000
 
#define DDS_CMD_RESET       0X10
#define DDS_CMD_ENABLE_RF   0x11
#define DDS_CMD_MULTIPLIER  0X12
#define DDS_CMD_MODE        0x13
#define DDS_CMD_FREQUENCYA  0X14
#define DDS_CMD_FREQUENCYB  0x15
#define DDS_CMD_PHASEA      0X16
#define DDS_CMD_PHASEB      0x17
#define DDS_CMD_AMPLITUDE1  0X18
#define DDS_CMD_AMPLITUDE2  0x19
#define DDS_CMD_READ        0x8000

//!  Clase DDS . 
/*!
  Esta clase contiene las funciones que permitan usar el DDS.
*/
class DDS{
	private:
		double           _clock;              // Work frequency in MHz

		char            _ctrlreg_multiplier;      // Multiplier 4- 20
		char            _ctrlreg_mode;            // Single, FSK, Ramped FSK, Chirp, BPSK
		bool            _ctrlreg_qdac_pwdn;       // Q DAC power down enable: 0 -> disable
		bool            _ctrlreg_ioupdclk;        // IO Update clock enable: 0 -> input
		bool            _ctrlreg_inv_sinc;        // Inverse sinc filter enable: 0 -> enable
		bool            _ctrlreg_osk_en;          // Enable AM: 0 -> disabled
		bool            _ctrlreg_osk_int;       	// ext/int output shaped control: 0 -> external
		bool            _ctrlreg_msb_lsb;     	// msb/lsb bit first: 0 -> MSB
		bool            _ctrlreg_sdo;        		// SDO pin active: 0 -> inactive.
 
		char            _frequency1[6];
		char            _frequency2[6];
		char            _phase1[2];
		char            _phase2[2];       
		char            _amplitudeI[2];
		char            _amplitudeQ[2];
		bool            _rf_enabled;
	   
		double          _factor_freq1;
		double          _factor_freq2;
		
		//DDS I/O
		PIN* 			_dds_cs;
    	PIN* 			_dds_udclk;
    	PIN* 			_dds_io_reset;
    	PIN* 			_dds_mreset;		
		int 			_power;
		
		//SPI 
		int 			_spi_device;
		int 			_spi_delay;

		
	
		
	public:

		//! Constructor DDS.
	    /*!
	      \param clock representa el valor inicial de la senal de referencia.
	      \param CS  indica el pin para el Chip Select.
	      \param UDCLK indica el pin para el Update Clock.
	      \param IO_RESET indica el pin para hacer el reset (forzar comunicacion) sin borrar el valor de los registros.
	      \param MRESET indica el pin para hacer un master reset.
	    */
		DDS( float clock, int CS, int UDCLK, int IO_RESET, int MRESET);
		
		//! Se da la configuracion por defecto que permita la comunicacion entre el uC con el DDS.
	    /*!
	      \return true si se realizo la configuracion, false si no se realizo.
	    */
		int init();
		
		//! Genera un reset total sobre el DDS.
	    /*!
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int reset();
		
		//! Genera un reset parcial sobre el DDS al no borrar los valores previos del registro.
	    /*!
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int io_reset();

		//! Determina si exite comunicacion entre el uC y el DDS.
	    /*!
			\return true si existe comunicacion, false si no existe.
	    */
		int  verifyconnection();

		//! Se envia el contenido a un registro 
	    /*!
			\param addr Se escoge el registro.
			\param ndata Se indica la extension del mensaje wr_spi_data.
			\param wr_spi_data indica el mensaje que se enviara, cada direccion corresponde un mensaje char de distinta extension.
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int writeData(char addr, char ndata, const char*wr_spi_data);

		//! Se envia el contenido a un registro y a continuacion lee el registro, si ambos son iguales indica sucess.
	    /*!
			\param addr Se escoge el registro.
			\param ndata Se indica la extension del mensaje wr_spi_data.
			\param wr_spi_data indica el mensaje que se enviara, cada direccion corresponde un mensaje char de distinta extension.
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int writeDataAndVerify(char addr, char ndata, const char* wr_spi_data);

		//! ControlRegister es el registro que configura el funcionamiento del DDS. A partir de la configuracion previa se genera el char que se debe enviar al DDS.
		/*!
			\return char* con la configuracion deseada.
	    */
		char* getControlRegister();

		//! Se envia la configuracion al ControlRegister.
	    /*!
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int writeControlRegister();

		//! Se lee el contenido a un registro.
	    /*!
			\param addr Se escoge el registro.
			\param ndata Se indica la extension del mensaje a leer (Datasheet).
			\return char* con el valor del registro.
	    */
		char* readData(char addr, char ndata);

		//! Se extrae char* con informacion sobre el modo de operacion.
	    /*!
			\return char* con dimension 1.
	    */
		char* rdMode();

		//! Se extrae char* con informacion sobre el multiplicador empleado.
	    /*!
			\return char* con dimension 2.
	    */
		char* rdMultiplier();

		//! Se extrae char* con informacion sobre el Phase1 empleado.
	    /*!
			\return char* con dimension 2.
	    */
		char* rdPhase1();

		//! Se extrae char* con informacion sobre el Phase2 empleado.
	    /*!
			\return char* con dimension 2.
	    */
		char* rdPhase2();

		//! Se extrae char* con informacion sobre la Frequency Tuning Word 1 empleado.
	    /*!
			\return char* con dimension 6.
	    */
		char* rdFrequency1();

		//! Se extrae char* con informacion sobre la Frequency Tuning Word 2 empleado.
	    /*!
			\return char* con dimension 6.
	    */
		char* rdFrequency2();

		//! Se extrae char* con informacion sobre el Output shaped keying I multiplier empleado.
	    /*!
			\return char* con dimesnion 2.
	    */
		char* rdAmplitudeI();

		//! Se extrae char* con informacion sobre el Output shaped keying Q multiplier empleado.
	    /*!
			\return char* con dimension 2.
	    */
		char* rdAmplitudeQ();

		//! Se extrae el valor nominal del clock empleado.
	    /*!
			\return value clock.
	    */
		float getclock();
		
		//! Se extrae el valor nominal de la Phase1 empleado.
	    /*!
			\return value Phase1 entre 0 y 360.
	    */
		float getPhase1();

		//! Se extrae el valor nominal de la Phase2 empleado.
	    /*!
			\return value Phase2 entre 0 y 360.
	    */
		float getPhase2();

		//! Se extrae el valor nominal de la AmplitudeI empleado.
	    /*!
			\return value AmplitudeI entre 0 y 1.
	    */
		float getAmplitudeI();

		//! Se extrae el valor nominal de la AmplitudeQ empleado.
	    /*!
			\return value AmplitudeI entre 0 y 1.
	    */
		float getAmplitudeQ();

		//! Se extrae el valor nominal del multiplicador empleado.
	    /*!
			\return value AmplitudeI entre 1 y 20.
	    */
		int getMultiplier();

		//! Se extrae el valor nominal del modo empleado.
	    /*!
			\return value AmplitudeI entre 1 y 5.
	    */
		int getMode();

		//! Se extrae el valor nominal del factor de proporcion.
	    /*!
			\return value factor de la Frecuencia 1 entre 0 y 1.
	    */
		double getFreqFactor1();

		//! Se extrae el valor nominal del factor de proporcion.
	    /*!
			\return value factor de la Frecuencia 2 entre 0 y 1.
	    */
		double getFreqFactor2();

		//! Se envia la configuracion del modo de operacion.
	    /*!
			\param mode char entre 0x0 0x5 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrMode(char mode);

		//! Se envia la configuracion para el multiplicador y el clock.
	    /*!
			\param multiplier entre 0x0 0x14 .
			\param  clock menor a 300000000(Datasheet).
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrMultiplier(char multiplier, float clock);

		//! Se envia la configuracion para la Phase1.
	    /*!
			\param phase entre 0 y 360. 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrPhase1(float phase);

		//! Se envia la configuracion para la Phase2.
	    /*!
			\param phase entre 0 y 360. 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrPhase2(float phase);

		//! Se envia la configuracion para la Frequency Tuning Word 1.
	    /*!
			\param freq char* de dimension 6. 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrFrequency1(char* freq);

		//! Se envia la configuracion para la Frequency Tuning Word 2.
	    /*!
			\param freq char* de dimension 6. 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrFrequency2(char* freq);

		//! Se envia la configuracion para Output shaped keying I multiplier.
	    /*!
			\param amplitude entre 0 y 1. 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrAmplitudeI(float amplitude);

		//! Se envia la configuracion para Output shaped keying Q multiplier.
	    /*!
			\param amplitude entre 0 y 1. 
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int wrAmplitudeQ(float amplitude);
		

		//! Determina si exite  generacion de senal en el DDS.
	    /*!
			\return true si existe senal, false si no existe.
	    */
		int isRFEnabled();

		//! Activa la generacion de onda de la ultima configuracion.
	    /*!
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int enableRF();

		//! Desactiva la generacion de onda.
	    /*!
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int disableRF();

		//! Enviar la configuracion para la genereacion de 49.92 Mhz.
	    /*!
			\return true si se realizo la configuracion, false si no se realizo.
	    */
		int defaultSettings();

		

		
		// char* setCommand(unsigned short cmd, char* payload, unsigned long payload_len);
		// char* getCmdAnswer();
		// unsigned long getCmdAnswerLen();
		// int setAllDevice(char* payload);
		// bool wasInitialized();
		
		// char* getModeStr();
		// char* rdControl();

		//! TOOL: Transforma de binaria (char*) a decimal. Empleada en los calculos de frecuencia.
	    /*!
			\return valor en decimal.
	    */
		double binary2decimal(char*);

		//! TOOL: Transforma de binaria (char*) a nominal. Empleada en los calculos de frecuencia.
	    /*!
			\return valor en nominal.
	    */
		double binary2freq(char*) ;

		//! TOOL: Transforma de freq a binatio char*. Empleada en los calculos de frecuencia.
	    /*!
			\return valor en binario.
	    */
		char* freq2binary(float );

		//! TOOL: Impremine un serial mediante la configuracion de Serial.
		void print(char*, char);

		//! Estatus de configuracion inicial DDS.
	    /*!
	    */
		bool _isConfig;
};


#endif