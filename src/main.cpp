#include <Arduino.h>
#include <wiring_private.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ZeroTimer.h>
#include <IBusBM.h>
#include <CRC.h>

#define MOTOR_POLES 2

#define HWT_PACKAGE_HEADER 0xFE
#define HWT_PACKAGE_SIZE   32

Uart Serial2(&sercom1, 5, 4, SERCOM_RX_PAD_1, UART_TX_PAD_0);
Adafruit_NeoPixel pixel(1, PIN_NEOPIXEL, NEO_GRB);
Adafruit_ZeroTimer zerotimer = Adafruit_ZeroTimer(3);
IBusBM IBus;

int16_t rpm = 0;
int16_t voltage = 0;
int16_t escTemp = 0;
int16_t motorTemp = 0;

// Serial2 interrupt handler
void SERCOM1_Handler() {
  Serial2.IrqHandler();
}

// Timer3 handler
void TC3_Handler() {
  Adafruit_ZeroTimer::timerHandler(3);
}

// Timer callback
void Timer_Callback()
{
  IBus.loop();
}

void setup() {
  Serial.begin(115200);

  // Initialize neopixel
  pixel.begin();
  pixel.setBrightness(32);
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));
  pixel.show();

  // Initialize first hardware serial interface
  Serial1.begin(115200);

  // Initialize i-bus telemetry
  IBus.begin(Serial1, IBUSBM_NOTIMER);

  // Add sensors
  IBus.addSensor(IBUSS_RPM);
  IBus.addSensor(IBUSS_EXTV);
  IBus.addSensor(IBUSS_TEMP);
  IBus.addSensor(IBUSS_TEMP);

  // Set default sensor data
  IBus.setSensorMeasurement(1, 0);
  IBus.setSensorMeasurement(2, 0);
  IBus.setSensorMeasurement(3, 400);
  IBus.setSensorMeasurement(4, 400);

  // Initialize timer
  zerotimer.enable(false);
  zerotimer.configure(TC_CLOCK_PRESCALER_DIV1, TC_COUNTER_SIZE_16BIT, TC_WAVE_GENERATION_MATCH_PWM);
  zerotimer.setCompare(0, 48000);
  zerotimer.setCallback(true, TC_CALLBACK_CC_CHANNEL0, Timer_Callback);
  zerotimer.enable(true);

  // Initialize second hardware serial interface
  Serial2.begin(115200);
  Serial2.setTimeout(50);
  pinPeripheral(5, PIO_SERCOM);
  pinPeripheral(4, PIO_SERCOM);
  
  // Show ready state
  pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  pixel.show();
}

void loop() {
  // Check for available data
  uint16_t bytesAvailable = Serial2.available();
  if(bytesAvailable > 0) {
    // Check for Hobbywing telemetry header
    uint8_t startByte = Serial2.peek();
    if(startByte == HWT_PACKAGE_HEADER) {
      uint8_t buf[HWT_PACKAGE_SIZE];
      uint16_t len = Serial2.readBytes(buf, HWT_PACKAGE_SIZE);
      // Check for Hobbywing telemetry data frame length
      if(len == HWT_PACKAGE_SIZE) {
        // Control bytes 1-4
        uint32_t ctrlBytes = 0;
        ctrlBytes |= buf[1];
        ctrlBytes |= buf[2] << 8;
        ctrlBytes |= buf[3] << 16;
        ctrlBytes |= buf[4] << 24;
        // Checksum bytes 30 and 31
        uint16_t checksum = 0;
        checksum |= buf[30];
        checksum |= buf[31] << 8;
        // Check for control bytes and checksum (CRC-16/MODBUS)
        if(ctrlBytes == 0x30030001 && checksum == calcCRC16(buf, HWT_PACKAGE_SIZE - 2, CRC16_MODBUS_POLYNOME, CRC16_MODBUS_INITIAL, CRC16_MODBUS_XOR_OUT, CRC16_MODBUS_REV_IN, CRC16_MODBUS_REV_OUT, CRC_YIELD_DISABLED)) {
          // RPM bytes 13 and 14
          rpm = 0;
          rpm |= buf[13];
          rpm |= buf[14] << 8;
          // Voltage byte 15
          voltage = buf[15];
          // ESC temperature byte 19
          escTemp = buf[19];
          // Motor temperature byte 21
          motorTemp = buf[21];
        } else {
          // Control byte or checksum invalid
          rpm = 0;
        }
        // Write to i-bus
        IBus.setSensorMeasurement(1, rpm * 10 / (MOTOR_POLES / 2));
        IBus.setSensorMeasurement(2, voltage * 10);
        IBus.setSensorMeasurement(3, 400 + (escTemp * 10));
        IBus.setSensorMeasurement(4, 400 + (motorTemp * 10));
      }
    } else {
      // Hobbywing telemetry header not found. Discard byte
      Serial.read();
    }
  }
}
