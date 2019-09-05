#define BAUD_RATE 57600


/* ## CHANGE THESE for your project @@ */

#define MOSFET_PIN_1 9 // pump controller MOSFET (PWM)
#define DOT_LEN 50 // group length for dot output

/*
  Example Pump Config

  at 12V, pump gives about 5ml in 4 sec (25 drops/s)
  manual waterer was approx 1 drop / 30 sec
  so time between waterings is (25 * 30) = 750 (12.5 mins)

  1 tsp = 5ml = 98 drops
*/

// config for your plant
unsigned long wateringTime = 500; // time to run pump (seconds)
unsigned long waterDelayTime = 432000; // time between waterings (seconds)


/* ## DO NOT CHANGE THESE ## */

#define DELAY_TIME 1000 // "heartbeat" count time (milliseconds)

// vars to track "heartbeat" counter
unsigned long delayStart = 0; // the time the delay started
bool wateringPlant = false;
unsigned long heartbeatCount = 0;



void setup() {
  Serial.begin(BAUD_RATE);

  // initialize system
  delayStart = millis();
  stopWaterPlant();
}


void loop() {

  // changing millisecond timer to a "heartbeat" counter
  //
  if ((millis() - delayStart) >= DELAY_TIME) {
    checkStatus();
    showOutput();
    heartbeatCount += 1;
    delayStart += DELAY_TIME; // this prevents drift in the delays
  }
}

void showOutput() {
  // show current count or dot
  if (heartbeatCount % DOT_LEN == 0) {
    if (heartbeatCount != 0) Serial.println("");
    Serial.print(heartbeatCount);
  } else {
    Serial.print(".");
  }
}

void checkStatus() {
  if (wateringPlant) {
    checkWateringPlant();
  }
  else {
    checkWaitingToWater();
  }
}

void checkWaitingToWater()
{
  if (heartbeatCount >= waterDelayTime) {
    startWaterPlant();
  }
}

void checkWateringPlant()
{
  if (heartbeatCount >= wateringTime) {
    stopWaterPlant();
  }
}

void startWaterPlant() {
  Serial.print("\nWatering Plant: ");
  Serial.println(wateringTime);

  analogWrite(MOSFET_PIN_1, 254);
  wateringPlant = true;
  heartbeatCount = 0;
}

void stopWaterPlant() {
  Serial.print("\nWaiting to Water: ");
  Serial.println(waterDelayTime);

  analogWrite(MOSFET_PIN_1, 0);
  wateringPlant = false;
  heartbeatCount = 0;
}
