
#include "msp430x14x.h" //430�Ĵ���ͷ�ļ�
#include "ADS1293.h"     //ADS1293����ͷ�ļ������������ļ������ú���
#include "Spi.h"        //SPIͨѶ�����ͷ�ļ�
#include "TI_ADS1293_register_settings.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  return 0;
}
