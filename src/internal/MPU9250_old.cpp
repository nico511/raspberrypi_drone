/*
 * Code is based on the MPU9250 code by Kris Winer on his github
 * link: https://github.com/kriswiner/MPU9250/blob/master/MPU9250BasicAHRS_t3.ino
 */
#include <syslog.h>
#include <chrono>
#include <thread>

#include "MPU9250.h"

//! Initialization for MPU9250
/*!
 \param bus I2C bus number
 \param address 8 bit address
 *
 * Default settings: 
 * 
 * 	FIFO List: Enabled
 * 	FSYNC: Disabled
 * 	DLPF_CFG: 1
 * 	SMPLRT_DIV: 4 (200hz)
 */

MPU9250::MPU9250(int bus, int address)
{
	/*
	min_az = 0.0;
	max_az = 0.0;
	min_gx = 0.0;
	max_gx = 0.0;
	min_gy = 0.0;
	max_gy = 0.0;
	min_gz = 0.0;
	max_gz = 0.0;

	ax_offset = 0.0;
	ay_offset = 0.0;
	az_offset = 0.0;

	gx_offset = 0.0;
	gy_offset = 0.0;
	gz_offset = 0.0;
	*/
	
	i2c = I2C(bus, address);
	
	
	//Writes binary 10000000 to register to reset all registers to default values
	i2c.write_byte(PWR_MGMT_1, 0x80);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	
	
	//writes binary 01000000 | 00000001 to register to enable FIFO, disable FSYNC, and set DLPF_CFG to 1 
	i2c.write_byte(CONFIG, (0x40 | 0x01);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//writes -- sample rate = (gyroscope output rate)/(1 + SMPLRT_DIV)  
	// in this init, it would be 1khz/(1 + 0x04) = 200hz sample rate
	i2c.write_byte(SMPLRT_DIV, 0x04);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//writes binary 00000001 to register to Auto select the best available clock source
	i2c.write_byte(PWR_MGMT_1, 0x01);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
			//uint8_t read_gyro_config = read_byte(GYRO_CONFIG);
			//i2c.write_byte(GYRO_CONFIG, read_gyro_config & ~0xE0);
	
	
	//set bits [7:3] to 0 and leaves bits [2:0] as is
	i2c.write_byte(GYRO_CONFIG, i2c.read_byte(GYRO_CONFIG) & 0x07);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//set bits [7:3] to 0 and leaves bits [2:0] as is
	i2c.write_byte(ACCEL_CONFIG, i2c.read_byte(ACCEL_CONFIG) & 0x07);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables 92hz digital low pass filter ([2:0] to 010) while keeping bits [7:3] the same 
	i2c.write_byte(ACCEL_CONFIG_2, i2c.read_byte(ACCEL_CONFIG_2) & 0xFA);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables LATCH_INT_EN and INT_ANYRD_2CLEAR while disabling the other options and keeping reserve pin as is.
	i2c.write_byte(INT_PIN_CFG, i2c.read_byte(INT_PIN_CFG) & 0x31);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables Raw Sensor Data Ready interrupt on bit 0
	i2c.write_byte(INT_ENABLE, i2c.read_byte(INT_ENABLE) | 0x01);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables FIFO operation mode and leaves other bits as is.
	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) | 0x40);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//disables all functions in FIFO_EN
	i2c.write_byte(FIFO_EN, 0x00);
}

int16_t MPU9250::readTempurature()
{
	return read_two_bytes_signed(TEMP_OUT_L, TEMP_OUT_H);
}

void MPU9250::enableFIFOOverflowISR()
{
	//enables Raw Sensor Data Ready interrupt on bit 0
	i2c.write_byte(INT_ENABLE, i2c.read_byte(INT_ENABLE) | 0x01);
	i2c.read_byte(INT_STATUS);
}

void MPU9250::disableFIFOOverflowISR()
{
	//Disables Raw Sensor Data Ready interrupt on bit 0
	i2c.write_byte(INT_ENABLE, i2c.read_byte(INT_ENABLE) & 0xFE); //0xFE = ~0x01
}

/* seperate the values of the fifo into bytes for reading as individual values
uint16_t MPU9250::numberOfBytesFIFO??()
{
	return i2c.read_two_bytes_signed(FIFO_COUNTL, FIFO_COUNTH) / 12; //seperating fifo into 6 2 byte ints
*/

//void MPU9250::readFIFO(int bytes)

