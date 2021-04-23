#include "MAX30102_main.h"

SAMPLE sampleBuff[BUFF_SIZE];

uint8_t heartRate = 0;
uint8_t spo2 = 0;

uint16_t redAC = 0;
uint32_t redDC = 0;
uint16_t iRedAC = 0;
uint32_t iRedDC = 0;

int16_t eachSampleDiff = 0;

void write_MAX(uint8_t addr, uint8_t data)
{
	I2C_WriteData(MAX30102_ADDRESS, 2, addr, data);
}

uint8_t read_MAX(uint8_t addr)
{
	return I2C_ReadData(MAX30102_ADDRESS, 1, addr);
}

void max30102_init(void)
{
	uint8_t data = 0;
	data = 0x40;
	write_MAX(RES_MODE_CONFIGURATION, data);
	check_comm();
    do
    {
		data = read_MAX(RES_MODE_CONFIGURATION);
    } while (data & 0x40);
	check_comm();
	data = 0x40;
	write_MAX(RES_INTERRUPT_ENABLE_1, data);
	
	data = 0x63;
	write_MAX(RES_SPO2_CONFIGURATION, data);
	
	data = 0x47;
	write_MAX(RES_LED_PLUSE_AMPLITUDE_1, data);
	write_MAX(RES_LED_PLUSE_AMPLITUDE_2, data);
	write_MAX(RES_PROXIMITY_MODE_LED_PLUSE_AMPLITUDE, data);
	check_comm();

	data = 0;
	write_MAX(RES_FIFO_WRITE_POINTER, data);
	write_MAX(RES_OVERFLOW_COUNTER, data);
	write_MAX(RES_FIFO_READ_POINTER, data);
    
	max30102_getStatus();
	
	data = 0x03;
	write_MAX(RES_MODE_CONFIGURATION, data);
}

uint8_t max30102_getUnreadSampleCount(void)
{
    uint8_t wr = 0, rd = 0;
	wr = read_MAX(RES_FIFO_WRITE_POINTER);
	rd = read_MAX(RES_FIFO_READ_POINTER);
    if ((wr - rd) < 0)
        return wr - rd + 32;
    else
        return wr - rd;
}

void max30102_getFIFO(SAMPLE *data, uint8_t sampleCount)
{
    uint8_t dataTemp[5 * 6];
	uint8_t i, j;
    if (sampleCount > 5)
        sampleCount = 5;
	/*
	for(j = 0; j < (6 * sampleCount); j++)
	{
		dataTemp[j] = read_MAX(RES_FIFO_DATA_REGISTER);
		check_comm();
	}
	*/
	I2C_readData_array(MAX30102_ADDRESS, dataTemp, sampleCount, RES_FIFO_DATA_REGISTER);
	//*dataTemp = I2C_ReadData(MAX30102_ADDRESS, 6 * sampleCount, RES_FIFO_DATA_REGISTER);
	
    //HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADDR_READ, RES_FIFO_DATA_REGISTER,
    //                 I2C_MEMADD_SIZE_8BIT, dataTemp,
    //                 6 * sampleCount, 25);
    for (i = 0; i < sampleCount; i++)
    {
        data[i].red = (((uint32_t)dataTemp[i * 6]) << 16 | ((uint32_t)dataTemp[i * 6 + 1]) << 8 | dataTemp[i * 6 + 2]) & 0x3ffff;
        data[i].iRed = (((uint32_t)dataTemp[i * 6 + 3]) << 16 | ((uint32_t)dataTemp[i * 6 + 4]) << 8 | dataTemp[i * 6 + 5]) & 0x3ffff;
    }
}

uint8_t max30102_getStatus(void)
{
    uint8_t data = 0, dataTemp = 0;
	dataTemp = read_MAX(RES_INTERRUPT_STATUS_1);
    
    data = dataTemp;
	dataTemp = read_MAX(RES_INTERRUPT_STATUS_2);
    
    return data | dataTemp;
}

