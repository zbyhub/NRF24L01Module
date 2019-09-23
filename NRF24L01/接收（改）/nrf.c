#include <reg52.h>
#include <intrins.h>
#include"NRF24L01.H"

extern uchar rx_buf[RX_PLOAD_WIDTH];
extern uchar tx_buf[TX_PLOAD_WIDTH];

/**************************************************延时程序*********************************************************/
void delayus(uint t)//延时（us）
{
    while(t--)
    {
        _nop_();		
    }
}
void delayms(uint t)//延时（ms）
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
/**************************************************读取程序***********************************************************/
uchar SPI_RW(uchar byte)//SPI基础程序（重要*****）
{
    uchar i;
       for(i=0; i<8; i++)          // 循环8次
       {
           MOSI = (byte & 0x80);   // byte最高位输出到MOSI
           byte <<= 1;             // 低一位移位到最高位
           SCK = 1;                // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
           byte |= MISO;           // 读MISO到byte最低位
           SCK = 0;                // SCK置低
       }
    return(byte);               // 返回读出的一字节
}
uchar SPI_write(uchar add,daa)//把daa数据写到add寄存器
{
    uchar status;
      CSN = 0;                          // CSN置低，开始传输数据
      status = SPI_RW(add);      // 选择寄存器，同时返回状态字
      SPI_RW(daa);             // 然后写数据到该寄存器
      CSN = 1;                   // CSN拉高，结束数据传输
      return(status);            // 返回状态寄存器
}
uchar SPI_read(uchar add)//从add寄存器读数据
{
    uchar dta;
    CSN=0;
    SPI_RW(add);
    dta=SPI_RW(0xff);
    CSN=1;
    return dta;
}
//用于读多字节寄存器数据（add:寄存器地址，*p:读取数据存放地址，num:读取字节个数）
uchar SPI_read_buf(uchar add, uchar *pbuf, uchar num)
{
    uchar status;
	uchar i;
	CSN = 0;
	status = SPI_RW(add);	 // 选择寄存器，同时返回状态字
	for(i=0;i<num;i++)
	    pbuf[i] = SPI_RW(0xff);
	CSN = 1;
	return(status);
}
//用于将数据写入多字节寄存器（add:寄存器地址，*p:要写入的数据的地址，num:写入字节个数)
uchar SPI_write_buf(uchar add, uchar *pbuf, uchar num)
{
    uchar status;
	uchar i;
	CSN = 0;			//SPI使能
	status = SPI_RW(add);
	for(i=0;i<num;i++)
	    SPI_RW(*pbuf++);
	CSN = 1;		    //SPI关闭
	return(status);    
}

void NRF24L01_Freq(uint fre)
{
    fre = fre - 2400;
   	CE = 0;
	SPI_write(WRITE_REG + RF_CH, fre);//选择通信频率 = (2400 + RF_CH)MHZ
}
void first()
{
    CE = 0;      //射频停止工作
	CSN = 1;	 //SPI操作禁止
	SCK = 0;	 //SPI时钟为0
	IRQ = 1;	 //中断复位可以接收中断信号
}
/***************************************************发送*********************************************************************/
/*NRF24L01  发射  初始化*/
void SendInit_NRF24L01(void)
{
	first();	    
	SPI_write(WRITE_REG + EN_AA, 0x01);//设置自动应答允许
	SPI_write(WRITE_REG + EN_RXADDR, 0x01);//设置0通道允许接收
    SPI_write(WRITE_REG + SETUP_RETR, 0x03);//配置自动重发次数(3次)   			 
	//SPI_write(WRITE_REG + RF_CH, 0x01);//选择通信频率 = (2400 + RF_CH)MHZ		 
	SPI_write(WRITE_REG + RF_SETUP, 0x0f);//配置发射参数（发射功率0dBm，数据传输速率2Mbps）
	SPI_write(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);//设置0通道有效数据宽度（就是要传送数据包的长度）
}
/*发射模式*/
void SetTx_Mode(void)
{
    CE = 0;
	SPI_write(WRITE_REG + CONFIG, 0x0e);//IRQ收发完成中断响应	
}
/*地址*/
void NRF24L01_Tx_ADDR(uchar *tx_addr)
{
    CE = 0;	//停止射频电路工作
	//SPI_write_buf(WRITE_REG + TX_ADDR, tx_addr, TX_ADR_WIDTH);//写本地地址
	//SPI_write_buf(WRITE_REG + RX_ADDR_P0, tx_addr, RX_ADR_WIDTH);//装载0通道地址
    SPI_write_buf(WRITE_REG + TX_ADDR, tx_addr, 5);//写本地地址
	SPI_write_buf(WRITE_REG + RX_ADDR_P0, tx_addr, 5);//装载0通道地址	
}
/*发送机数据发送*/
void NRF24L01_TxPacket(uchar *tx_buf)
{
    CE = 0;	//停止射频电路工作	
	SPI_write_buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);//装载要发送的数据	
	CE = 1;//置高CE，激发数据发送
	delayus(100);
	CE = 0;
}
/*****************************************************接收******************************************************************/
/*NRF24L01  接收  初始化*/
void RecInit_NRF24L01(void)
{	
    first();
	NRF24L01_Freq(2401);
	//SPI_write(WRITE_REG + EN_AA, channel);//设置自动应答允许
	//SPI_write(WRITE_REG + EN_RXADDR, channel);//设置0通道允许接收
	//SPI_write(WRITE_REG + RF_CH, 0x01);//选择通信频率 = (2400 + RF_CH)MHZ
	SPI_write(WRITE_REG + RF_SETUP, 0x0f);//配置发射参数（发射功率0dBm，数据传输速率2Mbps）
	//SPI_write(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);//设置0通道有效数据宽度（就是要传送数据包的长度）
}
/*接收机地址，数据接收*/
void NRF24L01_Rx_Channel(uchar channel)
{  
    CE = 0;	//停止射频电路工作
	SPI_write(WRITE_REG + EN_AA, channel);//设置自动应答允许
	SPI_write(WRITE_REG + EN_RXADDR, channel);//设置0通道允许接收
	//SPI_write(WRITE_REG + RX_PW_P0 + channel, RX_PLOAD_WIDTH);//设置0通道有效数据宽度（就是要传送数据包的长度）
	//SPI_write_buf(WRITE_REG + TX_ADDR, rx_addr, 5);//写本地地址
	//SPI_write_buf(WRITE_REG + RX_ADDR_P0 + channel, rx_addr, Addr_Num);//装载0通道地址	
}
void NRF24L01_Rx_ADDR(uchar *rx_addr,uchar channel,uchar Addr_Num)
{
	CE = 0;
    SPI_write(WRITE_REG + RX_PW_P0 + channel, RX_PLOAD_WIDTH);//设置0通道有效数据宽度（就是要传送数据包的长度）
	//SPI_write_buf(WRITE_REG + TX_ADDR, rx_addr, 5);//写本地地址
	SPI_write_buf(WRITE_REG + RX_ADDR_P0 + channel, rx_addr, Addr_Num);//装载0通道地址
}
 /*接收模式设定*/
void SetRx_Mode(void)
{
    CE = 0;
	SPI_write(WRITE_REG + CONFIG, 0x0f);//IRQ收发完成中断响应
	CE = 1;
}
/*数据读取*/
void NRF24L01_RxPacket(uchar *rx_buf)
{
    CE = 0;
	SPI_read_buf(RD_RX_PLOAD, rx_buf, RX_PLOAD_WIDTH);
	SPI_write(WRITE_REG + STATUS, 0xff);	
}