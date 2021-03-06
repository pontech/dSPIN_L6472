////////////////////////////////////////////////////////////
//ORIGINAL CODE 12/12/2011- Mike Hord, SparkFun Electronics
//LIBRARY Created by Adam Meyer of bildr Aug 18th 2012
//Released as MIT license
////////////////////////////////////////////////////////////



#ifndef L6472_h
#define L6472_h

//#include <WProgram.h>//#include <Arduino.h>
#include <SPI.h>


//#define SLAVE_SELECT_PIN 10  // Wire this to the CSN pin
#define MOSI             11  // Wire this to the SDI pin
#define MISO             12  // Wire this to the SDO pin
#define SCK              13  // Wire this to the CK pin
//#define RESET      		6   // Wire this to the STBY line
//#define BUSYN      		4   // Wire this to the BSYN line

#define STAT1            14  // Hooked to an LED on the test jig
#define STAT2            15  // Hooked to an LED on the test jig
#define SWITCH           8   // Hooked to the switch input and a pB on the jig


// Register address redefines.
//  See the Param_Handler() function for more info about these.
#define ABS_POS         0x01        //length: 22      reset value: 0
#define EL_POS          0x02        //length: 9       reset value: 0
#define MARK            0x03        //length: 22      reset value: 0
#define SPEED           0x04        //length: 20      reset value: 0
#define ACC             0x05        //length: 12      reset value: 08A
#define DECEL           0x06        //length: 12      reset value: 08A
#define MAX_SPEED       0x07        //length: 10      reset value: 41
#define MIN_SPEED       0x08        //length: 13      reset value: 0
#define FS_SPD          0x15        //length: 10      reset value: 027
#define TVAL_HOLD       0x09        //length: 8       reset value: 29
#define TVAL_RUN        0x0A        //length: 8       reset value: 29
#define TVAL_ACC        0x0B        //length: 8       reset value: 29
#define TVAL_DEC        0x0C        //length: 8       reset value: 29
#define RESERVED1     	0x0D        //length: 16
#define T_FAST          0x0E        //length: 8       reset value: 19
#define TON_MIN			    0x0F        //length: 7       reset value: 29
#define TOFF_MIN        0x10        //length: 7       reset value: 29
#define RESERVED2       0x11        //length: 8
#define ADC_OUT         0x12        //length: 5       reset value: 
#define OCD_TH          0x13        //length: 4       reset value: 8
#define RESERVED3       0x14        //length: 8
#define STEP_MODE       0x16        //length: 8       reset value: 7
#define ALARM_EN        0x17        //length: 8       reset value: FF
#define CONFIG          0x18        //length: 16      reset value: 2E88
#define STATUS          0x19        //length: 16      reset value:


// constant definitions for overcurrent thresholds. Write these values to 
//  register dSPIN_OCD_TH to set the level at which an overcurrent even occurs.
#define OCD_TH_375mA  0x00
#define OCD_TH_750mA  0x01
#define OCD_TH_1125mA 0x02
#define OCD_TH_1500mA 0x03
#define OCD_TH_1875mA 0x04
#define OCD_TH_2250mA 0x05
#define OCD_TH_2625mA 0x06
#define OCD_TH_3000mA 0x07
#define OCD_TH_3375mA 0x08
#define OCD_TH_3750mA 0x09
#define OCD_TH_4125mA 0x0A
#define OCD_TH_4500mA 0x0B
#define OCD_TH_4875mA 0x0C
#define OCD_TH_5250mA 0x0D
#define OCD_TH_5625mA 0x0E
#define OCD_TH_6000mA 0x0F

// STEP_MODE option values.
// First comes the "microsteps per step" options...
#define STEP_MODE_STEP_SEL 0x07  // Mask for these bits only.
#define STEP_SEL_1     0x00
#define STEP_SEL_1_2   0x01
#define STEP_SEL_1_4   0x02
#define STEP_SEL_1_8   0x03
#define STEP_SEL_1_16  0x04
#define STEP_SEL_1_32  0x05
#define STEP_SEL_1_64  0x06
#define STEP_SEL_1_128 0x07

// ...next, define the SYNC_EN bit. When set, the BUSYN pin will instead
//  output a clock related to the full-step frequency as defined by the
//  SYNC_SEL bits below.
#define STEP_MODE_SYNC_EN	 0x80  // Mask for this bit
#define SYNC_EN 0x80

