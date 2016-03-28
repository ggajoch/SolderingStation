#include <stdint.h>
#include <array>
#include <stm32f103xb.h>
#include <HD44780.h>
#include "HAL.h"
#include "CLI.h"
#include "LCD_trans.h"

WellerHAL *hal;

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
        hal->setBacklight(parameters[0]);
    }
} BacklightCmd_Inst;


class ContrastCmd : public CLI::Command {
public:
    ContrastCmd() : Command("contrast") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("contr: %f\r\n", parameters[0]);
        hal->setContrast(parameters[0]);
    }
} ContrastCmd_Inst;


class ADCCmd : public CLI::Command {
public:
    ADCCmd() : Command("adc") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("thermocouple: %d\r\n", hal->getThermocoupleReading());
    }
} ADCCmd_Inst;

class Heating : public CLI::Command {
public:
    Heating() : Command("heat") {
    }
    virtual void callback(const arv::array_view<float> & parameters) {
        printf("heating: %f\r\n", parameters[0]);
        hal->setHeating(parameters[0]);
    }
} Heating_Inst;

volatile bool TIM_TICK;

int main(void) {
    WellerHAL halInst;
    hal = &halInst;
    float val = 0;

    hal->setContrast(30);

    HD44780::Init();
    HD44780::PutText(0, 0, "abcdefg");


    HAL_Delay(1000);

    HD44780::PutText(0, 1, "defxyz");
    HAL_Delay(1000);
    HD44780::Clear();
    HAL_Delay(1000);
    HD44780::PutText(1, 1, "poiuytrewq");

    while (1) {
		if( TIM_TICK ) {
			TIM_TICK = false;
			uint16_t x = hal->getThermocoupleReading();
			char buf[16] = {' '};
			sprintf(buf, "adc: %d    ", x);
			HD44780::PutText(0, 0, buf);
			printf("tick\r\n");
		}
    }
}
