#include <util/delay.h>
#include "ADC.c"

struct adc_output;

void adc_init (void);
struct adc_output adc_read(); //volatile
void initPWM();