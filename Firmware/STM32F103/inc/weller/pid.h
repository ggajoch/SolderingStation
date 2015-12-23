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
        Kp = 0;
        Ki = 0;
        Kd = 0;
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
        float Ipart = Ki*integral;
        if( Ki != 0 ) {
            if( fabs(error) > 50 ) {  // big error, supress integral part
                integral = 0;
            } else {
                integral += error;
            }
            
    #define INTEGRAL_CUT 1000
            // limit integral part - when trying to reach low temperatures 
            // it can become really big
            if( Ipart > INTEGRAL_CUT ) { 
                Ipart = INTEGRAL_CUT;
                integral = INTEGRAL_CUT/Ki;
            } else if( integral < -INTEGRAL_CUT ) {
                integral = -INTEGRAL_CUT;
                integral = -INTEGRAL_CUT/Ki;
            }

            if( integral > 0 ) {
            	integral = 0;
            }
        }            
                
        float diff = error - prev_error;
        prev_error = error;
        return (Kp*error +  Ipart + Kd*diff);
    }
    void set_params(float Kp_, float Ki_, float Kd_) {
        this->Kp = Kp_;
        this->Ki = Ki_;
        this->Kd = Kd_;
    }
};

#endif  // PID_H_
