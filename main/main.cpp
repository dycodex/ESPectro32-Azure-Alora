/*
 * Created by Andri Yadi, Aug 8, 2017
*/

#include <esp_log.h>
#include "sdkconfig.h"

#include <Arduino.h>
#define _GLIBCXX_USE_C99
#include <string>

#include <ESPectro32_Board.h>
#include <AppSetting.h>

#include <WiFiManager.h>
#include "LedMatrixImages.h"
#include <ESPectro32_LedMatrix_Animation.h>
#include <ESPectro32_RGBLED_Animation.h>

#include <AloraSensorKit.h>
#include <AzureIoTHubMQTTClient.h>

#define IOTHUB_HOSTNAME         "dycodex.azure-devices.net"
#define DEVICE_ID               "espectro-01"
#define DEVICE_KEY              "nhWXiles4WXOsM7DQjnTCrTk8zuZGdK55hHYF3BniLU=" //Primary key of the device

AzureIoTHubMQTTClient client(IOTHUB_HOSTNAME, DEVICE_ID, DEVICE_KEY);

AloraSensorKit sensorKit;

WiFiManager wifiMgr;
ESPectro32_LedMatrix_Animation ledMatrixAnim;

unsigned long lastDataPublish = 0;

const static char *TAG ="APP";

extern "C" {
	void app_main(void);
}

void setup() {

	initArduino();
	Serial.begin(115200);

	ESP_LOGI(TAG, "It begins!");

	ESPectro32.begin();
	
	Wire.begin();	
	sensorKit.begin();

	delay(2000);

	// sensorKit.scanAndPrintI2C(Serial);
	// delay(1000);

	ledMatrixAnim.setLedMatrix(ESPectro32.LedMatrix());
	//Define frames
	ledMatrixAnim.addFrameWithData((uint8_t*)LED_MATRIX_WIFI_1);
	ledMatrixAnim.addFrameWithData((uint8_t*)LED_MATRIX_WIFI_2);
	ledMatrixAnim.addFrameWithData((uint8_t*)LED_MATRIX_WIFI_3);
	ledMatrixAnim.addFrameWithDataCallback([](ESPectro32_LedMatrix &ledM) {
		ledM.clear();
	});
	//Actually start animation
	ledMatrixAnim.start(1800, true);

	wifiMgr.onWiFiConnected([](bool newConn) {
		ESP_LOGI("WIFI", "IP: %s", wifiMgr.getStationIpAddress().c_str());
	});

	wifiMgr.begin(WIFI_MODE_STA, false);
	//wifiMgr.connectToAP("Andri's iPhone 6s", "11223344");
	wifiMgr.connectToAP("GERES10", "p@ssw0rd");

	//Actually start WiFi Manager
	wifiMgr.start();

	// Wait for connection, this will block
	if (wifiMgr.waitForConnection()) {
		ledMatrixAnim.stop();
		ESPectro32.LedMatrix().displayFrame(2);

		AZURE_INFO_PRINT("Connected");

		client.begin();
		client.start();

		client.onEvent([](AzureIoTHubMQTTClient::AzureIoTHubMQTTClientEvent e) {
			AZURE_INFO_PRINT("Network event: %d", e);
		});

		//Handle command from cloud
		client.onSubscriptionDataAvailable([](AzureIoTHubMQTTClient::mqtt_subscription_data_t &subsData) {
			AZURE_INFO_PRINT("Subscribed topic: %s", subsData.topic.c_str());
			AZURE_INFO_PRINT("Subscribed payload: %s", subsData.payload.c_str());
		});
	}
}

void loop() {
	
	sensorKit.run();
	
	if ((millis() - lastDataPublish > 5000)) {

		// String debStr;
		// sensorKit.printSensingTo(debStr);
		// Serial.println(F("Sensor Debug:"));
		// Serial.println(debStr);

		SensorValues sensorData = sensorKit.getLastSensorData();

		char payloadStr[150];
		sprintf(payloadStr, "{\"deviceId\":\"%s\",\"temperature\":%.2f,\"light\":%.2f,\"pressure\":%.2f,\"airQuality\":%d,\"humidity\":%.2f}",
				DEVICE_ID, 
				sensorData.T1, 
				sensorData.lux,
				sensorData.P,
				sensorData.gas,
				sensorData.H1);

		client.sendEvent(payloadStr);

		lastDataPublish = millis();
	}
}

void app_main(void)
{
	setup();

	for(;;) {
		loop();
	}
}

