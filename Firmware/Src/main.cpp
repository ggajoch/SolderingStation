#include <stdint.h>
#include <array>
#include <stm32f103xb.h>
#include <HD44780.h>
#include "HAL.h"
#include "CLI.h"
#include "LCD_trans.h"
#include "Weller.h"

class ABC : public CLI::Command {
public:
    ABC() : Command("abc") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("Got abc: %f\r\n", parameters[0]);
    }
} ABC_Inst;

class BacklightCmd : public CLI::Command {
public:
    BacklightCmd() : Command("back") {
    }
    void callback(const arv::array_view<float> & parameters) {
        printf("back: %f\r\n", parameters[0]);
        HAL::setBacklight(parameters[0]);
    }
} BacklightCmd_Inst;


class ContrastCmd : public CLI::Command {
public:
    ContrastCmd() : Command("contrast") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("contr: %f\r\n", parameters[0]);
        HAL::setContrast(parameters[0]);
    }
} ContrastCmd_Inst;


class ADCCmd : public CLI::Command {
public:
    ADCCmd() : Command("adc") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("thermocouple: %d\r\n", HAL::getThermocoupleReading());
    }
} ADCCmd_Inst;

class Heating : public CLI::Command {
public:
    Heating() : Command("heat") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("heating: %f\r\n", parameters[0]);
        HAL::setHeating(parameters[0]);
    }
} Heating_Inst;

class MidPoint_t : public CLI::Command {
public:
    MidPoint_t() : Command("mid") {}
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("mid: %f\r\n", parameters[0]);
        WellerController.setTemperature(parameters[0]);
    }
} MidPoint;

class PIDSet_t : public CLI::Command {
public:
    PIDSet_t() : Command("pid") {}
    virtual void callback(const arv::array_view<float> & parameters) {
        PID::PIDParams params = {parameters[0], parameters[1], parameters[2]};
        printf("pid: %f %f %f\r\n", params.Kp, params.Ki, params.Kd);
        WellerController.tempPID.set_params(params);
    }
} PIDSet;


int main(void) {
	HAL::Init();
    float val = 0;

    HAL::setContrast(30);

    HD44780::Init();
    HD44780::PutText(0, 0, "abcdefg");


    HAL_Delay(1000);

    HD44780::PutText(0, 1, "defxyz");
    HAL_Delay(1000);
    HD44780::Clear();
    HAL_Delay(1000);
    HD44780::PutText(1, 1, "poiuytrewq");

    while (1) {
		if( HAL::TIM_TICK ) {
			HAL::TIM_TICK = false;
            WellerController.tick();
		}
    }
}
