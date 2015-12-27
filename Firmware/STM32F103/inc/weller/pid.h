#ifndef PID_H_
#define PID_H_

#include <math.h>

class PID {
    float Kp, Ki, Kd;
    float prev_error;
    float integral;
    float mid;

 public:
    PID() {
        Kp = 10;
        Ki = 0.03;
        Kd = 300;
        mid = 25;
        prev_error = 0;
        integral = 0;
    }
    float setTarget(float value) {
        this->mid = value;
    }
    float getTarget() {
		return this->mid;
	}
    float tick(float temp) {
        float pwr = this->feed_error(mid - temp);
        if( pwr < 0 )
        	pwr = 0;
        if( pwr > 100 )
        	pwr = 100;
        return pwr;
    }
    float feed_error(float error) {
    	if( fabs(error) > 25 ) {
    		integral = 0;
    	} else {
    		integral += error;
    	}

    	if( error > 20 ) {
    		return 100;
    	}
    	if( error < -20 ) {
			return 0;
		}

        float Ipart = Ki*integral;

        float diff = error - prev_error;
        prev_error = error;
        return (Kp*error +  Ipart + Kd*diff);
    }
    void reset() {
    	this->integral = 0;
    	this->prev_error = 0;
    }
    void set_params(float Kp_, float Ki_, float Kd_) {
        this->Kp = Kp_;
        this->Ki = Ki_;
        this->Kd = Kd_;
    }
};

#endif  // PID_H_
