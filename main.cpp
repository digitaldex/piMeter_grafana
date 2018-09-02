#include "spiComm.h"
#include "influxdb.hpp"
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
	spiComm commObj;
	float WirkenergieL1, WirkenergieL2, WirkenergieL3;
	string result;

	influxdb_cpp::server_info si("server_ip", 8086, "db", "user", "pass");
	influxdb_cpp::query(result, "select WirkenergieL1, WirkenergieL2, WirkenergieL3 from energy ORDER BY DESC LIMIT 1", si);

	// Only for Startup
	// Parse Results and Set the Init Value of Energy Variables here

	while(1) {

		WirkenergieL1 += commObj.readEnergy(R_AWATTHR_HI_REGISTER);
		WirkenergieL2 += commObj.readEnergy(R_BWATTHR_HI_REGISTER);
		WirkenergieL3 += commObj.readEnergy(R_CWATTHR_HI_REGISTER);

		influxdb_cpp::builder()
		    .meas("energy")
		    .field("StromL1", commObj.readCurrent(R_AIRMS_REGISTER), 2)
		    .field("StromL2", commObj.readCurrent(R_BIRMS_REGISTER), 2)
		    .field("StromL3", commObj.readCurrent(R_CIRMS_REGISTER), 2)
		    .field("SpannungL1", commObj.readVoltage(R_AVRMS_REGISTER), 2)
		    .field("SpannungL2", commObj.readVoltage(R_BVRMS_REGISTER), 2)
		    .field("SpannungL3", commObj.readVoltage(R_CVRMS_REGISTER), 2)
		    .field("WirkleistungL1", commObj.readPower(R_AWATT_REGISTER), 2)
		    .field("WirkleistungL2", commObj.readPower(R_BWATT_REGISTER), 2)
		    .field("WirkleistungL3", commObj.readPower(R_CWATT_REGISTER), 2)   
		    .field("BlindleistungL1", commObj.readPower(R_AVAR_REGISTER), 2)
		    .field("BlindleistungL2", commObj.readPower(R_BVAR_REGISTER), 2)
		    .field("BlindleistungL3", commObj.readPower(R_CVAR_REGISTER), 2) 
		    .field("ScheinleistungL1", commObj.readPower(R_AVA_REGISTER), 2)
		    .field("ScheinleistungL2", commObj.readPower(R_BVA_REGISTER), 2)
		    .field("ScheinleistungL3", commObj.readPower(R_CVA_REGISTER), 2)
		    .field("WirkenergieL1", WirkenergieL1, 2)
		    .field("WirkenergieL2", WirkenergieL2, 2)
		    .field("WirkenergieL3", WirkenergieL3, 2) 
	    .post_http(si);

		sleep(5);
	}
}
