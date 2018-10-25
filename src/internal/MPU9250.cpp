#include <chrono>
#include <cmath>
#include <syslog.h>
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

// functions public

int8_t MPU9250 : whoAmI() // often 0x71 and sometimes 0x73
{
    return regRead(WHO_AM_I);
}

// register PWR_MGMT_1 (107) commands

int8_t MPU9250::PWR_MGMT_1_ResetToDefault() // bit 7
{
    // Writes binary 10000000 to register to reset all registers to default values
    return regConfig(PWR_MGMT_1, 0x80);
}

int8_t MPU9250::PWR_MGMT_1_CLKSEL(uint8_t val) // bits [2,0]
{
    return regConfig(PWR_MGMT_1, val, 2, 0);
}

int8_t MPU9250::PWR_MGMT_1(uint8_t val) // bits [2,0]
{
    return regConfig(PWR_MGMT_1, val);
}

// register ACCEL_CONFIG_2 (29) commands

int8_t MPU9250::ACCEL_CONFIG_2_accel_fchoice_b(uint8_t val)
{
    return regConfig(ACCEL_CONFIG_2, val, 3);
}

int8_t MPU9250::ACCEL_CONFIG_2_A_DLPFCFG(uint8_t val)
{
    return regConfig(ACCEL_CONFIG_2, val, 2, 0);
}

int8_t MPU9250::ACCEL_CONFIG_2(uint8_t val)
{
    return regConfig(ACCEL_CONFIG_2, val);
}

// register ACCEL_CONFIG (28) commands

int8_t MPU9250::ACCEL_CONFIG_ax_st_en(uint8_t val) // self test ACCEL x
{
    return regConfig(ACCEL_CONFIG, val, 7);
}

int8_t MPU9250::ACCEL_CONFIG_ay_st_en(uint8_t val) // self test ACCEL y
{
    return regConfig(ACCEL_CONFIG, val, 6);
}

int8_t MPU9250::ACCEL_CONFIG_az_st_en(uint8_t val) // self test ACCEL z
{
    return regConfig(ACCEL_CONFIG, val, 5);
}

int8_t MPU9250::ACCEL_CONFIG_ACCEL_FS_SEL(uint8_t val)
{
    return regConfig(ACCEL_CONFIG, val, 4, 3);
}

int8_t MPU9250::ACCEL_CONFIG(uint8_t val)
{
    return regConfig(ACCEL_CONFIG, val);
}

// register GYRO_CONFIG (27) commands

int8_t MPU9250::GYRO_CONFIG_XGYRO_CtenL(uint8_t val) // self test gyro x
    { return regConfig(GYRO_CONFIG, val, 7) }

int8_t MPU9250::GYRO_CONFIG_YGYRO_CtenL(uint8_t val) // self test gyro y
    { return regConfig(GYRO_CONFIG, val, 6) }

int8_t MPU9250::GYRO_CONFIG_ZGYRO_CtenL(uint8_t val) // self test gyro z
    { return regConfig(GYRO_CONFIG, val, 5) }

int8_t MPU9250::GYRO_CONFIG_GYRO_FS_SEL(uint8_t val)
{
    return regConfig(GYRO_CONFIG, val, 4, 3);
}

int8_t MPU9250::GYRO_CONFIG_Fchoice_b(uint8_t val)
{
    return regConfig(GYRO_CONFIG, val, 1, 0);
}

int8_t MPU9250::GYRO_CONFIG(uint8_t val)
{
    return regConfig(GYRO_CONFIG, val);
}

// register CONFIG (26) commands

int8_t MPU9250::CONFIG_FIFO_MODE(uint8_t val)
{
    return regConfig(CONFIG, val, 6);
}

int8_t MPU9250::CONFIG_EXT_SYNC_SET(uint8_t val) // enables fsync on axis of choice
{
    return regConfig(CONFIG, val, 5, 3);
}

int8_t MPU9250::CONFIG_DLPF_CFG(uint8_t val) // configures DLPF_CFG
{
    return regConfig(CONFIG, val, 2, 0);
}

int8_t MPU9250::CONFIG(uint8_t val) // configures DLPF_CFG
{
    return regConfig(CONFIG, val);
}

// register SMPLRT_DIV (25) commands

int8_t MPU9250::SMPLRT_DIV(uint8_t val) // sample rate = (gyroscope output rate (sometimes 1khz))/(1 + SMPLRT_DIV)
{
    return regConfig(SMPLRT_DIV, val);
}

// register INT_PIN_CFG (55) commands

int8_t MPU9250::INT_PIN_CFG_ACTL(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 7);
}

int8_t MPU9250::INT_PIN_CFG_OPEN(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 6);
}

int8_t MPU9250::INT_PIN_CFG_LATCH_INT_EN(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 5);
}

int8_t MPU9250::INT_PIN_CFG_INT_ANYRD_2CLEAR(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 4);
}

int8_t MPU9250::INT_PIN_CFG_ACTL_FSYNC(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 3);
}

int8_t MPU9250::INT_PIN_CFG_FSYNC_INT_MODE_EN(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 2);
}

int8_t MPU9250::INT_PIN_CFG_BYPASS_EN(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val, 1);
}

