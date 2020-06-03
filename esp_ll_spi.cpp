/*
 * Copyright (C) 2019 Seeed Technology Co.,Ltd.
 *
 * created 27 March 2020
 * by Peter Yang <turmary@126.com>
 *
 * MIT License.
 *
 */

// the device communicates using SPI, so include the library:
#include "esp_config.h"
#include "esp_ll_spi.h"

// pins used for the connection with the device
// the other you need are controlled by the SPI library):
#define SPIX             ARDUINO_SPI_TO_ESPAT
const int chipSelectPin = ARDUINO_SPI_PIN_CS;

#define SPI_STATE_MISO    0
#define SPI_STATE_MOSI  (!0)
const int chipSyncPin  =  ARDUINO_SPI_PIN_SYNC;

// SPI transfer tags, commonly used by target SPI AT device
enum {
	SPT_TAG_PRE = 0x55, /* Master initiate a TRANSFER */
	SPT_TAG_ACK = 0xBE, /* Slave  Acknowledgement */
	SPT_TAG_WR  = 0x80, /* Master WRITE  to Slave */
	SPT_TAG_RD  = 0x00, /* Master READ from Slave */
	SPT_TAG_DMY = 0xFF, /* dummy */
};

const int _WAIT_SLAVE_READY_US = 0;

enum {
	SPT_ERR_OK  = 0x00,
	SPT_ERR_DEC_SPC = 0x01,
};

static int spi_transfer_cs(uint8_t v) {
  // take the chip select low to select the device
  digitalWrite(chipSelectPin, LOW);

  v = SPIX.transfer(v);

  // take the chip select high to de-select
  digitalWrite(chipSelectPin, HIGH);

  return v;
}

static int spi_transfer16_cs(uint16_t v) {
  uint16_t r;

  r  = spi_transfer_cs(v >> 8) << 8;
  r |= spi_transfer_cs(v & 0xFF);
  return r;
}

int at_wait_io(int level) {
  int i;
  for (i = 0; digitalRead(chipSyncPin) != level; i++) {
    delayMicroseconds(10);
    if (i > 500 /*ms*/ * 100) {
      break;
    }
  }
  return 1;
}

int at_spi_write(const uint8_t* buf, uint16_t len, int loop_wait) {
  uint8_t v;
  int i;
  int r = 0;

  /* wait slave ready to transfer data */
  delayMicroseconds(_WAIT_SLAVE_READY_US);

  spi_transfer16_cs((SPT_TAG_PRE << 8) | SPT_TAG_WR);
  spi_transfer16_cs(len);


  /* wait slave ready to transfer data */
  at_wait_io(SPI_STATE_MISO);

  v = spi_transfer_cs(SPT_TAG_DMY);
  if (v != SPT_TAG_ACK) {
    /* device too slow between TAG_PRE and TAG_ACK */
    Serial.printf("No ACK, R%02X\r\n", v);
    r = -1;
    goto __ret;
  }

  v = spi_transfer_cs(SPT_TAG_DMY);
  if (v != SPT_ERR_OK && v != SPT_ERR_DEC_SPC) {
    r = -1000 - v; /* device not ready */
    goto __ret;
  }

  len = spi_transfer16_cs((SPT_TAG_DMY << 8) | SPT_TAG_DMY);


  at_wait_io(SPI_STATE_MOSI);
  for (i = 0; i < len; i++) {
    spi_transfer_cs(buf[i]);
  }

  at_wait_io(SPI_STATE_MOSI);

  /*
  Serial.print("Trans ");
  Serial.print(len);
  Serial.println("B");
  */
  r = len; /* success transfer len bytes */

__ret:
  return r;
}

