#include <reg52.h>
#include <intrins.h>
#include"NRF24L01.H"

extern uchar rx_buf[RX_PLOAD_WIDTH];
extern uchar tx_buf[TX_PLOAD_WIDTH];

/**************************************************��ʱ����*********************************************************/
void delayus(uint t)//��ʱ��us��
{
    while(t--)
    {
        _nop_();		
    }
}
void delayms(uint t)//��ʱ��ms��
{
    uint i;
    while(t--)
	{
	    for(i = 1000;i>1;i--)
		{
		    _nop_();		  
		}
	}
}
/**************************************************��ȡ����***********************************************************/
uchar SPI_RW(uchar byte)//SPI����������Ҫ*****��
{
    uchar i;
       for(i=0; i<8; i++)          // ѭ��8��
       {
           MOSI = (byte & 0x80);   // byte���λ�����MOSI
           byte <<= 1;             // ��һλ��λ�����λ
           SCK = 1;                // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
           byte |= MISO;           // ��MISO��byte���λ
           SCK = 0;                // SCK�õ�
       }
    return(byte);               // ���ض�����һ�ֽ�
}
uchar SPI_write(uchar add,daa)//��daa����д��add�Ĵ���
{
    uchar status;
      CSN = 0;                          // CSN�õͣ���ʼ��������
      status = SPI_RW(add);      // ѡ��Ĵ�����ͬʱ����״̬��
      SPI_RW(daa);             // Ȼ��д���ݵ��üĴ���
      CSN = 1;                   // CSN���ߣ��������ݴ���
      return(status);            // ����״̬�Ĵ���
}
uchar SPI_read(uchar add)//��add�Ĵ���������
{
    uchar dta;
    CSN=0;
    SPI_RW(add);
    dta=SPI_RW(0xff);
    CSN=1;
    return dta;
}
//���ڶ����ֽڼĴ������ݣ�add:�Ĵ�����ַ��*p:��ȡ���ݴ�ŵ�ַ��num:��ȡ�ֽڸ�����
uchar SPI_read_buf(uchar add, uchar *pbuf, uchar num)
{
    uchar status;
	uchar i;
	CSN = 0;
	status = SPI_RW(add);	 // ѡ��Ĵ�����ͬʱ����״̬��
	for(i=0;i<num;i++)
	    pbuf[i] = SPI_RW(0xff);
	CSN = 1;
	return(status);
}
//���ڽ�����д����ֽڼĴ�����add:�Ĵ�����ַ��*p:Ҫд������ݵĵ�ַ��num:д���ֽڸ���)
uchar SPI_write_buf(uchar add, uchar *pbuf, uchar num)
{
    uchar status;
	uchar i;
	CSN = 0;			//SPIʹ��
	status = SPI_RW(add);
	for(i=0;i<num;i++)
	    SPI_RW(*pbuf++);
	CSN = 1;		    //SPI�ر�
	return(status);    
}