int8_t MPU9250::INT_PIN_CFG(uint8_t val)
{
    return regConfig(INT_PIN_CFG, val);
}

// register INT_ENABLE (56) commands

int8_t MPU9250::INT_ENABLE_WOM_EN(uint8_t val) // wake on motion toggle
{
    return regConfig(INT_ENABLE, val, 6);
}

int8_t MPU9250::INT_ENABLE_FIFO_OVERFLOW_EN(uint8_t val)
{
    return regConfig(INT_ENABLE, val, 4);
}

int8_t MPU9250::INT_ENABLE_FSYNC_INT_EN(uint8_t val)
{
    return regConfig(INT_ENABLE, val, 3);
}

int8_t MPU9250::INT_ENABLE_RAW_RDY_EN(uint8_t val)
{
    return regConfig(INT_ENABLE, val, 0);
}

int8_t MPU9250::INT_ENABLE(uint8_t val)
{
    return regConfig(INT_ENABLE, val);
}

// register USER_CTRL (106) commands

int8_t MPU9250::USER_CTRL_FIFO_EN(uint8_t val)
{
    return regConfig(USER_CTRL, val, 6);
}

int8_t MPU9250::USER_CTRL_I2C_MST_EN(uint8_t val)
{
    return regConfig(USER_CTRL, val, 5);
}

int8_t MPU9250::USER_CTRL_I2C_IF_DIS()
{
    return regConfig(USER_CTRL, 1, 4);
}

int8_t MPU9250::USER_CTRL_FIFO_RST()
{
    return regConfig(USER_CTRL, 1, 2);
}

int8_t MPU9250::USER_CTRL_MST_RST()
{
    return regConfig(USER_CTRL, 1, 1);
}

int8_t MPU9250::USER_CTRL_SIG_COND_RST()
{
    return regConfig(USER_CTRL, 1, 0);
}

int8_t MPU9250::USER_CTRL(uint8_t val)
{
    return regConfig(USER_CTRL, val);
}

//register FIFO_EN (35) commands

int8_t MPU9250::FIFO_EN_TEMP_OUT(uint8_t val)
{
    return regConfig(FIFO_EN, val, 7);
}

int8_t MPU9250::FIFO_EN_GYRO_XOUT(uint8_t val)
{
    return regConfig(FIFO_EN, val, 6);
}

int8_t MPU9250::FIFO_EN_GYRO_YOUT(uint8_t val)
{
    return regConfig(FIFO_EN, val, 5);
}

int8_t MPU9250::FIFO_EN_GYRO_ZOUT(uint8_t val)
{
    return regConfig(FIFO_EN, val, 4);
}

int8_t MPU9250::FIFO_EN_ACCEL(uint8_t val)
{
    return regConfig(FIFO_EN, val, 3);
}

int8_t MPU9250::FIFO_EN_SLV_2(uint8_t val)
{
    return regConfig(FIFO_EN, val, 2);
}

int8_t MPU9250::FIFO_EN_SLV_1(uint8_t val)
{
    return regConfig(FIFO_EN, val, 1);
}

int8_t MPU9250::FIFO_EN_SLV_0(uint8_t val)
{
    return regConfig(FIFO_EN, val, 0);
}

int8_t MPU9250::FIFO_EN(uint8_t val)
{
    return regConfig(FIFO_EN, val);
}























void MPU9250::calibrate(float * return_gyro_bias_list, float * return_accel_bias_list)
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


// functions private

int8_t MPU9250::regRead(uint8_t reg)
{
    return i2c.read_byte(reg);
}

int8_t MPU9250::regConfig(uint8_t reg, uint8_t value)
{
    return i2c.write_byte(reg, value);
}

int8_t MPU9250::regConfig(uint8_t reg, uint8_t value, uint8_t bit)
{
    switch(bit) {
    case 0:
	return i2c.write_byte(reg, i2c.read_byte(reg) & !!value);
    case 1:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 1) | 0b11111101));
    case 2:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 2) | 0b11111011));
    case 3:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 3) | 0b11110111));
    case 4:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 4) | 0b11101111));
    case 5:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 5) | 0b11011111));
    case 6:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 6) | 0b10111111));
    case 7:
	return i2c.write_byte(reg, i2c.read_byte(reg) & ((!!value << 7) | 0b01111111));
    default:
	return -2;
    }
    return 0; // should never be reached
}

int8_t regConfig(uint8_t reg, uint8_t value, uint8_t bitH, uint8_t bitL)
{
    if(value <= 0x7F) // checks if value is not 8 bit
    {
	uint8_t desiredBit;
	uint8_t registerData = i2c.read_byte(reg);

	value <<= bitL; // shifts value over for correct 8 bit value

	for(int8_t index = bitH; index >= bitL; --index) {
	    desiredBit = !!(value &
	        (((uint8_t)1) << index)); // assigns the bit to be assigned from variable value at position index
	    if((!!(registerData & (((uint8_t)1) << index))) ^ (desiredBit)) // checks if bit needs to be changed
		registerData = registerData ^
		    (-(uint8_t)desiredBit ^ registerData) &
		        (((uint8_t)1) << index); // assigns the bit to registerData at index
	}

	return i2c.write_byte(reg, registerData);
    }

    return -2;
}