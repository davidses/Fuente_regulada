#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_INA219.h>


Adafruit_INA219 ina219;
//LiquidCrystal lcd(4,5,6,7,8,9);
LiquidCrystal_I2C lcd (0x27, 16, 2); // DIR, columnas, lineas 


float corriente = 0;
float voltaje = 0;
float potencia = 0;
float energia = 0;
  
void setup(void) 
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fuente Regulada ");
  lcd.setCursor(0,1);
  lcd.print("    FW: 0.2     ");
  delay(3000);
  lcd.clear();
  Serial.begin(9600);

  //while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
    //  delay(10);
  //}

  uint32_t currentFrequency;
    
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    lcd.setCursor(0,0);
    lcd.print(" Falla en chip  ");
    lcd.setCursor(0,1);
    lcd.print("     INA219     ");
    
    while (1) { delay(10); }
  }
  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Medicion de tension y corriente con INA219...");

  lcd.setCursor(5,0);
  lcd.print(" V");

  lcd.setCursor(13,0);
  lcd.print(" Am");

  lcd.setCursor(5,1);
  lcd.print(" W");

  lcd.setCursor(13,1);
  lcd.print(" Wh");
  
}

void loop(void) 
{
  
  voltaje = ina219.getBusVoltage_V();
  corriente = (ina219.getCurrent_mA() / 1000);   //// convertidos a Amperes
  potencia = voltaje * corriente;
  energia = energia + (potencia / 3600);
  
  printLcdVoltaje();
  printLcdCorriente();
  printLcdPotencia();
  printLcdEnergia();
  
  delay(1000);
}

void printLcdVoltaje(){
  if (voltaje < 0) {
    voltaje = 0;
    lcd.setCursor(0,0);
    lcd.print(voltaje,3);  
  }
  
  if (voltaje < 10) {
    lcd.setCursor(0,0);
    lcd.print(voltaje,3); 
  }

  if (voltaje >= 10){
    lcd.setCursor(0,0);
    lcd.print(voltaje,2);
  }
  
  Serial.print(voltaje,3); Serial.println(" v");
  
}

void printLcdCorriente(){
  if (corriente < 0){
    corriente = 0;
    lcd.setCursor(8,0);
    lcd.print("0.000");
  }
  
  if (corriente > 0) {
    lcd.setCursor(8,0);
    lcd.print(corriente,3);
  }

  Serial.print(corriente,3); Serial.println(" A");
  
}

void printLcdPotencia(){
  if (potencia < 0){
    potencia = 0;
    lcd.setCursor(0,1);
    lcd.print("0.000");
  }
  
  if (potencia > 0) {
    lcd.setCursor(0,1);
    lcd.print(potencia,3);
  }

  Serial.print(potencia,3); Serial.println(" W");

}

void printLcdEnergia(){
  if (energia < 0){
    energia = 0;
    lcd.setCursor(8,1);
    lcd.print("0.000"); 
  }
  
  if (energia > 0) {
    lcd.setCursor(8,1);
    lcd.print(energia,3);
  }

  Serial.print(energia,3); Serial.println(" wh");
  Serial.println("");

}
