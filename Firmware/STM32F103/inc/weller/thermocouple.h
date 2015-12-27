class Thermocouple {
	float gain;
	float intercept;
public:
	Thermocouple() {
		intercept = 27.93104;
		gain = 0.1096;
	}
	float getTemp(uint16_t adc_reading) {
		float res = intercept + gain * adc_reading;
		return res;
	}
	void set_params(float gain_, float intercept_) {
		gain = gain_;
		intercept = intercept_;
	}
};
