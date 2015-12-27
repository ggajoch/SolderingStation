#include "weller/weller.h"

void uart_write_str(const char * str) {
	USART3_Dev.puts(str);
	USART3_Dev.puts("\r\n");
}

CLI PCConn(uart_write_str);



void PC_pid_coeffs(float * params, uint8_t nr) {
	if( nr > 2 ) {
		printf("PID set |%f, %f %f|\r\n", params[0], params[1], params[2]);
		solderingIron.pid.set_params(params[0], params[1], params[2]);
	}
}

void PC_on(float * params, uint8_t nr) {
	printf("ON |%d|: %f\r\n", nr, params[0]);
}

void PC_mid(float * params, uint8_t nr) {
	printf("MID |%d|: %f\r\n", nr, params[0]);
	solderingIron.pid.setTarget(params[0]);
}


void PC_temp_calc(float * params, uint8_t nr) {
	if( nr > 1 ) {
		printf("Thermocouple set |%f, %f|\r\n", params[0], params[1]);
		solderingIron.temp.set_params(params[0], params[1]);
	}
}

//---------------------------------------------------------------

void PC_sendTemp(float temp) {
	PCConn.sendCommand("temp", &temp, 1);
}

void PC_sendTick(float temp, float filtered, float power, float powerAVG) {
	float tab[] = {temp, filtered, power, powerAVG};
	PCConn.sendCommand("tick", tab, 4);
}

void PC_sendAdc(float adc, float filtered) {
	float tab[] = {adc, filtered};
	PCConn.sendCommand("adc", tab, 2);
}

//---------------------------------------------------------------

void PC_Init() {
	PCConn.registerCommand({"calc", PC_temp_calc});
	PCConn.registerCommand({"pid", PC_pid_coeffs});
	PCConn.registerCommand({"mid", PC_mid});
	PCConn.registerCommand({"on", PC_on});
}
