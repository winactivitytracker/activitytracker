
/*
* gps.h
*
*  Created on: Nov 15, 2019
*      Author: Bulanov Konstantin
*/

#define GPS_DEBUG	0
#define	GPS_USART	&huart1
#define GPSBUFSIZE  128       // GPS buffer size

typedef struct{

	// calculated values
	float dec_longitude;
	float dec_latitude;
	float altitude_ft;
	float dec_speed_km;

	// GGA - Global Positioning System Fixed Data
	float nmea_longitude;
	float nmea_latitude;
	float utc_time;
	char ns, ew;
	int lock;
	int satelites;
	float hdop;
	float msl_altitude;
	char msl_units;

	// RMC - Recommended Minimmum Specific GNS Data
	char rmc_status;
	float speed_k;
	float course_d;
	int date;

	// GLL
	char gll_status;

	// VTG - Course over ground, ground speed
	float course_t; // ground speed true
	char course_t_unit;
	float course_m; // magnetic
	char course_m_unit;
	char speed_k_unit;
	float speed_km; // speed km/hr
	char speed_km_unit;

	uint8_t		rxBuffer[512];
	uint16_t	rxIndex;
	uint8_t		rxTmp;
	uint32_t	LastTime;

} GPS_t;

typedef struct{
	uint8_t		length; 	//active minutes
	uint8_t		lastActiveMinute;
	uint8_t 	activityPM[254];

	enum 		Activity{noMovement = 1, walking = 2, running = 3, unknown = 0};
	enum 		Activity currentActivity;
	enum 		Activity totalActivity;

}Activity_T;

#if (GPS_DEBUG == 1)
void GPS_print(char *data);
#endif

void GPS_Init();
void GSP_USBPrint(char *data);
void GPS_print_val(char *data, int value);
void GPS_UART_CallBack();
int GPS_validate(char *nmeastr);
void GPS_parse(char *GPSstrParse);
float GPS_nmea_to_dec(float deg_coord, char nsew);
