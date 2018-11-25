															  
#ifndef __I2C_H__
#define __I2C_H__

extern void I2C_INIT(void);
extern void I2C_START(void);
extern void I2C_WRITE( char data );
extern unsigned char I2C_READ_ACK( void );
extern unsigned char I2C_READ_NACK( void );
extern void I2C_MULTIREAD( char* arr , int bytes );
extern void I2C_STOP( void );
extern void I2C_func(unsigned char addr, unsigned char dat);

#endif