// ...last, define the SYNC_SEL modes. The clock output is defined by
//  the full-step frequency and the value in these bits- see the datasheet
//  for a matrix describing that relationship (page 46).
#define STEP_MODE_SYNC_SEL 0x70
#define SYNC_SEL_1_2 0x00
#define SYNC_SEL_1   0x10
#define SYNC_SEL_2   0x20
#define SYNC_SEL_4   0x30
#define SYNC_SEL_8   0x40
#define SYNC_SEL_16  0x50
#define SYNC_SEL_32  0x60
#define SYNC_SEL_64  0x70

// Bit names for the ALARM_EN register.
//  Each of these bits defines one potential alarm condition.
//  When one of these conditions occurs and the respective bit in ALARM_EN is set,
//  the FLAG pin will go low. The register must be queried to determine which event
//  caused the alarm.
#define ALARM_EN_OVERCURRENT       0x01
#define ALARM_EN_THERMAL_SHUTDOWN  0x02
#define ALARM_EN_THERMAL_WARNING   0x04
#define ALARM_EN_UNDER_VOLTAGE     0x08
#define ALARM_EN_STALL_DET_A       0x10
#define ALARM_EN_STALL_DET_B       0x20
#define ALARM_EN_SW_TURN_ON        0x40
#define ALARM_EN_WRONG_NPERF_CMD   0x80

// CONFIG register renames.

// Oscillator options.
// The dSPIN needs to know what the clock frequency is because it uses that for some
//  calculations during operation.
#define CONFIG_OSC_SEL                 0x000F // Mask for this bit field.
#define CONFIG_INT_16MHZ               0x0000 // Internal 16MHz, no output
#define CONFIG_INT_16MHZ_OSCOUT_2MHZ   0x0008 // Default; internal 16MHz, 2MHz output
#define CONFIG_INT_16MHZ_OSCOUT_4MHZ   0x0009 // Internal 16MHz, 4MHz output
#define CONFIG_INT_16MHZ_OSCOUT_8MHZ   0x000A // Internal 16MHz, 8MHz output
#define CONFIG_INT_16MHZ_OSCOUT_16MHZ  0x000B // Internal 16MHz, 16MHz output
#define CONFIG_EXT_8MHZ_XTAL_DRIVE     0x0004 // External 8MHz crystal
#define CONFIG_EXT_16MHZ_XTAL_DRIVE    0x0005 // External 16MHz crystal
#define CONFIG_EXT_24MHZ_XTAL_DRIVE    0x0006 // External 24MHz crystal
#define CONFIG_EXT_32MHZ_XTAL_DRIVE    0x0007 // External 32MHz crystal
#define CONFIG_EXT_8MHZ_OSCOUT_INVERT  0x000C // External 8MHz crystal, output inverted
#define CONFIG_EXT_16MHZ_OSCOUT_INVERT 0x000D // External 16MHz crystal, output inverted
#define CONFIG_EXT_24MHZ_OSCOUT_INVERT 0x000E // External 24MHz crystal, output inverted
#define CONFIG_EXT_32MHZ_OSCOUT_INVERT 0x000F // External 32MHz crystal, output inverted

// Configure the functionality of the external switch input
#define CONFIG_SW_MODE                 0x0010 // Mask for this bit.
#define CONFIG_SW_HARD_STOP            0x0000 // Default; hard stop motor on switch.
#define CONFIG_SW_USER                 0x0010 // Tie to the GoUntil and ReleaseSW
                                              //  commands to provide jog function.
                                              //  See page 25 of datasheet.

// Configure the motor voltage compensation mode (see page 34 of datasheet)
#define CONFIG_TQREG               	   0x0020  // Mask for this bit.
#define CONFIG_TQ_INTERNAL             0x0000  // Disable motor voltage compensation.
#define CONFIG_TQ_EXTERNAL             0x0020  // Enable motor voltage compensation.

// Configure overcurrent detection event handling
#define CONFIG_OC_SD                   0x0080  // Mask for this bit.
#define CONFIG_OC_SD_DISABLE           0x0000  // Bridges do NOT shutdown on OC detect
#define CONFIG_OC_SD_ENABLE            0x0080  // Bridges shutdown on OC detect

// Configure the slew rate of the power bridge output
#define CONFIG_POW_SR                  0x0300  // Mask for this bit field.
#define CONFIG_SR_320V_us              0x0000  // 320V/us
#define CONFIG_SR_75V_us               0x0100  // 75V/us
#define CONFIG_SR_110V_us              0x0200  // 110V/us
#define CONFIG_SR_270V_us              0x0300  // 270V/us

