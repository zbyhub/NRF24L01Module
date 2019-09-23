#include <reg52.h>
#include <intrins.h>
#include"NRF24L01.H"

uchar const TX_ADDRESS_0[5] = {0xe7,0xe7,0xe7,0xe7,0xe7};
uchar const TX_ADDRESS_1[5] = {0xc2,0xc2,0xc2,0xc2,0xc2};
uchar const TX_ADDRESS_2[5] = {0xc3,0xc2,0xc2,0xc2,0xc2};
uchar const TX_ADDRESS_3[5] = {0xc4,0xc2,0xc2,0xc2,0xc2};
uchar const TX_ADDRESS_4[5] = {0xc5,0xc2,0xc2,0xc2,0xc2};
uchar const TX_ADDRESS_5[5] = {0xc6,0xc2,0xc2,0xc2,0xc2};

uchar rx_buf[TX_PLOAD_WIDTH];
uchar tx_buf[RX_PLOAD_WIDTH] = {31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31};



void ConfigUART(unsigned int baud)
{
	SCON = 0x50;
	TMOD &= 0X0F;
	TMOD |= 0x20;
	TH1 = 256 - (11059200/12/32)/baud;
	TL1 = TH1;
	ET1 = 0;
	TR1 = 1;	
}
void main()
{   
	uchar i;
	//unsigned char x[4];
	/************************************uchar chan = 0;***********************/
	uchar j;
	ConfigUART(9600);
    SendInit_NRF24L01();	
	NRF24L01_Freq(2401);
	SetTx_Mode();
	while(1)
	{		   
	    tx_buf[1] = '7';
		tx_buf[2] = '.';
		tx_buf[3] = '7';
		SPI_write(WRITE_REG + STATUS, 0xff);
		SPI_write(FLUSH_TX, 0xff);
/******************************************************************(µÿ÷∑±‰ªª≥Ã–Ú)
		switch(chan)
		{
		    case 0 : NRF24L01_Tx_ADDR(TX_ADDRESS_0); chan++; break;
			case 1 : NRF24L01_Tx_ADDR(TX_ADDRESS_1); chan++; break;
			case 2 : NRF24L01_Tx_ADDR(TX_ADDRESS_2); chan++; break;
			case 3 : NRF24L01_Tx_ADDR(TX_ADDRESS_3); chan++; break;
			case 4 : NRF24L01_Tx_ADDR(TX_ADDRESS_4); chan++; break;
			case 5 : NRF24L01_Tx_ADDR(TX_ADDRESS_5); chan=0; break;
		}
*********************************************************************************/		
		NRF24L01_TxPacket(tx_buf);		
		delayms(500);
		
	}
}	