void MPU9250::calibrateMPU9250(float * return_gyro_bias_list, float * return_accel_bias_list)
{
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint8_t mask_bit[3] = {0, 0, 0};// Define array to hold mask bit for each accelerometer bias axis
	
	uint16_t data_sets, fifo_count;
	
	int16_t accel_tmp[3] = {0, 0, 0}, gyro_tmp[3] = {0, 0, 0}; // temperary variables to hold averaging data

	int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
	int32_t accel_bias_regular[3] = {0, 0, 0};

	uint32_t mask; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers

	//Writes binary 10000000 to register to reset all registers to default values
	i2c.write_byte(PWR_MGMT_1, 0x80);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	i2c.write_byte(PWR_MGMT_1, 0x01); // set clock source to PLL with x-axis gyroscope refernece
	i2c.write_byte(PWR_MGMT_2, i2c.read_byte(PWR_MGMT_2) & 0xC0); // enables accel and gyro on all axises
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	i2c.write_byte(INT_ENABLE, i2c.read_byte(INT_ENABLE) & 0xA6); //writes 10100110 - disables interupts
	i2c.write_byte(FIFO_EN, 0x00); // diable writing to FIFO
	i2c.write_byte(PWR_MGMT_1, 0x00); //sets clock to internal 20Mhz
	i2c.write_byte(I2C_MST_CTRL, 0x00); //disable i2c master
	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) & 0x88); //diable i2c master mode and FIFO over serial
	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) & 0x8C); // reset FIFO module
	std::this_thread::sleep_for(std::chrono::milliseconds(15));

	i2c.write_byte(CONFIG, i2c.read_byte(CONFIG) & 0x81); // sets DLPF_CFG to 1 (bandwith 184hz for gyro)
	//sets sample rate = (gyroscope output rate)/(1 + SMPLRT_DIV)  
	// it would be 1khz/(1 + 0x00) = 1khz sample rate
	i2c.write_byte(SMPLRT_DIV, 0x00); // 1khz sample rate
	i2c.write_byte(GYRO_CONFIG, i2c.read_byte(GYRO_CONFIG) & 0x04); //sets fchoice to 00 and gyro_fs_sel to 00 (250dps sensitivity)
	i2c.write_byte(ACCEL_CONFIG, i2c.read_byte(ACCEL_CONFIG) & 0x07); //sets accel_fs_sel to 00 (+-2g sensitivity)

	uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
	uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) & 0xC8);
	i2c.write_byte(FIFO_EN, 0x78); //enable gyro and accel in FIFO que (max size of que is 512 bytes)
	std::this_thread::sleep_for(std::chrono::milliseconds(40));

	i2c.write_byte(FIFO_EN, 0x00); // diable writing to FIFO

	//i2c.read_bytes(FIFO_COUNTH, 2, &data[0]);
	//fifo_count = ((uint16_t) data[0] << 8 ) | data[1];

	//same as commented out statement above but easier to read and understand
	fifo_count = ((uint16_t) i2c.read_byte(FIFO_COUNTH) << 8 ) | i2c.read_byte(FIFO_COUNTL)); 

	data_sets = fifo_count/12; //sets of data -- fifo_count is divided by 12 becuase there are 6 shorts being read

	for(uint8_t index = 0; index < data_sets; index++)
	{
		i2c.read_bytes(FIFO_R_W, 12, data); //read data

		accel_tmp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
		accel_tmp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
		accel_tmp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
		gyro_tmp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
		gyro_tmp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
		gyro_tmp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

		accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
		accel_bias[1] += (int32_t) accel_temp[1];
		accel_bias[2] += (int32_t) accel_temp[2];
		gyro_bias[0]  += (int32_t) gyro_temp[0];
		gyro_bias[1]  += (int32_t) gyro_temp[1];
		gyro_bias[2]  += (int32_t) gyro_temp[2];
	}

	accel_bias[0] /= (int32_t) data_sets; // the summed values of the accellerometer divided by data_sets to get avarage value
	accel_bias[1] /= (int32_t) data_sets;
	accel_bias[2] /= (int32_t) data_sets;
	gyro_bias[0]  /= (int32_t) data_sets;
	gyro_bias[1]  /= (int32_t) data_sets;
	gyro_bias[2]  /= (int32_t) data_sets;

	if(accel_bias[2] > 0L)   // Remove gravity from the z-axis accelerometer bias calculation
		accel_bias[2] -= (int32_t) accelsensitivity;
	else
		accel_bias[2] += (int32_t) accelsensitivity;

	// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
	data[3] = (-gyro_bias[1]/4)       & 0xFF;
	data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
	data[5] = (-gyro_bias[2]/4)       & 0xFF;

	//push gyro biases to hardware registers
	i2c.write_byte(XG_OFFSET_H, data[0]);
	i2c.write_byte(XG_OFFSET_L, data[1]);
	i2c.write_byte(YG_OFFSET_H, data[2]);
	i2c.write_byte(YG_OFFSET_L, data[3]);
	i2c.write_byte(ZG_OFFSET_H, data[4]);
	i2c.write_byte(ZG_OFFSET_L, data[5]);

	return_gyro_bias_list[0] = (float) gyro_bias[0]/(float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
	return_gyro_bias_list[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
	return_gyro_bias_list[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

	// #  accelerometer  #
	i2c.read_bytes(XA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	i2c.read_bytes(YA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	i2c.read_bytes(ZA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

	mask = 1uL; // Assigns mask for temperature compensation bit 0 of lower byte of accelerometer bias registers

	for(uint8_t index = 0; index < 3; index++) {
    if((accel_bias_reg[index] & mask)) mask_bit[index] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }
}

v2.

#include <syslog.h>
#include <chrono>
#include <thread>

#include "MPU9250.h"

//! Initialization for MPU9250
/*!
 \param bus I2C bus number
 \param address 8 bit address
 */

MPU9250::MPU9250(int bus, int address)
{
	i2c = I2C(bus, address);
}

MPU9250::begin()
{
	//Writes binary 10000000 to register to reset all registers to default values
	i2c.write_byte(PWR_MGMT_1, 0x80);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//writes binary 01000000 | 00000001 to register to enable FIFO, disable FSYNC, and set DLPF_CFG to 1 
	i2c.write_byte(CONFIG, (0x40 | 0x01);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//writes -- sample rate = (gyroscope output rate)/(1 + SMPLRT_DIV)  
	// in this init, it would be 1khz/(1 + 0x04) = 200hz sample rate
	i2c.write_byte(SMPLRT_DIV, 0x04);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//writes binary 00000001 to register to Auto select the best available clock source
	i2c.write_byte(PWR_MGMT_1, 0x01);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
			//uint8_t read_gyro_config = read_byte(GYRO_CONFIG);
			//i2c.write_byte(GYRO_CONFIG, read_gyro_config & ~0xE0);
	
	
	//set bits [7:3] to 0 and leaves bits [2:0] as is
	i2c.write_byte(GYRO_CONFIG, i2c.read_byte(GYRO_CONFIG) & 0x07);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//set bits [7:3] to 0 and leaves bits [2:0] as is
	i2c.write_byte(ACCEL_CONFIG, i2c.read_byte(ACCEL_CONFIG) & 0x07);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables 92hz digital low pass filter ([2:0] to 010) while keeping bits [7:3] the same 
	i2c.write_byte(ACCEL_CONFIG_2, i2c.read_byte(ACCEL_CONFIG_2) & 0xFA);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables LATCH_INT_EN and INT_ANYRD_2CLEAR while disabling the other options and keeping reserve pin as is.
	i2c.write_byte(INT_PIN_CFG, i2c.read_byte(INT_PIN_CFG) & 0x31);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables Raw Sensor Data Ready interrupt on bit 0
	i2c.write_byte(INT_ENABLE, i2c.read_byte(INT_ENABLE) | 0x01);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//enables FIFO operation mode and leaves other bits as is.
	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) | 0x40);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//disables all functions in FIFO_EN
	i2c.write_byte(FIFO_EN, 0x00);
}

void MPU9250::calibrateMPU9250(float * return_gyro_bias_list, float * return_accel_bias_list)
{
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint8_t mask_bit[3] = {0, 0, 0};// Define array to hold mask bit for each accelerometer bias axis
	
	uint16_t data_sets, fifo_count;
	
	int16_t accel_tmp[3] = {0, 0, 0}, gyro_tmp[3] = {0, 0, 0}; // temperary variables to hold averaging data

	int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
	int32_t accel_bias_regular[3] = {0, 0, 0};

	uint32_t mask; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers

	//Writes binary 10000000 to register to reset all registers to default values
	i2c.write_byte(PWR_MGMT_1, 0x80);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	i2c.write_byte(PWR_MGMT_1, 0x01); // set clock source to PLL with x-axis gyroscope refernece
	i2c.write_byte(PWR_MGMT_2, i2c.read_byte(PWR_MGMT_2) & 0xC0); // enables accel and gyro on all axises
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	i2c.write_byte(INT_ENABLE, i2c.read_byte(INT_ENABLE) & 0xA6); //writes 10100110 - disables interupts
	i2c.write_byte(FIFO_EN, 0x00); // diable writing to FIFO
	i2c.write_byte(PWR_MGMT_1, 0x00); //sets clock to internal 20Mhz
	i2c.write_byte(I2C_MST_CTRL, 0x00); //disable i2c master
	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) & 0x88); //diable i2c master mode and FIFO over serial
	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) & 0x8C); // reset FIFO module
	std::this_thread::sleep_for(std::chrono::milliseconds(15));

	i2c.write_byte(CONFIG, i2c.read_byte(CONFIG) & 0x81); // sets DLPF_CFG to 1 (bandwith 184hz for gyro)
	//sets sample rate = (gyroscope output rate)/(1 + SMPLRT_DIV)  
	// it would be 1khz/(1 + 0x00) = 1khz sample rate
	i2c.write_byte(SMPLRT_DIV, 0x00); // 1khz sample rate
	i2c.write_byte(GYRO_CONFIG, i2c.read_byte(GYRO_CONFIG) & 0x04); //sets fchoice to 00 and gyro_fs_sel to 00 (250dps sensitivity)
	i2c.write_byte(ACCEL_CONFIG, i2c.read_byte(ACCEL_CONFIG) & 0x07); //sets accel_fs_sel to 00 (+-2g sensitivity)

	uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
	uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

	i2c.write_byte(USER_CTRL, i2c.read_byte(USER_CTRL) & 0xC8);
	i2c.write_byte(FIFO_EN, 0x78); //enable gyro and accel in FIFO que (max size of que is 512 bytes)
	std::this_thread::sleep_for(std::chrono::milliseconds(40));

	i2c.write_byte(FIFO_EN, 0x00); // diable writing to FIFO

	//read fifo_countH and fifo_countL registers
	i2c.read_bytes(FIFO_COUNTH, 2, &data[0]);
	fifo_count = ((uint16_t) data[0] << 8 ) | data[1];

	//same as statement above but easier to read and understand
	//fifo_count = ((uint16_t) i2c.read_byte(FIFO_COUNTH) << 8 ) | i2c.read_byte(FIFO_COUNTL)); 

	data_sets = fifo_count/12; //sets of data -- fifo_count is divided by 12 becuase there are 6 shorts being read

	for(uint8_t index = 0; index < data_sets; index++)
	{
		i2c.read_bytes(FIFO_R_W, 12, data); //read data

		accel_tmp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
		accel_tmp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
		accel_tmp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
		gyro_tmp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
		gyro_tmp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
		gyro_tmp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

		accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
		accel_bias[1] += (int32_t) accel_temp[1];
		accel_bias[2] += (int32_t) accel_temp[2];
		gyro_bias[0]  += (int32_t) gyro_temp[0];
		gyro_bias[1]  += (int32_t) gyro_temp[1];
		gyro_bias[2]  += (int32_t) gyro_temp[2];
	}

	accel_bias[0] /= (int32_t) data_sets; // the summed values of the accellerometer divided by data_sets to get avarage value
	accel_bias[1] /= (int32_t) data_sets;
	accel_bias[2] /= (int32_t) data_sets;
	gyro_bias[0]  /= (int32_t) data_sets;
	gyro_bias[1]  /= (int32_t) data_sets;
	gyro_bias[2]  /= (int32_t) data_sets;

	if(accel_bias[2] > 0L)   // Remove gravity from the z-axis accelerometer bias calculation
		accel_bias[2] -= (int32_t) accelsensitivity;
	else
		accel_bias[2] += (int32_t) accelsensitivity;

	// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
	data[3] = (-gyro_bias[1]/4)       & 0xFF;
	data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
	data[5] = (-gyro_bias[2]/4)       & 0xFF;

	//push gyro biases to hardware registers
	i2c.write_byte(XG_OFFSET_H, data[0]);
	i2c.write_byte(XG_OFFSET_L, data[1]);
	i2c.write_byte(YG_OFFSET_H, data[2]);
	i2c.write_byte(YG_OFFSET_L, data[3]);
	i2c.write_byte(ZG_OFFSET_H, data[4]);
	i2c.write_byte(ZG_OFFSET_L, data[5]);

	return_gyro_bias_list[0] = (float) gyro_bias[0]/(float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
	return_gyro_bias_list[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
	return_gyro_bias_list[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

	// #  accelerometer  #
	i2c.read_bytes(XA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	i2c.read_bytes(YA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	i2c.read_bytes(ZA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

	mask = 1uL; // Assigns mask for temperature compensation bit 0 of lower byte of accelerometer bias registers

	for(uint8_t index = 0; index < 3; index++)
	{
		if(accel_bias_reg[index] & mask)
			mask_bit[index] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
	}
}