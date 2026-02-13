#define RXCH1 2

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RXCH1, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long pwmch1 = pulseIn(RXCH1, HIGH);
  Serial.println(pwmch1);

}
