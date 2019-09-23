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
uchar tx_buf[RX_PLOAD_WIDTH] = {31,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};



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
	//unsigned char i;
	//unsigned char j = 1;
	//unsigned char x[4];
	ConfigUART(9600);
	RecInit_NRF24L01();
    NRF24L01_Rx_Channel(0x01);
    NRF24L01_Rx_ADDR(TX_ADDRESS_0,0,5);
    SetRx_Mode();//该步骤开启接收（有CE = 1）    	
	EA = 1;
	EX0 = 1;
	IT0 = 0;
	while(1);
 	
	    //unsigned char i;    
	    //while(!RI);
	    //IE0 = 1;
	    //RI = 0;	
		//x++;
		//if(x == 0x0f)x = 0x01;
		/*if(IRQ == 0)
		{
            NRF24L01_RxPacket(rx_buf);
			   
		    for(i = 1;i < 32;i++)
	        {
	           SBUF = rx_buf[i];
		       while(!TI);
		       TI = 0;
 	        }
			//SPI_write(WRITE_REG + STATUS, 0xff);
			//SPI_write(FLUSH_RX, 0xff);
		    CE = 1;	
		}*/	   
}
void Int0() interrupt 0
{
    unsigned char i;
    NRF24L01_RxPacket(rx_buf);
	for(i = 0;i < 32;i++)
	{
	    SBUF = rx_buf[i];
		while(!TI);
		TI = 0;
	}	
	IE0 = 0;
	CE = 1;
} 