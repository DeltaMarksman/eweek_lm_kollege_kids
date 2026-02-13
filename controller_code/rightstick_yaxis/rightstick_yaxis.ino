#define RXCH1 2   // Right Stick X
#define RXCH2 3   // Right Stick Y

#define LEFT_MOTOR_PIN 9
#define RIGHT_MOTOR_PIN 10

struct RC_Input {
  unsigned long pwm_ch1;
  unsigned long pwm_ch2;
};

RC_Input rc;

void setup() {
  pinMode(RXCH1, INPUT);
  pinMode(RXCH2, INPUT);

  pinMode(LEFT_MOTOR_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN, OUTPUT);

  Serial.begin(9600);
}

void readReceiver(RC_Input &data) {
  data.pwm_ch1 = pulseIn(RXCH1, HIGH);
  data.pwm_ch2 = pulseIn(RXCH2, HIGH);
}

// Convert 1000–2000 to -1 to +1
float normalize(long value) {
  return (value - 1500.0) / 500.0;
}

// Deadband to stop drift
float deadband(float value, float db = 0.05) {
  if (abs(value) < db) return 0.0;
  return value;
}

// Convert -1..+1 to 1000..2000
int toPWM(float cmd) {
  cmd = constrain(cmd, -1.0, 1.0);
  return 1500 + cmd * 500;
}

void tankControlRightStick(RC_Input &rc) {

  float throttle = normalize(rc.pwm_ch2); // Forward / Back
  float steering = normalize(rc.pwm_ch1); // Left / Right

  throttle = deadband(throttle);
  steering = deadband(steering);

  // Arcade mix
  float leftCmd  = throttle + steering;
  float rightCmd = throttle - steering;

  leftCmd  = constrain(leftCmd, -1.0, 1.0);
  rightCmd = constrain(rightCmd, -1.0, 1.0);

  int leftPWM  = toPWM(leftCmd);
  int rightPWM = toPWM(rightCmd);

  // Send to motor controller
  analogWrite(LEFT_MOTOR_PIN, map(leftPWM, 1000, 2000, 0, 255));
  analogWrite(RIGHT_MOTOR_PIN, map(rightPWM, 1000, 2000, 0, 255));

  Serial.print("Left: ");
  Serial.print(leftPWM);
  Serial.print("  Right: ");
  Serial.println(rightPWM);
}

void loop() {
  readReceiver(rc);
  tankControlRightStick(rc);
}
