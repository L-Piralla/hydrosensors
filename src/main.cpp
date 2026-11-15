#include <Arduino.h>

const int SENSOR_PIN = 4;

const int AVG_LEN = 16;

const int DRY_MV = 2200;
const int WET_MV = 900;

const int DRY_HUM_THR = 30;
const int MED_HUM_THR = 60;
const int OK_HUM_THR = 90;

enum class Status { DRY, MEDIUM, OK, WET };


int avgMv(int, int);

int humidityPercentage(int);

Status humidityStatus(int);

void writeLed(Status);

void setup() {
    Serial.begin(115200);
}

void loop() {
    int avg_mv = avgMv(SENSOR_PIN, AVG_LEN);
    int hum_percentage = humidityPercentage(avg_mv);

    Serial.printf("mV=%d,  humidity=%d%%\n", avg_mv, hum_percentage);

    writeLed(humidityStatus(hum_percentage));
    delay(1000);
}

int avgMv(int pin, int avg_len) {
    int avg_mv=0;
    for(int i = 0; i < avg_len; i++) {
        avg_mv += analogReadMilliVolts(pin);
        delay(2);
    }
    return avg_mv / avg_len;
}

int humidityPercentage(int mv) {
    int percentage = (DRY_MV - mv) * 100 / (DRY_MV - WET_MV);
    return constrain(percentage, 0, 100);
}

Status humidityStatus(int hum_perc) {
    if (hum_perc <= DRY_HUM_THR)
        return Status::DRY;
    if (hum_perc <= MED_HUM_THR)
        return Status::MEDIUM;
    if (hum_perc <= OK_HUM_THR)
        return Status::OK;
    return Status::WET;
}

void writeLed(Status status) {
    switch(status){
        case Status::DRY:
            rgbLedWrite(RGB_BUILTIN, 128, 0, 0);
            break;
        case Status::MEDIUM:
            rgbLedWrite(RGB_BUILTIN, 96, 32, 0);
            break;
        case Status::OK:
            rgbLedWrite(RGB_BUILTIN, 0, 128, 0);
            break;
        case Status::WET:
            rgbLedWrite(RGB_BUILTIN, 0, 0, 128);
            break;
    }
}