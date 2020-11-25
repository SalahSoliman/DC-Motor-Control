// init pins
int motor_pwm = 9;
int encoder_a = 2;
int encoder_b = 3;

// encoder params
const int encoder_ppr = 12;
volatile int encoder_pulses = 0;
float current_speed = 0.0;
int pwm_signal;

// time params
unsigned long time_now;
unsigned long time_start;
unsigned long time_elapsed;
unsigned int time_step = 200; //ms

// control parameters
double p = 20.0;
double i = 0.0;
double d = 0.0;
double set_point = 50; // deg/s
double error = 0.0;
double error_last = 0.0;
double error_accum = 0.0;
double action = 0.0;

void setup()
{
  Serial.begin(9600);
  pinMode(motor_pwm, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoder_a), countA, CHANGE);
}

void loop()
{
  time_now = millis();
  time_elapsed = time_now - time_start;

  // control loop each time step
  if (time_elapsed > time_step)
  {
    // get current speed
    current_speed = (encoder_pulses * 180.0 * 1000.0)/ (1.0 * encoder_ppr * time_elapsed); // deg/s
    encoder_pulses = 0;

    // calculate error
    error = set_point - current_speed;
    error_accum += error;

    //calculate the control action
    action = p * error + i * (((error + error_last) / 2.0 + error_accum) * time_elapsed) + d * ((error - error_last) / 1.0 * time_elapsed);

    Serial.println(action);
    pwm_signal = int(constrain(action, 0, 255));
    analogWrite(motor_pwm, pwm_signal);
    
    
    error_last = error;
    time_start = time_now;
  }
}
void countA()
{
  encoder_pulses++;
}