void NRF24L01_Freq(uint fre)
{
    fre = fre - 2400;
   	CE = 0;
	SPI_write(WRITE_REG + RF_CH, fre);//ѡ��ͨ��Ƶ�� = (2400 + RF_CH)MHZ
}
void first()
{
    CE = 0;      //��Ƶֹͣ����
	CSN = 1;	 //SPI������ֹ
	SCK = 0;	 //SPIʱ��Ϊ0
	IRQ = 1;	 //�жϸ�λ���Խ����ж��ź�
}
/***************************************************����*********************************************************************/
/*NRF24L01  ����  ��ʼ��*/
void SendInit_NRF24L01(void)
{
	first();	    
	SPI_write(WRITE_REG + EN_AA, 0x01);//�����Զ�Ӧ������
	SPI_write(WRITE_REG + EN_RXADDR, 0x01);//����0ͨ���������
    SPI_write(WRITE_REG + SETUP_RETR, 0x03);//�����Զ��ط�����(3��)   			 
	//SPI_write(WRITE_REG + RF_CH, 0x01);//ѡ��ͨ��Ƶ�� = (2400 + RF_CH)MHZ		 
	SPI_write(WRITE_REG + RF_SETUP, 0x0f);//���÷�����������书��0dBm�����ݴ�������2Mbps��
	SPI_write(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);//����0ͨ����Ч���ݿ�ȣ�����Ҫ�������ݰ��ĳ��ȣ�
}
/*����ģʽ*/
void SetTx_Mode(void)
{
    CE = 0;
	SPI_write(WRITE_REG + CONFIG, 0x0e);//IRQ�շ�����ж���Ӧ	
}
/*��ַ*/
void NRF24L01_Tx_ADDR(uchar *tx_addr)
{
    CE = 0;	//ֹͣ��Ƶ��·����
	//SPI_write_buf(WRITE_REG + TX_ADDR, tx_addr, TX_ADR_WIDTH);//д���ص�ַ
	//SPI_write_buf(WRITE_REG + RX_ADDR_P0, tx_addr, RX_ADR_WIDTH);//װ��0ͨ����ַ
    SPI_write_buf(WRITE_REG + TX_ADDR, tx_addr, 5);//д���ص�ַ
	SPI_write_buf(WRITE_REG + RX_ADDR_P0, tx_addr, 5);//װ��0ͨ����ַ	
}
/*���ͻ����ݷ���*/
void NRF24L01_TxPacket(uchar *tx_buf)
{
    CE = 0;	//ֹͣ��Ƶ��·����	
	SPI_write_buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);//װ��Ҫ���͵�����	
	CE = 1;//�ø�CE���������ݷ���
	delayus(100);
	CE = 0;
}
/*****************************************************����******************************************************************/
/*NRF24L01  ����  ��ʼ��*/
void RecInit_NRF24L01(void)
{	
    first();
	NRF24L01_Freq(2401);
	//SPI_write(WRITE_REG + EN_AA, channel);//�����Զ�Ӧ������
	//SPI_write(WRITE_REG + EN_RXADDR, channel);//����0ͨ���������
	//SPI_write(WRITE_REG + RF_CH, 0x01);//ѡ��ͨ��Ƶ�� = (2400 + RF_CH)MHZ
	SPI_write(WRITE_REG + RF_SETUP, 0x0f);//���÷�����������书��0dBm�����ݴ�������2Mbps��
	//SPI_write(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);//����0ͨ����Ч���ݿ�ȣ�����Ҫ�������ݰ��ĳ��ȣ�
}
/*���ջ���ַ�����ݽ���*/
void NRF24L01_Rx_Channel(uchar channel)
{  
    CE = 0;	//ֹͣ��Ƶ��·����
	SPI_write(WRITE_REG + EN_AA, channel);//�����Զ�Ӧ������
	SPI_write(WRITE_REG + EN_RXADDR, channel);//����0ͨ���������
	//SPI_write(WRITE_REG + RX_PW_P0 + channel, RX_PLOAD_WIDTH);//����0ͨ����Ч���ݿ�ȣ�����Ҫ�������ݰ��ĳ��ȣ�
	//SPI_write_buf(WRITE_REG + TX_ADDR, rx_addr, 5);//д���ص�ַ
	//SPI_write_buf(WRITE_REG + RX_ADDR_P0 + channel, rx_addr, Addr_Num);//װ��0ͨ����ַ	
}
void NRF24L01_Rx_ADDR(uchar *rx_addr,uchar channel,uchar Addr_Num)
{
	CE = 0;
    SPI_write(WRITE_REG + RX_PW_P0 + channel, RX_PLOAD_WIDTH);//����0ͨ����Ч���ݿ�ȣ�����Ҫ�������ݰ��ĳ��ȣ�
	//SPI_write_buf(WRITE_REG + TX_ADDR, rx_addr, 5);//д���ص�ַ
	SPI_write_buf(WRITE_REG + RX_ADDR_P0 + channel, rx_addr, Addr_Num);//װ��0ͨ����ַ
}
 /*����ģʽ�趨*/
void SetRx_Mode(void)
{
    CE = 0;
	SPI_write(WRITE_REG + CONFIG, 0x0f);//IRQ�շ�����ж���Ӧ
	CE = 1;
}
/*���ݶ�ȡ*/
void NRF24L01_RxPacket(uchar *rx_buf)
{
    CE = 0;
	SPI_read_buf(RD_RX_PLOAD, rx_buf, RX_PLOAD_WIDTH);
	SPI_write(WRITE_REG + STATUS, 0xff);	
}