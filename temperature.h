#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_



//for NTC sensor
#define FILTER_SIZE 5
#define MIDDLE_IDX (FILTER_SIZE / 2)
#define TEMP_INTERVAL_CHECK 1000

#define SERIES_RESISTOR 4700.0      // Подтягивающий постоянный резистор (4.7 кОм)
#define NOMINAL_RESISTANCE 10000.0  // Сопротивление NTC при 25°C (10 кОм)
#define NOMINAL_TEMPERATURE 25.0    // Номинальная температура датчика в °C
#define B_COEFFICIENT 3950.0        // B-коэффициент термистора (обычно 3950)

uint32_t lastCheckTemp = 0;
uint8_t needTemp = 0;

static uint16_t sortedAdc[FILTER_SIZE] = {0};





// TODO: Temperature sensor type: thermistor or 1-Wire?
uint8_t initTempSensor(void){
	if (TEMP_SENSOR){
		uint16_t val = analogRead(TEMP_SENSOR);
		delay(1);
		val += analogRead(TEMP_SENSOR);
		delay(1);
		val += analogRead(TEMP_SENSOR);
		val = val / 3;
		if (val > 400 && val < 1000){
			return 1;
		}
	}

 return 0;
}


void storeRawAdc(uint16_t value) {
	uint8_t i, kolLow = 0;

	for (i = 0; i < FILTER_SIZE; i++ ){
		if (value > sortedAdc[i]){
			kolLow++;
		} else break;
	}

	if (kolLow < MIDDLE_IDX ){
		for (i = FILTER_SIZE-1; i > kolLow; i--){
		   	sortedAdc[i] = sortedAdc[i - 1];
		 }
		 sortedAdc[kolLow] = value;
     } else{
	  	kolLow--;
		for(i = 0; i < kolLow; i++){
		   	sortedAdc[i] = sortedAdc[i + 1];
		 }
		 sortedAdc[kolLow] = value;
	 }
}


uint16_t getMedianAdc() {

  for (int i = 0; i < FILTER_SIZE; i++) {
    if (sortedAdc[i] == 0) {
      return 0;
    }
  }

  return sortedAdc[MIDDLE_IDX];
}



int8_t convertAdcToCelsiusMath(uint16_t adcVal) {

  float resistance = SERIES_RESISTOR / (1023.0 / (float)adcVal - 1.0);

  float steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);                            // Натуральный логарифм
  steinhart /= B_COEFFICIENT;                            // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);     // + 1/To
  steinhart = 1.0 / steinhart;                           // Переводим в Кельвины
  steinhart -= 273.15;                                   // Переводим в Цельсии


  if (steinhart >= 0.0) {
    return (int8_t)(steinhart + 0.5);
  } else {
    return (int8_t)(steinhart - 0.5);
  }
}










#endif
