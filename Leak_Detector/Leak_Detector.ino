#define BLYNK_PRINT Serial

#include <gas_leak_detector_inferencing.h>
#include <Multichannel_Gas_GMXXX.h>
#include <Wire.h>
#include "TFT_eSPI.h"
#include <rpcWiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleWioTerminal.h>

char auth[] = "your_blynk_auth_code";
char ssid[] = "your_wifi_ssid";
char pass[] = "your_wifi_password";

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

GAS_GMXXX<TwoWire> gas;
 
float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
long lastNotification = 0;

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}
 
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    Serial.println("Edge Impulse Inferencing Demo");
    pinMode(WIO_BUZZER, OUTPUT);

    tft.begin();
    tft.setRotation(3);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_LIGHTGREY);
    tft.drawString("Connecting to ", 70, 80);
    tft.setTextSize(4);
    tft.drawString("Blynk", 80, 120);

    Blynk.begin(auth, ssid, pass);
    gas.begin(Wire, 0x08); // use the hardware I2C  

    tft.fillScreen(TFT_BLACK);
    tft.drawFastHLine(0, 100, 320, TFT_DARKGREY); //Drawing horizontal line
    
}
 
void loop()
{
    Blynk.run();
    ei_printf("++++++++++++++++++++++++++++++++++\nEdge Impulse standalone inferencing (Arduino)\n");
 
    //read sensor data

    
    for(int i=0;i<EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;i=i+4){
      
      features[i]=gas.getGM102B();  
      features[i+1] = gas.getGM302B();
      features[i+2]=gas.getGM502B();
      features[i+3]=gas.getGM702B();
      
      delay(EI_CLASSIFIER_INTERVAL_MS);
    }
    
    // ei_printf(data.c_str());
    // ei_printf("\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
      ei_printf("The size of your 'features' array is not correct. Expected %lu items, but had %lu\n",
                EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
      delay(1000);
      return;
    }
    
    ei_impulse_result_t result = { 0 };
 
    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = &raw_feature_get_data;
 
    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
    ei_printf("run_classifier returned: %d\n", res);
 
    if (res != 0) return;
 
    

    float score = 0;
    String label = "";

    // human-readable predictions
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
        if(result.classification[ix].value > score){
          score = result.classification[ix].value;
          label = result.classification[ix].label;
        }
    }
    ei_printf("$$$$ Detected %s with score %f \n", label.c_str(), score);

    spr.createSprite(320, 98);
    
    if(label == "leakage"){
      
      spr.fillSprite(TFT_RED);
    }
    else{
      spr.fillSprite(TFT_DARKGREEN);
    }
    spr.setTextColor(TFT_LIGHTGREY);
    spr.setTextSize(4);
    spr.drawString(label, 90, 30);
    spr.pushSprite(0, 0);
    spr.deleteSprite();

    spr.createSprite(130, 50);
    spr.fillSprite(TFT_BLACK);
    spr.setTextColor(TFT_LIGHTGREY);
    spr.setTextSize(6);
    spr.drawNumber((int) (score*100), 0,10);
    spr.drawString("%", 105, 10);
    spr.pushSprite(100, 120);
    spr.deleteSprite();
    
    Blynk.virtualWrite(V0,label);
    Blynk.virtualWrite(V1,(int) (score*100));

    if(label == "leakage"){
      analogWrite(WIO_BUZZER,128);
      delay(1000);
      analogWrite(WIO_BUZZER,0);

      if (lastNotification == 0 || (millis() - lastNotification) > 60 * 1000) {
        lastNotification = millis();
        Blynk.notify("Leakage has been detected.");
      }
    }





    
    
    //delay(100);
}
 
/**
 * @brief      Printf function uses vsnprintf and output using Arduino Serial
 *
 * @param[in]  format     Variable argument list
 */
void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };
 
    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);
 
    if (r > 0) {
        Serial.write(print_buf);
    }
}