// Configure the Predictive current control
#define CONFIG_PRED_EN                 0x8000 

// Configure the switching peroid
#define CONFIG_TSW                     0x7C00      // mask for this bit field
#define CONFIG_TSW_4_us                (0x00)<<10
#define CONFIG_TSW_4B_us               (0x01)<<10
#define CONFIG_TSW_8_us                (0x02)<<10
#define CONFIG_TSW_12_us               (0x03)<<10
#define CONFIG_TSW_16_us               (0x04)<<10
#define CONFIG_TSW_20_us               (0x05)<<10
#define CONFIG_TSW_24_us               (0x06)<<10
#define CONFIG_TSW_28_us               (0x07)<<10
#define CONFIG_TSW_32_us               (0x08)<<10
#define CONFIG_TSW_36_us               (0x09)<<10
#define CONFIG_TSW_40_us               (0x0A)<<10
#define CONFIG_TSW_44_us               (0x0B)<<10
#define CONFIG_TSW_48_us               (0x0C)<<10
#define CONFIG_TSW_52_us               (0x0D)<<10
#define CONFIG_TSW_56_us               (0x0E)<<10
#define CONFIG_TSW_60_us               (0x0F)<<10

#define CONFIG_TSW_64_us               (0x10)<<10
#define CONFIG_TSW_68_us               (0x11)<<10
#define CONFIG_TSW_72_us               (0x12)<<10
#define CONFIG_TSW_76_us               (0x13)<<10
#define CONFIG_TSW_80_us               (0x14)<<10
#define CONFIG_TSW_84_us               (0x15)<<10
#define CONFIG_TSW_88_us               (0x16)<<10
#define CONFIG_TSW_92_us               (0x17)<<10
#define CONFIG_TSW_96_us               (0x18)<<10
#define CONFIG_TSW_100_us              (0x19)<<10
#define CONFIG_TSW_104_us              (0x1A)<<10
#define CONFIG_TSW_108_us              (0x1B)<<10
#define CONFIG_TSW_112_us              (0x1C)<<10
#define CONFIG_TSW_116_us              (0x1D)<<10
#define CONFIG_TSW_120_us              (0x1E)<<10
#define CONFIG_TSW_124_us              (0x1F)<<10


// Status register bit renames- read-only bits conferring information about the
//  device to the user.
#define STATUS_HIZ                     0x0001 // high when bridges are in HiZ mode
#define STATUS_BUSY                    0x0002 // mirrors BUSY pin
#define STATUS_SW_F                    0x0004 // low when switch open, high when closed
#define STATUS_SW_EVN                  0x0008 // active high, set on switch falling edge,
                                                    //  cleared by reading STATUS
#define STATUS_DIR                     0x0010 // Indicates current motor direction.
                                                    //  High is FWD, Low is REV.
#define STATUS_NOTPERF_CMD             0x0080 // Last command not performed.
#define STATUS_WRONG_CMD               0x0100 // Last command not valid.
#define STATUS_UVLO                    0x0200 // Undervoltage lockout is active
#define STATUS_TH_WRN                  0x0400 // Thermal warning
#define STATUS_TH_SD                   0x0800 // Thermal shutdown
#define STATUS_OCD                     0x1000 // Overcurrent detected
#define STATUS_STEP_LOSS_A             0x2000 // Stall detected on A bridge
#define STATUS_STEP_LOSS_B             0x4000 // Stall detected on B bridge
#define STATUS_SCK_MOD                 0x8000 // Step clock mode is active

// Status register motor status field
#define STATUS_MOT_STATUS                0x0060      // field mask
#define STATUS_MOT_STATUS_STOPPED       (0x0000)<<13 // Motor stopped
#define STATUS_MOT_STATUS_ACCELERATION  (0x0001)<<13 // Motor accelerating
#define STATUS_MOT_STATUS_DECELERATION  (0x0002)<<13 // Motor decelerating
#define STATUS_MOT_STATUS_CONST_SPD     (0x0003)<<13 // Motor at constant speed


//dSPIN commands
#define NOP                  0x00
#define SET_PARAM            0x00
#define GET_PARAM            0x20
#define RUN                  0x50
#define STEP_CLOCK           0x58
#define MOVE                 0x40
#define GOTO                 0x60
#define GOTO_DIR             0x68
#define GO_UNTIL             0x82
#define RELEASE_SW           0x92
#define GO_HOME              0x70
#define GO_MARK              0x78
#define RESET_POS            0xD8
#define RESET_DEVICE         0xC0
#define SOFT_STOP            0xB0
#define HARD_STOP            0xB8
#define SOFT_HIZ             0xA0
#define HARD_HIZ             0xA8
#define GET_STATUS           0xD0

