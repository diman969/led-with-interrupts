#include <Arduino.h>
#define LED_GPIO 19
#define INTERRUPT_PIN 18

bool ledState{};

volatile int interruptCounter{};
int numberOfInterrupts{};

long debouncingTime{100};
volatile unsigned long last_micros{};

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterrupt(){
  portENTER_CRITICAL_ISR(&mux);
  if((long)(micros() - last_micros) >= debouncingTime *1000){
    interruptCounter++;
    last_micros = micros();
  }
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_GPIO, OUTPUT);
  Serial.println("Monitoring interrupts: ");
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(interruptCounter > 0){
    portENTER_CRITICAL(&mux);
    interruptCounter--;
    portEXIT_CRITICAL(&mux);

    digitalWrite(LED_GPIO, ledState);
    ledState = !ledState;

    numberOfInterrupts++;
    Serial.print("Number of Interrupts is: ");
    Serial.println(numberOfInterrupts);
  }
}