void max30102_OFF(void)
{
    uint8_t data = 0;
	data = read_MAX(RES_MODE_CONFIGURATION);
    //HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data |= 0x80;
	write_MAX(RES_MODE_CONFIGURATION, data);
    //HAL_I2C_Mem_Write(&hi2c2, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void max30102_ON(void)
{
    uint8_t data = 0;
	data = read_MAX(RES_MODE_CONFIGURATION);
    //HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data &= ~(0x80);
	write_MAX(RES_MODE_CONFIGURATION, data);
    //HAL_I2C_Mem_Write(&hi2c2, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void filter(SAMPLE *s)
{
    uint8_t i;
    uint32_t red = 0;
    uint32_t ired = 0;
    for (i = 0; i < FILTER_LEVEL - 1; i++)
    {
        red += sampleBuff[i].red;
        ired += sampleBuff[i].iRed;
    }
    s->red = (red + s->red) / FILTER_LEVEL;
    s->iRed = (ired + s->iRed) / FILTER_LEVEL;
}

void buffInsert(SAMPLE s)
{
    uint8_t i;
    for (i = BUFF_SIZE - 1; i > 0; i--)
    {
        sampleBuff[i].red = sampleBuff[i - 1].red;
        sampleBuff[i].iRed = sampleBuff[i - 1].iRed;
    }
    sampleBuff[0].red = s.red;
    sampleBuff[0].iRed = s.iRed;
}

void calAcDc(uint16_t *rac, uint32_t *rdc, uint16_t *iac, uint32_t *idc)
{
    uint32_t rMax = sampleBuff[0].red;
    uint32_t rMin = sampleBuff[0].red;
    uint32_t iMax = sampleBuff[0].iRed;
    uint32_t iMin = sampleBuff[0].iRed;

    uint8_t i;
    for (i = 0; i < BUFF_SIZE; i++)
    {
        if (sampleBuff[i].red > rMax)
            rMax = sampleBuff[i].red;
        if (sampleBuff[i].red < rMin)
            rMin = sampleBuff[i].red;
        if (sampleBuff[i].iRed > iMax)
            iMax = sampleBuff[i].iRed;
        if (sampleBuff[i].iRed < iMin)
            iMin = sampleBuff[i].iRed;
    }
    *rac = rMax - rMin;
    *rdc = (rMax + rMin) / 2;
    *iac = iMax - iMin;
    *idc = (iMax + iMin) / 2;
}

void max30102_cal(void)
{
	uint8_t i, ii;
	float R;
	uint32_t totalTime;
    static uint8_t eachBeatSampleCount = 0;    
    static uint8_t lastTenBeatSampleCount[10]; 
    static uint32_t last_iRed = 0;      
    uint8_t unreadSampleCount = max30102_getUnreadSampleCount();
    SAMPLE sampleBuffTemp[5];
    max30102_getFIFO(sampleBuffTemp, unreadSampleCount);  
	//check_comm();	
    for (i = 0; i < unreadSampleCount; i++)
    {
        if (sampleBuffTemp[i].iRed < 40000) 
        {
            heartRate = 0;
            spo2 = 0;
            eachSampleDiff = 0;
            continue;
        }
        buffInsert(sampleBuffTemp[i]);
        calAcDc(&redAC, &redDC, &iRedAC, &iRedDC);
        filter(&sampleBuffTemp[i]);
        //spo2
        R = (((float)(redAC)) / ((float)(redDC))) / (((float)(iRedAC)) / ((float)(iRedDC)));
        if (R >= 0.36 && R < 0.66)
            spo2 = (uint8_t)(107 - 20 * R);
        else if (R >= 0.66 && R < 1)
            spo2 = (uint8_t)(129.64 - 54 * R);
        //30-250ppm  count:200-12
        eachSampleDiff = last_iRed - sampleBuffTemp[i].iRed;
        if (eachSampleDiff > 50 && eachBeatSampleCount > 12)
        {
            for (ii = 9; ii > 0; ii--)
                lastTenBeatSampleCount[i] = lastTenBeatSampleCount[i - 1];
            lastTenBeatSampleCount[0] = eachBeatSampleCount;
            totalTime = 0;
            for (ii = 0; ii < 10; ii++)
                totalTime += lastTenBeatSampleCount[i];
            heartRate = (uint8_t)(60.0 * 10 / 0.02 / ((float)totalTime));
            eachBeatSampleCount = 0;
        }
        last_iRed = sampleBuffTemp[i].iRed;
        eachBeatSampleCount++;
    }
}

uint8_t max30102_getHeartRate(void) { return heartRate; }
uint8_t max30102_getSpO2(void) { return spo2; }
int16_t max30102_getDiff(void) { return eachSampleDiff; }