// dSPIN direction options
#define FWD  0x01
#define REV  0x00

// dSPIN action options
#define ACTION_RESET  0x00
#define ACTION_COPY   0x01

#define ACT_ACTIVE_LO   0x00
#define ACT_ACTIVE_HI   0x08

typedef struct {
	bool paused;
	bool pauseMotionInterrupted;
} pause_state;

class L6472{

  public:

    L6472(unsigned char BOARD_ID, int MOSIPin, int MISOPin, int SCKPin, int SSPin, int RSTPin, bool (*Safe_Move)(bool) = NULL);

	void setupPort();
	void stp300_MI(int32_t steps);
	void stp300_II(int32_t steps);
	void command(char* input, Stream* IOStream);
	int parseNumber(char* s);
	unsigned char findSpaceOffset(char* s);
	void BoardId(unsigned char newId);	

	//int init(float current, float hold_current);
	int init(float current, float hold_current, bool userawcurrent = false);
	int init2(float current, float hold_current);
	
    void setMicroSteps(int microSteps);
    void setCurrent(float current);
    void setCurrentHold(float current);
    void setMaxSpeed(int speed);
    void setMinSpeed(int speed);
    void setAcc(float acceleration);
    void setDec(float deceleration);
    void setOverCurrent(unsigned int ma_current);
    void setThresholdSpeed(float threshold);
    void setStallCurrent(float ma_current);
    
	unsigned int ParamHandler(byte param, unsigned int value);
	void SetLowSpeedOpt(boolean enable);
	
	void run(byte dir, float spd);
	void Step_Clock(byte dir);
	
	void goHome();
	void setAsHome();
	
	void goMark();
	void move(long n_step);
	void goTo(long pos);
	void goTo_DIR(byte dir, long pos);
	void goUntil(byte act, byte dir, unsigned long spd);
	bool sensorStop(char pin, bool rising, bool positive);
	boolean isBusy();
	
	void releaseSW(byte act, byte dir);
	
	float getSpeed();
	void stp300_HM(long newposition); // setPos
	int32_t stp300_RC();
	int32_t stp300_RD();
	int32_t stp300_RT();
	int32_t stp300_RX();
	void setMark();
	void setMark(int32_t value);
	
	inline void stp300_SP(void);
	inline void stp300_SO(void);
	void stp300_H0(void);
	void stp300_HI(void);
	void resetPos();
	void resetDev();
	void softStop();
	void hardStop();
	void softFree();
	void free();
	int getStatus();
    
	bool stp300_PA(uint8_t new_paused);
	pause_state stp300_RPA(void);

	unsigned int GetParam(byte param);
  	void SetParam(byte param, unsigned int value);
  	
  	bool getHRunning();
  private:
  	int32_t convert(uint32_t val);
  	int32_t convert_alt(uint32_t val);
  	
	void stp300_H(bool positive_direction);
	bool SendPausedStringIfNeeded(char* _str, char* _lineend);
	unsigned long AccCalc(float stepsPerSecPerSec);
	unsigned long DecCalc(float stepsPerSecPerSec);
	unsigned long RevAccDecCalc(long stepsPerTickPerTick);
	unsigned long MaxSpdCalc(float stepsPerSec);
	unsigned long RevMaxSpdCalc(long stepsPerTick);
	unsigned long MinSpdCalc(float stepsPerSec);
	unsigned long RevMinSpdCalc(long stepsPerTick);
	unsigned long FSCalc(float stepsPerSec);
	unsigned long TVALCalc(float amps);   
	unsigned long IntSpdCalc(float stepsPerSec);
	unsigned long SpdCalc(float stepsPerSec);
	unsigned int Param(unsigned int value, byte bit_len);
	byte Xfer(byte data); 

	int _MOSI;
	int _MISO;
	int _SCK;
	int _SS;
	int _RST;
	unsigned char _BOARD_ID;
	Stream* _IOStream;
	int32_t _DestinationPosition;
	int32_t _current;
	int32_t _current_holding;
	bool _HRunning;
	bool _Hmoveingpos;
	bool _paused;
	bool _pauseMotionInterrupted;
	int32_t _pDestinationPosition;
	bool (*_Safe_Move)(bool);
};
#endif