int at_spi_read(uint8_t* buf, uint16_t len, int loop_wait) {
  uint8_t v;
  int i;
  int r = 0;

  /* wait slave ready to transfer data */
  delayMicroseconds(_WAIT_SLAVE_READY_US);

  spi_transfer16_cs((SPT_TAG_PRE << 8) | SPT_TAG_RD);
  spi_transfer16_cs(len);


  /* wait slave ready to transfer data */
  at_wait_io(SPI_STATE_MISO);
  v = spi_transfer_cs(SPT_TAG_DMY);
  if (v != SPT_TAG_ACK) {
    /* device too slow between TAG_PRE and TAG_ACK */
    Serial.printf("No ACK, R%02X\r\n", v);
    r = -1;
    goto __ret;
  }

  v = spi_transfer_cs(SPT_TAG_DMY);
  if (v != SPT_ERR_OK && v != SPT_ERR_DEC_SPC) {
    r = -1000 - v; /* device not ready */
    goto __ret;
  }

  len = spi_transfer16_cs((SPT_TAG_DMY << 8) | SPT_TAG_DMY);

  at_wait_io(SPI_STATE_MOSI);

  if (len) {
    at_wait_io(SPI_STATE_MISO);

    for (i = 0; i < len; i++) {
      buf[i] = spi_transfer_cs(SPT_TAG_DMY);
    }
    r = len; /* success transfer len bytes */

    at_wait_io(SPI_STATE_MOSI);
  }

__ret:

  return r;
}

int at_spi_begin(void) {
  // Reset SPI slave device(RTL8720D)
  pinMode(RTL8720D_CHIP_PU, OUTPUT);
  digitalWrite(RTL8720D_CHIP_PU, LOW);

  // start the SPI library:
  SPIX.begin();
  // Start SPI transaction at a quarter of the MAX frequency
  SPIX.beginTransaction(SPISettings(MAX_SPI / 4, MSBFIRST, SPI_MODE0));

  // initalize the  data ready and chip select pins:
  pinMode(chipSyncPin, INPUT);
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, HIGH);

  // When RTL8720D startup, set pin UART_LOG_TXD to lowlevel
  // will force the device enter UARTBURN mode.
  // Explicit high level will prevent above things.
  pinMode(PIN_SERIAL2_RX, OUTPUT);
  digitalWrite(PIN_SERIAL2_RX, HIGH);

  // reset duration
  delay(20);
  // Release RTL8720D reset, start bootup.
  digitalWrite(RTL8720D_CHIP_PU, HIGH);
  // give the slave time to set up
  delay(500);
  pinMode(PIN_SERIAL2_RX, INPUT);

  return 0;
}

#if 0
void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Begin SPI:");
  at_spi_begin();

  Serial.println("\nConnecting");

  // give the slave time to set up
  delay(100);
  Serial.println("Ready! Enter some AT commands");

  return;
}

#define U_BUF_SZ 254
uint8_t u_buf[U_BUF_SZ + 2];
#define S_BUF_SZ 254
uint8_t s_buf[S_BUF_SZ + 2];
int idx = 0;

void loop() {
  uint8_t c;
  int r;

  while (Serial.available()) {
    c = Serial.read();

    /* not empty line or \r\n */
    if (idx == 0) {
      if (c == '\r' || c == '\n') continue;
    }

    /* process all \r, \n, \r\n */
    if (c == '\n') c = '\r';
    u_buf[idx] = c;
    if (idx < U_BUF_SZ)
      idx++;

    /* maybe leave a char '\n' in Serial object RX buffer */
    if (c == '\r')
      break;
  }

  if (idx && u_buf[idx - 1] == '\r') {
    u_buf[idx] = '\0';

    r = at_spi_write(u_buf, idx);
    if (r < 0) {
      Serial.print("AT_WRITE ERR ");
      Serial.println(r);
      delay(1000);
    }
    idx = 0;
  }

  r = at_spi_read(s_buf, S_BUF_SZ);
  if (r < 0) {
    Serial.print("AT_READ ERR ");
    Serial.println(r);
    delay(1000);

  } else if (r >= 0) {
    int i;

    for (i = 0; i < r; i++) {
      char obuf[0x10];
      uint8_t b = s_buf[i];
      int o;

      o = 0;
      if (b < 0x20 || b == '[' || b >= 0x7F) {
      	o += sprintf(obuf + o, "[%02X", s_buf[i]);
      } else {
        o += sprintf(obuf + o, "%c", s_buf[i]);
      }
      Serial.print(obuf);
      if (b == 0x0A) {
      	Serial.println();
      }
    }
    // Serial.println("Read OK");
  }
  return;
}
#endif