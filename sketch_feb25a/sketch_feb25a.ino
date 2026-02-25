#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// --- Configuration ---
#define LO_PLUS_PIN 2   // Leads-Off Positive pin on ESP32
#define LO_MINUS_PIN 4  // Leads-Off Negative pin on ESP32 (Using 4 instead of 3 for safety)

// Create the ADS1115 object handles the I2C communication
Adafruit_ADS1115 ads; 

void setup(void) {
  // Start Serial communication very fast for real-time plotting
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect. Needed for native USB

  Serial.println("WaveForm Prototype Test Starting...");

  // Setup Leads-Off Detection Pins as Inputs
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);

  // Initialize the ADS1115 ADC
  Serial.println("Initializing ADS1115...");
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115! Check wiring (SDA/SCL/ADDR/Power).");
    while (1); // Stop here if the chip isn't found
  }
  Serial.println("ADS1115 Connected!");

  // Set Gain. The AD8232 runs on 3.3V, so GAIN_ONE (+/- 4.096V range) 
  // provides the best balance of safety and precision.
  ads.setGain(GAIN_ONE); 
}

void loop(void) {
  int16_t rawADCreading;

  // 1. Check for Leads Off Detection first
  // If either pin goes HIGH, it means a pad has fallen off the skin.
  if((digitalRead(LO_PLUS_PIN) == 1) || (digitalRead(LO_MINUS_PIN) == 1)){
    // Print a flatline value to indicate leads are off
    Serial.println(0); 
  }
  else {
    // 2. Leads are on, so read the high-precision signal
    // Read from channel A0 on the ADS1115
    rawADCreading = ads.readADC_SingleEnded(0); 
    
    // Print the raw value to the serial plotter
    Serial.println(rawADCreading);
  }

  // A small delay to keep the data rate manageable for the plotter (~100Hz)
  delay(10);
}