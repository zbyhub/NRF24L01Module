#ifndef _NREF24L01_H
#define _NREF24L01_H
/*********************************************************************************************************/
typedef unsigned int uint;
typedef unsigned char uchar;
#define TX_PLOAD_WIDTH 32   
#define RX_PLOAD_WIDTH 32

/*********************************************************************************************************/
void delayus(uint t);
void delayms(uint t);
/*********************************************************************************************************/
uchar SPI_RW(uchar byte);//SPI基础程序（重要）
uchar SPI_write(uchar add,daa);//把daa数据写到add寄存器
uchar SPI_read(uchar add);//从add寄存器读数据
uchar SPI_read_buf(uchar add, uchar *pbuf, uchar num);
uchar SPI_write_buf(uchar add, uchar *pbuf, uchar num);
/*********************************************************************************************************/
void SendInit_NRF24L01(void);//发送初始化(已包含频率设定)
void SetTx_Mode(void);
void NRF24L01_Tx_ADDR(uchar *tx_addr);
void NRF24L01_TxPacket(uchar *tx_buf);//该步骤开启发射（有CE = 1开启发射和CE = 0关闭发射语句，不包含【中断清零SPI_write(WRITE_REG + STATUS, 0xff)】和【数据清除SPI_write(FLUSH_TX, 0xff)】
/*********************************************************************************************************/
void RecInit_NRF24L01(void);//接收初始化(已包含频率设定)
void NRF24L01_Rx_Channel(uchar channel);
void NRF24L01_Rx_ADDR(uchar *rx_addr,uchar channel,uchar Addr_Num);
void SetRx_Mode(void);//该步骤开启接收（有CE = 1）
void NRF24L01_RxPacket(uchar *rx_buf);//（内部包含中断清零）
/*********************************************************************************************************/
void NRF24L01_Freq(uint fre);
void first();
/*********************************************************************************************************/
sbit CE   = P1^0;
sbit CSN  = P1^1;
sbit IRQ  = P1^2;
sbit MISO = P1^3;
sbit MOSI = P1^4;
sbit SCK  = P1^5;

/***********************************************************************/


//NRF24L01寄存器指令
#define READ_REG        0x00   // 读寄存器指令
#define WRITE_REG       0x20  // 写寄存器指令
#define RD_RX_PLOAD     0x61   // 读取接收数据指令
#define WR_TX_PLOAD     0xA0   // 写待发数据指令
#define FLUSH_TX        0xE1  // 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2   // 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3   // 定义重复装载数据指令
#define NOP             0xFF   // 保留
//SPI(nRF24L01)寄存器地址
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
#endif