#define RIGHT_STICK_X   2
#define RIGHT_STICK_Y   3
#define MOTOR_B_P       6
#define MOTOR_B_N       7
#define MOTOR_A_N       8
#define MOTOR_A_P       9
#define MOTOR_B_EN      10
#define MOTOR_A_EN      11

// Variables
bool driving_forwards = true;

void setup() {
  // Begin serial
  Serial.begin(115200);

  // Initialize controls
  pinMode(  RIGHT_STICK_X ,   INPUT   );
  pinMode(  RIGHT_STICK_Y ,   INPUT   );

  // Motor
  pinMode(  MOTOR_B_P     ,   OUTPUT  );
  pinMode(  MOTOR_B_N     ,   OUTPUT  );
  pinMode(  MOTOR_A_P     ,   OUTPUT  );
  pinMode(  MOTOR_A_N     ,   OUTPUT  );
  pinMode(  MOTOR_B_EN    ,   OUTPUT  );
  pinMode(  MOTOR_A_EN    ,   OUTPUT  );

}

void setDirection(int MOTOR_P, int MOTOR_N, bool forwards) {
  digitalWrite(MOTOR_P, forwards ? HIGH : LOW);
  digitalWrite(MOTOR_N, forwards ? LOW : HIGH);
}

void loop() {
  // Get control values
  int steering = map(pulseIn(RIGHT_STICK_X, HIGH), 1000, 1989, -255, 255);
  int throttle = map(pulseIn(RIGHT_STICK_Y, HIGH), 995, 1989, -255, 255);
  
  // Create deadzones
  if (abs(steering) <= 3)
    steering = 0;

  if (abs(throttle) <= 3)
    throttle = 0;

  // If remote is off, return
  if (abs(throttle) > 260)
    return;
  
  Serial.println(steering);
  Serial.println(throttle);
  Serial.println();


  // Write to motors
  if (throttle > 0 && !driving_forwards) {
    driving_forwards = true;
    setDirection(MOTOR_A_P, MOTOR_A_N, true);
    setDirection(MOTOR_B_P, MOTOR_B_N, true);
  }

  if (throttle < 0 && driving_forwards) {
    driving_forwards = false;
    setDirection(MOTOR_A_P, MOTOR_A_N, false);
    setDirection(MOTOR_B_P, MOTOR_B_N, false);
  }
  analogWrite(MOTOR_A_EN, abs(throttle));
  analogWrite(MOTOR_B_EN, abs(throttle));
}
