ARM GAS  /tmp/ccPZc5IS.s 			page 1


   1              		.syntax unified
   2              		.arch armv7-a
   3              		.eabi_attribute 27, 3
   4              		.eabi_attribute 28, 1
   5              		.fpu vfpv3-d16
   6              		.eabi_attribute 20, 1
   7              		.eabi_attribute 21, 1
   8              		.eabi_attribute 23, 3
   9              		.eabi_attribute 24, 1
  10              		.eabi_attribute 25, 1
  11              		.eabi_attribute 26, 2
  12              		.eabi_attribute 30, 2
  13              		.eabi_attribute 34, 1
  14              		.eabi_attribute 18, 4
  15              		.thumb
  16              		.file	"stage1_test.c"
  17              		.text
  18              	.Ltext0:
  19              		.cfi_sections	.debug_frame
  20              		.align	2
  21              		.thumb
  22              		.thumb_func
  23              		.type	componentID.part.0, %function
  24              	componentID.part.0:
  25              	.LFB32:
  26              		.file 1 "stage1_test.c"
   1:stage1_test.c **** #include <stdio.h>
   2:stage1_test.c **** #include <string.h>
   3:stage1_test.c **** #include <time.h>
   4:stage1_test.c **** 
   5:stage1_test.c **** #define TEST1 15
   6:stage1_test.c **** #define TEST2 115
   7:stage1_test.c **** #define TEST3 2
   8:stage1_test.c **** #define TEST4 49
   9:stage1_test.c **** #define TEST5 3
  10:stage1_test.c **** #define TEST6 14
  11:stage1_test.c **** #define BUTTON 112
  12:stage1_test.c **** 
  13:stage1_test.c **** int testComponent(int,struct timespec*); 
  14:stage1_test.c **** int componentID(int);
  15:stage1_test.c **** int detectProblems(int*);
  16:stage1_test.c **** int detectShort(int,int);
  17:stage1_test.c **** int detectShortsAll();
  18:stage1_test.c **** int readADC(int);
  19:stage1_test.c **** void writeGPIO(int,int);
  20:stage1_test.c **** int readGPIO(int);
  21:stage1_test.c **** void setDirGPIO(int,int);
  22:stage1_test.c **** 
  23:stage1_test.c **** int main (void) {
  24:stage1_test.c **** 	//setDirGPIO(7,0);
  25:stage1_test.c **** 	//writeGPIO(7,1);
  26:stage1_test.c **** 	int adc_value, i, counter;
  27:stage1_test.c **** 	int test_pins[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
  28:stage1_test.c **** 	printf("pins: %d, %d, %d, %d, %d\n",test_pins[0],test_pins[1],
  29:stage1_test.c **** 		test_pins[2],test_pins[3],test_pins[4]);
  30:stage1_test.c **** 	counter = 0;
  31:stage1_test.c **** 	struct timespec time_des;
ARM GAS  /tmp/ccPZc5IS.s 			page 2


  32:stage1_test.c **** 	struct timespec time_des_test;
  33:stage1_test.c **** 	time_des.tv_sec = 0;
  34:stage1_test.c **** 	time_des.tv_nsec = 1000000; //10ms
  35:stage1_test.c **** 	time_des_test.tv_sec = 0;
  36:stage1_test.c **** 	time_des_test.tv_nsec = 10000000;
  37:stage1_test.c **** 	while (1) {
  38:stage1_test.c **** 		if (readGPIO(BUTTON)) {
  39:stage1_test.c **** 			/*setDirGPIO(2,0);
  40:stage1_test.c **** 			setDirGPIO(15,1);
  41:stage1_test.c **** 			writeGPIO(2,1);
  42:stage1_test.c **** 			nanosleep(&time_des,NULL);
  43:stage1_test.c **** 			int component = componentID(readADC(0));
  44:stage1_test.c **** 			if (component == 0) {
  45:stage1_test.c **** 				setDirGPIO(2,1);
  46:stage1_test.c **** 				setDirGPIO(15,0);
  47:stage1_test.c **** 				writeGPIO(15,1);
  48:stage1_test.c **** 				nanosleep(&time_des_test,NULL);
  49:stage1_test.c **** 				if (readGPIO(2)) {component = 5;}
  50:stage1_test.c **** 				else {component = 3;}
  51:stage1_test.c **** 			}*/
  52:stage1_test.c **** 			int component[5];
  53:stage1_test.c **** 			component[0] = testComponent(TEST1,&time_des); 
  54:stage1_test.c **** 			component[1] = testComponent(TEST2,&time_des); 
  55:stage1_test.c **** 			component[2] = testComponent(TEST3,&time_des); 
  56:stage1_test.c **** 			component[3] = testComponent(TEST4,&time_des); 
  57:stage1_test.c **** 			component[4] = testComponent(TEST5,&time_des); 
  58:stage1_test.c **** 			int problem = detectProblems(component);
  59:stage1_test.c **** 			if (problem == 3)
  60:stage1_test.c **** 				printf("Probe is not fully engaged and "
  61:stage1_test.c **** 					"some test leads are shorted.\n");
  62:stage1_test.c **** 			else if (problem == 2)
  63:stage1_test.c **** 				printf("Probe is not fully engaged.\n");
  64:stage1_test.c **** 			else if (problem == 1)
  65:stage1_test.c **** 				printf("Some test leads are shorted.\n");
  66:stage1_test.c **** 			else if (problem == 0) {
  67:stage1_test.c **** 				printf("Test is acceptable.\n");
  68:stage1_test.c **** 				int j;
  69:stage1_test.c **** 				for (j=0; j<5; j++) {
  70:stage1_test.c **** 					printf("%d: ",j+1);
  71:stage1_test.c **** 					switch (component[j]) {
  72:stage1_test.c **** 						case 0:
  73:stage1_test.c **** 							break;
  74:stage1_test.c **** 						case 1:
  75:stage1_test.c **** 							printf("Wire\n");
  76:stage1_test.c **** 							break;
  77:stage1_test.c **** 						case 2:
  78:stage1_test.c **** 							printf("Resistor\n");
  79:stage1_test.c **** 							break;
  80:stage1_test.c **** 						case 3:
  81:stage1_test.c **** 							printf("Capacitor\n");
  82:stage1_test.c **** 							break;
  83:stage1_test.c **** 						case 4:
  84:stage1_test.c **** 							printf("Inductor\n");
  85:stage1_test.c **** 							break;
  86:stage1_test.c **** 						case 5:
  87:stage1_test.c **** 							printf("Diode\n");
  88:stage1_test.c **** 							break;
ARM GAS  /tmp/ccPZc5IS.s 			page 3


  89:stage1_test.c **** 					}	
  90:stage1_test.c **** 				}
  91:stage1_test.c **** 				printf("Code: %d %d %d %d %d\n",component[0], 
  92:stage1_test.c **** 					component[1],component[2],
  93:stage1_test.c **** 					component[3],component[4]);
  94:stage1_test.c **** 			}
  95:stage1_test.c **** 			//writeGPIO(2,0);
  96:stage1_test.c **** 		}
  97:stage1_test.c **** 		//adc_value = readADC(0);
  98:stage1_test.c **** 		//printf("\rAIN0: %d mV      ",adc_value);
  99:stage1_test.c **** 		nanosleep(&time_des,NULL);
 100:stage1_test.c **** 		//writeGPIO(7,readGPIO(14));
 101:stage1_test.c **** 		//if (readGPIO(20)) break;
 102:stage1_test.c **** 	}
 103:stage1_test.c **** 	printf("\n");
 104:stage1_test.c **** 	//writeGPIO(7,0);
 105:stage1_test.c **** 	return 0;
 106:stage1_test.c **** }
 107:stage1_test.c **** 
 108:stage1_test.c **** int testComponent(int gpio, struct timespec *time_des) {
 109:stage1_test.c **** 	setDirGPIO(TEST1,1);
 110:stage1_test.c **** 	setDirGPIO(TEST2,1);
 111:stage1_test.c **** 	setDirGPIO(TEST3,1);
 112:stage1_test.c **** 	setDirGPIO(TEST4,1);
 113:stage1_test.c **** 	setDirGPIO(TEST5,1);
 114:stage1_test.c **** 	setDirGPIO(TEST6,1);
 115:stage1_test.c **** 
 116:stage1_test.c **** 	setDirGPIO(gpio,0);
 117:stage1_test.c **** 	writeGPIO(gpio,1);
 118:stage1_test.c **** 
 119:stage1_test.c **** 	nanosleep(time_des,NULL);
 120:stage1_test.c **** 	int component = componentID(readADC(0));
 121:stage1_test.c **** 	writeGPIO(gpio,0);
 122:stage1_test.c **** 	if (component == 0) {
 123:stage1_test.c **** 		setDirGPIO(gpio,1);
 124:stage1_test.c **** 		setDirGPIO(TEST6,0);
 125:stage1_test.c **** 		writeGPIO(TEST6,1);
 126:stage1_test.c **** 		nanosleep(time_des,NULL);
 127:stage1_test.c **** 		if (readGPIO(gpio)) {component = 5;}
 128:stage1_test.c **** 		else {component = 3;}
 129:stage1_test.c **** 		writeGPIO(TEST6,0);
 130:stage1_test.c **** 	}
 131:stage1_test.c **** 	setDirGPIO(TEST6,1);
 132:stage1_test.c **** 	return component;
 133:stage1_test.c **** }
 134:stage1_test.c **** 
 135:stage1_test.c **** int componentID(int mV) {
  27              		.loc 1 135 0
  28              		.cfi_startproc
  29              		@ args = 0, pretend = 0, frame = 0
  30              		@ frame_needed = 0, uses_anonymous_args = 0
  31              		@ link register save eliminated.
  32              	.LVL0:
 136:stage1_test.c **** 	if (mV < 50) return 0;
 137:stage1_test.c **** 	else if ( (mV > 120) && (mV < 275) ) return 2;
  33              		.loc 1 137 0
  34 0000 A0F17903 		sub	r3, r0, #121
ARM GAS  /tmp/ccPZc5IS.s 			page 4


  35 0004 992B     		cmp	r3, #153
  36 0006 01D8     		bhi	.L7
  37 0008 0220     		movs	r0, #2
  38              	.LVL1:
  39 000a 7047     		bx	lr
  40              	.LVL2:
  41              	.L7:
 138:stage1_test.c **** 	else if ( (mV > 900) && (mV < 1100) ) return 4;
  42              		.loc 1 138 0
  43 000c 6FF46173 		mvn	r3, #900
  44 0010 C318     		adds	r3, r0, r3
  45 0012 C62B     		cmp	r3, #198
  46 0014 01D8     		bhi	.L8
  47 0016 0420     		movs	r0, #4
  48              	.LVL3:
  49 0018 7047     		bx	lr
  50              	.LVL4:
  51              	.L8:
 139:stage1_test.c **** 	else if ( (mV > 1350) && (mV < 1450) ) return 5;
  52              		.loc 1 139 0
  53 001a A0F24753 		subw	r3, r0, #1351
  54 001e 622B     		cmp	r3, #98
  55 0020 01D8     		bhi	.L9
  56 0022 0520     		movs	r0, #5
  57              	.LVL5:
 140:stage1_test.c **** 	else if (mV > 1700) return 1;
 141:stage1_test.c **** 	else return 0;
 142:stage1_test.c **** }
  58              		.loc 1 142 0
  59 0024 7047     		bx	lr
  60              	.LVL6:
  61              	.L9:
 140:stage1_test.c **** 	else if (mV > 1700) return 1;
  62              		.loc 1 140 0
  63 0026 40F2A463 		movw	r3, #1700
  64 002a 9842     		cmp	r0, r3
  65 002c D4BF     		ite	le
  66 002e 0020     		movle	r0, #0
  67              	.LVL7:
  68 0030 0120     		movgt	r0, #1
  69 0032 7047     		bx	lr
  70              		.cfi_endproc
  71              	.LFE32:
  72              		.size	componentID.part.0, .-componentID.part.0
  73              		.align	2
  74              		.global	componentID
  75              		.thumb
  76              		.thumb_func
  77              		.type	componentID, %function
  78              	componentID:
  79              	.LFB25:
 135:stage1_test.c **** 	if (mV < 50) return 0;
  80              		.loc 1 135 0
  81              		.cfi_startproc
  82              		@ args = 0, pretend = 0, frame = 0
  83              		@ frame_needed = 0, uses_anonymous_args = 0
  84              		@ link register save eliminated.
ARM GAS  /tmp/ccPZc5IS.s 			page 5


  85              	.LVL8:
 136:stage1_test.c **** 	else if ( (mV > 120) && (mV < 275) ) return 2;
  86              		.loc 1 136 0
  87 0034 3128     		cmp	r0, #49
  88 0036 00DD     		ble	.L11
  89 0038 E2E7     		b	componentID.part.0
  90              	.LVL9:
  91              	.L11:
  92              		.loc 1 142 0
  93 003a 0020     		movs	r0, #0
  94              	.LVL10:
  95 003c 7047     		bx	lr
  96              		.cfi_endproc
  97              	.LFE25:
  98              		.size	componentID, .-componentID
  99 003e 00BF     		.align	2
 100              		.global	readADC
 101              		.thumb
 102              		.thumb_func
 103              		.type	readADC, %function
 104              	readADC:
 105              	.LFB28:
 143:stage1_test.c **** 
 144:stage1_test.c **** int detectProblems(int *components) {
 145:stage1_test.c **** 	int i,j;
 146:stage1_test.c **** 	int opens = 0;
 147:stage1_test.c **** 	int shorts = 0;
 148:stage1_test.c **** 	int test_gpios[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
 149:stage1_test.c **** 	int pins[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
 150:stage1_test.c **** 	for (i=0; i<5; i++) {
 151:stage1_test.c **** 		int x = pins[i];
 152:stage1_test.c **** 		int y = *(pins+i);
 153:stage1_test.c **** 		for (j=0; j<5; j++) {
 154:stage1_test.c **** 			if ( (i != j) && (components[i] == components[j]) ) {
 155:stage1_test.c **** 				if (components[i] == 3) {
 156:stage1_test.c **** 					if (detectShort(test_gpios[i],
 157:stage1_test.c **** 						test_gpios[j]))
 158:stage1_test.c **** 						 shorts++;
 159:stage1_test.c **** 					else 
 160:stage1_test.c **** 						opens++;
 161:stage1_test.c **** 				}
 162:stage1_test.c **** 				else
 163:stage1_test.c **** 					shorts++;
 164:stage1_test.c **** 			}
 165:stage1_test.c **** 		}
 166:stage1_test.c **** 	}
 167:stage1_test.c **** 	if (shorts && opens) return 3;
 168:stage1_test.c **** 	else if (opens && !shorts) return 2;
 169:stage1_test.c **** 	else if (shorts && !opens) return 1;
 170:stage1_test.c **** 	else return 0;
 171:stage1_test.c **** }
 172:stage1_test.c **** 
 173:stage1_test.c **** int detectShort(int gpio1, int gpio2) {
 174:stage1_test.c **** 	int shorted = 0;
 175:stage1_test.c **** 	setDirGPIO(gpio1,0);
 176:stage1_test.c **** 	writeGPIO(gpio1,1);
 177:stage1_test.c **** 	if (readGPIO(gpio2)) {
ARM GAS  /tmp/ccPZc5IS.s 			page 6


 178:stage1_test.c **** 		shorted = 1;
 179:stage1_test.c **** 		//printf("GPIOs %d and %d are shorted together!\n",gpio1,gpio2);
 180:stage1_test.c **** 	}
 181:stage1_test.c **** 	writeGPIO(gpio1,0);
 182:stage1_test.c **** 	setDirGPIO(gpio1,1);
 183:stage1_test.c **** 	return shorted;
 184:stage1_test.c **** }
 185:stage1_test.c **** 
 186:stage1_test.c **** int readADC(int channel) {
 106              		.loc 1 186 0
 107              		.cfi_startproc
 108              		@ args = 0, pretend = 0, frame = 120
 109              		@ frame_needed = 0, uses_anonymous_args = 0
 110              	.LVL11:
 111 0040 10B5     		push	{r4, lr}
 112              		.cfi_def_cfa_offset 8
 113              		.cfi_offset 4, -8
 114              		.cfi_offset 14, -4
 187:stage1_test.c **** 	FILE *an_file;
 188:stage1_test.c **** 	char file_path[100] = "/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0/in_volt
 115              		.loc 1 188 0
 116 0042 40F20001 		movw	r1, #:lower16:.LANCHOR0
 186:stage1_test.c **** 	FILE *an_file;
 117              		.loc 1 186 0
 118 0046 9EB0     		sub	sp, sp, #120
 119              		.cfi_def_cfa_offset 128
 186:stage1_test.c **** 	FILE *an_file;
 120              		.loc 1 186 0
 121 0048 0446     		mov	r4, r0
 122              		.loc 1 188 0
 123 004a C0F20001 		movt	r1, #:upper16:.LANCHOR0
 124 004e 05A8     		add	r0, sp, #20
 125              	.LVL12:
 126 0050 4F22     		movs	r2, #79
 127 0052 FFF7FEFF 		bl	memcpy
 128              	.LVL13:
 129 0056 0DF16300 		add	r0, sp, #99
 130 005a 0021     		movs	r1, #0
 131 005c 1522     		movs	r2, #21
 132 005e FFF7FEFF 		bl	memset
 133              	.LVL14:
 189:stage1_test.c **** 	char channel_str[10];
 190:stage1_test.c **** 	sprintf(channel_str,"%d_raw",channel);
 134              		.loc 1 190 0
 135 0062 40F20001 		movw	r1, #:lower16:.LC2
 136 0066 2246     		mov	r2, r4
 137 0068 02A8     		add	r0, sp, #8
 138 006a C0F20001 		movt	r1, #:upper16:.LC2
 139 006e FFF7FEFF 		bl	sprintf
 140              	.LVL15:
 191:stage1_test.c **** 	strcat(file_path,channel_str);
 141              		.loc 1 191 0
 142 0072 02A9     		add	r1, sp, #8
 143 0074 05A8     		add	r0, sp, #20
 144 0076 FFF7FEFF 		bl	strcat
 145              	.LVL16:
 192:stage1_test.c **** 
ARM GAS  /tmp/ccPZc5IS.s 			page 7


 193:stage1_test.c **** 	int adc_value;
 194:stage1_test.c **** 	an_file = fopen(file_path, "r");
 146              		.loc 1 194 0
 147 007a 40F20001 		movw	r1, #:lower16:.LC3
 148 007e 05A8     		add	r0, sp, #20
 149 0080 C0F20001 		movt	r1, #:upper16:.LC3
 150 0084 FFF7FEFF 		bl	fopen
 151              	.LVL17:
 195:stage1_test.c **** 	//fseek(an_value,0,SEEK_SET);
 196:stage1_test.c **** 	fscanf(an_file,"%d",&adc_value);
 152              		.loc 1 196 0
 153 0088 40F20001 		movw	r1, #:lower16:.LC4
 154 008c 01AA     		add	r2, sp, #4
 155 008e C0F20001 		movt	r1, #:upper16:.LC4
 194:stage1_test.c **** 	//fseek(an_value,0,SEEK_SET);
 156              		.loc 1 194 0
 157 0092 0446     		mov	r4, r0
 158              	.LVL18:
 159              		.loc 1 196 0
 160 0094 FFF7FEFF 		bl	__isoc99_fscanf
 161              	.LVL19:
 197:stage1_test.c **** 	fclose(an_file);
 162              		.loc 1 197 0
 163 0098 2046     		mov	r0, r4
 164 009a FFF7FEFF 		bl	fclose
 165              	.LVL20:
 198:stage1_test.c **** 	
 199:stage1_test.c **** 	//printf("value: %d,",adc_value);
 200:stage1_test.c **** 	adc_value = (int) (((float)adc_value / 4095.0) * 1800);
 166              		.loc 1 200 0
 167 009e DDED016A 		flds	s13, [sp, #4]	@ int
 168 00a2 9FED097B 		fldd	d7, .L14
 169 00a6 B8EEE66A 		fsitos	s12, s13
 170 00aa 9FED095B 		fldd	d5, .L14+8
 171 00ae B7EEC66A 		fcvtds	d6, s12
 172 00b2 86EE077B 		fdivd	d7, d6, d7
 173 00b6 27EE057B 		fmuld	d7, d7, d5
 174              	.LVL21:
 201:stage1_test.c **** 	//returns actual voltage in mV (lose resolution, change to out of 4096)
 202:stage1_test.c **** 	//printf("%d\n",adc_value);
 203:stage1_test.c **** 	return adc_value;
 204:stage1_test.c **** }
 175              		.loc 1 204 0
 176 00ba FDEEC77B 		ftosizd	s15, d7
 177              	.LVL22:
 178 00be 17EE900A 		fmrs	r0, s15	@ int
 179 00c2 1EB0     		add	sp, sp, #120
 180              		.cfi_def_cfa_offset 8
 181              		@ sp needed
 182 00c4 10BD     		pop	{r4, pc}
 183              	.LVL23:
 184              	.L15:
 185 00c6 00BF     		.align	3
 186              	.L14:
 187 00c8 00000000 		.word	0
 188 00cc 00FEAF40 		.word	1085275648
 189 00d0 00000000 		.word	0
ARM GAS  /tmp/ccPZc5IS.s 			page 8


 190 00d4 00209C40 		.word	1083973632
 191              		.cfi_endproc
 192              	.LFE28:
 193              		.size	readADC, .-readADC
 194              		.align	2
 195              		.global	writeGPIO
 196              		.thumb
 197              		.thumb_func
 198              		.type	writeGPIO, %function
 199              	writeGPIO:
 200              	.LFB29:
 205:stage1_test.c **** 
 206:stage1_test.c **** void writeGPIO(int pin_number, int value) {
 201              		.loc 1 206 0
 202              		.cfi_startproc
 203              		@ args = 0, pretend = 0, frame = 64
 204              		@ frame_needed = 0, uses_anonymous_args = 0
 205              	.LVL24:
 206 00d8 F0B5     		push	{r4, r5, r6, r7, lr}
 207              		.cfi_def_cfa_offset 20
 208              		.cfi_offset 4, -20
 209              		.cfi_offset 5, -16
 210              		.cfi_offset 6, -12
 211              		.cfi_offset 7, -8
 212              		.cfi_offset 14, -4
 213 00da 0746     		mov	r7, r0
 207:stage1_test.c **** 	FILE *f_gpio;
 208:stage1_test.c **** 	char file_path[50] = "/sys/class/gpio/gpio";
 214              		.loc 1 208 0
 215 00dc 184D     		ldr	r5, .L18
 206:stage1_test.c **** 	FILE *f_gpio;
 216              		.loc 1 206 0
 217 00de 0E46     		mov	r6, r1
 218 00e0 91B0     		sub	sp, sp, #68
 219              		.cfi_def_cfa_offset 88
 220              		.loc 1 208 0
 221 00e2 03AC     		add	r4, sp, #12
 222 00e4 0FCD     		ldmia	r5!, {r0, r1, r2, r3}
 223              	.LVL25:
 224 00e6 0FC4     		stmia	r4!, {r0, r1, r2, r3}
 225 00e8 1D22     		movs	r2, #29
 226 00ea 95E80300 		ldmia	r5, {r0, r1}
 227 00ee 44F8040B 		str	r0, [r4], #4
 228 00f2 0DF12100 		add	r0, sp, #33
 229 00f6 2170     		strb	r1, [r4]
 230 00f8 0021     		movs	r1, #0
 231 00fa FFF7FEFF 		bl	memset
 232              	.LVL26:
 209:stage1_test.c **** 	char file_end[9];
 210:stage1_test.c **** 	sprintf(file_end,"%d/value",pin_number);
 233              		.loc 1 210 0
 234 00fe 40F20001 		movw	r1, #:lower16:.LC6
 235 0102 3A46     		mov	r2, r7
 236 0104 6846     		mov	r0, sp
 237 0106 C0F20001 		movt	r1, #:upper16:.LC6
 238 010a FFF7FEFF 		bl	sprintf
 239              	.LVL27:
ARM GAS  /tmp/ccPZc5IS.s 			page 9


 211:stage1_test.c **** 	strcat(file_path,file_end);
 240              		.loc 1 211 0
 241 010e 6946     		mov	r1, sp
 242 0110 03A8     		add	r0, sp, #12
 243 0112 FFF7FEFF 		bl	strcat
 244              	.LVL28:
 212:stage1_test.c **** 	f_gpio = fopen(file_path,"w");
 245              		.loc 1 212 0
 246 0116 40F20001 		movw	r1, #:lower16:.LC7
 247 011a 03A8     		add	r0, sp, #12
 248 011c C0F20001 		movt	r1, #:upper16:.LC7
 249 0120 FFF7FEFF 		bl	fopen
 250              	.LVL29:
 213:stage1_test.c **** 	fprintf(f_gpio,"%d",value);
 251              		.loc 1 213 0
 252 0124 40F20001 		movw	r1, #:lower16:.LC4
 253 0128 3246     		mov	r2, r6
 254 012a C0F20001 		movt	r1, #:upper16:.LC4
 212:stage1_test.c **** 	f_gpio = fopen(file_path,"w");
 255              		.loc 1 212 0
 256 012e 0446     		mov	r4, r0
 257              	.LVL30:
 258              		.loc 1 213 0
 259 0130 FFF7FEFF 		bl	fprintf
 260              	.LVL31:
 214:stage1_test.c **** 	fclose(f_gpio);
 261              		.loc 1 214 0
 262 0134 2046     		mov	r0, r4
 263 0136 FFF7FEFF 		bl	fclose
 264              	.LVL32:
 215:stage1_test.c **** }
 265              		.loc 1 215 0
 266 013a 11B0     		add	sp, sp, #68
 267              		.cfi_def_cfa_offset 20
 268              		@ sp needed
 269 013c F0BD     		pop	{r4, r5, r6, r7, pc}
 270              	.LVL33:
 271              	.L19:
 272 013e 00BF     		.align	2
 273              	.L18:
 274 0140 64000000 		.word	.LANCHOR0+100
 275              		.cfi_endproc
 276              	.LFE29:
 277              		.size	writeGPIO, .-writeGPIO
 278              		.align	2
 279              		.global	readGPIO
 280              		.thumb
 281              		.thumb_func
 282              		.type	readGPIO, %function
 283              	readGPIO:
 284              	.LFB30:
 216:stage1_test.c **** 
 217:stage1_test.c **** int readGPIO(int pin_number) {
 285              		.loc 1 217 0
 286              		.cfi_startproc
 287              		@ args = 0, pretend = 0, frame = 72
 288              		@ frame_needed = 0, uses_anonymous_args = 0
ARM GAS  /tmp/ccPZc5IS.s 			page 10


 289              	.LVL34:
 290 0144 70B5     		push	{r4, r5, r6, lr}
 291              		.cfi_def_cfa_offset 16
 292              		.cfi_offset 4, -16
 293              		.cfi_offset 5, -12
 294              		.cfi_offset 6, -8
 295              		.cfi_offset 14, -4
 296 0146 0646     		mov	r6, r0
 218:stage1_test.c **** 	int gpio_value;
 219:stage1_test.c **** 	FILE *f_gpio;
 220:stage1_test.c **** 	char file_path[50] = "/sys/class/gpio/gpio";
 297              		.loc 1 220 0
 298 0148 184D     		ldr	r5, .L22
 217:stage1_test.c **** 	int gpio_value;
 299              		.loc 1 217 0
 300 014a 92B0     		sub	sp, sp, #72
 301              		.cfi_def_cfa_offset 88
 302              		.loc 1 220 0
 303 014c 05AC     		add	r4, sp, #20
 304 014e 0FCD     		ldmia	r5!, {r0, r1, r2, r3}
 305              	.LVL35:
 306 0150 0FC4     		stmia	r4!, {r0, r1, r2, r3}
 307 0152 1D22     		movs	r2, #29
 308 0154 95E80300 		ldmia	r5, {r0, r1}
 309 0158 44F8040B 		str	r0, [r4], #4
 310 015c 0DF12900 		add	r0, sp, #41
 311 0160 2170     		strb	r1, [r4]
 312 0162 0021     		movs	r1, #0
 313 0164 FFF7FEFF 		bl	memset
 314              	.LVL36:
 221:stage1_test.c **** 	char file_end[9];
 222:stage1_test.c **** 	sprintf(file_end,"%d/value",pin_number);
 315              		.loc 1 222 0
 316 0168 40F20001 		movw	r1, #:lower16:.LC6
 317 016c 3246     		mov	r2, r6
 318 016e 02A8     		add	r0, sp, #8
 319 0170 C0F20001 		movt	r1, #:upper16:.LC6
 320 0174 FFF7FEFF 		bl	sprintf
 321              	.LVL37:
 223:stage1_test.c **** 	strcat(file_path,file_end);
 322              		.loc 1 223 0
 323 0178 02A9     		add	r1, sp, #8
 324 017a 05A8     		add	r0, sp, #20
 325 017c FFF7FEFF 		bl	strcat
 326              	.LVL38:
 224:stage1_test.c **** 	f_gpio = fopen(file_path,"r");
 327              		.loc 1 224 0
 328 0180 40F20001 		movw	r1, #:lower16:.LC3
 329 0184 05A8     		add	r0, sp, #20
 330 0186 C0F20001 		movt	r1, #:upper16:.LC3
 331 018a FFF7FEFF 		bl	fopen
 332              	.LVL39:
 225:stage1_test.c **** 	fscanf(f_gpio,"%d",&gpio_value);
 333              		.loc 1 225 0
 334 018e 40F20001 		movw	r1, #:lower16:.LC4
 335 0192 01AA     		add	r2, sp, #4
 336 0194 C0F20001 		movt	r1, #:upper16:.LC4
ARM GAS  /tmp/ccPZc5IS.s 			page 11


 224:stage1_test.c **** 	f_gpio = fopen(file_path,"r");
 337              		.loc 1 224 0
 338 0198 0446     		mov	r4, r0
 339              	.LVL40:
 340              		.loc 1 225 0
 341 019a FFF7FEFF 		bl	__isoc99_fscanf
 342              	.LVL41:
 226:stage1_test.c **** 	fclose(f_gpio);
 343              		.loc 1 226 0
 344 019e 2046     		mov	r0, r4
 345 01a0 FFF7FEFF 		bl	fclose
 346              	.LVL42:
 227:stage1_test.c **** 	return gpio_value;
 228:stage1_test.c **** }
 347              		.loc 1 228 0
 348 01a4 0198     		ldr	r0, [sp, #4]
 349 01a6 12B0     		add	sp, sp, #72
 350              		.cfi_def_cfa_offset 16
 351              		@ sp needed
 352 01a8 70BD     		pop	{r4, r5, r6, pc}
 353              	.LVL43:
 354              	.L23:
 355 01aa 00BF     		.align	2
 356              	.L22:
 357 01ac 64000000 		.word	.LANCHOR0+100
 358              		.cfi_endproc
 359              	.LFE30:
 360              		.size	readGPIO, .-readGPIO
 361              		.align	2
 362              		.global	setDirGPIO
 363              		.thumb
 364              		.thumb_func
 365              		.type	setDirGPIO, %function
 366              	setDirGPIO:
 367              	.LFB31:
 229:stage1_test.c **** 
 230:stage1_test.c **** void setDirGPIO(int pin_number, int direction) {
 368              		.loc 1 230 0
 369              		.cfi_startproc
 370              		@ args = 0, pretend = 0, frame = 72
 371              		@ frame_needed = 0, uses_anonymous_args = 0
 372              	.LVL44:
 373 01b0 F0B5     		push	{r4, r5, r6, r7, lr}
 374              		.cfi_def_cfa_offset 20
 375              		.cfi_offset 4, -20
 376              		.cfi_offset 5, -16
 377              		.cfi_offset 6, -12
 378              		.cfi_offset 7, -8
 379              		.cfi_offset 14, -4
 380 01b2 0746     		mov	r7, r0
 231:stage1_test.c **** 	FILE *f_gpio;
 232:stage1_test.c **** 	char file_path[50] = "/sys/class/gpio/gpio";
 381              		.loc 1 232 0
 382 01b4 204D     		ldr	r5, .L29
 230:stage1_test.c **** 	FILE *f_gpio;
 383              		.loc 1 230 0
 384 01b6 0E46     		mov	r6, r1
ARM GAS  /tmp/ccPZc5IS.s 			page 12


 385 01b8 93B0     		sub	sp, sp, #76
 386              		.cfi_def_cfa_offset 96
 387              		.loc 1 232 0
 388 01ba 05AC     		add	r4, sp, #20
 389 01bc 0FCD     		ldmia	r5!, {r0, r1, r2, r3}
 390              	.LVL45:
 391 01be 0FC4     		stmia	r4!, {r0, r1, r2, r3}
 392 01c0 1D22     		movs	r2, #29
 393 01c2 95E80300 		ldmia	r5, {r0, r1}
 394 01c6 44F8040B 		str	r0, [r4], #4
 395 01ca 0DF12900 		add	r0, sp, #41
 396 01ce 2170     		strb	r1, [r4]
 397 01d0 0021     		movs	r1, #0
 398 01d2 FFF7FEFF 		bl	memset
 399              	.LVL46:
 233:stage1_test.c **** 	char file_end[15];
 234:stage1_test.c **** 	sprintf(file_end,"%d/direction",pin_number);
 400              		.loc 1 234 0
 401 01d6 40F20001 		movw	r1, #:lower16:.LC8
 402 01da 3A46     		mov	r2, r7
 403 01dc C0F20001 		movt	r1, #:upper16:.LC8
 404 01e0 01A8     		add	r0, sp, #4
 405 01e2 FFF7FEFF 		bl	sprintf
 406              	.LVL47:
 235:stage1_test.c **** 	strcat(file_path,file_end);
 407              		.loc 1 235 0
 408 01e6 01A9     		add	r1, sp, #4
 409 01e8 05A8     		add	r0, sp, #20
 410 01ea FFF7FEFF 		bl	strcat
 411              	.LVL48:
 236:stage1_test.c **** 	f_gpio = fopen(file_path,"w");
 412              		.loc 1 236 0
 413 01ee 40F20001 		movw	r1, #:lower16:.LC7
 414 01f2 05A8     		add	r0, sp, #20
 415 01f4 C0F20001 		movt	r1, #:upper16:.LC7
 416 01f8 FFF7FEFF 		bl	fopen
 417              	.LVL49:
 418 01fc 0446     		mov	r4, r0
 419              	.LVL50:
 237:stage1_test.c **** 	if (direction) fprintf(f_gpio,"in");
 420              		.loc 1 237 0
 421 01fe 6EB9     		cbnz	r6, .L28
 238:stage1_test.c **** 	else fprintf(f_gpio,"out");
 422              		.loc 1 238 0
 423 0200 40F20000 		movw	r0, #:lower16:.LC10
 424              	.LVL51:
 425 0204 0121     		movs	r1, #1
 426 0206 0322     		movs	r2, #3
 427 0208 2346     		mov	r3, r4
 428 020a C0F20000 		movt	r0, #:upper16:.LC10
 429 020e FFF7FEFF 		bl	fwrite
 430              	.LVL52:
 239:stage1_test.c **** 	fclose(f_gpio);
 431              		.loc 1 239 0
 432 0212 2046     		mov	r0, r4
 433 0214 FFF7FEFF 		bl	fclose
 434              	.LVL53:
ARM GAS  /tmp/ccPZc5IS.s 			page 13


 240:stage1_test.c **** }
 435              		.loc 1 240 0
 436 0218 13B0     		add	sp, sp, #76
 437              		.cfi_remember_state
 438              		.cfi_def_cfa_offset 20
 439              		@ sp needed
 440 021a F0BD     		pop	{r4, r5, r6, r7, pc}
 441              	.LVL54:
 442              	.L28:
 443              		.cfi_restore_state
 237:stage1_test.c **** 	else fprintf(f_gpio,"out");
 444              		.loc 1 237 0 discriminator 1
 445 021c 40F20000 		movw	r0, #:lower16:.LC9
 446              	.LVL55:
 447 0220 0121     		movs	r1, #1
 448 0222 0222     		movs	r2, #2
 449 0224 2346     		mov	r3, r4
 450 0226 C0F20000 		movt	r0, #:upper16:.LC9
 451 022a FFF7FEFF 		bl	fwrite
 452              	.LVL56:
 239:stage1_test.c **** 	fclose(f_gpio);
 453              		.loc 1 239 0 discriminator 1
 454 022e 2046     		mov	r0, r4
 455 0230 FFF7FEFF 		bl	fclose
 456              	.LVL57:
 457              		.loc 1 240 0 discriminator 1
 458 0234 13B0     		add	sp, sp, #76
 459              		.cfi_def_cfa_offset 20
 460              		@ sp needed
 461 0236 F0BD     		pop	{r4, r5, r6, r7, pc}
 462              	.LVL58:
 463              	.L30:
 464              		.align	2
 465              	.L29:
 466 0238 64000000 		.word	.LANCHOR0+100
 467              		.cfi_endproc
 468              	.LFE31:
 469              		.size	setDirGPIO, .-setDirGPIO
 470              		.align	2
 471              		.global	testComponent
 472              		.thumb
 473              		.thumb_func
 474              		.type	testComponent, %function
 475              	testComponent:
 476              	.LFB24:
 108:stage1_test.c **** 	setDirGPIO(TEST1,1);
 477              		.loc 1 108 0
 478              		.cfi_startproc
 479              		@ args = 0, pretend = 0, frame = 0
 480              		@ frame_needed = 0, uses_anonymous_args = 0
 481              	.LVL59:
 482 023c 70B5     		push	{r4, r5, r6, lr}
 483              		.cfi_def_cfa_offset 16
 484              		.cfi_offset 4, -16
 485              		.cfi_offset 5, -12
 486              		.cfi_offset 6, -8
 487              		.cfi_offset 14, -4
ARM GAS  /tmp/ccPZc5IS.s 			page 14


 108:stage1_test.c **** 	setDirGPIO(TEST1,1);
 488              		.loc 1 108 0
 489 023e 0446     		mov	r4, r0
 490 0240 0E46     		mov	r6, r1
 109:stage1_test.c **** 	setDirGPIO(TEST2,1);
 491              		.loc 1 109 0
 492 0242 0F20     		movs	r0, #15
 493              	.LVL60:
 494 0244 0121     		movs	r1, #1
 495              	.LVL61:
 496 0246 FFF7FEFF 		bl	setDirGPIO
 497              	.LVL62:
 110:stage1_test.c **** 	setDirGPIO(TEST3,1);
 498              		.loc 1 110 0
 499 024a 7320     		movs	r0, #115
 500 024c 0121     		movs	r1, #1
 501 024e FFF7FEFF 		bl	setDirGPIO
 502              	.LVL63:
 111:stage1_test.c **** 	setDirGPIO(TEST4,1);
 503              		.loc 1 111 0
 504 0252 0220     		movs	r0, #2
 505 0254 0121     		movs	r1, #1
 506 0256 FFF7FEFF 		bl	setDirGPIO
 507              	.LVL64:
 112:stage1_test.c **** 	setDirGPIO(TEST5,1);
 508              		.loc 1 112 0
 509 025a 3120     		movs	r0, #49
 510 025c 0121     		movs	r1, #1
 511 025e FFF7FEFF 		bl	setDirGPIO
 512              	.LVL65:
 113:stage1_test.c **** 	setDirGPIO(TEST6,1);
 513              		.loc 1 113 0
 514 0262 0320     		movs	r0, #3
 515 0264 0121     		movs	r1, #1
 516 0266 FFF7FEFF 		bl	setDirGPIO
 517              	.LVL66:
 114:stage1_test.c **** 
 518              		.loc 1 114 0
 519 026a 0E20     		movs	r0, #14
 520 026c 0121     		movs	r1, #1
 521 026e FFF7FEFF 		bl	setDirGPIO
 522              	.LVL67:
 116:stage1_test.c **** 	writeGPIO(gpio,1);
 523              		.loc 1 116 0
 524 0272 2046     		mov	r0, r4
 525 0274 0021     		movs	r1, #0
 526 0276 FFF7FEFF 		bl	setDirGPIO
 527              	.LVL68:
 117:stage1_test.c **** 
 528              		.loc 1 117 0
 529 027a 2046     		mov	r0, r4
 530 027c 0121     		movs	r1, #1
 531 027e FFF7FEFF 		bl	writeGPIO
 532              	.LVL69:
 119:stage1_test.c **** 	int component = componentID(readADC(0));
 533              		.loc 1 119 0
 534 0282 0021     		movs	r1, #0
ARM GAS  /tmp/ccPZc5IS.s 			page 15


 535 0284 3046     		mov	r0, r6
 536 0286 FFF7FEFF 		bl	nanosleep
 537              	.LVL70:
 120:stage1_test.c **** 	writeGPIO(gpio,0);
 538              		.loc 1 120 0
 539 028a 0020     		movs	r0, #0
 540 028c FFF7FEFF 		bl	readADC
 541              	.LVL71:
 542              	.LBB4:
 543              	.LBB5:
 136:stage1_test.c **** 	else if ( (mV > 120) && (mV < 275) ) return 2;
 544              		.loc 1 136 0
 545 0290 3128     		cmp	r0, #49
 546 0292 0DDD     		ble	.L32
 547 0294 FFF7B4FE 		bl	componentID.part.0
 548              	.LVL72:
 549              	.LBE5:
 550              	.LBE4:
 121:stage1_test.c **** 	if (component == 0) {
 551              		.loc 1 121 0
 552 0298 0021     		movs	r1, #0
 553              	.LBB7:
 554              	.LBB6:
 555 029a 0546     		mov	r5, r0
 556              	.LBE6:
 557              	.LBE7:
 558 029c 2046     		mov	r0, r4
 559 029e FFF7FEFF 		bl	writeGPIO
 560              	.LVL73:
 122:stage1_test.c **** 		setDirGPIO(gpio,1);
 561              		.loc 1 122 0
 562 02a2 4DB1     		cbz	r5, .L35
 563              	.L33:
 564              	.LVL74:
 131:stage1_test.c **** 	return component;
 565              		.loc 1 131 0
 566 02a4 0E20     		movs	r0, #14
 567 02a6 0121     		movs	r1, #1
 568 02a8 FFF7FEFF 		bl	setDirGPIO
 569              	.LVL75:
 133:stage1_test.c **** 
 570              		.loc 1 133 0
 571 02ac 2846     		mov	r0, r5
 572 02ae 70BD     		pop	{r4, r5, r6, pc}
 573              	.LVL76:
 574              	.L32:
 121:stage1_test.c **** 	if (component == 0) {
 575              		.loc 1 121 0
 576 02b0 2046     		mov	r0, r4
 577              	.LVL77:
 578 02b2 0021     		movs	r1, #0
 579 02b4 FFF7FEFF 		bl	writeGPIO
 580              	.LVL78:
 581              	.L35:
 123:stage1_test.c **** 		setDirGPIO(TEST6,0);
 582              		.loc 1 123 0
 583 02b8 2046     		mov	r0, r4
ARM GAS  /tmp/ccPZc5IS.s 			page 16


 584 02ba 0121     		movs	r1, #1
 585 02bc FFF7FEFF 		bl	setDirGPIO
 586              	.LVL79:
 124:stage1_test.c **** 		writeGPIO(TEST6,1);
 587              		.loc 1 124 0
 588 02c0 0E20     		movs	r0, #14
 589 02c2 0021     		movs	r1, #0
 590 02c4 FFF7FEFF 		bl	setDirGPIO
 591              	.LVL80:
 125:stage1_test.c **** 		nanosleep(time_des,NULL);
 592              		.loc 1 125 0
 593 02c8 0E20     		movs	r0, #14
 594 02ca 0121     		movs	r1, #1
 595 02cc FFF7FEFF 		bl	writeGPIO
 596              	.LVL81:
 126:stage1_test.c **** 		if (readGPIO(gpio)) {component = 5;}
 597              		.loc 1 126 0
 598 02d0 0021     		movs	r1, #0
 599 02d2 3046     		mov	r0, r6
 600 02d4 FFF7FEFF 		bl	nanosleep
 601              	.LVL82:
 127:stage1_test.c **** 		else {component = 3;}
 602              		.loc 1 127 0
 603 02d8 2046     		mov	r0, r4
 604 02da FFF7FEFF 		bl	readGPIO
 605              	.LVL83:
 129:stage1_test.c **** 	}
 606              		.loc 1 129 0
 607 02de 0021     		movs	r1, #0
 128:stage1_test.c **** 		writeGPIO(TEST6,0);
 608              		.loc 1 128 0
 609 02e0 8842     		cmp	r0, r1
 129:stage1_test.c **** 	}
 610              		.loc 1 129 0
 611 02e2 4FF00E00 		mov	r0, #14
 128:stage1_test.c **** 		writeGPIO(TEST6,0);
 612              		.loc 1 128 0
 613 02e6 14BF     		ite	ne
 614 02e8 0525     		movne	r5, #5
 615 02ea 0325     		moveq	r5, #3
 616              	.LVL84:
 129:stage1_test.c **** 	}
 617              		.loc 1 129 0
 618 02ec FFF7FEFF 		bl	writeGPIO
 619              	.LVL85:
 620 02f0 D8E7     		b	.L33
 621              		.cfi_endproc
 622              	.LFE24:
 623              		.size	testComponent, .-testComponent
 624 02f2 00BF     		.align	2
 625              		.global	detectShort
 626              		.thumb
 627              		.thumb_func
 628              		.type	detectShort, %function
 629              	detectShort:
 630              	.LFB27:
 173:stage1_test.c **** 	int shorted = 0;
ARM GAS  /tmp/ccPZc5IS.s 			page 17


 631              		.loc 1 173 0
 632              		.cfi_startproc
 633              		@ args = 0, pretend = 0, frame = 0
 634              		@ frame_needed = 0, uses_anonymous_args = 0
 635              	.LVL86:
 636 02f4 38B5     		push	{r3, r4, r5, lr}
 637              		.cfi_def_cfa_offset 16
 638              		.cfi_offset 3, -16
 639              		.cfi_offset 4, -12
 640              		.cfi_offset 5, -8
 641              		.cfi_offset 14, -4
 173:stage1_test.c **** 	int shorted = 0;
 642              		.loc 1 173 0
 643 02f6 0446     		mov	r4, r0
 644 02f8 0D46     		mov	r5, r1
 175:stage1_test.c **** 	writeGPIO(gpio1,1);
 645              		.loc 1 175 0
 646 02fa 0021     		movs	r1, #0
 647              	.LVL87:
 648 02fc FFF7FEFF 		bl	setDirGPIO
 649              	.LVL88:
 176:stage1_test.c **** 	if (readGPIO(gpio2)) {
 650              		.loc 1 176 0
 651 0300 2046     		mov	r0, r4
 652 0302 0121     		movs	r1, #1
 653 0304 FFF7FEFF 		bl	writeGPIO
 654              	.LVL89:
 177:stage1_test.c **** 		shorted = 1;
 655              		.loc 1 177 0
 656 0308 2846     		mov	r0, r5
 657 030a FFF7FEFF 		bl	readGPIO
 658              	.LVL90:
 181:stage1_test.c **** 	setDirGPIO(gpio1,1);
 659              		.loc 1 181 0
 660 030e 0021     		movs	r1, #0
 177:stage1_test.c **** 		shorted = 1;
 661              		.loc 1 177 0
 662 0310 0546     		mov	r5, r0
 663              	.LVL91:
 181:stage1_test.c **** 	setDirGPIO(gpio1,1);
 664              		.loc 1 181 0
 665 0312 2046     		mov	r0, r4
 666              	.LVL92:
 667 0314 FFF7FEFF 		bl	writeGPIO
 668              	.LVL93:
 182:stage1_test.c **** 	return shorted;
 669              		.loc 1 182 0
 670 0318 2046     		mov	r0, r4
 671 031a 0121     		movs	r1, #1
 672 031c FFF7FEFF 		bl	setDirGPIO
 673              	.LVL94:
 184:stage1_test.c **** 
 674              		.loc 1 184 0
 675 0320 281C     		adds	r0, r5, #0
 676 0322 18BF     		it	ne
 677 0324 0120     		movne	r0, #1
 678 0326 38BD     		pop	{r3, r4, r5, pc}
ARM GAS  /tmp/ccPZc5IS.s 			page 18


 679              		.cfi_endproc
 680              	.LFE27:
 681              		.size	detectShort, .-detectShort
 682              		.align	2
 683              		.global	detectProblems
 684              		.thumb
 685              		.thumb_func
 686              		.type	detectProblems, %function
 687              	detectProblems:
 688              	.LFB26:
 144:stage1_test.c **** 	int i,j;
 689              		.loc 1 144 0
 690              		.cfi_startproc
 691              		@ args = 0, pretend = 0, frame = 24
 692              		@ frame_needed = 0, uses_anonymous_args = 0
 693              	.LVL95:
 694 0328 2DE9F043 		push	{r4, r5, r6, r7, r8, r9, lr}
 695              		.cfi_def_cfa_offset 28
 696              		.cfi_offset 4, -28
 697              		.cfi_offset 5, -24
 698              		.cfi_offset 6, -20
 699              		.cfi_offset 7, -16
 700              		.cfi_offset 8, -12
 701              		.cfi_offset 9, -8
 702              		.cfi_offset 14, -4
 703 032c 0646     		mov	r6, r0
 148:stage1_test.c **** 	int pins[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
 704              		.loc 1 148 0
 705 032e DFF898E0 		ldr	lr, .L56
 144:stage1_test.c **** 	int i,j;
 706              		.loc 1 144 0
 707 0332 87B0     		sub	sp, sp, #28
 708              		.cfi_def_cfa_offset 56
 148:stage1_test.c **** 	int pins[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
 709              		.loc 1 148 0
 710 0334 0DF10408 		add	r8, sp, #4
 147:stage1_test.c **** 	int test_gpios[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
 711              		.loc 1 147 0
 712 0338 0027     		movs	r7, #0
 146:stage1_test.c **** 	int shorts = 0;
 713              		.loc 1 146 0
 714 033a B946     		mov	r9, r7
 150:stage1_test.c **** 		int x = pins[i];
 715              		.loc 1 150 0
 716 033c 3D46     		mov	r5, r7
 148:stage1_test.c **** 	int pins[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
 717              		.loc 1 148 0
 718 033e BEE80F00 		ldmia	lr!, {r0, r1, r2, r3}
 719              	.LVL96:
 720 0342 4446     		mov	r4, r8
 721 0344 DEF800E0 		ldr	lr, [lr]
 722 0348 0FC4     		stmia	r4!, {r0, r1, r2, r3}
 723 034a C4F800E0 		str	lr, [r4]
 724              	.LVL97:
 725              	.L42:
 144:stage1_test.c **** 	int i,j;
 726              		.loc 1 144 0
ARM GAS  /tmp/ccPZc5IS.s 			page 19


 727 034e 0024     		movs	r4, #0
 728              	.LVL98:
 729              	.L46:
 730              	.LBB8:
 154:stage1_test.c **** 				if (components[i] == 3) {
 731              		.loc 1 154 0
 732 0350 A542     		cmp	r5, r4
 733 0352 05D0     		beq	.L43
 154:stage1_test.c **** 				if (components[i] == 3) {
 734              		.loc 1 154 0 is_stmt 0 discriminator 1
 735 0354 56F82530 		ldr	r3, [r6, r5, lsl #2]
 736 0358 56F82420 		ldr	r2, [r6, r4, lsl #2]
 737 035c 9342     		cmp	r3, r2
 738 035e 13D0     		beq	.L53
 739              	.L43:
 153:stage1_test.c **** 			if ( (i != j) && (components[i] == components[j]) ) {
 740              		.loc 1 153 0 is_stmt 1 discriminator 2
 741 0360 0134     		adds	r4, r4, #1
 742              	.LVL99:
 743 0362 052C     		cmp	r4, #5
 744 0364 F4D1     		bne	.L46
 745              	.LBE8:
 150:stage1_test.c **** 		int x = pins[i];
 746              		.loc 1 150 0 discriminator 2
 747 0366 0135     		adds	r5, r5, #1
 748              	.LVL100:
 749 0368 052D     		cmp	r5, #5
 750 036a F0D1     		bne	.L42
 167:stage1_test.c **** 	else if (opens && !shorts) return 2;
 751              		.loc 1 167 0
 752 036c 3B1C     		adds	r3, r7, #0
 753 036e 18BF     		it	ne
 754 0370 0123     		movne	r3, #1
 755 0372 19F10002 		adds	r2, r9, #0
 756 0376 18BF     		it	ne
 757 0378 0122     		movne	r2, #1
 758 037a 1A42     		tst	r2, r3
 759 037c 18BF     		it	ne
 760 037e 0320     		movne	r0, #3
 761 0380 11D0     		beq	.L54
 762              	.L48:
 171:stage1_test.c **** 
 763              		.loc 1 171 0
 764 0382 07B0     		add	sp, sp, #28
 765              		.cfi_remember_state
 766              		.cfi_def_cfa_offset 28
 767              		@ sp needed
 768 0384 BDE8F083 		pop	{r4, r5, r6, r7, r8, r9, pc}
 769              	.LVL101:
 770              	.L53:
 771              		.cfi_restore_state
 772              	.LBB9:
 155:stage1_test.c **** 					if (detectShort(test_gpios[i],
 773              		.loc 1 155 0
 774 0388 032B     		cmp	r3, #3
 775 038a 01D0     		beq	.L55
 776              	.L44:
ARM GAS  /tmp/ccPZc5IS.s 			page 20


 163:stage1_test.c **** 			}
 777              		.loc 1 163 0
 778 038c 0137     		adds	r7, r7, #1
 779              	.LVL102:
 780 038e E7E7     		b	.L43
 781              	.L55:
 156:stage1_test.c **** 						test_gpios[j]))
 782              		.loc 1 156 0
 783 0390 58F82500 		ldr	r0, [r8, r5, lsl #2]
 784 0394 58F82410 		ldr	r1, [r8, r4, lsl #2]
 785 0398 FFF7FEFF 		bl	detectShort
 786              	.LVL103:
 787 039c 0028     		cmp	r0, #0
 788 039e F5D1     		bne	.L44
 160:stage1_test.c **** 				}
 789              		.loc 1 160 0
 790 03a0 09F10109 		add	r9, r9, #1
 791              	.LVL104:
 792 03a4 DCE7     		b	.L43
 793              	.L54:
 794              	.LBE9:
 168:stage1_test.c **** 	else if (shorts && !opens) return 1;
 795              		.loc 1 168 0
 796 03a6 002F     		cmp	r7, #0
 797 03a8 14BF     		ite	ne
 798 03aa 0022     		movne	r2, #0
 799 03ac 02F00102 		andeq	r2, r2, #1
 167:stage1_test.c **** 	else if (opens && !shorts) return 2;
 800              		.loc 1 167 0
 801 03b0 B9F1000F 		cmp	r9, #0
 802 03b4 14BF     		ite	ne
 803 03b6 0020     		movne	r0, #0
 804 03b8 03F00100 		andeq	r0, r3, #1
 168:stage1_test.c **** 	else if (shorts && !opens) return 1;
 805              		.loc 1 168 0
 806 03bc 002A     		cmp	r2, #0
 807 03be E0D0     		beq	.L48
 808 03c0 0220     		movs	r0, #2
 171:stage1_test.c **** 
 809              		.loc 1 171 0
 810 03c2 07B0     		add	sp, sp, #28
 811              		.cfi_def_cfa_offset 28
 812              		@ sp needed
 813 03c4 BDE8F083 		pop	{r4, r5, r6, r7, r8, r9, pc}
 814              	.LVL105:
 815              	.L57:
 816              		.align	2
 817              	.L56:
 818 03c8 98000000 		.word	.LANCHOR0+152
 819              		.cfi_endproc
 820              	.LFE26:
 821              		.size	detectProblems, .-detectProblems
 822              		.section	.text.startup,"ax",%progbits
 823              		.align	2
 824              		.global	main
 825              		.thumb
 826              		.thumb_func
ARM GAS  /tmp/ccPZc5IS.s 			page 21


 827              		.type	main, %function
 828              	main:
 829              	.LFB23:
  23:stage1_test.c **** 	//setDirGPIO(7,0);
 830              		.loc 1 23 0
 831              		.cfi_startproc
 832              		@ args = 0, pretend = 0, frame = 32
 833              		@ frame_needed = 0, uses_anonymous_args = 0
 834              	.LVL106:
 835 0000 30B5     		push	{r4, r5, lr}
 836              		.cfi_def_cfa_offset 12
 837              		.cfi_offset 4, -12
 838              		.cfi_offset 5, -8
 839              		.cfi_offset 14, -4
 840 0002 8BB0     		sub	sp, sp, #44
 841              		.cfi_def_cfa_offset 56
  28:stage1_test.c **** 		test_pins[2],test_pins[3],test_pins[4]);
 842              		.loc 1 28 0
 843 0004 40F20000 		movw	r0, #:lower16:.LC11
 844 0008 3123     		movs	r3, #49
 845 000a 7322     		movs	r2, #115
 846 000c 0093     		str	r3, [sp]
 847 000e C0F20000 		movt	r0, #:upper16:.LC11
 848 0012 0323     		movs	r3, #3
 849 0014 0F21     		movs	r1, #15
 850 0016 0193     		str	r3, [sp, #4]
 851 0018 0223     		movs	r3, #2
 852 001a FFF7FEFF 		bl	printf
 853              	.LVL107:
  34:stage1_test.c **** 	time_des_test.tv_sec = 0;
 854              		.loc 1 34 0
 855 001e 44F24023 		movw	r3, #16960
  33:stage1_test.c **** 	time_des.tv_nsec = 1000000; //10ms
 856              		.loc 1 33 0
 857 0022 0022     		movs	r2, #0
  34:stage1_test.c **** 	time_des_test.tv_sec = 0;
 858              		.loc 1 34 0
 859 0024 C0F20F03 		movt	r3, 15
  33:stage1_test.c **** 	time_des.tv_nsec = 1000000; //10ms
 860              		.loc 1 33 0
 861 0028 0392     		str	r2, [sp, #12]
  34:stage1_test.c **** 	time_des_test.tv_sec = 0;
 862              		.loc 1 34 0
 863 002a 0493     		str	r3, [sp, #16]
 864              	.LVL108:
 865 002c 03E0     		b	.L72
 866              	.L59:
  99:stage1_test.c **** 		//writeGPIO(7,readGPIO(14));
 867              		.loc 1 99 0
 868 002e 03A8     		add	r0, sp, #12
 869 0030 0021     		movs	r1, #0
 870 0032 FFF7FEFF 		bl	nanosleep
 871              	.LVL109:
 872              	.L72:
  38:stage1_test.c **** 			/*setDirGPIO(2,0);
 873              		.loc 1 38 0
 874 0036 7020     		movs	r0, #112
ARM GAS  /tmp/ccPZc5IS.s 			page 22


 875 0038 FFF7FEFF 		bl	readGPIO
 876              	.LVL110:
 877 003c 0028     		cmp	r0, #0
 878 003e F6D0     		beq	.L59
 879              	.LBB10:
  53:stage1_test.c **** 			component[1] = testComponent(TEST2,&time_des); 
 880              		.loc 1 53 0
 881 0040 03A9     		add	r1, sp, #12
 882 0042 0F20     		movs	r0, #15
 883 0044 FFF7FEFF 		bl	testComponent
 884              	.LVL111:
  54:stage1_test.c **** 			component[2] = testComponent(TEST3,&time_des); 
 885              		.loc 1 54 0
 886 0048 03A9     		add	r1, sp, #12
  53:stage1_test.c **** 			component[1] = testComponent(TEST2,&time_des); 
 887              		.loc 1 53 0
 888 004a 0590     		str	r0, [sp, #20]
  54:stage1_test.c **** 			component[2] = testComponent(TEST3,&time_des); 
 889              		.loc 1 54 0
 890 004c 7320     		movs	r0, #115
 891 004e FFF7FEFF 		bl	testComponent
 892              	.LVL112:
  55:stage1_test.c **** 			component[3] = testComponent(TEST4,&time_des); 
 893              		.loc 1 55 0
 894 0052 03A9     		add	r1, sp, #12
  54:stage1_test.c **** 			component[2] = testComponent(TEST3,&time_des); 
 895              		.loc 1 54 0
 896 0054 0690     		str	r0, [sp, #24]
  55:stage1_test.c **** 			component[3] = testComponent(TEST4,&time_des); 
 897              		.loc 1 55 0
 898 0056 0220     		movs	r0, #2
 899 0058 FFF7FEFF 		bl	testComponent
 900              	.LVL113:
  56:stage1_test.c **** 			component[4] = testComponent(TEST5,&time_des); 
 901              		.loc 1 56 0
 902 005c 03A9     		add	r1, sp, #12
  55:stage1_test.c **** 			component[3] = testComponent(TEST4,&time_des); 
 903              		.loc 1 55 0
 904 005e 0790     		str	r0, [sp, #28]
  56:stage1_test.c **** 			component[4] = testComponent(TEST5,&time_des); 
 905              		.loc 1 56 0
 906 0060 3120     		movs	r0, #49
 907 0062 FFF7FEFF 		bl	testComponent
 908              	.LVL114:
  57:stage1_test.c **** 			int problem = detectProblems(component);
 909              		.loc 1 57 0
 910 0066 03A9     		add	r1, sp, #12
  56:stage1_test.c **** 			component[4] = testComponent(TEST5,&time_des); 
 911              		.loc 1 56 0
 912 0068 0890     		str	r0, [sp, #32]
  57:stage1_test.c **** 			int problem = detectProblems(component);
 913              		.loc 1 57 0
 914 006a 0320     		movs	r0, #3
 915 006c FFF7FEFF 		bl	testComponent
 916              	.LVL115:
 917 0070 0990     		str	r0, [sp, #36]
  58:stage1_test.c **** 			if (problem == 3)
ARM GAS  /tmp/ccPZc5IS.s 			page 23


 918              		.loc 1 58 0
 919 0072 05A8     		add	r0, sp, #20
 920 0074 FFF7FEFF 		bl	detectProblems
 921              	.LVL116:
  59:stage1_test.c **** 				printf("Probe is not fully engaged and "
 922              		.loc 1 59 0
 923 0078 0328     		cmp	r0, #3
  58:stage1_test.c **** 			if (problem == 3)
 924              		.loc 1 58 0
 925 007a 0146     		mov	r1, r0
 926              	.LVL117:
  59:stage1_test.c **** 				printf("Probe is not fully engaged and "
 927              		.loc 1 59 0
 928 007c 51D0     		beq	.L78
  62:stage1_test.c **** 				printf("Probe is not fully engaged.\n");
 929              		.loc 1 62 0
 930 007e 0228     		cmp	r0, #2
 931 0080 56D0     		beq	.L79
  64:stage1_test.c **** 				printf("Some test leads are shorted.\n");
 932              		.loc 1 64 0
 933 0082 0128     		cmp	r0, #1
 934 0084 5BD0     		beq	.L80
  66:stage1_test.c **** 				printf("Test is acceptable.\n");
 935              		.loc 1 66 0
 936 0086 0028     		cmp	r0, #0
 937 0088 D1D1     		bne	.L59
 938              	.LBB11:
  67:stage1_test.c **** 				int j;
 939              		.loc 1 67 0
 940 008a 40F20000 		movw	r0, #:lower16:.LC15
 941              	.LVL118:
  69:stage1_test.c **** 					printf("%d: ",j+1);
 942              		.loc 1 69 0
 943 008e 0C46     		mov	r4, r1
  67:stage1_test.c **** 				int j;
 944              		.loc 1 67 0
 945 0090 C0F20000 		movt	r0, #:upper16:.LC15
 946 0094 05AD     		add	r5, sp, #20
 947 0096 FFF7FEFF 		bl	puts
 948              	.LVL119:
 949              	.L71:
  70:stage1_test.c **** 					switch (component[j]) {
 950              		.loc 1 70 0
 951 009a 0134     		adds	r4, r4, #1
 952              	.LVL120:
 953 009c 40F20000 		movw	r0, #:lower16:.LC16
 954 00a0 C0F20000 		movt	r0, #:upper16:.LC16
 955 00a4 2146     		mov	r1, r4
 956 00a6 FFF7FEFF 		bl	printf
 957              	.LVL121:
  71:stage1_test.c **** 						case 0:
 958              		.loc 1 71 0
 959 00aa 55F8043B 		ldr	r3, [r5], #4
 960 00ae 013B     		subs	r3, r3, #1
 961 00b0 042B     		cmp	r3, #4
 962 00b2 0AD8     		bhi	.L64
 963 00b4 DFE803F0 		tbb	[pc, r3]
ARM GAS  /tmp/ccPZc5IS.s 			page 24


 964              	.L66:
 965 00b8 27       		.byte	(.L65-.L66)/2
 966 00b9 20       		.byte	(.L67-.L66)/2
 967 00ba 19       		.byte	(.L68-.L66)/2
 968 00bb 03       		.byte	(.L69-.L66)/2
 969 00bc 2E       		.byte	(.L70-.L66)/2
 970 00bd 00       		.p2align 1
 971              	.L69:
  84:stage1_test.c **** 							break;
 972              		.loc 1 84 0
 973 00be 40F20000 		movw	r0, #:lower16:.LC20
 974 00c2 C0F20000 		movt	r0, #:upper16:.LC20
 975 00c6 FFF7FEFF 		bl	puts
 976              	.LVL122:
 977              	.L64:
  69:stage1_test.c **** 					printf("%d: ",j+1);
 978              		.loc 1 69 0 discriminator 1
 979 00ca 052C     		cmp	r4, #5
 980 00cc E5D1     		bne	.L71
  91:stage1_test.c **** 					component[1],component[2],
 981              		.loc 1 91 0
 982 00ce 089D     		ldr	r5, [sp, #32]
 983 00d0 40F20000 		movw	r0, #:lower16:.LC22
 984 00d4 099C     		ldr	r4, [sp, #36]
 985              	.LVL123:
 986 00d6 C0F20000 		movt	r0, #:upper16:.LC22
 987 00da 0599     		ldr	r1, [sp, #20]
 988 00dc 069A     		ldr	r2, [sp, #24]
 989 00de 079B     		ldr	r3, [sp, #28]
 990 00e0 0095     		str	r5, [sp]
 991 00e2 0194     		str	r4, [sp, #4]
 992 00e4 FFF7FEFF 		bl	printf
 993              	.LVL124:
 994 00e8 A1E7     		b	.L59
 995              	.LVL125:
 996              	.L68:
  81:stage1_test.c **** 							break;
 997              		.loc 1 81 0
 998 00ea 40F20000 		movw	r0, #:lower16:.LC19
 999 00ee C0F20000 		movt	r0, #:upper16:.LC19
 1000 00f2 FFF7FEFF 		bl	puts
 1001              	.LVL126:
  82:stage1_test.c **** 						case 4:
 1002              		.loc 1 82 0
 1003 00f6 E8E7     		b	.L64
 1004              	.L67:
  78:stage1_test.c **** 							break;
 1005              		.loc 1 78 0
 1006 00f8 40F20000 		movw	r0, #:lower16:.LC18
 1007 00fc C0F20000 		movt	r0, #:upper16:.LC18
 1008 0100 FFF7FEFF 		bl	puts
 1009              	.LVL127:
  79:stage1_test.c **** 						case 3:
 1010              		.loc 1 79 0
 1011 0104 E1E7     		b	.L64
 1012              	.L65:
  75:stage1_test.c **** 							break;
ARM GAS  /tmp/ccPZc5IS.s 			page 25


 1013              		.loc 1 75 0
 1014 0106 40F20000 		movw	r0, #:lower16:.LC17
 1015 010a C0F20000 		movt	r0, #:upper16:.LC17
 1016 010e FFF7FEFF 		bl	puts
 1017              	.LVL128:
  76:stage1_test.c **** 						case 2:
 1018              		.loc 1 76 0
 1019 0112 DAE7     		b	.L64
 1020              	.L70:
  87:stage1_test.c **** 							break;
 1021              		.loc 1 87 0
 1022 0114 40F20000 		movw	r0, #:lower16:.LC21
 1023 0118 C0F20000 		movt	r0, #:upper16:.LC21
 1024 011c FFF7FEFF 		bl	puts
 1025              	.LVL129:
  88:stage1_test.c **** 					}	
 1026              		.loc 1 88 0
 1027 0120 D3E7     		b	.L64
 1028              	.LVL130:
 1029              	.L78:
 1030              	.LBE11:
  60:stage1_test.c **** 					"some test leads are shorted.\n");
 1031              		.loc 1 60 0
 1032 0122 40F20000 		movw	r0, #:lower16:.LC12
 1033              	.LVL131:
 1034 0126 C0F20000 		movt	r0, #:upper16:.LC12
 1035 012a FFF7FEFF 		bl	puts
 1036              	.LVL132:
 1037 012e 7EE7     		b	.L59
 1038              	.LVL133:
 1039              	.L79:
  63:stage1_test.c **** 			else if (problem == 1)
 1040              		.loc 1 63 0
 1041 0130 40F20000 		movw	r0, #:lower16:.LC13
 1042              	.LVL134:
 1043 0134 C0F20000 		movt	r0, #:upper16:.LC13
 1044 0138 FFF7FEFF 		bl	puts
 1045              	.LVL135:
 1046 013c 77E7     		b	.L59
 1047              	.LVL136:
 1048              	.L80:
  65:stage1_test.c **** 			else if (problem == 0) {
 1049              		.loc 1 65 0
 1050 013e 40F20000 		movw	r0, #:lower16:.LC14
 1051              	.LVL137:
 1052 0142 C0F20000 		movt	r0, #:upper16:.LC14
 1053 0146 FFF7FEFF 		bl	puts
 1054              	.LVL138:
 1055 014a 70E7     		b	.L59
 1056              	.LBE10:
 1057              		.cfi_endproc
 1058              	.LFE23:
 1059              		.size	main, .-main
 1060              		.section	.rodata
 1061              		.align	2
 1062              	.LANCHOR0 = . + 0
 1063              	.LC1:
ARM GAS  /tmp/ccPZc5IS.s 			page 26


 1064 0000 2F737973 		.ascii	"/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x"
 1064      2F646576 
 1064      69636573 
 1064      2F706C61 
 1064      74666F72 
 1065 0033 2D616463 		.ascii	"-adc/iio:device0/in_voltage\000"
 1065      2F69696F 
 1065      3A646576 
 1065      69636530 
 1065      2F696E5F 
 1066 004f 00000000 		.space	21
 1066      00000000 
 1066      00000000 
 1066      00000000 
 1066      00000000 
 1067              	.LC5:
 1068 0064 2F737973 		.ascii	"/sys/class/gpio/gpio\000"
 1068      2F636C61 
 1068      73732F67 
 1068      70696F2F 
 1068      6770696F 
 1069 0079 00000000 		.space	29
 1069      00000000 
 1069      00000000 
 1069      00000000 
 1069      00000000 
 1070 0096 0000     		.space	2
 1071              	.LC0:
 1072 0098 0F000000 		.word	15
 1073 009c 73000000 		.word	115
 1074 00a0 02000000 		.word	2
 1075 00a4 31000000 		.word	49
 1076 00a8 03000000 		.word	3
 1077              		.section	.rodata.str1.4,"aMS",%progbits,1
 1078              		.align	2
 1079              	.LC2:
 1080 0000 25645F72 		.ascii	"%d_raw\000"
 1080      617700
 1081 0007 00       		.space	1
 1082              	.LC3:
 1083 0008 7200     		.ascii	"r\000"
 1084 000a 0000     		.space	2
 1085              	.LC4:
 1086 000c 256400   		.ascii	"%d\000"
 1087 000f 00       		.space	1
 1088              	.LC6:
 1089 0010 25642F76 		.ascii	"%d/value\000"
 1089      616C7565 
 1089      00
 1090 0019 000000   		.space	3
 1091              	.LC7:
 1092 001c 7700     		.ascii	"w\000"
 1093 001e 0000     		.space	2
 1094              	.LC8:
 1095 0020 25642F64 		.ascii	"%d/direction\000"
 1095      69726563 
 1095      74696F6E 
ARM GAS  /tmp/ccPZc5IS.s 			page 27


 1095      00
 1096 002d 000000   		.space	3
 1097              	.LC9:
 1098 0030 696E00   		.ascii	"in\000"
 1099 0033 00       		.space	1
 1100              	.LC10:
 1101 0034 6F757400 		.ascii	"out\000"
 1102              	.LC11:
 1103 0038 70696E73 		.ascii	"pins: %d, %d, %d, %d, %d\012\000"
 1103      3A202564 
 1103      2C202564 
 1103      2C202564 
 1103      2C202564 
 1104 0052 0000     		.space	2
 1105              	.LC12:
 1106 0054 50726F62 		.ascii	"Probe is not fully engaged and some test leads are "
 1106      65206973 
 1106      206E6F74 
 1106      2066756C 
 1106      6C792065 
 1107 0087 73686F72 		.ascii	"shorted.\000"
 1107      7465642E 
 1107      00
 1108              	.LC13:
 1109 0090 50726F62 		.ascii	"Probe is not fully engaged.\000"
 1109      65206973 
 1109      206E6F74 
 1109      2066756C 
 1109      6C792065 
 1110              	.LC14:
 1111 00ac 536F6D65 		.ascii	"Some test leads are shorted.\000"
 1111      20746573 
 1111      74206C65 
 1111      61647320 
 1111      61726520 
 1112 00c9 000000   		.space	3
 1113              	.LC15:
 1114 00cc 54657374 		.ascii	"Test is acceptable.\000"
 1114      20697320 
 1114      61636365 
 1114      70746162 
 1114      6C652E00 
 1115              	.LC16:
 1116 00e0 25643A20 		.ascii	"%d: \000"
 1116      00
 1117 00e5 000000   		.space	3
 1118              	.LC17:
 1119 00e8 57697265 		.ascii	"Wire\000"
 1119      00
 1120 00ed 000000   		.space	3
 1121              	.LC18:
 1122 00f0 52657369 		.ascii	"Resistor\000"
 1122      73746F72 
 1122      00
 1123 00f9 000000   		.space	3
 1124              	.LC19:
 1125 00fc 43617061 		.ascii	"Capacitor\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 28


 1125      6369746F 
 1125      7200
 1126 0106 0000     		.space	2
 1127              	.LC20:
 1128 0108 496E6475 		.ascii	"Inductor\000"
 1128      63746F72 
 1128      00
 1129 0111 000000   		.space	3
 1130              	.LC21:
 1131 0114 44696F64 		.ascii	"Diode\000"
 1131      6500
 1132 011a 0000     		.space	2
 1133              	.LC22:
 1134 011c 436F6465 		.ascii	"Code: %d %d %d %d %d\012\000"
 1134      3A202564 
 1134      20256420 
 1134      25642025 
 1134      64202564 
 1135 0132 0000     		.text
 1136              	.Letext0:
 1137              		.file 2 "/usr/lib/gcc/arm-linux-gnueabihf/4.9/include/stddef.h"
 1138              		.file 3 "/usr/include/arm-linux-gnueabihf/bits/types.h"
 1139              		.file 4 "/usr/include/stdio.h"
 1140              		.file 5 "/usr/include/libio.h"
 1141              		.file 6 "/usr/include/time.h"
 1142              		.file 7 "/usr/include/string.h"
 1143              		.file 8 "<built-in>"
 1144 03cc AFF30080 		.section	.debug_info,"",%progbits
 1145              	.Ldebug_info0:
 1146 0000 24100000 		.4byte	0x1024
 1147 0004 0400     		.2byte	0x4
 1148 0006 00000000 		.4byte	.Ldebug_abbrev0
 1149 000a 04       		.byte	0x4
 1150 000b 01       		.uleb128 0x1
 1151 000c 67030000 		.4byte	.LASF107
 1152 0010 01       		.byte	0x1
 1153 0011 62010000 		.4byte	.LASF108
 1154 0015 E2010000 		.4byte	.LASF109
 1155 0019 30000000 		.4byte	.Ldebug_ranges0+0x30
 1156 001d 00000000 		.4byte	0
 1157 0021 00000000 		.4byte	.Ldebug_line0
 1158 0025 02       		.uleb128 0x2
 1159 0026 08       		.byte	0x8
 1160 0027 04       		.byte	0x4
 1161 0028 31010000 		.4byte	.LASF0
 1162 002c 03       		.uleb128 0x3
 1163 002d 04       		.byte	0x4
 1164 002e 05       		.byte	0x5
 1165 002f 696E7400 		.ascii	"int\000"
 1166 0033 04       		.uleb128 0x4
 1167 0034 34000000 		.4byte	.LASF9
 1168 0038 02       		.byte	0x2
 1169 0039 D4       		.byte	0xd4
 1170 003a 3E000000 		.4byte	0x3e
 1171 003e 02       		.uleb128 0x2
 1172 003f 04       		.byte	0x4
 1173 0040 07       		.byte	0x7
ARM GAS  /tmp/ccPZc5IS.s 			page 29


 1174 0041 87010000 		.4byte	.LASF1
 1175 0045 02       		.uleb128 0x2
 1176 0046 01       		.byte	0x1
 1177 0047 08       		.byte	0x8
 1178 0048 05030000 		.4byte	.LASF2
 1179 004c 02       		.uleb128 0x2
 1180 004d 02       		.byte	0x2
 1181 004e 07       		.byte	0x7
 1182 004f CF010000 		.4byte	.LASF3
 1183 0053 02       		.uleb128 0x2
 1184 0054 04       		.byte	0x4
 1185 0055 07       		.byte	0x7
 1186 0056 A2010000 		.4byte	.LASF4
 1187 005a 02       		.uleb128 0x2
 1188 005b 01       		.byte	0x1
 1189 005c 06       		.byte	0x6
 1190 005d BB000000 		.4byte	.LASF5
 1191 0061 02       		.uleb128 0x2
 1192 0062 02       		.byte	0x2
 1193 0063 05       		.byte	0x5
 1194 0064 1F030000 		.4byte	.LASF6
 1195 0068 02       		.uleb128 0x2
 1196 0069 08       		.byte	0x8
 1197 006a 05       		.byte	0x5
 1198 006b AD000000 		.4byte	.LASF7
 1199 006f 02       		.uleb128 0x2
 1200 0070 08       		.byte	0x8
 1201 0071 07       		.byte	0x7
 1202 0072 6A000000 		.4byte	.LASF8
 1203 0076 04       		.uleb128 0x4
 1204 0077 28010000 		.4byte	.LASF10
 1205 007b 03       		.byte	0x3
 1206 007c 37       		.byte	0x37
 1207 007d 68000000 		.4byte	0x68
 1208 0081 04       		.uleb128 0x4
 1209 0082 07000000 		.4byte	.LASF11
 1210 0086 03       		.byte	0x3
 1211 0087 83       		.byte	0x83
 1212 0088 8C000000 		.4byte	0x8c
 1213 008c 02       		.uleb128 0x2
 1214 008d 04       		.byte	0x4
 1215 008e 05       		.byte	0x5
 1216 008f 00010000 		.4byte	.LASF12
 1217 0093 04       		.uleb128 0x4
 1218 0094 D6030000 		.4byte	.LASF13
 1219 0098 03       		.byte	0x3
 1220 0099 84       		.byte	0x84
 1221 009a 76000000 		.4byte	0x76
 1222 009e 02       		.uleb128 0x2
 1223 009f 04       		.byte	0x4
 1224 00a0 07       		.byte	0x7
 1225 00a1 6D020000 		.4byte	.LASF14
 1226 00a5 04       		.uleb128 0x4
 1227 00a6 2E040000 		.4byte	.LASF15
 1228 00aa 03       		.byte	0x3
 1229 00ab 8B       		.byte	0x8b
 1230 00ac 8C000000 		.4byte	0x8c
ARM GAS  /tmp/ccPZc5IS.s 			page 30


 1231 00b0 05       		.uleb128 0x5
 1232 00b1 04       		.byte	0x4
 1233 00b2 04       		.uleb128 0x4
 1234 00b3 90020000 		.4byte	.LASF16
 1235 00b7 03       		.byte	0x3
 1236 00b8 AF       		.byte	0xaf
 1237 00b9 8C000000 		.4byte	0x8c
 1238 00bd 06       		.uleb128 0x6
 1239 00be 04       		.byte	0x4
 1240 00bf C3000000 		.4byte	0xc3
 1241 00c3 02       		.uleb128 0x2
 1242 00c4 01       		.byte	0x1
 1243 00c5 08       		.byte	0x8
 1244 00c6 C4030000 		.4byte	.LASF17
 1245 00ca 04       		.uleb128 0x4
 1246 00cb 49030000 		.4byte	.LASF18
 1247 00cf 04       		.byte	0x4
 1248 00d0 30       		.byte	0x30
 1249 00d1 D5000000 		.4byte	0xd5
 1250 00d5 07       		.uleb128 0x7
 1251 00d6 C5020000 		.4byte	.LASF48
 1252 00da 98       		.byte	0x98
 1253 00db 05       		.byte	0x5
 1254 00dc F5       		.byte	0xf5
 1255 00dd 55020000 		.4byte	0x255
 1256 00e1 08       		.uleb128 0x8
 1257 00e2 09010000 		.4byte	.LASF19
 1258 00e6 05       		.byte	0x5
 1259 00e7 F6       		.byte	0xf6
 1260 00e8 2C000000 		.4byte	0x2c
 1261 00ec 00       		.byte	0
 1262 00ed 08       		.uleb128 0x8
 1263 00ee 0F000000 		.4byte	.LASF20
 1264 00f2 05       		.byte	0x5
 1265 00f3 FB       		.byte	0xfb
 1266 00f4 BD000000 		.4byte	0xbd
 1267 00f8 04       		.byte	0x4
 1268 00f9 08       		.uleb128 0x8
 1269 00fa F3000000 		.4byte	.LASF21
 1270 00fe 05       		.byte	0x5
 1271 00ff FC       		.byte	0xfc
 1272 0100 BD000000 		.4byte	0xbd
 1273 0104 08       		.byte	0x8
 1274 0105 08       		.uleb128 0x8
 1275 0106 E0030000 		.4byte	.LASF22
 1276 010a 05       		.byte	0x5
 1277 010b FD       		.byte	0xfd
 1278 010c BD000000 		.4byte	0xbd
 1279 0110 0C       		.byte	0xc
 1280 0111 08       		.uleb128 0x8
 1281 0112 84040000 		.4byte	.LASF23
 1282 0116 05       		.byte	0x5
 1283 0117 FE       		.byte	0xfe
 1284 0118 BD000000 		.4byte	0xbd
 1285 011c 10       		.byte	0x10
 1286 011d 08       		.uleb128 0x8
 1287 011e BB010000 		.4byte	.LASF24
ARM GAS  /tmp/ccPZc5IS.s 			page 31


 1288 0122 05       		.byte	0x5
 1289 0123 FF       		.byte	0xff
 1290 0124 BD000000 		.4byte	0xbd
 1291 0128 14       		.byte	0x14
 1292 0129 09       		.uleb128 0x9
 1293 012a A2020000 		.4byte	.LASF25
 1294 012e 05       		.byte	0x5
 1295 012f 0001     		.2byte	0x100
 1296 0131 BD000000 		.4byte	0xbd
 1297 0135 18       		.byte	0x18
 1298 0136 09       		.uleb128 0x9
 1299 0137 4F000000 		.4byte	.LASF26
 1300 013b 05       		.byte	0x5
 1301 013c 0101     		.2byte	0x101
 1302 013e BD000000 		.4byte	0xbd
 1303 0142 1C       		.byte	0x1c
 1304 0143 09       		.uleb128 0x9
 1305 0144 10010000 		.4byte	.LASF27
 1306 0148 05       		.byte	0x5
 1307 0149 0201     		.2byte	0x102
 1308 014b BD000000 		.4byte	0xbd
 1309 014f 20       		.byte	0x20
 1310 0150 09       		.uleb128 0x9
 1311 0151 ED010000 		.4byte	.LASF28
 1312 0155 05       		.byte	0x5
 1313 0156 0401     		.2byte	0x104
 1314 0158 BD000000 		.4byte	0xbd
 1315 015c 24       		.byte	0x24
 1316 015d 09       		.uleb128 0x9
 1317 015e 5E040000 		.4byte	.LASF29
 1318 0162 05       		.byte	0x5
 1319 0163 0501     		.2byte	0x105
 1320 0165 BD000000 		.4byte	0xbd
 1321 0169 28       		.byte	0x28
 1322 016a 09       		.uleb128 0x9
 1323 016b F5030000 		.4byte	.LASF30
 1324 016f 05       		.byte	0x5
 1325 0170 0601     		.2byte	0x106
 1326 0172 BD000000 		.4byte	0xbd
 1327 0176 2C       		.byte	0x2c
 1328 0177 09       		.uleb128 0x9
 1329 0178 FC020000 		.4byte	.LASF31
 1330 017c 05       		.byte	0x5
 1331 017d 0801     		.2byte	0x108
 1332 017f 8D020000 		.4byte	0x28d
 1333 0183 30       		.byte	0x30
 1334 0184 09       		.uleb128 0x9
 1335 0185 1C000000 		.4byte	.LASF32
 1336 0189 05       		.byte	0x5
 1337 018a 0A01     		.2byte	0x10a
 1338 018c 93020000 		.4byte	0x293
 1339 0190 34       		.byte	0x34
 1340 0191 09       		.uleb128 0x9
 1341 0192 EB000000 		.4byte	.LASF33
 1342 0196 05       		.byte	0x5
 1343 0197 0C01     		.2byte	0x10c
 1344 0199 2C000000 		.4byte	0x2c
ARM GAS  /tmp/ccPZc5IS.s 			page 32


 1345 019d 38       		.byte	0x38
 1346 019e 09       		.uleb128 0x9
 1347 019f 23020000 		.4byte	.LASF34
 1348 01a3 05       		.byte	0x5
 1349 01a4 1001     		.2byte	0x110
 1350 01a6 2C000000 		.4byte	0x2c
 1351 01aa 3C       		.byte	0x3c
 1352 01ab 09       		.uleb128 0x9
 1353 01ac 43010000 		.4byte	.LASF35
 1354 01b0 05       		.byte	0x5
 1355 01b1 1201     		.2byte	0x112
 1356 01b3 81000000 		.4byte	0x81
 1357 01b7 40       		.byte	0x40
 1358 01b8 09       		.uleb128 0x9
 1359 01b9 1C010000 		.4byte	.LASF36
 1360 01bd 05       		.byte	0x5
 1361 01be 1601     		.2byte	0x116
 1362 01c0 4C000000 		.4byte	0x4c
 1363 01c4 44       		.byte	0x44
 1364 01c5 09       		.uleb128 0x9
 1365 01c6 3A030000 		.4byte	.LASF37
 1366 01ca 05       		.byte	0x5
 1367 01cb 1701     		.2byte	0x117
 1368 01cd 5A000000 		.4byte	0x5a
 1369 01d1 46       		.byte	0x46
 1370 01d2 09       		.uleb128 0x9
 1371 01d3 3B000000 		.4byte	.LASF38
 1372 01d7 05       		.byte	0x5
 1373 01d8 1801     		.2byte	0x118
 1374 01da 99020000 		.4byte	0x299
 1375 01de 47       		.byte	0x47
 1376 01df 09       		.uleb128 0x9
 1377 01e0 15020000 		.4byte	.LASF39
 1378 01e4 05       		.byte	0x5
 1379 01e5 1C01     		.2byte	0x11c
 1380 01e7 A9020000 		.4byte	0x2a9
 1381 01eb 48       		.byte	0x48
 1382 01ec 09       		.uleb128 0x9
 1383 01ed 4F010000 		.4byte	.LASF40
 1384 01f1 05       		.byte	0x5
 1385 01f2 2501     		.2byte	0x125
 1386 01f4 93000000 		.4byte	0x93
 1387 01f8 50       		.byte	0x50
 1388 01f9 09       		.uleb128 0x9
 1389 01fa 0B040000 		.4byte	.LASF41
 1390 01fe 05       		.byte	0x5
 1391 01ff 2E01     		.2byte	0x12e
 1392 0201 B0000000 		.4byte	0xb0
 1393 0205 58       		.byte	0x58
 1394 0206 09       		.uleb128 0x9
 1395 0207 12040000 		.4byte	.LASF42
 1396 020b 05       		.byte	0x5
 1397 020c 2F01     		.2byte	0x12f
 1398 020e B0000000 		.4byte	0xb0
 1399 0212 5C       		.byte	0x5c
 1400 0213 09       		.uleb128 0x9
 1401 0214 19040000 		.4byte	.LASF43
ARM GAS  /tmp/ccPZc5IS.s 			page 33


 1402 0218 05       		.byte	0x5
 1403 0219 3001     		.2byte	0x130
 1404 021b B0000000 		.4byte	0xb0
 1405 021f 60       		.byte	0x60
 1406 0220 09       		.uleb128 0x9
 1407 0221 20040000 		.4byte	.LASF44
 1408 0225 05       		.byte	0x5
 1409 0226 3101     		.2byte	0x131
 1410 0228 B0000000 		.4byte	0xb0
 1411 022c 64       		.byte	0x64
 1412 022d 09       		.uleb128 0x9
 1413 022e 27040000 		.4byte	.LASF45
 1414 0232 05       		.byte	0x5
 1415 0233 3201     		.2byte	0x132
 1416 0235 33000000 		.4byte	0x33
 1417 0239 68       		.byte	0x68
 1418 023a 09       		.uleb128 0x9
 1419 023b 2B020000 		.4byte	.LASF46
 1420 023f 05       		.byte	0x5
 1421 0240 3401     		.2byte	0x134
 1422 0242 2C000000 		.4byte	0x2c
 1423 0246 6C       		.byte	0x6c
 1424 0247 09       		.uleb128 0x9
 1425 0248 37040000 		.4byte	.LASF47
 1426 024c 05       		.byte	0x5
 1427 024d 3601     		.2byte	0x136
 1428 024f AF020000 		.4byte	0x2af
 1429 0253 70       		.byte	0x70
 1430 0254 00       		.byte	0
 1431 0255 0A       		.uleb128 0xa
 1432 0256 BA020000 		.4byte	.LASF110
 1433 025a 05       		.byte	0x5
 1434 025b 9A       		.byte	0x9a
 1435 025c 07       		.uleb128 0x7
 1436 025d 76010000 		.4byte	.LASF49
 1437 0261 0C       		.byte	0xc
 1438 0262 05       		.byte	0x5
 1439 0263 A0       		.byte	0xa0
 1440 0264 8D020000 		.4byte	0x28d
 1441 0268 08       		.uleb128 0x8
 1442 0269 D0030000 		.4byte	.LASF50
 1443 026d 05       		.byte	0x5
 1444 026e A1       		.byte	0xa1
 1445 026f 8D020000 		.4byte	0x28d
 1446 0273 00       		.byte	0
 1447 0274 08       		.uleb128 0x8
 1448 0275 C9010000 		.4byte	.LASF51
 1449 0279 05       		.byte	0x5
 1450 027a A2       		.byte	0xa2
 1451 027b 93020000 		.4byte	0x293
 1452 027f 04       		.byte	0x4
 1453 0280 08       		.uleb128 0x8
 1454 0281 F7020000 		.4byte	.LASF52
 1455 0285 05       		.byte	0x5
 1456 0286 A6       		.byte	0xa6
 1457 0287 2C000000 		.4byte	0x2c
 1458 028b 08       		.byte	0x8
ARM GAS  /tmp/ccPZc5IS.s 			page 34


 1459 028c 00       		.byte	0
 1460 028d 06       		.uleb128 0x6
 1461 028e 04       		.byte	0x4
 1462 028f 5C020000 		.4byte	0x25c
 1463 0293 06       		.uleb128 0x6
 1464 0294 04       		.byte	0x4
 1465 0295 D5000000 		.4byte	0xd5
 1466 0299 0B       		.uleb128 0xb
 1467 029a C3000000 		.4byte	0xc3
 1468 029e A9020000 		.4byte	0x2a9
 1469 02a2 0C       		.uleb128 0xc
 1470 02a3 9E000000 		.4byte	0x9e
 1471 02a7 00       		.byte	0
 1472 02a8 00       		.byte	0
 1473 02a9 06       		.uleb128 0x6
 1474 02aa 04       		.byte	0x4
 1475 02ab 55020000 		.4byte	0x255
 1476 02af 0B       		.uleb128 0xb
 1477 02b0 C3000000 		.4byte	0xc3
 1478 02b4 BF020000 		.4byte	0x2bf
 1479 02b8 0C       		.uleb128 0xc
 1480 02b9 9E000000 		.4byte	0x9e
 1481 02bd 27       		.byte	0x27
 1482 02be 00       		.byte	0
 1483 02bf 06       		.uleb128 0x6
 1484 02c0 04       		.byte	0x4
 1485 02c1 C5020000 		.4byte	0x2c5
 1486 02c5 0D       		.uleb128 0xd
 1487 02c6 C3000000 		.4byte	0xc3
 1488 02ca 07       		.uleb128 0x7
 1489 02cb 31030000 		.4byte	.LASF53
 1490 02cf 08       		.byte	0x8
 1491 02d0 06       		.byte	0x6
 1492 02d1 78       		.byte	0x78
 1493 02d2 EF020000 		.4byte	0x2ef
 1494 02d6 08       		.uleb128 0x8
 1495 02d7 89020000 		.4byte	.LASF54
 1496 02db 06       		.byte	0x6
 1497 02dc 7A       		.byte	0x7a
 1498 02dd A5000000 		.4byte	0xa5
 1499 02e1 00       		.byte	0
 1500 02e2 08       		.uleb128 0x8
 1501 02e3 65020000 		.4byte	.LASF55
 1502 02e7 06       		.byte	0x6
 1503 02e8 7B       		.byte	0x7b
 1504 02e9 B2000000 		.4byte	0xb2
 1505 02ed 04       		.byte	0x4
 1506 02ee 00       		.byte	0
 1507 02ef 0E       		.uleb128 0xe
 1508 02f0 8A000000 		.4byte	.LASF94
 1509 02f4 01       		.byte	0x1
 1510 02f5 87       		.byte	0x87
 1511 02f6 2C000000 		.4byte	0x2c
 1512 02fa 01       		.byte	0x1
 1513 02fb 0A030000 		.4byte	0x30a
 1514 02ff 0F       		.uleb128 0xf
 1515 0300 6D5600   		.ascii	"mV\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 35


 1516 0303 01       		.byte	0x1
 1517 0304 87       		.byte	0x87
 1518 0305 2C000000 		.4byte	0x2c
 1519 0309 00       		.byte	0
 1520 030a 10       		.uleb128 0x10
 1521 030b EF020000 		.4byte	0x2ef
 1522 030f 00000000 		.4byte	.LFB32
 1523 0313 34000000 		.4byte	.LFE32-.LFB32
 1524 0317 01       		.uleb128 0x1
 1525 0318 9C       		.byte	0x9c
 1526 0319 27030000 		.4byte	0x327
 1527 031d 11       		.uleb128 0x11
 1528 031e FF020000 		.4byte	0x2ff
 1529 0322 00000000 		.4byte	.LLST0
 1530 0326 00       		.byte	0
 1531 0327 10       		.uleb128 0x10
 1532 0328 EF020000 		.4byte	0x2ef
 1533 032c 34000000 		.4byte	.LFB25
 1534 0330 0A000000 		.4byte	.LFE25-.LFB25
 1535 0334 01       		.uleb128 0x1
 1536 0335 9C       		.byte	0x9c
 1537 0336 55030000 		.4byte	0x355
 1538 033a 11       		.uleb128 0x11
 1539 033b FF020000 		.4byte	0x2ff
 1540 033f 6C000000 		.4byte	.LLST1
 1541 0343 12       		.uleb128 0x12
 1542 0344 3A000000 		.4byte	.LVL9
 1543 0348 0A030000 		.4byte	0x30a
 1544 034c 13       		.uleb128 0x13
 1545 034d 01       		.uleb128 0x1
 1546 034e 50       		.byte	0x50
 1547 034f 03       		.uleb128 0x3
 1548 0350 F3       		.byte	0xf3
 1549 0351 01       		.uleb128 0x1
 1550 0352 50       		.byte	0x50
 1551 0353 00       		.byte	0
 1552 0354 00       		.byte	0
 1553 0355 14       		.uleb128 0x14
 1554 0356 4C040000 		.4byte	.LASF65
 1555 035a 01       		.byte	0x1
 1556 035b BA       		.byte	0xba
 1557 035c 2C000000 		.4byte	0x2c
 1558 0360 40000000 		.4byte	.LFB28
 1559 0364 98000000 		.4byte	.LFE28-.LFB28
 1560 0368 01       		.uleb128 0x1
 1561 0369 9C       		.byte	0x9c
 1562 036a 8E040000 		.4byte	0x48e
 1563 036e 15       		.uleb128 0x15
 1564 036f 4E030000 		.4byte	.LASF60
 1565 0373 01       		.byte	0x1
 1566 0374 BA       		.byte	0xba
 1567 0375 2C000000 		.4byte	0x2c
 1568 0379 A6000000 		.4byte	.LLST2
 1569 037d 16       		.uleb128 0x16
 1570 037e D8000000 		.4byte	.LASF56
 1571 0382 01       		.byte	0x1
 1572 0383 BB       		.byte	0xbb
ARM GAS  /tmp/ccPZc5IS.s 			page 36


 1573 0384 8E040000 		.4byte	0x48e
 1574 0388 D2000000 		.4byte	.LLST3
 1575 038c 17       		.uleb128 0x17
 1576 038d 93040000 		.4byte	.LASF57
 1577 0391 01       		.byte	0x1
 1578 0392 BC       		.byte	0xbc
 1579 0393 94040000 		.4byte	0x494
 1580 0397 03       		.uleb128 0x3
 1581 0398 91       		.byte	0x91
 1582 0399 947F     		.sleb128 -108
 1583 039b 17       		.uleb128 0x17
 1584 039c 13030000 		.4byte	.LASF58
 1585 03a0 01       		.byte	0x1
 1586 03a1 BD       		.byte	0xbd
 1587 03a2 A4040000 		.4byte	0x4a4
 1588 03a6 03       		.uleb128 0x3
 1589 03a7 91       		.byte	0x91
 1590 03a8 887F     		.sleb128 -120
 1591 03aa 16       		.uleb128 0x16
 1592 03ab 5D030000 		.4byte	.LASF59
 1593 03af 01       		.byte	0x1
 1594 03b0 C1       		.byte	0xc1
 1595 03b1 2C000000 		.4byte	0x2c
 1596 03b5 F0000000 		.4byte	.LLST4
 1597 03b9 18       		.uleb128 0x18
 1598 03ba 56000000 		.4byte	.LVL13
 1599 03be CA0E0000 		.4byte	0xeca
 1600 03c2 DD030000 		.4byte	0x3dd
 1601 03c6 13       		.uleb128 0x13
 1602 03c7 01       		.uleb128 0x1
 1603 03c8 52       		.byte	0x52
 1604 03c9 02       		.uleb128 0x2
 1605 03ca 08       		.byte	0x8
 1606 03cb 4F       		.byte	0x4f
 1607 03cc 13       		.uleb128 0x13
 1608 03cd 01       		.uleb128 0x1
 1609 03ce 51       		.byte	0x51
 1610 03cf 05       		.uleb128 0x5
 1611 03d0 03       		.byte	0x3
 1612 03d1 00000000 		.4byte	.LANCHOR0
 1613 03d5 13       		.uleb128 0x13
 1614 03d6 01       		.uleb128 0x1
 1615 03d7 50       		.byte	0x50
 1616 03d8 03       		.uleb128 0x3
 1617 03d9 91       		.byte	0x91
 1618 03da 947F     		.sleb128 -108
 1619 03dc 00       		.byte	0
 1620 03dd 18       		.uleb128 0x18
 1621 03de 62000000 		.4byte	.LVL14
 1622 03e2 EE0E0000 		.4byte	0xeee
 1623 03e6 FB030000 		.4byte	0x3fb
 1624 03ea 13       		.uleb128 0x13
 1625 03eb 01       		.uleb128 0x1
 1626 03ec 52       		.byte	0x52
 1627 03ed 01       		.uleb128 0x1
 1628 03ee 45       		.byte	0x45
 1629 03ef 13       		.uleb128 0x13
ARM GAS  /tmp/ccPZc5IS.s 			page 37


 1630 03f0 01       		.uleb128 0x1
 1631 03f1 51       		.byte	0x51
 1632 03f2 01       		.uleb128 0x1
 1633 03f3 30       		.byte	0x30
 1634 03f4 13       		.uleb128 0x13
 1635 03f5 01       		.uleb128 0x1
 1636 03f6 50       		.byte	0x50
 1637 03f7 02       		.uleb128 0x2
 1638 03f8 91       		.byte	0x91
 1639 03f9 63       		.sleb128 -29
 1640 03fa 00       		.byte	0
 1641 03fb 18       		.uleb128 0x18
 1642 03fc 72000000 		.4byte	.LVL15
 1643 0400 0B0F0000 		.4byte	0xf0b
 1644 0404 1F040000 		.4byte	0x41f
 1645 0408 13       		.uleb128 0x13
 1646 0409 01       		.uleb128 0x1
 1647 040a 52       		.byte	0x52
 1648 040b 02       		.uleb128 0x2
 1649 040c 74       		.byte	0x74
 1650 040d 00       		.sleb128 0
 1651 040e 13       		.uleb128 0x13
 1652 040f 01       		.uleb128 0x1
 1653 0410 51       		.byte	0x51
 1654 0411 05       		.uleb128 0x5
 1655 0412 03       		.byte	0x3
 1656 0413 00000000 		.4byte	.LC2
 1657 0417 13       		.uleb128 0x13
 1658 0418 01       		.uleb128 0x1
 1659 0419 50       		.byte	0x50
 1660 041a 03       		.uleb128 0x3
 1661 041b 91       		.byte	0x91
 1662 041c 887F     		.sleb128 -120
 1663 041e 00       		.byte	0
 1664 041f 18       		.uleb128 0x18
 1665 0420 7A000000 		.4byte	.LVL16
 1666 0424 270F0000 		.4byte	0xf27
 1667 0428 3B040000 		.4byte	0x43b
 1668 042c 13       		.uleb128 0x13
 1669 042d 01       		.uleb128 0x1
 1670 042e 51       		.byte	0x51
 1671 042f 03       		.uleb128 0x3
 1672 0430 91       		.byte	0x91
 1673 0431 887F     		.sleb128 -120
 1674 0433 13       		.uleb128 0x13
 1675 0434 01       		.uleb128 0x1
 1676 0435 50       		.byte	0x50
 1677 0436 03       		.uleb128 0x3
 1678 0437 91       		.byte	0x91
 1679 0438 947F     		.sleb128 -108
 1680 043a 00       		.byte	0
 1681 043b 18       		.uleb128 0x18
 1682 043c 88000000 		.4byte	.LVL17
 1683 0440 410F0000 		.4byte	0xf41
 1684 0444 59040000 		.4byte	0x459
 1685 0448 13       		.uleb128 0x13
 1686 0449 01       		.uleb128 0x1
ARM GAS  /tmp/ccPZc5IS.s 			page 38


 1687 044a 51       		.byte	0x51
 1688 044b 05       		.uleb128 0x5
 1689 044c 03       		.byte	0x3
 1690 044d 08000000 		.4byte	.LC3
 1691 0451 13       		.uleb128 0x13
 1692 0452 01       		.uleb128 0x1
 1693 0453 50       		.byte	0x50
 1694 0454 03       		.uleb128 0x3
 1695 0455 91       		.byte	0x91
 1696 0456 947F     		.sleb128 -108
 1697 0458 00       		.byte	0
 1698 0459 18       		.uleb128 0x18
 1699 045a 98000000 		.4byte	.LVL19
 1700 045e 5C0F0000 		.4byte	0xf5c
 1701 0462 7D040000 		.4byte	0x47d
 1702 0466 13       		.uleb128 0x13
 1703 0467 01       		.uleb128 0x1
 1704 0468 52       		.byte	0x52
 1705 0469 03       		.uleb128 0x3
 1706 046a 91       		.byte	0x91
 1707 046b 847F     		.sleb128 -124
 1708 046d 13       		.uleb128 0x13
 1709 046e 01       		.uleb128 0x1
 1710 046f 51       		.byte	0x51
 1711 0470 05       		.uleb128 0x5
 1712 0471 03       		.byte	0x3
 1713 0472 0C000000 		.4byte	.LC4
 1714 0476 13       		.uleb128 0x13
 1715 0477 01       		.uleb128 0x1
 1716 0478 50       		.byte	0x50
 1717 0479 02       		.uleb128 0x2
 1718 047a 74       		.byte	0x74
 1719 047b 00       		.sleb128 0
 1720 047c 00       		.byte	0
 1721 047d 19       		.uleb128 0x19
 1722 047e 9E000000 		.4byte	.LVL20
 1723 0482 7C0F0000 		.4byte	0xf7c
 1724 0486 13       		.uleb128 0x13
 1725 0487 01       		.uleb128 0x1
 1726 0488 50       		.byte	0x50
 1727 0489 02       		.uleb128 0x2
 1728 048a 74       		.byte	0x74
 1729 048b 00       		.sleb128 0
 1730 048c 00       		.byte	0
 1731 048d 00       		.byte	0
 1732 048e 06       		.uleb128 0x6
 1733 048f 04       		.byte	0x4
 1734 0490 CA000000 		.4byte	0xca
 1735 0494 0B       		.uleb128 0xb
 1736 0495 C3000000 		.4byte	0xc3
 1737 0499 A4040000 		.4byte	0x4a4
 1738 049d 0C       		.uleb128 0xc
 1739 049e 9E000000 		.4byte	0x9e
 1740 04a2 63       		.byte	0x63
 1741 04a3 00       		.byte	0
 1742 04a4 0B       		.uleb128 0xb
 1743 04a5 C3000000 		.4byte	0xc3
ARM GAS  /tmp/ccPZc5IS.s 			page 39


 1744 04a9 B4040000 		.4byte	0x4b4
 1745 04ad 0C       		.uleb128 0xc
 1746 04ae 9E000000 		.4byte	0x9e
 1747 04b2 09       		.byte	0x9
 1748 04b3 00       		.byte	0
 1749 04b4 1A       		.uleb128 0x1a
 1750 04b5 B0020000 		.4byte	.LASF68
 1751 04b9 01       		.byte	0x1
 1752 04ba CE       		.byte	0xce
 1753 04bb D8000000 		.4byte	.LFB29
 1754 04bf 6C000000 		.4byte	.LFE29-.LFB29
 1755 04c3 01       		.uleb128 0x1
 1756 04c4 9C       		.byte	0x9c
 1757 04c5 C0050000 		.4byte	0x5c0
 1758 04c9 15       		.uleb128 0x15
 1759 04ca 57010000 		.4byte	.LASF61
 1760 04ce 01       		.byte	0x1
 1761 04cf CE       		.byte	0xce
 1762 04d0 2C000000 		.4byte	0x2c
 1763 04d4 16010000 		.4byte	.LLST5
 1764 04d8 15       		.uleb128 0x15
 1765 04d9 70010000 		.4byte	.LASF62
 1766 04dd 01       		.byte	0x1
 1767 04de CE       		.byte	0xce
 1768 04df 2C000000 		.4byte	0x2c
 1769 04e3 42010000 		.4byte	.LLST6
 1770 04e7 16       		.uleb128 0x16
 1771 04e8 B4010000 		.4byte	.LASF63
 1772 04ec 01       		.byte	0x1
 1773 04ed CF       		.byte	0xcf
 1774 04ee 8E040000 		.4byte	0x48e
 1775 04f2 6E010000 		.4byte	.LLST7
 1776 04f6 17       		.uleb128 0x17
 1777 04f7 93040000 		.4byte	.LASF57
 1778 04fb 01       		.byte	0x1
 1779 04fc D0       		.byte	0xd0
 1780 04fd C0050000 		.4byte	0x5c0
 1781 0501 03       		.uleb128 0x3
 1782 0502 91       		.byte	0x91
 1783 0503 B47F     		.sleb128 -76
 1784 0505 17       		.uleb128 0x17
 1785 0506 CE020000 		.4byte	.LASF64
 1786 050a 01       		.byte	0x1
 1787 050b D1       		.byte	0xd1
 1788 050c D0050000 		.4byte	0x5d0
 1789 0510 03       		.uleb128 0x3
 1790 0511 91       		.byte	0x91
 1791 0512 A87F     		.sleb128 -88
 1792 0514 18       		.uleb128 0x18
 1793 0515 FE000000 		.4byte	.LVL26
 1794 0519 EE0E0000 		.4byte	0xeee
 1795 051d 32050000 		.4byte	0x532
 1796 0521 13       		.uleb128 0x13
 1797 0522 01       		.uleb128 0x1
 1798 0523 52       		.byte	0x52
 1799 0524 01       		.uleb128 0x1
 1800 0525 4D       		.byte	0x4d
ARM GAS  /tmp/ccPZc5IS.s 			page 40


 1801 0526 13       		.uleb128 0x13
 1802 0527 01       		.uleb128 0x1
 1803 0528 51       		.byte	0x51
 1804 0529 01       		.uleb128 0x1
 1805 052a 30       		.byte	0x30
 1806 052b 13       		.uleb128 0x13
 1807 052c 01       		.uleb128 0x1
 1808 052d 50       		.byte	0x50
 1809 052e 02       		.uleb128 0x2
 1810 052f 91       		.byte	0x91
 1811 0530 49       		.sleb128 -55
 1812 0531 00       		.byte	0
 1813 0532 18       		.uleb128 0x18
 1814 0533 0E010000 		.4byte	.LVL27
 1815 0537 0B0F0000 		.4byte	0xf0b
 1816 053b 55050000 		.4byte	0x555
 1817 053f 13       		.uleb128 0x13
 1818 0540 01       		.uleb128 0x1
 1819 0541 52       		.byte	0x52
 1820 0542 02       		.uleb128 0x2
 1821 0543 77       		.byte	0x77
 1822 0544 00       		.sleb128 0
 1823 0545 13       		.uleb128 0x13
 1824 0546 01       		.uleb128 0x1
 1825 0547 51       		.byte	0x51
 1826 0548 05       		.uleb128 0x5
 1827 0549 03       		.byte	0x3
 1828 054a 10000000 		.4byte	.LC6
 1829 054e 13       		.uleb128 0x13
 1830 054f 01       		.uleb128 0x1
 1831 0550 50       		.byte	0x50
 1832 0551 02       		.uleb128 0x2
 1833 0552 7D       		.byte	0x7d
 1834 0553 00       		.sleb128 0
 1835 0554 00       		.byte	0
 1836 0555 18       		.uleb128 0x18
 1837 0556 16010000 		.4byte	.LVL28
 1838 055a 270F0000 		.4byte	0xf27
 1839 055e 6F050000 		.4byte	0x56f
 1840 0562 13       		.uleb128 0x13
 1841 0563 01       		.uleb128 0x1
 1842 0564 51       		.byte	0x51
 1843 0565 02       		.uleb128 0x2
 1844 0566 7D       		.byte	0x7d
 1845 0567 00       		.sleb128 0
 1846 0568 13       		.uleb128 0x13
 1847 0569 01       		.uleb128 0x1
 1848 056a 50       		.byte	0x50
 1849 056b 02       		.uleb128 0x2
 1850 056c 74       		.byte	0x74
 1851 056d 6C       		.sleb128 -20
 1852 056e 00       		.byte	0
 1853 056f 18       		.uleb128 0x18
 1854 0570 24010000 		.4byte	.LVL29
 1855 0574 410F0000 		.4byte	0xf41
 1856 0578 8C050000 		.4byte	0x58c
 1857 057c 13       		.uleb128 0x13
ARM GAS  /tmp/ccPZc5IS.s 			page 41


 1858 057d 01       		.uleb128 0x1
 1859 057e 51       		.byte	0x51
 1860 057f 05       		.uleb128 0x5
 1861 0580 03       		.byte	0x3
 1862 0581 1C000000 		.4byte	.LC7
 1863 0585 13       		.uleb128 0x13
 1864 0586 01       		.uleb128 0x1
 1865 0587 50       		.byte	0x50
 1866 0588 02       		.uleb128 0x2
 1867 0589 74       		.byte	0x74
 1868 058a 6C       		.sleb128 -20
 1869 058b 00       		.byte	0
 1870 058c 18       		.uleb128 0x18
 1871 058d 34010000 		.4byte	.LVL31
 1872 0591 910F0000 		.4byte	0xf91
 1873 0595 AF050000 		.4byte	0x5af
 1874 0599 13       		.uleb128 0x13
 1875 059a 01       		.uleb128 0x1
 1876 059b 52       		.byte	0x52
 1877 059c 02       		.uleb128 0x2
 1878 059d 76       		.byte	0x76
 1879 059e 00       		.sleb128 0
 1880 059f 13       		.uleb128 0x13
 1881 05a0 01       		.uleb128 0x1
 1882 05a1 51       		.byte	0x51
 1883 05a2 05       		.uleb128 0x5
 1884 05a3 03       		.byte	0x3
 1885 05a4 0C000000 		.4byte	.LC4
 1886 05a8 13       		.uleb128 0x13
 1887 05a9 01       		.uleb128 0x1
 1888 05aa 50       		.byte	0x50
 1889 05ab 02       		.uleb128 0x2
 1890 05ac 74       		.byte	0x74
 1891 05ad 00       		.sleb128 0
 1892 05ae 00       		.byte	0
 1893 05af 19       		.uleb128 0x19
 1894 05b0 3A010000 		.4byte	.LVL32
 1895 05b4 7C0F0000 		.4byte	0xf7c
 1896 05b8 13       		.uleb128 0x13
 1897 05b9 01       		.uleb128 0x1
 1898 05ba 50       		.byte	0x50
 1899 05bb 02       		.uleb128 0x2
 1900 05bc 74       		.byte	0x74
 1901 05bd 00       		.sleb128 0
 1902 05be 00       		.byte	0
 1903 05bf 00       		.byte	0
 1904 05c0 0B       		.uleb128 0xb
 1905 05c1 C3000000 		.4byte	0xc3
 1906 05c5 D0050000 		.4byte	0x5d0
 1907 05c9 0C       		.uleb128 0xc
 1908 05ca 9E000000 		.4byte	0x9e
 1909 05ce 31       		.byte	0x31
 1910 05cf 00       		.byte	0
 1911 05d0 0B       		.uleb128 0xb
 1912 05d1 C3000000 		.4byte	0xc3
 1913 05d5 E0050000 		.4byte	0x5e0
 1914 05d9 0C       		.uleb128 0xc
ARM GAS  /tmp/ccPZc5IS.s 			page 42


 1915 05da 9E000000 		.4byte	0x9e
 1916 05de 08       		.byte	0x8
 1917 05df 00       		.byte	0
 1918 05e0 14       		.uleb128 0x14
 1919 05e1 02040000 		.4byte	.LASF66
 1920 05e5 01       		.byte	0x1
 1921 05e6 D9       		.byte	0xd9
 1922 05e7 2C000000 		.4byte	0x2c
 1923 05eb 44010000 		.4byte	.LFB30
 1924 05ef 6C000000 		.4byte	.LFE30-.LFB30
 1925 05f3 01       		.uleb128 0x1
 1926 05f4 9C       		.byte	0x9c
 1927 05f5 F3060000 		.4byte	0x6f3
 1928 05f9 15       		.uleb128 0x15
 1929 05fa 57010000 		.4byte	.LASF61
 1930 05fe 01       		.byte	0x1
 1931 05ff D9       		.byte	0xd9
 1932 0600 2C000000 		.4byte	0x2c
 1933 0604 8C010000 		.4byte	.LLST8
 1934 0608 17       		.uleb128 0x17
 1935 0609 76020000 		.4byte	.LASF67
 1936 060d 01       		.byte	0x1
 1937 060e DA       		.byte	0xda
 1938 060f 2C000000 		.4byte	0x2c
 1939 0613 03       		.uleb128 0x3
 1940 0614 91       		.byte	0x91
 1941 0615 AC7F     		.sleb128 -84
 1942 0617 16       		.uleb128 0x16
 1943 0618 B4010000 		.4byte	.LASF63
 1944 061c 01       		.byte	0x1
 1945 061d DB       		.byte	0xdb
 1946 061e 8E040000 		.4byte	0x48e
 1947 0622 B8010000 		.4byte	.LLST9
 1948 0626 17       		.uleb128 0x17
 1949 0627 93040000 		.4byte	.LASF57
 1950 062b 01       		.byte	0x1
 1951 062c DC       		.byte	0xdc
 1952 062d C0050000 		.4byte	0x5c0
 1953 0631 03       		.uleb128 0x3
 1954 0632 91       		.byte	0x91
 1955 0633 BC7F     		.sleb128 -68
 1956 0635 17       		.uleb128 0x17
 1957 0636 CE020000 		.4byte	.LASF64
 1958 063a 01       		.byte	0x1
 1959 063b DD       		.byte	0xdd
 1960 063c D0050000 		.4byte	0x5d0
 1961 0640 03       		.uleb128 0x3
 1962 0641 91       		.byte	0x91
 1963 0642 B07F     		.sleb128 -80
 1964 0644 18       		.uleb128 0x18
 1965 0645 68010000 		.4byte	.LVL36
 1966 0649 EE0E0000 		.4byte	0xeee
 1967 064d 62060000 		.4byte	0x662
 1968 0651 13       		.uleb128 0x13
 1969 0652 01       		.uleb128 0x1
 1970 0653 52       		.byte	0x52
 1971 0654 01       		.uleb128 0x1
ARM GAS  /tmp/ccPZc5IS.s 			page 43


 1972 0655 4D       		.byte	0x4d
 1973 0656 13       		.uleb128 0x13
 1974 0657 01       		.uleb128 0x1
 1975 0658 51       		.byte	0x51
 1976 0659 01       		.uleb128 0x1
 1977 065a 30       		.byte	0x30
 1978 065b 13       		.uleb128 0x13
 1979 065c 01       		.uleb128 0x1
 1980 065d 50       		.byte	0x50
 1981 065e 02       		.uleb128 0x2
 1982 065f 91       		.byte	0x91
 1983 0660 51       		.sleb128 -47
 1984 0661 00       		.byte	0
 1985 0662 18       		.uleb128 0x18
 1986 0663 78010000 		.4byte	.LVL37
 1987 0667 0B0F0000 		.4byte	0xf0b
 1988 066b 86060000 		.4byte	0x686
 1989 066f 13       		.uleb128 0x13
 1990 0670 01       		.uleb128 0x1
 1991 0671 52       		.byte	0x52
 1992 0672 02       		.uleb128 0x2
 1993 0673 76       		.byte	0x76
 1994 0674 00       		.sleb128 0
 1995 0675 13       		.uleb128 0x13
 1996 0676 01       		.uleb128 0x1
 1997 0677 51       		.byte	0x51
 1998 0678 05       		.uleb128 0x5
 1999 0679 03       		.byte	0x3
 2000 067a 10000000 		.4byte	.LC6
 2001 067e 13       		.uleb128 0x13
 2002 067f 01       		.uleb128 0x1
 2003 0680 50       		.byte	0x50
 2004 0681 03       		.uleb128 0x3
 2005 0682 91       		.byte	0x91
 2006 0683 B07F     		.sleb128 -80
 2007 0685 00       		.byte	0
 2008 0686 18       		.uleb128 0x18
 2009 0687 80010000 		.4byte	.LVL38
 2010 068b 270F0000 		.4byte	0xf27
 2011 068f A1060000 		.4byte	0x6a1
 2012 0693 13       		.uleb128 0x13
 2013 0694 01       		.uleb128 0x1
 2014 0695 51       		.byte	0x51
 2015 0696 03       		.uleb128 0x3
 2016 0697 91       		.byte	0x91
 2017 0698 B07F     		.sleb128 -80
 2018 069a 13       		.uleb128 0x13
 2019 069b 01       		.uleb128 0x1
 2020 069c 50       		.byte	0x50
 2021 069d 02       		.uleb128 0x2
 2022 069e 74       		.byte	0x74
 2023 069f 6C       		.sleb128 -20
 2024 06a0 00       		.byte	0
 2025 06a1 18       		.uleb128 0x18
 2026 06a2 8E010000 		.4byte	.LVL39
 2027 06a6 410F0000 		.4byte	0xf41
 2028 06aa BE060000 		.4byte	0x6be
ARM GAS  /tmp/ccPZc5IS.s 			page 44


 2029 06ae 13       		.uleb128 0x13
 2030 06af 01       		.uleb128 0x1
 2031 06b0 51       		.byte	0x51
 2032 06b1 05       		.uleb128 0x5
 2033 06b2 03       		.byte	0x3
 2034 06b3 08000000 		.4byte	.LC3
 2035 06b7 13       		.uleb128 0x13
 2036 06b8 01       		.uleb128 0x1
 2037 06b9 50       		.byte	0x50
 2038 06ba 02       		.uleb128 0x2
 2039 06bb 74       		.byte	0x74
 2040 06bc 6C       		.sleb128 -20
 2041 06bd 00       		.byte	0
 2042 06be 18       		.uleb128 0x18
 2043 06bf 9E010000 		.4byte	.LVL41
 2044 06c3 5C0F0000 		.4byte	0xf5c
 2045 06c7 E2060000 		.4byte	0x6e2
 2046 06cb 13       		.uleb128 0x13
 2047 06cc 01       		.uleb128 0x1
 2048 06cd 52       		.byte	0x52
 2049 06ce 03       		.uleb128 0x3
 2050 06cf 91       		.byte	0x91
 2051 06d0 AC7F     		.sleb128 -84
 2052 06d2 13       		.uleb128 0x13
 2053 06d3 01       		.uleb128 0x1
 2054 06d4 51       		.byte	0x51
 2055 06d5 05       		.uleb128 0x5
 2056 06d6 03       		.byte	0x3
 2057 06d7 0C000000 		.4byte	.LC4
 2058 06db 13       		.uleb128 0x13
 2059 06dc 01       		.uleb128 0x1
 2060 06dd 50       		.byte	0x50
 2061 06de 02       		.uleb128 0x2
 2062 06df 74       		.byte	0x74
 2063 06e0 00       		.sleb128 0
 2064 06e1 00       		.byte	0
 2065 06e2 19       		.uleb128 0x19
 2066 06e3 A4010000 		.4byte	.LVL42
 2067 06e7 7C0F0000 		.4byte	0xf7c
 2068 06eb 13       		.uleb128 0x13
 2069 06ec 01       		.uleb128 0x1
 2070 06ed 50       		.byte	0x50
 2071 06ee 02       		.uleb128 0x2
 2072 06ef 74       		.byte	0x74
 2073 06f0 00       		.sleb128 0
 2074 06f1 00       		.byte	0
 2075 06f2 00       		.byte	0
 2076 06f3 1A       		.uleb128 0x1a
 2077 06f4 38010000 		.4byte	.LASF69
 2078 06f8 01       		.byte	0x1
 2079 06f9 E6       		.byte	0xe6
 2080 06fa B0010000 		.4byte	.LFB31
 2081 06fe 8C000000 		.4byte	.LFE31-.LFB31
 2082 0702 01       		.uleb128 0x1
 2083 0703 9C       		.byte	0x9c
 2084 0704 40080000 		.4byte	0x840
 2085 0708 15       		.uleb128 0x15
ARM GAS  /tmp/ccPZc5IS.s 			page 45


 2086 0709 57010000 		.4byte	.LASF61
 2087 070d 01       		.byte	0x1
 2088 070e E6       		.byte	0xe6
 2089 070f 2C000000 		.4byte	0x2c
 2090 0713 D6010000 		.4byte	.LLST10
 2091 0717 15       		.uleb128 0x15
 2092 0718 45000000 		.4byte	.LASF70
 2093 071c 01       		.byte	0x1
 2094 071d E6       		.byte	0xe6
 2095 071e 2C000000 		.4byte	0x2c
 2096 0722 02020000 		.4byte	.LLST11
 2097 0726 16       		.uleb128 0x16
 2098 0727 B4010000 		.4byte	.LASF63
 2099 072b 01       		.byte	0x1
 2100 072c E7       		.byte	0xe7
 2101 072d 8E040000 		.4byte	0x48e
 2102 0731 2E020000 		.4byte	.LLST12
 2103 0735 17       		.uleb128 0x17
 2104 0736 93040000 		.4byte	.LASF57
 2105 073a 01       		.byte	0x1
 2106 073b E8       		.byte	0xe8
 2107 073c C0050000 		.4byte	0x5c0
 2108 0740 03       		.uleb128 0x3
 2109 0741 91       		.byte	0x91
 2110 0742 B47F     		.sleb128 -76
 2111 0744 17       		.uleb128 0x17
 2112 0745 CE020000 		.4byte	.LASF64
 2113 0749 01       		.byte	0x1
 2114 074a E9       		.byte	0xe9
 2115 074b 40080000 		.4byte	0x840
 2116 074f 03       		.uleb128 0x3
 2117 0750 91       		.byte	0x91
 2118 0751 A47F     		.sleb128 -92
 2119 0753 18       		.uleb128 0x18
 2120 0754 D6010000 		.4byte	.LVL46
 2121 0758 EE0E0000 		.4byte	0xeee
 2122 075c 71070000 		.4byte	0x771
 2123 0760 13       		.uleb128 0x13
 2124 0761 01       		.uleb128 0x1
 2125 0762 52       		.byte	0x52
 2126 0763 01       		.uleb128 0x1
 2127 0764 4D       		.byte	0x4d
 2128 0765 13       		.uleb128 0x13
 2129 0766 01       		.uleb128 0x1
 2130 0767 51       		.byte	0x51
 2131 0768 01       		.uleb128 0x1
 2132 0769 30       		.byte	0x30
 2133 076a 13       		.uleb128 0x13
 2134 076b 01       		.uleb128 0x1
 2135 076c 50       		.byte	0x50
 2136 076d 02       		.uleb128 0x2
 2137 076e 91       		.byte	0x91
 2138 076f 49       		.sleb128 -55
 2139 0770 00       		.byte	0
 2140 0771 18       		.uleb128 0x18
 2141 0772 E6010000 		.4byte	.LVL47
 2142 0776 0B0F0000 		.4byte	0xf0b
ARM GAS  /tmp/ccPZc5IS.s 			page 46


 2143 077a 95070000 		.4byte	0x795
 2144 077e 13       		.uleb128 0x13
 2145 077f 01       		.uleb128 0x1
 2146 0780 52       		.byte	0x52
 2147 0781 02       		.uleb128 0x2
 2148 0782 77       		.byte	0x77
 2149 0783 00       		.sleb128 0
 2150 0784 13       		.uleb128 0x13
 2151 0785 01       		.uleb128 0x1
 2152 0786 51       		.byte	0x51
 2153 0787 05       		.uleb128 0x5
 2154 0788 03       		.byte	0x3
 2155 0789 20000000 		.4byte	.LC8
 2156 078d 13       		.uleb128 0x13
 2157 078e 01       		.uleb128 0x1
 2158 078f 50       		.byte	0x50
 2159 0790 03       		.uleb128 0x3
 2160 0791 91       		.byte	0x91
 2161 0792 A47F     		.sleb128 -92
 2162 0794 00       		.byte	0
 2163 0795 18       		.uleb128 0x18
 2164 0796 EE010000 		.4byte	.LVL48
 2165 079a 270F0000 		.4byte	0xf27
 2166 079e B0070000 		.4byte	0x7b0
 2167 07a2 13       		.uleb128 0x13
 2168 07a3 01       		.uleb128 0x1
 2169 07a4 51       		.byte	0x51
 2170 07a5 03       		.uleb128 0x3
 2171 07a6 91       		.byte	0x91
 2172 07a7 A47F     		.sleb128 -92
 2173 07a9 13       		.uleb128 0x13
 2174 07aa 01       		.uleb128 0x1
 2175 07ab 50       		.byte	0x50
 2176 07ac 02       		.uleb128 0x2
 2177 07ad 74       		.byte	0x74
 2178 07ae 6C       		.sleb128 -20
 2179 07af 00       		.byte	0
 2180 07b0 18       		.uleb128 0x18
 2181 07b1 FC010000 		.4byte	.LVL49
 2182 07b5 410F0000 		.4byte	0xf41
 2183 07b9 CD070000 		.4byte	0x7cd
 2184 07bd 13       		.uleb128 0x13
 2185 07be 01       		.uleb128 0x1
 2186 07bf 51       		.byte	0x51
 2187 07c0 05       		.uleb128 0x5
 2188 07c1 03       		.byte	0x3
 2189 07c2 1C000000 		.4byte	.LC7
 2190 07c6 13       		.uleb128 0x13
 2191 07c7 01       		.uleb128 0x1
 2192 07c8 50       		.byte	0x50
 2193 07c9 02       		.uleb128 0x2
 2194 07ca 74       		.byte	0x74
 2195 07cb 6C       		.sleb128 -20
 2196 07cc 00       		.byte	0
 2197 07cd 18       		.uleb128 0x18
 2198 07ce 12020000 		.4byte	.LVL52
 2199 07d2 AD0F0000 		.4byte	0xfad
ARM GAS  /tmp/ccPZc5IS.s 			page 47


 2200 07d6 F4070000 		.4byte	0x7f4
 2201 07da 13       		.uleb128 0x13
 2202 07db 01       		.uleb128 0x1
 2203 07dc 53       		.byte	0x53
 2204 07dd 02       		.uleb128 0x2
 2205 07de 74       		.byte	0x74
 2206 07df 00       		.sleb128 0
 2207 07e0 13       		.uleb128 0x13
 2208 07e1 01       		.uleb128 0x1
 2209 07e2 52       		.byte	0x52
 2210 07e3 01       		.uleb128 0x1
 2211 07e4 33       		.byte	0x33
 2212 07e5 13       		.uleb128 0x13
 2213 07e6 01       		.uleb128 0x1
 2214 07e7 51       		.byte	0x51
 2215 07e8 01       		.uleb128 0x1
 2216 07e9 31       		.byte	0x31
 2217 07ea 13       		.uleb128 0x13
 2218 07eb 01       		.uleb128 0x1
 2219 07ec 50       		.byte	0x50
 2220 07ed 05       		.uleb128 0x5
 2221 07ee 03       		.byte	0x3
 2222 07ef 34000000 		.4byte	.LC10
 2223 07f3 00       		.byte	0
 2224 07f4 18       		.uleb128 0x18
 2225 07f5 18020000 		.4byte	.LVL53
 2226 07f9 7C0F0000 		.4byte	0xf7c
 2227 07fd 08080000 		.4byte	0x808
 2228 0801 13       		.uleb128 0x13
 2229 0802 01       		.uleb128 0x1
 2230 0803 50       		.byte	0x50
 2231 0804 02       		.uleb128 0x2
 2232 0805 74       		.byte	0x74
 2233 0806 00       		.sleb128 0
 2234 0807 00       		.byte	0
 2235 0808 18       		.uleb128 0x18
 2236 0809 2E020000 		.4byte	.LVL56
 2237 080d AD0F0000 		.4byte	0xfad
 2238 0811 2F080000 		.4byte	0x82f
 2239 0815 13       		.uleb128 0x13
 2240 0816 01       		.uleb128 0x1
 2241 0817 53       		.byte	0x53
 2242 0818 02       		.uleb128 0x2
 2243 0819 74       		.byte	0x74
 2244 081a 00       		.sleb128 0
 2245 081b 13       		.uleb128 0x13
 2246 081c 01       		.uleb128 0x1
 2247 081d 52       		.byte	0x52
 2248 081e 01       		.uleb128 0x1
 2249 081f 32       		.byte	0x32
 2250 0820 13       		.uleb128 0x13
 2251 0821 01       		.uleb128 0x1
 2252 0822 51       		.byte	0x51
 2253 0823 01       		.uleb128 0x1
 2254 0824 31       		.byte	0x31
 2255 0825 13       		.uleb128 0x13
 2256 0826 01       		.uleb128 0x1
ARM GAS  /tmp/ccPZc5IS.s 			page 48


 2257 0827 50       		.byte	0x50
 2258 0828 05       		.uleb128 0x5
 2259 0829 03       		.byte	0x3
 2260 082a 30000000 		.4byte	.LC9
 2261 082e 00       		.byte	0
 2262 082f 19       		.uleb128 0x19
 2263 0830 34020000 		.4byte	.LVL57
 2264 0834 7C0F0000 		.4byte	0xf7c
 2265 0838 13       		.uleb128 0x13
 2266 0839 01       		.uleb128 0x1
 2267 083a 50       		.byte	0x50
 2268 083b 02       		.uleb128 0x2
 2269 083c 74       		.byte	0x74
 2270 083d 00       		.sleb128 0
 2271 083e 00       		.byte	0
 2272 083f 00       		.byte	0
 2273 0840 0B       		.uleb128 0xb
 2274 0841 C3000000 		.4byte	0xc3
 2275 0845 50080000 		.4byte	0x850
 2276 0849 0C       		.uleb128 0xc
 2277 084a 9E000000 		.4byte	0x9e
 2278 084e 0E       		.byte	0xe
 2279 084f 00       		.byte	0
 2280 0850 14       		.uleb128 0x14
 2281 0851 E3020000 		.4byte	.LASF71
 2282 0855 01       		.byte	0x1
 2283 0856 6C       		.byte	0x6c
 2284 0857 2C000000 		.4byte	0x2c
 2285 085b 3C020000 		.4byte	.LFB24
 2286 085f B6000000 		.4byte	.LFE24-.LFB24
 2287 0863 01       		.uleb128 0x1
 2288 0864 9C       		.byte	0x9c
 2289 0865 810A0000 		.4byte	0xa81
 2290 0869 15       		.uleb128 0x15
 2291 086a 6E040000 		.4byte	.LASF72
 2292 086e 01       		.byte	0x1
 2293 086f 6C       		.byte	0x6c
 2294 0870 2C000000 		.4byte	0x2c
 2295 0874 62020000 		.4byte	.LLST13
 2296 0878 15       		.uleb128 0x15
 2297 0879 81000000 		.4byte	.LASF73
 2298 087d 01       		.byte	0x1
 2299 087e 6C       		.byte	0x6c
 2300 087f 810A0000 		.4byte	0xa81
 2301 0883 80020000 		.4byte	.LLST14
 2302 0887 16       		.uleb128 0x16
 2303 0888 54040000 		.4byte	.LASF74
 2304 088c 01       		.byte	0x1
 2305 088d 78       		.byte	0x78
 2306 088e 2C000000 		.4byte	0x2c
 2307 0892 9E020000 		.4byte	.LLST15
 2308 0896 1B       		.uleb128 0x1b
 2309 0897 EF020000 		.4byte	0x2ef
 2310 089b 90020000 		.4byte	.LBB4
 2311 089f 00000000 		.4byte	.Ldebug_ranges0+0
 2312 08a3 01       		.byte	0x1
 2313 08a4 78       		.byte	0x78
ARM GAS  /tmp/ccPZc5IS.s 			page 49


 2314 08a5 BC080000 		.4byte	0x8bc
 2315 08a9 11       		.uleb128 0x11
 2316 08aa FF020000 		.4byte	0x2ff
 2317 08ae BC020000 		.4byte	.LLST16
 2318 08b2 1C       		.uleb128 0x1c
 2319 08b3 98020000 		.4byte	.LVL72
 2320 08b7 0A030000 		.4byte	0x30a
 2321 08bb 00       		.byte	0
 2322 08bc 18       		.uleb128 0x18
 2323 08bd 4A020000 		.4byte	.LVL62
 2324 08c1 F3060000 		.4byte	0x6f3
 2325 08c5 D4080000 		.4byte	0x8d4
 2326 08c9 13       		.uleb128 0x13
 2327 08ca 01       		.uleb128 0x1
 2328 08cb 51       		.byte	0x51
 2329 08cc 01       		.uleb128 0x1
 2330 08cd 31       		.byte	0x31
 2331 08ce 13       		.uleb128 0x13
 2332 08cf 01       		.uleb128 0x1
 2333 08d0 50       		.byte	0x50
 2334 08d1 01       		.uleb128 0x1
 2335 08d2 3F       		.byte	0x3f
 2336 08d3 00       		.byte	0
 2337 08d4 18       		.uleb128 0x18
 2338 08d5 52020000 		.4byte	.LVL63
 2339 08d9 F3060000 		.4byte	0x6f3
 2340 08dd ED080000 		.4byte	0x8ed
 2341 08e1 13       		.uleb128 0x13
 2342 08e2 01       		.uleb128 0x1
 2343 08e3 51       		.byte	0x51
 2344 08e4 01       		.uleb128 0x1
 2345 08e5 31       		.byte	0x31
 2346 08e6 13       		.uleb128 0x13
 2347 08e7 01       		.uleb128 0x1
 2348 08e8 50       		.byte	0x50
 2349 08e9 02       		.uleb128 0x2
 2350 08ea 08       		.byte	0x8
 2351 08eb 73       		.byte	0x73
 2352 08ec 00       		.byte	0
 2353 08ed 18       		.uleb128 0x18
 2354 08ee 5A020000 		.4byte	.LVL64
 2355 08f2 F3060000 		.4byte	0x6f3
 2356 08f6 05090000 		.4byte	0x905
 2357 08fa 13       		.uleb128 0x13
 2358 08fb 01       		.uleb128 0x1
 2359 08fc 51       		.byte	0x51
 2360 08fd 01       		.uleb128 0x1
 2361 08fe 31       		.byte	0x31
 2362 08ff 13       		.uleb128 0x13
 2363 0900 01       		.uleb128 0x1
 2364 0901 50       		.byte	0x50
 2365 0902 01       		.uleb128 0x1
 2366 0903 32       		.byte	0x32
 2367 0904 00       		.byte	0
 2368 0905 18       		.uleb128 0x18
 2369 0906 62020000 		.4byte	.LVL65
 2370 090a F3060000 		.4byte	0x6f3
ARM GAS  /tmp/ccPZc5IS.s 			page 50


 2371 090e 1E090000 		.4byte	0x91e
 2372 0912 13       		.uleb128 0x13
 2373 0913 01       		.uleb128 0x1
 2374 0914 51       		.byte	0x51
 2375 0915 01       		.uleb128 0x1
 2376 0916 31       		.byte	0x31
 2377 0917 13       		.uleb128 0x13
 2378 0918 01       		.uleb128 0x1
 2379 0919 50       		.byte	0x50
 2380 091a 02       		.uleb128 0x2
 2381 091b 08       		.byte	0x8
 2382 091c 31       		.byte	0x31
 2383 091d 00       		.byte	0
 2384 091e 18       		.uleb128 0x18
 2385 091f 6A020000 		.4byte	.LVL66
 2386 0923 F3060000 		.4byte	0x6f3
 2387 0927 36090000 		.4byte	0x936
 2388 092b 13       		.uleb128 0x13
 2389 092c 01       		.uleb128 0x1
 2390 092d 51       		.byte	0x51
 2391 092e 01       		.uleb128 0x1
 2392 092f 31       		.byte	0x31
 2393 0930 13       		.uleb128 0x13
 2394 0931 01       		.uleb128 0x1
 2395 0932 50       		.byte	0x50
 2396 0933 01       		.uleb128 0x1
 2397 0934 33       		.byte	0x33
 2398 0935 00       		.byte	0
 2399 0936 18       		.uleb128 0x18
 2400 0937 72020000 		.4byte	.LVL67
 2401 093b F3060000 		.4byte	0x6f3
 2402 093f 4E090000 		.4byte	0x94e
 2403 0943 13       		.uleb128 0x13
 2404 0944 01       		.uleb128 0x1
 2405 0945 51       		.byte	0x51
 2406 0946 01       		.uleb128 0x1
 2407 0947 31       		.byte	0x31
 2408 0948 13       		.uleb128 0x13
 2409 0949 01       		.uleb128 0x1
 2410 094a 50       		.byte	0x50
 2411 094b 01       		.uleb128 0x1
 2412 094c 3E       		.byte	0x3e
 2413 094d 00       		.byte	0
 2414 094e 18       		.uleb128 0x18
 2415 094f 7A020000 		.4byte	.LVL68
 2416 0953 F3060000 		.4byte	0x6f3
 2417 0957 67090000 		.4byte	0x967
 2418 095b 13       		.uleb128 0x13
 2419 095c 01       		.uleb128 0x1
 2420 095d 51       		.byte	0x51
 2421 095e 01       		.uleb128 0x1
 2422 095f 30       		.byte	0x30
 2423 0960 13       		.uleb128 0x13
 2424 0961 01       		.uleb128 0x1
 2425 0962 50       		.byte	0x50
 2426 0963 02       		.uleb128 0x2
 2427 0964 74       		.byte	0x74
ARM GAS  /tmp/ccPZc5IS.s 			page 51


 2428 0965 00       		.sleb128 0
 2429 0966 00       		.byte	0
 2430 0967 18       		.uleb128 0x18
 2431 0968 82020000 		.4byte	.LVL69
 2432 096c B4040000 		.4byte	0x4b4
 2433 0970 80090000 		.4byte	0x980
 2434 0974 13       		.uleb128 0x13
 2435 0975 01       		.uleb128 0x1
 2436 0976 51       		.byte	0x51
 2437 0977 01       		.uleb128 0x1
 2438 0978 31       		.byte	0x31
 2439 0979 13       		.uleb128 0x13
 2440 097a 01       		.uleb128 0x1
 2441 097b 50       		.byte	0x50
 2442 097c 02       		.uleb128 0x2
 2443 097d 74       		.byte	0x74
 2444 097e 00       		.sleb128 0
 2445 097f 00       		.byte	0
 2446 0980 18       		.uleb128 0x18
 2447 0981 8A020000 		.4byte	.LVL70
 2448 0985 D50F0000 		.4byte	0xfd5
 2449 0989 99090000 		.4byte	0x999
 2450 098d 13       		.uleb128 0x13
 2451 098e 01       		.uleb128 0x1
 2452 098f 51       		.byte	0x51
 2453 0990 01       		.uleb128 0x1
 2454 0991 30       		.byte	0x30
 2455 0992 13       		.uleb128 0x13
 2456 0993 01       		.uleb128 0x1
 2457 0994 50       		.byte	0x50
 2458 0995 02       		.uleb128 0x2
 2459 0996 76       		.byte	0x76
 2460 0997 00       		.sleb128 0
 2461 0998 00       		.byte	0
 2462 0999 18       		.uleb128 0x18
 2463 099a 90020000 		.4byte	.LVL71
 2464 099e 55030000 		.4byte	0x355
 2465 09a2 AC090000 		.4byte	0x9ac
 2466 09a6 13       		.uleb128 0x13
 2467 09a7 01       		.uleb128 0x1
 2468 09a8 50       		.byte	0x50
 2469 09a9 01       		.uleb128 0x1
 2470 09aa 30       		.byte	0x30
 2471 09ab 00       		.byte	0
 2472 09ac 18       		.uleb128 0x18
 2473 09ad A2020000 		.4byte	.LVL73
 2474 09b1 B4040000 		.4byte	0x4b4
 2475 09b5 C5090000 		.4byte	0x9c5
 2476 09b9 13       		.uleb128 0x13
 2477 09ba 01       		.uleb128 0x1
 2478 09bb 51       		.byte	0x51
 2479 09bc 01       		.uleb128 0x1
 2480 09bd 30       		.byte	0x30
 2481 09be 13       		.uleb128 0x13
 2482 09bf 01       		.uleb128 0x1
 2483 09c0 50       		.byte	0x50
 2484 09c1 02       		.uleb128 0x2
ARM GAS  /tmp/ccPZc5IS.s 			page 52


 2485 09c2 74       		.byte	0x74
 2486 09c3 00       		.sleb128 0
 2487 09c4 00       		.byte	0
 2488 09c5 18       		.uleb128 0x18
 2489 09c6 AC020000 		.4byte	.LVL75
 2490 09ca F3060000 		.4byte	0x6f3
 2491 09ce DD090000 		.4byte	0x9dd
 2492 09d2 13       		.uleb128 0x13
 2493 09d3 01       		.uleb128 0x1
 2494 09d4 51       		.byte	0x51
 2495 09d5 01       		.uleb128 0x1
 2496 09d6 31       		.byte	0x31
 2497 09d7 13       		.uleb128 0x13
 2498 09d8 01       		.uleb128 0x1
 2499 09d9 50       		.byte	0x50
 2500 09da 01       		.uleb128 0x1
 2501 09db 3E       		.byte	0x3e
 2502 09dc 00       		.byte	0
 2503 09dd 18       		.uleb128 0x18
 2504 09de B8020000 		.4byte	.LVL78
 2505 09e2 B4040000 		.4byte	0x4b4
 2506 09e6 F6090000 		.4byte	0x9f6
 2507 09ea 13       		.uleb128 0x13
 2508 09eb 01       		.uleb128 0x1
 2509 09ec 51       		.byte	0x51
 2510 09ed 01       		.uleb128 0x1
 2511 09ee 30       		.byte	0x30
 2512 09ef 13       		.uleb128 0x13
 2513 09f0 01       		.uleb128 0x1
 2514 09f1 50       		.byte	0x50
 2515 09f2 02       		.uleb128 0x2
 2516 09f3 74       		.byte	0x74
 2517 09f4 00       		.sleb128 0
 2518 09f5 00       		.byte	0
 2519 09f6 18       		.uleb128 0x18
 2520 09f7 C0020000 		.4byte	.LVL79
 2521 09fb F3060000 		.4byte	0x6f3
 2522 09ff 0F0A0000 		.4byte	0xa0f
 2523 0a03 13       		.uleb128 0x13
 2524 0a04 01       		.uleb128 0x1
 2525 0a05 51       		.byte	0x51
 2526 0a06 01       		.uleb128 0x1
 2527 0a07 31       		.byte	0x31
 2528 0a08 13       		.uleb128 0x13
 2529 0a09 01       		.uleb128 0x1
 2530 0a0a 50       		.byte	0x50
 2531 0a0b 02       		.uleb128 0x2
 2532 0a0c 74       		.byte	0x74
 2533 0a0d 00       		.sleb128 0
 2534 0a0e 00       		.byte	0
 2535 0a0f 18       		.uleb128 0x18
 2536 0a10 C8020000 		.4byte	.LVL80
 2537 0a14 F3060000 		.4byte	0x6f3
 2538 0a18 270A0000 		.4byte	0xa27
 2539 0a1c 13       		.uleb128 0x13
 2540 0a1d 01       		.uleb128 0x1
 2541 0a1e 51       		.byte	0x51
ARM GAS  /tmp/ccPZc5IS.s 			page 53


 2542 0a1f 01       		.uleb128 0x1
 2543 0a20 30       		.byte	0x30
 2544 0a21 13       		.uleb128 0x13
 2545 0a22 01       		.uleb128 0x1
 2546 0a23 50       		.byte	0x50
 2547 0a24 01       		.uleb128 0x1
 2548 0a25 3E       		.byte	0x3e
 2549 0a26 00       		.byte	0
 2550 0a27 18       		.uleb128 0x18
 2551 0a28 D0020000 		.4byte	.LVL81
 2552 0a2c B4040000 		.4byte	0x4b4
 2553 0a30 3F0A0000 		.4byte	0xa3f
 2554 0a34 13       		.uleb128 0x13
 2555 0a35 01       		.uleb128 0x1
 2556 0a36 51       		.byte	0x51
 2557 0a37 01       		.uleb128 0x1
 2558 0a38 31       		.byte	0x31
 2559 0a39 13       		.uleb128 0x13
 2560 0a3a 01       		.uleb128 0x1
 2561 0a3b 50       		.byte	0x50
 2562 0a3c 01       		.uleb128 0x1
 2563 0a3d 3E       		.byte	0x3e
 2564 0a3e 00       		.byte	0
 2565 0a3f 18       		.uleb128 0x18
 2566 0a40 D8020000 		.4byte	.LVL82
 2567 0a44 D50F0000 		.4byte	0xfd5
 2568 0a48 580A0000 		.4byte	0xa58
 2569 0a4c 13       		.uleb128 0x13
 2570 0a4d 01       		.uleb128 0x1
 2571 0a4e 51       		.byte	0x51
 2572 0a4f 01       		.uleb128 0x1
 2573 0a50 30       		.byte	0x30
 2574 0a51 13       		.uleb128 0x13
 2575 0a52 01       		.uleb128 0x1
 2576 0a53 50       		.byte	0x50
 2577 0a54 02       		.uleb128 0x2
 2578 0a55 76       		.byte	0x76
 2579 0a56 00       		.sleb128 0
 2580 0a57 00       		.byte	0
 2581 0a58 18       		.uleb128 0x18
 2582 0a59 DE020000 		.4byte	.LVL83
 2583 0a5d E0050000 		.4byte	0x5e0
 2584 0a61 6C0A0000 		.4byte	0xa6c
 2585 0a65 13       		.uleb128 0x13
 2586 0a66 01       		.uleb128 0x1
 2587 0a67 50       		.byte	0x50
 2588 0a68 02       		.uleb128 0x2
 2589 0a69 74       		.byte	0x74
 2590 0a6a 00       		.sleb128 0
 2591 0a6b 00       		.byte	0
 2592 0a6c 19       		.uleb128 0x19
 2593 0a6d F0020000 		.4byte	.LVL85
 2594 0a71 B4040000 		.4byte	0x4b4
 2595 0a75 13       		.uleb128 0x13
 2596 0a76 01       		.uleb128 0x1
 2597 0a77 51       		.byte	0x51
 2598 0a78 01       		.uleb128 0x1
ARM GAS  /tmp/ccPZc5IS.s 			page 54


 2599 0a79 30       		.byte	0x30
 2600 0a7a 13       		.uleb128 0x13
 2601 0a7b 01       		.uleb128 0x1
 2602 0a7c 50       		.byte	0x50
 2603 0a7d 01       		.uleb128 0x1
 2604 0a7e 3E       		.byte	0x3e
 2605 0a7f 00       		.byte	0
 2606 0a80 00       		.byte	0
 2607 0a81 06       		.uleb128 0x6
 2608 0a82 04       		.byte	0x4
 2609 0a83 CA020000 		.4byte	0x2ca
 2610 0a87 14       		.uleb128 0x14
 2611 0a88 59020000 		.4byte	.LASF75
 2612 0a8c 01       		.byte	0x1
 2613 0a8d AD       		.byte	0xad
 2614 0a8e 2C000000 		.4byte	0x2c
 2615 0a92 F4020000 		.4byte	.LFB27
 2616 0a96 34000000 		.4byte	.LFE27-.LFB27
 2617 0a9a 01       		.uleb128 0x1
 2618 0a9b 9C       		.byte	0x9c
 2619 0a9c 420B0000 		.4byte	0xb42
 2620 0aa0 15       		.uleb128 0x15
 2621 0aa1 D7020000 		.4byte	.LASF76
 2622 0aa5 01       		.byte	0x1
 2623 0aa6 AD       		.byte	0xad
 2624 0aa7 2C000000 		.4byte	0x2c
 2625 0aab DA020000 		.4byte	.LLST17
 2626 0aaf 15       		.uleb128 0x15
 2627 0ab0 DD020000 		.4byte	.LASF77
 2628 0ab4 01       		.byte	0x1
 2629 0ab5 AD       		.byte	0xad
 2630 0ab6 2C000000 		.4byte	0x2c
 2631 0aba F8020000 		.4byte	.LLST18
 2632 0abe 16       		.uleb128 0x16
 2633 0abf 81020000 		.4byte	.LASF78
 2634 0ac3 01       		.byte	0x1
 2635 0ac4 AE       		.byte	0xae
 2636 0ac5 2C000000 		.4byte	0x2c
 2637 0ac9 24030000 		.4byte	.LLST19
 2638 0acd 18       		.uleb128 0x18
 2639 0ace 00030000 		.4byte	.LVL88
 2640 0ad2 F3060000 		.4byte	0x6f3
 2641 0ad6 E60A0000 		.4byte	0xae6
 2642 0ada 13       		.uleb128 0x13
 2643 0adb 01       		.uleb128 0x1
 2644 0adc 51       		.byte	0x51
 2645 0add 01       		.uleb128 0x1
 2646 0ade 30       		.byte	0x30
 2647 0adf 13       		.uleb128 0x13
 2648 0ae0 01       		.uleb128 0x1
 2649 0ae1 50       		.byte	0x50
 2650 0ae2 02       		.uleb128 0x2
 2651 0ae3 74       		.byte	0x74
 2652 0ae4 00       		.sleb128 0
 2653 0ae5 00       		.byte	0
 2654 0ae6 18       		.uleb128 0x18
 2655 0ae7 08030000 		.4byte	.LVL89
ARM GAS  /tmp/ccPZc5IS.s 			page 55


 2656 0aeb B4040000 		.4byte	0x4b4
 2657 0aef FF0A0000 		.4byte	0xaff
 2658 0af3 13       		.uleb128 0x13
 2659 0af4 01       		.uleb128 0x1
 2660 0af5 51       		.byte	0x51
 2661 0af6 01       		.uleb128 0x1
 2662 0af7 31       		.byte	0x31
 2663 0af8 13       		.uleb128 0x13
 2664 0af9 01       		.uleb128 0x1
 2665 0afa 50       		.byte	0x50
 2666 0afb 02       		.uleb128 0x2
 2667 0afc 74       		.byte	0x74
 2668 0afd 00       		.sleb128 0
 2669 0afe 00       		.byte	0
 2670 0aff 18       		.uleb128 0x18
 2671 0b00 0E030000 		.4byte	.LVL90
 2672 0b04 E0050000 		.4byte	0x5e0
 2673 0b08 130B0000 		.4byte	0xb13
 2674 0b0c 13       		.uleb128 0x13
 2675 0b0d 01       		.uleb128 0x1
 2676 0b0e 50       		.byte	0x50
 2677 0b0f 02       		.uleb128 0x2
 2678 0b10 75       		.byte	0x75
 2679 0b11 00       		.sleb128 0
 2680 0b12 00       		.byte	0
 2681 0b13 18       		.uleb128 0x18
 2682 0b14 18030000 		.4byte	.LVL93
 2683 0b18 B4040000 		.4byte	0x4b4
 2684 0b1c 2C0B0000 		.4byte	0xb2c
 2685 0b20 13       		.uleb128 0x13
 2686 0b21 01       		.uleb128 0x1
 2687 0b22 51       		.byte	0x51
 2688 0b23 01       		.uleb128 0x1
 2689 0b24 30       		.byte	0x30
 2690 0b25 13       		.uleb128 0x13
 2691 0b26 01       		.uleb128 0x1
 2692 0b27 50       		.byte	0x50
 2693 0b28 02       		.uleb128 0x2
 2694 0b29 74       		.byte	0x74
 2695 0b2a 00       		.sleb128 0
 2696 0b2b 00       		.byte	0
 2697 0b2c 19       		.uleb128 0x19
 2698 0b2d 20030000 		.4byte	.LVL94
 2699 0b31 F3060000 		.4byte	0x6f3
 2700 0b35 13       		.uleb128 0x13
 2701 0b36 01       		.uleb128 0x1
 2702 0b37 51       		.byte	0x51
 2703 0b38 01       		.uleb128 0x1
 2704 0b39 31       		.byte	0x31
 2705 0b3a 13       		.uleb128 0x13
 2706 0b3b 01       		.uleb128 0x1
 2707 0b3c 50       		.byte	0x50
 2708 0b3d 02       		.uleb128 0x2
 2709 0b3e 74       		.byte	0x74
 2710 0b3f 00       		.sleb128 0
 2711 0b40 00       		.byte	0
 2712 0b41 00       		.byte	0
ARM GAS  /tmp/ccPZc5IS.s 			page 56


 2713 0b42 14       		.uleb128 0x14
 2714 0b43 9E000000 		.4byte	.LASF79
 2715 0b47 01       		.byte	0x1
 2716 0b48 90       		.byte	0x90
 2717 0b49 2C000000 		.4byte	0x2c
 2718 0b4d 28030000 		.4byte	.LFB26
 2719 0b51 A4000000 		.4byte	.LFE26-.LFB26
 2720 0b55 01       		.uleb128 0x1
 2721 0b56 9C       		.byte	0x9c
 2722 0b57 DD0B0000 		.4byte	0xbdd
 2723 0b5b 15       		.uleb128 0x15
 2724 0b5c 4E020000 		.4byte	.LASF80
 2725 0b60 01       		.byte	0x1
 2726 0b61 90       		.byte	0x90
 2727 0b62 DD0B0000 		.4byte	0xbdd
 2728 0b66 5C030000 		.4byte	.LLST20
 2729 0b6a 1D       		.uleb128 0x1d
 2730 0b6b 6900     		.ascii	"i\000"
 2731 0b6d 01       		.byte	0x1
 2732 0b6e 91       		.byte	0x91
 2733 0b6f 2C000000 		.4byte	0x2c
 2734 0b73 88030000 		.4byte	.LLST21
 2735 0b77 1D       		.uleb128 0x1d
 2736 0b78 6A00     		.ascii	"j\000"
 2737 0b7a 01       		.byte	0x1
 2738 0b7b 91       		.byte	0x91
 2739 0b7c 2C000000 		.4byte	0x2c
 2740 0b80 9B030000 		.4byte	.LLST22
 2741 0b84 16       		.uleb128 0x16
 2742 0b85 94010000 		.4byte	.LASF81
 2743 0b89 01       		.byte	0x1
 2744 0b8a 92       		.byte	0x92
 2745 0b8b 2C000000 		.4byte	0x2c
 2746 0b8f BA030000 		.4byte	.LLST23
 2747 0b93 16       		.uleb128 0x16
 2748 0b94 2D000000 		.4byte	.LASF82
 2749 0b98 01       		.byte	0x1
 2750 0b99 93       		.byte	0x93
 2751 0b9a 2C000000 		.4byte	0x2c
 2752 0b9e D9030000 		.4byte	.LLST24
 2753 0ba2 17       		.uleb128 0x17
 2754 0ba3 E0000000 		.4byte	.LASF83
 2755 0ba7 01       		.byte	0x1
 2756 0ba8 94       		.byte	0x94
 2757 0ba9 E30B0000 		.4byte	0xbe3
 2758 0bad 02       		.uleb128 0x2
 2759 0bae 91       		.byte	0x91
 2760 0baf 4C       		.sleb128 -52
 2761 0bb0 1E       		.uleb128 0x1e
 2762 0bb1 40040000 		.4byte	.LASF84
 2763 0bb5 01       		.byte	0x1
 2764 0bb6 95       		.byte	0x95
 2765 0bb7 E30B0000 		.4byte	0xbe3
 2766 0bbb 1F       		.uleb128 0x1f
 2767 0bbc 18000000 		.4byte	.Ldebug_ranges0+0x18
 2768 0bc0 20       		.uleb128 0x20
 2769 0bc1 7800     		.ascii	"x\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 57


 2770 0bc3 01       		.byte	0x1
 2771 0bc4 97       		.byte	0x97
 2772 0bc5 2C000000 		.4byte	0x2c
 2773 0bc9 20       		.uleb128 0x20
 2774 0bca 7900     		.ascii	"y\000"
 2775 0bcc 01       		.byte	0x1
 2776 0bcd 98       		.byte	0x98
 2777 0bce 2C000000 		.4byte	0x2c
 2778 0bd2 1C       		.uleb128 0x1c
 2779 0bd3 9C030000 		.4byte	.LVL103
 2780 0bd7 870A0000 		.4byte	0xa87
 2781 0bdb 00       		.byte	0
 2782 0bdc 00       		.byte	0
 2783 0bdd 06       		.uleb128 0x6
 2784 0bde 04       		.byte	0x4
 2785 0bdf 2C000000 		.4byte	0x2c
 2786 0be3 0B       		.uleb128 0xb
 2787 0be4 2C000000 		.4byte	0x2c
 2788 0be8 F30B0000 		.4byte	0xbf3
 2789 0bec 0C       		.uleb128 0xc
 2790 0bed 9E000000 		.4byte	0x9e
 2791 0bf1 04       		.byte	0x4
 2792 0bf2 00       		.byte	0
 2793 0bf3 14       		.uleb128 0x14
 2794 0bf4 7F040000 		.4byte	.LASF85
 2795 0bf8 01       		.byte	0x1
 2796 0bf9 17       		.byte	0x17
 2797 0bfa 2C000000 		.4byte	0x2c
 2798 0bfe 00000000 		.4byte	.LFB23
 2799 0c02 4C010000 		.4byte	.LFE23-.LFB23
 2800 0c06 01       		.uleb128 0x1
 2801 0c07 9C       		.byte	0x9c
 2802 0c08 B40E0000 		.4byte	0xeb4
 2803 0c0c 1E       		.uleb128 0x1e
 2804 0c0d 5D030000 		.4byte	.LASF59
 2805 0c11 01       		.byte	0x1
 2806 0c12 1A       		.byte	0x1a
 2807 0c13 2C000000 		.4byte	0x2c
 2808 0c17 20       		.uleb128 0x20
 2809 0c18 6900     		.ascii	"i\000"
 2810 0c1a 01       		.byte	0x1
 2811 0c1b 1A       		.byte	0x1a
 2812 0c1c 2C000000 		.4byte	0x2c
 2813 0c20 21       		.uleb128 0x21
 2814 0c21 29030000 		.4byte	.LASF86
 2815 0c25 01       		.byte	0x1
 2816 0c26 1A       		.byte	0x1a
 2817 0c27 2C000000 		.4byte	0x2c
 2818 0c2b 00       		.byte	0
 2819 0c2c 17       		.uleb128 0x17
 2820 0c2d 23000000 		.4byte	.LASF87
 2821 0c31 01       		.byte	0x1
 2822 0c32 1B       		.byte	0x1b
 2823 0c33 E30B0000 		.4byte	0xbe3
 2824 0c37 16       		.uleb128 0x16
 2825 0c38 3F       		.byte	0x3f
 2826 0c39 9F       		.byte	0x9f
ARM GAS  /tmp/ccPZc5IS.s 			page 58


 2827 0c3a 93       		.byte	0x93
 2828 0c3b 04       		.uleb128 0x4
 2829 0c3c 08       		.byte	0x8
 2830 0c3d 73       		.byte	0x73
 2831 0c3e 9F       		.byte	0x9f
 2832 0c3f 93       		.byte	0x93
 2833 0c40 04       		.uleb128 0x4
 2834 0c41 32       		.byte	0x32
 2835 0c42 9F       		.byte	0x9f
 2836 0c43 93       		.byte	0x93
 2837 0c44 04       		.uleb128 0x4
 2838 0c45 08       		.byte	0x8
 2839 0c46 31       		.byte	0x31
 2840 0c47 9F       		.byte	0x9f
 2841 0c48 93       		.byte	0x93
 2842 0c49 04       		.uleb128 0x4
 2843 0c4a 33       		.byte	0x33
 2844 0c4b 9F       		.byte	0x9f
 2845 0c4c 93       		.byte	0x93
 2846 0c4d 04       		.uleb128 0x4
 2847 0c4e 17       		.uleb128 0x17
 2848 0c4f 81000000 		.4byte	.LASF73
 2849 0c53 01       		.byte	0x1
 2850 0c54 1F       		.byte	0x1f
 2851 0c55 CA020000 		.4byte	0x2ca
 2852 0c59 02       		.uleb128 0x2
 2853 0c5a 91       		.byte	0x91
 2854 0c5b 54       		.sleb128 -44
 2855 0c5c 17       		.uleb128 0x17
 2856 0c5d 5C000000 		.4byte	.LASF88
 2857 0c61 01       		.byte	0x1
 2858 0c62 20       		.byte	0x20
 2859 0c63 CA020000 		.4byte	0x2ca
 2860 0c67 0C       		.uleb128 0xc
 2861 0c68 30       		.byte	0x30
 2862 0c69 9F       		.byte	0x9f
 2863 0c6a 93       		.byte	0x93
 2864 0c6b 04       		.uleb128 0x4
 2865 0c6c 0C       		.byte	0xc
 2866 0c6d 80969800 		.4byte	0x989680
 2867 0c71 9F       		.byte	0x9f
 2868 0c72 93       		.byte	0x93
 2869 0c73 04       		.uleb128 0x4
 2870 0c74 22       		.uleb128 0x22
 2871 0c75 40000000 		.4byte	.LBB10
 2872 0c79 0C010000 		.4byte	.LBE10-.LBB10
 2873 0c7d 560E0000 		.4byte	0xe56
 2874 0c81 17       		.uleb128 0x17
 2875 0c82 54040000 		.4byte	.LASF74
 2876 0c86 01       		.byte	0x1
 2877 0c87 34       		.byte	0x34
 2878 0c88 E30B0000 		.4byte	0xbe3
 2879 0c8c 02       		.uleb128 0x2
 2880 0c8d 91       		.byte	0x91
 2881 0c8e 5C       		.sleb128 -36
 2882 0c8f 16       		.uleb128 0x16
 2883 0c90 96000000 		.4byte	.LASF89
ARM GAS  /tmp/ccPZc5IS.s 			page 59


 2884 0c94 01       		.byte	0x1
 2885 0c95 3A       		.byte	0x3a
 2886 0c96 2C000000 		.4byte	0x2c
 2887 0c9a F8030000 		.4byte	.LLST25
 2888 0c9e 22       		.uleb128 0x22
 2889 0c9f 8A000000 		.4byte	.LBB11
 2890 0ca3 98000000 		.4byte	.LBE11-.LBB11
 2891 0ca7 810D0000 		.4byte	0xd81
 2892 0cab 1D       		.uleb128 0x1d
 2893 0cac 6A00     		.ascii	"j\000"
 2894 0cae 01       		.byte	0x1
 2895 0caf 44       		.byte	0x44
 2896 0cb0 2C000000 		.4byte	0x2c
 2897 0cb4 63040000 		.4byte	.LLST26
 2898 0cb8 18       		.uleb128 0x18
 2899 0cb9 9A000000 		.4byte	.LVL119
 2900 0cbd FB0F0000 		.4byte	0xffb
 2901 0cc1 CF0C0000 		.4byte	0xccf
 2902 0cc5 13       		.uleb128 0x13
 2903 0cc6 01       		.uleb128 0x1
 2904 0cc7 50       		.byte	0x50
 2905 0cc8 05       		.uleb128 0x5
 2906 0cc9 03       		.byte	0x3
 2907 0cca CC000000 		.4byte	.LC15
 2908 0cce 00       		.byte	0
 2909 0ccf 18       		.uleb128 0x18
 2910 0cd0 AA000000 		.4byte	.LVL121
 2911 0cd4 14100000 		.4byte	0x1014
 2912 0cd8 EC0C0000 		.4byte	0xcec
 2913 0cdc 13       		.uleb128 0x13
 2914 0cdd 01       		.uleb128 0x1
 2915 0cde 51       		.byte	0x51
 2916 0cdf 02       		.uleb128 0x2
 2917 0ce0 74       		.byte	0x74
 2918 0ce1 00       		.sleb128 0
 2919 0ce2 13       		.uleb128 0x13
 2920 0ce3 01       		.uleb128 0x1
 2921 0ce4 50       		.byte	0x50
 2922 0ce5 05       		.uleb128 0x5
 2923 0ce6 03       		.byte	0x3
 2924 0ce7 E0000000 		.4byte	.LC16
 2925 0ceb 00       		.byte	0
 2926 0cec 18       		.uleb128 0x18
 2927 0ced CA000000 		.4byte	.LVL122
 2928 0cf1 FB0F0000 		.4byte	0xffb
 2929 0cf5 030D0000 		.4byte	0xd03
 2930 0cf9 13       		.uleb128 0x13
 2931 0cfa 01       		.uleb128 0x1
 2932 0cfb 50       		.byte	0x50
 2933 0cfc 05       		.uleb128 0x5
 2934 0cfd 03       		.byte	0x3
 2935 0cfe 08010000 		.4byte	.LC20
 2936 0d02 00       		.byte	0
 2937 0d03 18       		.uleb128 0x18
 2938 0d04 E8000000 		.4byte	.LVL124
 2939 0d08 14100000 		.4byte	0x1014
 2940 0d0c 280D0000 		.4byte	0xd28
ARM GAS  /tmp/ccPZc5IS.s 			page 60


 2941 0d10 13       		.uleb128 0x13
 2942 0d11 01       		.uleb128 0x1
 2943 0d12 50       		.byte	0x50
 2944 0d13 05       		.uleb128 0x5
 2945 0d14 03       		.byte	0x3
 2946 0d15 1C010000 		.4byte	.LC22
 2947 0d19 13       		.uleb128 0x13
 2948 0d1a 02       		.uleb128 0x2
 2949 0d1b 7D       		.byte	0x7d
 2950 0d1c 04       		.sleb128 4
 2951 0d1d 02       		.uleb128 0x2
 2952 0d1e 74       		.byte	0x74
 2953 0d1f 00       		.sleb128 0
 2954 0d20 13       		.uleb128 0x13
 2955 0d21 02       		.uleb128 0x2
 2956 0d22 7D       		.byte	0x7d
 2957 0d23 00       		.sleb128 0
 2958 0d24 02       		.uleb128 0x2
 2959 0d25 75       		.byte	0x75
 2960 0d26 00       		.sleb128 0
 2961 0d27 00       		.byte	0
 2962 0d28 18       		.uleb128 0x18
 2963 0d29 F6000000 		.4byte	.LVL126
 2964 0d2d FB0F0000 		.4byte	0xffb
 2965 0d31 3F0D0000 		.4byte	0xd3f
 2966 0d35 13       		.uleb128 0x13
 2967 0d36 01       		.uleb128 0x1
 2968 0d37 50       		.byte	0x50
 2969 0d38 05       		.uleb128 0x5
 2970 0d39 03       		.byte	0x3
 2971 0d3a FC000000 		.4byte	.LC19
 2972 0d3e 00       		.byte	0
 2973 0d3f 18       		.uleb128 0x18
 2974 0d40 04010000 		.4byte	.LVL127
 2975 0d44 FB0F0000 		.4byte	0xffb
 2976 0d48 560D0000 		.4byte	0xd56
 2977 0d4c 13       		.uleb128 0x13
 2978 0d4d 01       		.uleb128 0x1
 2979 0d4e 50       		.byte	0x50
 2980 0d4f 05       		.uleb128 0x5
 2981 0d50 03       		.byte	0x3
 2982 0d51 F0000000 		.4byte	.LC18
 2983 0d55 00       		.byte	0
 2984 0d56 18       		.uleb128 0x18
 2985 0d57 12010000 		.4byte	.LVL128
 2986 0d5b FB0F0000 		.4byte	0xffb
 2987 0d5f 6D0D0000 		.4byte	0xd6d
 2988 0d63 13       		.uleb128 0x13
 2989 0d64 01       		.uleb128 0x1
 2990 0d65 50       		.byte	0x50
 2991 0d66 05       		.uleb128 0x5
 2992 0d67 03       		.byte	0x3
 2993 0d68 E8000000 		.4byte	.LC17
 2994 0d6c 00       		.byte	0
 2995 0d6d 19       		.uleb128 0x19
 2996 0d6e 20010000 		.4byte	.LVL129
 2997 0d72 FB0F0000 		.4byte	0xffb
ARM GAS  /tmp/ccPZc5IS.s 			page 61


 2998 0d76 13       		.uleb128 0x13
 2999 0d77 01       		.uleb128 0x1
 3000 0d78 50       		.byte	0x50
 3001 0d79 05       		.uleb128 0x5
 3002 0d7a 03       		.byte	0x3
 3003 0d7b 14010000 		.4byte	.LC21
 3004 0d7f 00       		.byte	0
 3005 0d80 00       		.byte	0
 3006 0d81 18       		.uleb128 0x18
 3007 0d82 48000000 		.4byte	.LVL111
 3008 0d86 50080000 		.4byte	0x850
 3009 0d8a 9A0D0000 		.4byte	0xd9a
 3010 0d8e 13       		.uleb128 0x13
 3011 0d8f 01       		.uleb128 0x1
 3012 0d90 51       		.byte	0x51
 3013 0d91 02       		.uleb128 0x2
 3014 0d92 91       		.byte	0x91
 3015 0d93 54       		.sleb128 -44
 3016 0d94 13       		.uleb128 0x13
 3017 0d95 01       		.uleb128 0x1
 3018 0d96 50       		.byte	0x50
 3019 0d97 01       		.uleb128 0x1
 3020 0d98 3F       		.byte	0x3f
 3021 0d99 00       		.byte	0
 3022 0d9a 18       		.uleb128 0x18
 3023 0d9b 52000000 		.4byte	.LVL112
 3024 0d9f 50080000 		.4byte	0x850
 3025 0da3 B40D0000 		.4byte	0xdb4
 3026 0da7 13       		.uleb128 0x13
 3027 0da8 01       		.uleb128 0x1
 3028 0da9 51       		.byte	0x51
 3029 0daa 02       		.uleb128 0x2
 3030 0dab 91       		.byte	0x91
 3031 0dac 54       		.sleb128 -44
 3032 0dad 13       		.uleb128 0x13
 3033 0dae 01       		.uleb128 0x1
 3034 0daf 50       		.byte	0x50
 3035 0db0 02       		.uleb128 0x2
 3036 0db1 08       		.byte	0x8
 3037 0db2 73       		.byte	0x73
 3038 0db3 00       		.byte	0
 3039 0db4 18       		.uleb128 0x18
 3040 0db5 5C000000 		.4byte	.LVL113
 3041 0db9 50080000 		.4byte	0x850
 3042 0dbd CD0D0000 		.4byte	0xdcd
 3043 0dc1 13       		.uleb128 0x13
 3044 0dc2 01       		.uleb128 0x1
 3045 0dc3 51       		.byte	0x51
 3046 0dc4 02       		.uleb128 0x2
 3047 0dc5 91       		.byte	0x91
 3048 0dc6 54       		.sleb128 -44
 3049 0dc7 13       		.uleb128 0x13
 3050 0dc8 01       		.uleb128 0x1
 3051 0dc9 50       		.byte	0x50
 3052 0dca 01       		.uleb128 0x1
 3053 0dcb 32       		.byte	0x32
 3054 0dcc 00       		.byte	0
ARM GAS  /tmp/ccPZc5IS.s 			page 62


 3055 0dcd 18       		.uleb128 0x18
 3056 0dce 66000000 		.4byte	.LVL114
 3057 0dd2 50080000 		.4byte	0x850
 3058 0dd6 E70D0000 		.4byte	0xde7
 3059 0dda 13       		.uleb128 0x13
 3060 0ddb 01       		.uleb128 0x1
 3061 0ddc 51       		.byte	0x51
 3062 0ddd 02       		.uleb128 0x2
 3063 0dde 91       		.byte	0x91
 3064 0ddf 54       		.sleb128 -44
 3065 0de0 13       		.uleb128 0x13
 3066 0de1 01       		.uleb128 0x1
 3067 0de2 50       		.byte	0x50
 3068 0de3 02       		.uleb128 0x2
 3069 0de4 08       		.byte	0x8
 3070 0de5 31       		.byte	0x31
 3071 0de6 00       		.byte	0
 3072 0de7 18       		.uleb128 0x18
 3073 0de8 70000000 		.4byte	.LVL115
 3074 0dec 50080000 		.4byte	0x850
 3075 0df0 000E0000 		.4byte	0xe00
 3076 0df4 13       		.uleb128 0x13
 3077 0df5 01       		.uleb128 0x1
 3078 0df6 51       		.byte	0x51
 3079 0df7 02       		.uleb128 0x2
 3080 0df8 91       		.byte	0x91
 3081 0df9 54       		.sleb128 -44
 3082 0dfa 13       		.uleb128 0x13
 3083 0dfb 01       		.uleb128 0x1
 3084 0dfc 50       		.byte	0x50
 3085 0dfd 01       		.uleb128 0x1
 3086 0dfe 33       		.byte	0x33
 3087 0dff 00       		.byte	0
 3088 0e00 18       		.uleb128 0x18
 3089 0e01 78000000 		.4byte	.LVL116
 3090 0e05 420B0000 		.4byte	0xb42
 3091 0e09 140E0000 		.4byte	0xe14
 3092 0e0d 13       		.uleb128 0x13
 3093 0e0e 01       		.uleb128 0x1
 3094 0e0f 50       		.byte	0x50
 3095 0e10 02       		.uleb128 0x2
 3096 0e11 91       		.byte	0x91
 3097 0e12 5C       		.sleb128 -36
 3098 0e13 00       		.byte	0
 3099 0e14 18       		.uleb128 0x18
 3100 0e15 2E010000 		.4byte	.LVL132
 3101 0e19 FB0F0000 		.4byte	0xffb
 3102 0e1d 2B0E0000 		.4byte	0xe2b
 3103 0e21 13       		.uleb128 0x13
 3104 0e22 01       		.uleb128 0x1
 3105 0e23 50       		.byte	0x50
 3106 0e24 05       		.uleb128 0x5
 3107 0e25 03       		.byte	0x3
 3108 0e26 54000000 		.4byte	.LC12
 3109 0e2a 00       		.byte	0
 3110 0e2b 18       		.uleb128 0x18
 3111 0e2c 3C010000 		.4byte	.LVL135
ARM GAS  /tmp/ccPZc5IS.s 			page 63


 3112 0e30 FB0F0000 		.4byte	0xffb
 3113 0e34 420E0000 		.4byte	0xe42
 3114 0e38 13       		.uleb128 0x13
 3115 0e39 01       		.uleb128 0x1
 3116 0e3a 50       		.byte	0x50
 3117 0e3b 05       		.uleb128 0x5
 3118 0e3c 03       		.byte	0x3
 3119 0e3d 90000000 		.4byte	.LC13
 3120 0e41 00       		.byte	0
 3121 0e42 19       		.uleb128 0x19
 3122 0e43 4A010000 		.4byte	.LVL138
 3123 0e47 FB0F0000 		.4byte	0xffb
 3124 0e4b 13       		.uleb128 0x13
 3125 0e4c 01       		.uleb128 0x1
 3126 0e4d 50       		.byte	0x50
 3127 0e4e 05       		.uleb128 0x5
 3128 0e4f 03       		.byte	0x3
 3129 0e50 AC000000 		.4byte	.LC14
 3130 0e54 00       		.byte	0
 3131 0e55 00       		.byte	0
 3132 0e56 18       		.uleb128 0x18
 3133 0e57 1E000000 		.4byte	.LVL107
 3134 0e5b 14100000 		.4byte	0x1014
 3135 0e5f 8A0E0000 		.4byte	0xe8a
 3136 0e63 13       		.uleb128 0x13
 3137 0e64 01       		.uleb128 0x1
 3138 0e65 53       		.byte	0x53
 3139 0e66 01       		.uleb128 0x1
 3140 0e67 32       		.byte	0x32
 3141 0e68 13       		.uleb128 0x13
 3142 0e69 01       		.uleb128 0x1
 3143 0e6a 52       		.byte	0x52
 3144 0e6b 02       		.uleb128 0x2
 3145 0e6c 08       		.byte	0x8
 3146 0e6d 73       		.byte	0x73
 3147 0e6e 13       		.uleb128 0x13
 3148 0e6f 01       		.uleb128 0x1
 3149 0e70 51       		.byte	0x51
 3150 0e71 01       		.uleb128 0x1
 3151 0e72 3F       		.byte	0x3f
 3152 0e73 13       		.uleb128 0x13
 3153 0e74 01       		.uleb128 0x1
 3154 0e75 50       		.byte	0x50
 3155 0e76 05       		.uleb128 0x5
 3156 0e77 03       		.byte	0x3
 3157 0e78 38000000 		.4byte	.LC11
 3158 0e7c 13       		.uleb128 0x13
 3159 0e7d 02       		.uleb128 0x2
 3160 0e7e 7D       		.byte	0x7d
 3161 0e7f 04       		.sleb128 4
 3162 0e80 01       		.uleb128 0x1
 3163 0e81 33       		.byte	0x33
 3164 0e82 13       		.uleb128 0x13
 3165 0e83 02       		.uleb128 0x2
 3166 0e84 7D       		.byte	0x7d
 3167 0e85 00       		.sleb128 0
 3168 0e86 02       		.uleb128 0x2
ARM GAS  /tmp/ccPZc5IS.s 			page 64


 3169 0e87 08       		.byte	0x8
 3170 0e88 31       		.byte	0x31
 3171 0e89 00       		.byte	0
 3172 0e8a 18       		.uleb128 0x18
 3173 0e8b 36000000 		.4byte	.LVL109
 3174 0e8f D50F0000 		.4byte	0xfd5
 3175 0e93 A30E0000 		.4byte	0xea3
 3176 0e97 13       		.uleb128 0x13
 3177 0e98 01       		.uleb128 0x1
 3178 0e99 51       		.byte	0x51
 3179 0e9a 01       		.uleb128 0x1
 3180 0e9b 30       		.byte	0x30
 3181 0e9c 13       		.uleb128 0x13
 3182 0e9d 01       		.uleb128 0x1
 3183 0e9e 50       		.byte	0x50
 3184 0e9f 02       		.uleb128 0x2
 3185 0ea0 91       		.byte	0x91
 3186 0ea1 54       		.sleb128 -44
 3187 0ea2 00       		.byte	0
 3188 0ea3 19       		.uleb128 0x19
 3189 0ea4 3C000000 		.4byte	.LVL110
 3190 0ea8 E0050000 		.4byte	0x5e0
 3191 0eac 13       		.uleb128 0x13
 3192 0ead 01       		.uleb128 0x1
 3193 0eae 50       		.byte	0x50
 3194 0eaf 02       		.uleb128 0x2
 3195 0eb0 08       		.byte	0x8
 3196 0eb1 70       		.byte	0x70
 3197 0eb2 00       		.byte	0
 3198 0eb3 00       		.byte	0
 3199 0eb4 23       		.uleb128 0x23
 3200 0eb5 81010000 		.4byte	.LASF90
 3201 0eb9 04       		.byte	0x4
 3202 0eba A8       		.byte	0xa8
 3203 0ebb 93020000 		.4byte	0x293
 3204 0ebf 23       		.uleb128 0x23
 3205 0ec0 47020000 		.4byte	.LASF91
 3206 0ec4 04       		.byte	0x4
 3207 0ec5 A9       		.byte	0xa9
 3208 0ec6 93020000 		.4byte	0x293
 3209 0eca 24       		.uleb128 0x24
 3210 0ecb EE030000 		.4byte	.LASF92
 3211 0ecf B0000000 		.4byte	0xb0
 3212 0ed3 E70E0000 		.4byte	0xee7
 3213 0ed7 25       		.uleb128 0x25
 3214 0ed8 B0000000 		.4byte	0xb0
 3215 0edc 25       		.uleb128 0x25
 3216 0edd E70E0000 		.4byte	0xee7
 3217 0ee1 25       		.uleb128 0x25
 3218 0ee2 9E000000 		.4byte	0x9e
 3219 0ee6 00       		.byte	0
 3220 0ee7 06       		.uleb128 0x6
 3221 0ee8 04       		.byte	0x4
 3222 0ee9 ED0E0000 		.4byte	0xeed
 3223 0eed 26       		.uleb128 0x26
 3224 0eee 24       		.uleb128 0x24
 3225 0eef 45040000 		.4byte	.LASF93
ARM GAS  /tmp/ccPZc5IS.s 			page 65


 3226 0ef3 B0000000 		.4byte	0xb0
 3227 0ef7 0B0F0000 		.4byte	0xf0b
 3228 0efb 25       		.uleb128 0x25
 3229 0efc B0000000 		.4byte	0xb0
 3230 0f00 25       		.uleb128 0x25
 3231 0f01 2C000000 		.4byte	0x2c
 3232 0f05 25       		.uleb128 0x25
 3233 0f06 9E000000 		.4byte	0x9e
 3234 0f0a 00       		.byte	0
 3235 0f0b 27       		.uleb128 0x27
 3236 0f0c 1B020000 		.4byte	.LASF95
 3237 0f10 04       		.byte	0x4
 3238 0f11 6C01     		.2byte	0x16c
 3239 0f13 2C000000 		.4byte	0x2c
 3240 0f17 270F0000 		.4byte	0xf27
 3241 0f1b 25       		.uleb128 0x25
 3242 0f1c BD000000 		.4byte	0xbd
 3243 0f20 25       		.uleb128 0x25
 3244 0f21 BF020000 		.4byte	0x2bf
 3245 0f25 28       		.uleb128 0x28
 3246 0f26 00       		.byte	0
 3247 0f27 29       		.uleb128 0x29
 3248 0f28 31020000 		.4byte	.LASF96
 3249 0f2c 07       		.byte	0x7
 3250 0f2d 89       		.byte	0x89
 3251 0f2e BD000000 		.4byte	0xbd
 3252 0f32 410F0000 		.4byte	0xf41
 3253 0f36 25       		.uleb128 0x25
 3254 0f37 BD000000 		.4byte	0xbd
 3255 0f3b 25       		.uleb128 0x25
 3256 0f3c BF020000 		.4byte	0x2bf
 3257 0f40 00       		.byte	0
 3258 0f41 27       		.uleb128 0x27
 3259 0f42 F1020000 		.4byte	.LASF97
 3260 0f46 04       		.byte	0x4
 3261 0f47 1001     		.2byte	0x110
 3262 0f49 8E040000 		.4byte	0x48e
 3263 0f4d 5C0F0000 		.4byte	0xf5c
 3264 0f51 25       		.uleb128 0x25
 3265 0f52 BF020000 		.4byte	0x2bf
 3266 0f56 25       		.uleb128 0x25
 3267 0f57 BF020000 		.4byte	0x2bf
 3268 0f5b 00       		.byte	0
 3269 0f5c 2A       		.uleb128 0x2a
 3270 0f5d 56030000 		.4byte	.LASF100
 3271 0f61 04       		.byte	0x4
 3272 0f62 BB01     		.2byte	0x1bb
 3273 0f64 FB010000 		.4byte	.LASF111
 3274 0f68 2C000000 		.4byte	0x2c
 3275 0f6c 7C0F0000 		.4byte	0xf7c
 3276 0f70 25       		.uleb128 0x25
 3277 0f71 8E040000 		.4byte	0x48e
 3278 0f75 25       		.uleb128 0x25
 3279 0f76 BF020000 		.4byte	0x2bf
 3280 0f7a 28       		.uleb128 0x28
 3281 0f7b 00       		.byte	0
 3282 0f7c 29       		.uleb128 0x29
ARM GAS  /tmp/ccPZc5IS.s 			page 66


 3283 0f7d C9030000 		.4byte	.LASF98
 3284 0f81 04       		.byte	0x4
 3285 0f82 ED       		.byte	0xed
 3286 0f83 2C000000 		.4byte	0x2c
 3287 0f87 910F0000 		.4byte	0xf91
 3288 0f8b 25       		.uleb128 0x25
 3289 0f8c 8E040000 		.4byte	0x48e
 3290 0f90 00       		.byte	0
 3291 0f91 27       		.uleb128 0x27
 3292 0f92 9A010000 		.4byte	.LASF99
 3293 0f96 04       		.byte	0x4
 3294 0f97 6401     		.2byte	0x164
 3295 0f99 2C000000 		.4byte	0x2c
 3296 0f9d AD0F0000 		.4byte	0xfad
 3297 0fa1 25       		.uleb128 0x25
 3298 0fa2 8E040000 		.4byte	0x48e
 3299 0fa6 25       		.uleb128 0x25
 3300 0fa7 BF020000 		.4byte	0x2bf
 3301 0fab 28       		.uleb128 0x28
 3302 0fac 00       		.byte	0
 3303 0fad 2B       		.uleb128 0x2b
 3304 0fae C7000000 		.4byte	.LASF101
 3305 0fb2 08       		.byte	0x8
 3306 0fb3 00       		.byte	0
 3307 0fb4 73040000 		.4byte	.LASF104
 3308 0fb8 3E000000 		.4byte	0x3e
 3309 0fbc D50F0000 		.4byte	0xfd5
 3310 0fc0 25       		.uleb128 0x25
 3311 0fc1 E70E0000 		.4byte	0xee7
 3312 0fc5 25       		.uleb128 0x25
 3313 0fc6 3E000000 		.4byte	0x3e
 3314 0fca 25       		.uleb128 0x25
 3315 0fcb 3E000000 		.4byte	0x3e
 3316 0fcf 25       		.uleb128 0x25
 3317 0fd0 B0000000 		.4byte	0xb0
 3318 0fd4 00       		.byte	0
 3319 0fd5 27       		.uleb128 0x27
 3320 0fd6 0B020000 		.4byte	.LASF102
 3321 0fda 06       		.byte	0x6
 3322 0fdb 4E01     		.2byte	0x14e
 3323 0fdd 2C000000 		.4byte	0x2c
 3324 0fe1 F00F0000 		.4byte	0xff0
 3325 0fe5 25       		.uleb128 0x25
 3326 0fe6 F00F0000 		.4byte	0xff0
 3327 0fea 25       		.uleb128 0x25
 3328 0feb 810A0000 		.4byte	0xa81
 3329 0fef 00       		.byte	0
 3330 0ff0 06       		.uleb128 0x6
 3331 0ff1 04       		.byte	0x4
 3332 0ff2 F60F0000 		.4byte	0xff6
 3333 0ff6 0D       		.uleb128 0xd
 3334 0ff7 CA020000 		.4byte	0x2ca
 3335 0ffb 2B       		.uleb128 0x2b
 3336 0ffc 38020000 		.4byte	.LASF103
 3337 1000 08       		.byte	0x8
 3338 1001 00       		.byte	0
 3339 1002 7A040000 		.4byte	.LASF105
ARM GAS  /tmp/ccPZc5IS.s 			page 67


 3340 1006 2C000000 		.4byte	0x2c
 3341 100a 14100000 		.4byte	0x1014
 3342 100e 25       		.uleb128 0x25
 3343 100f BF020000 		.4byte	0x2bf
 3344 1013 00       		.byte	0
 3345 1014 2C       		.uleb128 0x2c
 3346 1015 00000000 		.4byte	.LASF106
 3347 1019 04       		.byte	0x4
 3348 101a 6A01     		.2byte	0x16a
 3349 101c 2C000000 		.4byte	0x2c
 3350 1020 25       		.uleb128 0x25
 3351 1021 BF020000 		.4byte	0x2bf
 3352 1025 28       		.uleb128 0x28
 3353 1026 00       		.byte	0
 3354 1027 00       		.byte	0
 3355              		.section	.debug_abbrev,"",%progbits
 3356              	.Ldebug_abbrev0:
 3357 0000 01       		.uleb128 0x1
 3358 0001 11       		.uleb128 0x11
 3359 0002 01       		.byte	0x1
 3360 0003 25       		.uleb128 0x25
 3361 0004 0E       		.uleb128 0xe
 3362 0005 13       		.uleb128 0x13
 3363 0006 0B       		.uleb128 0xb
 3364 0007 03       		.uleb128 0x3
 3365 0008 0E       		.uleb128 0xe
 3366 0009 1B       		.uleb128 0x1b
 3367 000a 0E       		.uleb128 0xe
 3368 000b 55       		.uleb128 0x55
 3369 000c 17       		.uleb128 0x17
 3370 000d 11       		.uleb128 0x11
 3371 000e 01       		.uleb128 0x1
 3372 000f 10       		.uleb128 0x10
 3373 0010 17       		.uleb128 0x17
 3374 0011 00       		.byte	0
 3375 0012 00       		.byte	0
 3376 0013 02       		.uleb128 0x2
 3377 0014 24       		.uleb128 0x24
 3378 0015 00       		.byte	0
 3379 0016 0B       		.uleb128 0xb
 3380 0017 0B       		.uleb128 0xb
 3381 0018 3E       		.uleb128 0x3e
 3382 0019 0B       		.uleb128 0xb
 3383 001a 03       		.uleb128 0x3
 3384 001b 0E       		.uleb128 0xe
 3385 001c 00       		.byte	0
 3386 001d 00       		.byte	0
 3387 001e 03       		.uleb128 0x3
 3388 001f 24       		.uleb128 0x24
 3389 0020 00       		.byte	0
 3390 0021 0B       		.uleb128 0xb
 3391 0022 0B       		.uleb128 0xb
 3392 0023 3E       		.uleb128 0x3e
 3393 0024 0B       		.uleb128 0xb
 3394 0025 03       		.uleb128 0x3
 3395 0026 08       		.uleb128 0x8
 3396 0027 00       		.byte	0
ARM GAS  /tmp/ccPZc5IS.s 			page 68


 3397 0028 00       		.byte	0
 3398 0029 04       		.uleb128 0x4
 3399 002a 16       		.uleb128 0x16
 3400 002b 00       		.byte	0
 3401 002c 03       		.uleb128 0x3
 3402 002d 0E       		.uleb128 0xe
 3403 002e 3A       		.uleb128 0x3a
 3404 002f 0B       		.uleb128 0xb
 3405 0030 3B       		.uleb128 0x3b
 3406 0031 0B       		.uleb128 0xb
 3407 0032 49       		.uleb128 0x49
 3408 0033 13       		.uleb128 0x13
 3409 0034 00       		.byte	0
 3410 0035 00       		.byte	0
 3411 0036 05       		.uleb128 0x5
 3412 0037 0F       		.uleb128 0xf
 3413 0038 00       		.byte	0
 3414 0039 0B       		.uleb128 0xb
 3415 003a 0B       		.uleb128 0xb
 3416 003b 00       		.byte	0
 3417 003c 00       		.byte	0
 3418 003d 06       		.uleb128 0x6
 3419 003e 0F       		.uleb128 0xf
 3420 003f 00       		.byte	0
 3421 0040 0B       		.uleb128 0xb
 3422 0041 0B       		.uleb128 0xb
 3423 0042 49       		.uleb128 0x49
 3424 0043 13       		.uleb128 0x13
 3425 0044 00       		.byte	0
 3426 0045 00       		.byte	0
 3427 0046 07       		.uleb128 0x7
 3428 0047 13       		.uleb128 0x13
 3429 0048 01       		.byte	0x1
 3430 0049 03       		.uleb128 0x3
 3431 004a 0E       		.uleb128 0xe
 3432 004b 0B       		.uleb128 0xb
 3433 004c 0B       		.uleb128 0xb
 3434 004d 3A       		.uleb128 0x3a
 3435 004e 0B       		.uleb128 0xb
 3436 004f 3B       		.uleb128 0x3b
 3437 0050 0B       		.uleb128 0xb
 3438 0051 01       		.uleb128 0x1
 3439 0052 13       		.uleb128 0x13
 3440 0053 00       		.byte	0
 3441 0054 00       		.byte	0
 3442 0055 08       		.uleb128 0x8
 3443 0056 0D       		.uleb128 0xd
 3444 0057 00       		.byte	0
 3445 0058 03       		.uleb128 0x3
 3446 0059 0E       		.uleb128 0xe
 3447 005a 3A       		.uleb128 0x3a
 3448 005b 0B       		.uleb128 0xb
 3449 005c 3B       		.uleb128 0x3b
 3450 005d 0B       		.uleb128 0xb
 3451 005e 49       		.uleb128 0x49
 3452 005f 13       		.uleb128 0x13
 3453 0060 38       		.uleb128 0x38
ARM GAS  /tmp/ccPZc5IS.s 			page 69


 3454 0061 0B       		.uleb128 0xb
 3455 0062 00       		.byte	0
 3456 0063 00       		.byte	0
 3457 0064 09       		.uleb128 0x9
 3458 0065 0D       		.uleb128 0xd
 3459 0066 00       		.byte	0
 3460 0067 03       		.uleb128 0x3
 3461 0068 0E       		.uleb128 0xe
 3462 0069 3A       		.uleb128 0x3a
 3463 006a 0B       		.uleb128 0xb
 3464 006b 3B       		.uleb128 0x3b
 3465 006c 05       		.uleb128 0x5
 3466 006d 49       		.uleb128 0x49
 3467 006e 13       		.uleb128 0x13
 3468 006f 38       		.uleb128 0x38
 3469 0070 0B       		.uleb128 0xb
 3470 0071 00       		.byte	0
 3471 0072 00       		.byte	0
 3472 0073 0A       		.uleb128 0xa
 3473 0074 16       		.uleb128 0x16
 3474 0075 00       		.byte	0
 3475 0076 03       		.uleb128 0x3
 3476 0077 0E       		.uleb128 0xe
 3477 0078 3A       		.uleb128 0x3a
 3478 0079 0B       		.uleb128 0xb
 3479 007a 3B       		.uleb128 0x3b
 3480 007b 0B       		.uleb128 0xb
 3481 007c 00       		.byte	0
 3482 007d 00       		.byte	0
 3483 007e 0B       		.uleb128 0xb
 3484 007f 01       		.uleb128 0x1
 3485 0080 01       		.byte	0x1
 3486 0081 49       		.uleb128 0x49
 3487 0082 13       		.uleb128 0x13
 3488 0083 01       		.uleb128 0x1
 3489 0084 13       		.uleb128 0x13
 3490 0085 00       		.byte	0
 3491 0086 00       		.byte	0
 3492 0087 0C       		.uleb128 0xc
 3493 0088 21       		.uleb128 0x21
 3494 0089 00       		.byte	0
 3495 008a 49       		.uleb128 0x49
 3496 008b 13       		.uleb128 0x13
 3497 008c 2F       		.uleb128 0x2f
 3498 008d 0B       		.uleb128 0xb
 3499 008e 00       		.byte	0
 3500 008f 00       		.byte	0
 3501 0090 0D       		.uleb128 0xd
 3502 0091 26       		.uleb128 0x26
 3503 0092 00       		.byte	0
 3504 0093 49       		.uleb128 0x49
 3505 0094 13       		.uleb128 0x13
 3506 0095 00       		.byte	0
 3507 0096 00       		.byte	0
 3508 0097 0E       		.uleb128 0xe
 3509 0098 2E       		.uleb128 0x2e
 3510 0099 01       		.byte	0x1
ARM GAS  /tmp/ccPZc5IS.s 			page 70


 3511 009a 3F       		.uleb128 0x3f
 3512 009b 19       		.uleb128 0x19
 3513 009c 03       		.uleb128 0x3
 3514 009d 0E       		.uleb128 0xe
 3515 009e 3A       		.uleb128 0x3a
 3516 009f 0B       		.uleb128 0xb
 3517 00a0 3B       		.uleb128 0x3b
 3518 00a1 0B       		.uleb128 0xb
 3519 00a2 27       		.uleb128 0x27
 3520 00a3 19       		.uleb128 0x19
 3521 00a4 49       		.uleb128 0x49
 3522 00a5 13       		.uleb128 0x13
 3523 00a6 20       		.uleb128 0x20
 3524 00a7 0B       		.uleb128 0xb
 3525 00a8 01       		.uleb128 0x1
 3526 00a9 13       		.uleb128 0x13
 3527 00aa 00       		.byte	0
 3528 00ab 00       		.byte	0
 3529 00ac 0F       		.uleb128 0xf
 3530 00ad 05       		.uleb128 0x5
 3531 00ae 00       		.byte	0
 3532 00af 03       		.uleb128 0x3
 3533 00b0 08       		.uleb128 0x8
 3534 00b1 3A       		.uleb128 0x3a
 3535 00b2 0B       		.uleb128 0xb
 3536 00b3 3B       		.uleb128 0x3b
 3537 00b4 0B       		.uleb128 0xb
 3538 00b5 49       		.uleb128 0x49
 3539 00b6 13       		.uleb128 0x13
 3540 00b7 00       		.byte	0
 3541 00b8 00       		.byte	0
 3542 00b9 10       		.uleb128 0x10
 3543 00ba 2E       		.uleb128 0x2e
 3544 00bb 01       		.byte	0x1
 3545 00bc 31       		.uleb128 0x31
 3546 00bd 13       		.uleb128 0x13
 3547 00be 11       		.uleb128 0x11
 3548 00bf 01       		.uleb128 0x1
 3549 00c0 12       		.uleb128 0x12
 3550 00c1 06       		.uleb128 0x6
 3551 00c2 40       		.uleb128 0x40
 3552 00c3 18       		.uleb128 0x18
 3553 00c4 9742     		.uleb128 0x2117
 3554 00c6 19       		.uleb128 0x19
 3555 00c7 01       		.uleb128 0x1
 3556 00c8 13       		.uleb128 0x13
 3557 00c9 00       		.byte	0
 3558 00ca 00       		.byte	0
 3559 00cb 11       		.uleb128 0x11
 3560 00cc 05       		.uleb128 0x5
 3561 00cd 00       		.byte	0
 3562 00ce 31       		.uleb128 0x31
 3563 00cf 13       		.uleb128 0x13
 3564 00d0 02       		.uleb128 0x2
 3565 00d1 17       		.uleb128 0x17
 3566 00d2 00       		.byte	0
 3567 00d3 00       		.byte	0
ARM GAS  /tmp/ccPZc5IS.s 			page 71


 3568 00d4 12       		.uleb128 0x12
 3569 00d5 898201   		.uleb128 0x4109
 3570 00d8 01       		.byte	0x1
 3571 00d9 11       		.uleb128 0x11
 3572 00da 01       		.uleb128 0x1
 3573 00db 9542     		.uleb128 0x2115
 3574 00dd 19       		.uleb128 0x19
 3575 00de 31       		.uleb128 0x31
 3576 00df 13       		.uleb128 0x13
 3577 00e0 00       		.byte	0
 3578 00e1 00       		.byte	0
 3579 00e2 13       		.uleb128 0x13
 3580 00e3 8A8201   		.uleb128 0x410a
 3581 00e6 00       		.byte	0
 3582 00e7 02       		.uleb128 0x2
 3583 00e8 18       		.uleb128 0x18
 3584 00e9 9142     		.uleb128 0x2111
 3585 00eb 18       		.uleb128 0x18
 3586 00ec 00       		.byte	0
 3587 00ed 00       		.byte	0
 3588 00ee 14       		.uleb128 0x14
 3589 00ef 2E       		.uleb128 0x2e
 3590 00f0 01       		.byte	0x1
 3591 00f1 3F       		.uleb128 0x3f
 3592 00f2 19       		.uleb128 0x19
 3593 00f3 03       		.uleb128 0x3
 3594 00f4 0E       		.uleb128 0xe
 3595 00f5 3A       		.uleb128 0x3a
 3596 00f6 0B       		.uleb128 0xb
 3597 00f7 3B       		.uleb128 0x3b
 3598 00f8 0B       		.uleb128 0xb
 3599 00f9 27       		.uleb128 0x27
 3600 00fa 19       		.uleb128 0x19
 3601 00fb 49       		.uleb128 0x49
 3602 00fc 13       		.uleb128 0x13
 3603 00fd 11       		.uleb128 0x11
 3604 00fe 01       		.uleb128 0x1
 3605 00ff 12       		.uleb128 0x12
 3606 0100 06       		.uleb128 0x6
 3607 0101 40       		.uleb128 0x40
 3608 0102 18       		.uleb128 0x18
 3609 0103 9742     		.uleb128 0x2117
 3610 0105 19       		.uleb128 0x19
 3611 0106 01       		.uleb128 0x1
 3612 0107 13       		.uleb128 0x13
 3613 0108 00       		.byte	0
 3614 0109 00       		.byte	0
 3615 010a 15       		.uleb128 0x15
 3616 010b 05       		.uleb128 0x5
 3617 010c 00       		.byte	0
 3618 010d 03       		.uleb128 0x3
 3619 010e 0E       		.uleb128 0xe
 3620 010f 3A       		.uleb128 0x3a
 3621 0110 0B       		.uleb128 0xb
 3622 0111 3B       		.uleb128 0x3b
 3623 0112 0B       		.uleb128 0xb
 3624 0113 49       		.uleb128 0x49
ARM GAS  /tmp/ccPZc5IS.s 			page 72


 3625 0114 13       		.uleb128 0x13
 3626 0115 02       		.uleb128 0x2
 3627 0116 17       		.uleb128 0x17
 3628 0117 00       		.byte	0
 3629 0118 00       		.byte	0
 3630 0119 16       		.uleb128 0x16
 3631 011a 34       		.uleb128 0x34
 3632 011b 00       		.byte	0
 3633 011c 03       		.uleb128 0x3
 3634 011d 0E       		.uleb128 0xe
 3635 011e 3A       		.uleb128 0x3a
 3636 011f 0B       		.uleb128 0xb
 3637 0120 3B       		.uleb128 0x3b
 3638 0121 0B       		.uleb128 0xb
 3639 0122 49       		.uleb128 0x49
 3640 0123 13       		.uleb128 0x13
 3641 0124 02       		.uleb128 0x2
 3642 0125 17       		.uleb128 0x17
 3643 0126 00       		.byte	0
 3644 0127 00       		.byte	0
 3645 0128 17       		.uleb128 0x17
 3646 0129 34       		.uleb128 0x34
 3647 012a 00       		.byte	0
 3648 012b 03       		.uleb128 0x3
 3649 012c 0E       		.uleb128 0xe
 3650 012d 3A       		.uleb128 0x3a
 3651 012e 0B       		.uleb128 0xb
 3652 012f 3B       		.uleb128 0x3b
 3653 0130 0B       		.uleb128 0xb
 3654 0131 49       		.uleb128 0x49
 3655 0132 13       		.uleb128 0x13
 3656 0133 02       		.uleb128 0x2
 3657 0134 18       		.uleb128 0x18
 3658 0135 00       		.byte	0
 3659 0136 00       		.byte	0
 3660 0137 18       		.uleb128 0x18
 3661 0138 898201   		.uleb128 0x4109
 3662 013b 01       		.byte	0x1
 3663 013c 11       		.uleb128 0x11
 3664 013d 01       		.uleb128 0x1
 3665 013e 31       		.uleb128 0x31
 3666 013f 13       		.uleb128 0x13
 3667 0140 01       		.uleb128 0x1
 3668 0141 13       		.uleb128 0x13
 3669 0142 00       		.byte	0
 3670 0143 00       		.byte	0
 3671 0144 19       		.uleb128 0x19
 3672 0145 898201   		.uleb128 0x4109
 3673 0148 01       		.byte	0x1
 3674 0149 11       		.uleb128 0x11
 3675 014a 01       		.uleb128 0x1
 3676 014b 31       		.uleb128 0x31
 3677 014c 13       		.uleb128 0x13
 3678 014d 00       		.byte	0
 3679 014e 00       		.byte	0
 3680 014f 1A       		.uleb128 0x1a
 3681 0150 2E       		.uleb128 0x2e
ARM GAS  /tmp/ccPZc5IS.s 			page 73


 3682 0151 01       		.byte	0x1
 3683 0152 3F       		.uleb128 0x3f
 3684 0153 19       		.uleb128 0x19
 3685 0154 03       		.uleb128 0x3
 3686 0155 0E       		.uleb128 0xe
 3687 0156 3A       		.uleb128 0x3a
 3688 0157 0B       		.uleb128 0xb
 3689 0158 3B       		.uleb128 0x3b
 3690 0159 0B       		.uleb128 0xb
 3691 015a 27       		.uleb128 0x27
 3692 015b 19       		.uleb128 0x19
 3693 015c 11       		.uleb128 0x11
 3694 015d 01       		.uleb128 0x1
 3695 015e 12       		.uleb128 0x12
 3696 015f 06       		.uleb128 0x6
 3697 0160 40       		.uleb128 0x40
 3698 0161 18       		.uleb128 0x18
 3699 0162 9742     		.uleb128 0x2117
 3700 0164 19       		.uleb128 0x19
 3701 0165 01       		.uleb128 0x1
 3702 0166 13       		.uleb128 0x13
 3703 0167 00       		.byte	0
 3704 0168 00       		.byte	0
 3705 0169 1B       		.uleb128 0x1b
 3706 016a 1D       		.uleb128 0x1d
 3707 016b 01       		.byte	0x1
 3708 016c 31       		.uleb128 0x31
 3709 016d 13       		.uleb128 0x13
 3710 016e 52       		.uleb128 0x52
 3711 016f 01       		.uleb128 0x1
 3712 0170 55       		.uleb128 0x55
 3713 0171 17       		.uleb128 0x17
 3714 0172 58       		.uleb128 0x58
 3715 0173 0B       		.uleb128 0xb
 3716 0174 59       		.uleb128 0x59
 3717 0175 0B       		.uleb128 0xb
 3718 0176 01       		.uleb128 0x1
 3719 0177 13       		.uleb128 0x13
 3720 0178 00       		.byte	0
 3721 0179 00       		.byte	0
 3722 017a 1C       		.uleb128 0x1c
 3723 017b 898201   		.uleb128 0x4109
 3724 017e 00       		.byte	0
 3725 017f 11       		.uleb128 0x11
 3726 0180 01       		.uleb128 0x1
 3727 0181 31       		.uleb128 0x31
 3728 0182 13       		.uleb128 0x13
 3729 0183 00       		.byte	0
 3730 0184 00       		.byte	0
 3731 0185 1D       		.uleb128 0x1d
 3732 0186 34       		.uleb128 0x34
 3733 0187 00       		.byte	0
 3734 0188 03       		.uleb128 0x3
 3735 0189 08       		.uleb128 0x8
 3736 018a 3A       		.uleb128 0x3a
 3737 018b 0B       		.uleb128 0xb
 3738 018c 3B       		.uleb128 0x3b
ARM GAS  /tmp/ccPZc5IS.s 			page 74


 3739 018d 0B       		.uleb128 0xb
 3740 018e 49       		.uleb128 0x49
 3741 018f 13       		.uleb128 0x13
 3742 0190 02       		.uleb128 0x2
 3743 0191 17       		.uleb128 0x17
 3744 0192 00       		.byte	0
 3745 0193 00       		.byte	0
 3746 0194 1E       		.uleb128 0x1e
 3747 0195 34       		.uleb128 0x34
 3748 0196 00       		.byte	0
 3749 0197 03       		.uleb128 0x3
 3750 0198 0E       		.uleb128 0xe
 3751 0199 3A       		.uleb128 0x3a
 3752 019a 0B       		.uleb128 0xb
 3753 019b 3B       		.uleb128 0x3b
 3754 019c 0B       		.uleb128 0xb
 3755 019d 49       		.uleb128 0x49
 3756 019e 13       		.uleb128 0x13
 3757 019f 00       		.byte	0
 3758 01a0 00       		.byte	0
 3759 01a1 1F       		.uleb128 0x1f
 3760 01a2 0B       		.uleb128 0xb
 3761 01a3 01       		.byte	0x1
 3762 01a4 55       		.uleb128 0x55
 3763 01a5 17       		.uleb128 0x17
 3764 01a6 00       		.byte	0
 3765 01a7 00       		.byte	0
 3766 01a8 20       		.uleb128 0x20
 3767 01a9 34       		.uleb128 0x34
 3768 01aa 00       		.byte	0
 3769 01ab 03       		.uleb128 0x3
 3770 01ac 08       		.uleb128 0x8
 3771 01ad 3A       		.uleb128 0x3a
 3772 01ae 0B       		.uleb128 0xb
 3773 01af 3B       		.uleb128 0x3b
 3774 01b0 0B       		.uleb128 0xb
 3775 01b1 49       		.uleb128 0x49
 3776 01b2 13       		.uleb128 0x13
 3777 01b3 00       		.byte	0
 3778 01b4 00       		.byte	0
 3779 01b5 21       		.uleb128 0x21
 3780 01b6 34       		.uleb128 0x34
 3781 01b7 00       		.byte	0
 3782 01b8 03       		.uleb128 0x3
 3783 01b9 0E       		.uleb128 0xe
 3784 01ba 3A       		.uleb128 0x3a
 3785 01bb 0B       		.uleb128 0xb
 3786 01bc 3B       		.uleb128 0x3b
 3787 01bd 0B       		.uleb128 0xb
 3788 01be 49       		.uleb128 0x49
 3789 01bf 13       		.uleb128 0x13
 3790 01c0 1C       		.uleb128 0x1c
 3791 01c1 0B       		.uleb128 0xb
 3792 01c2 00       		.byte	0
 3793 01c3 00       		.byte	0
 3794 01c4 22       		.uleb128 0x22
 3795 01c5 0B       		.uleb128 0xb
ARM GAS  /tmp/ccPZc5IS.s 			page 75


 3796 01c6 01       		.byte	0x1
 3797 01c7 11       		.uleb128 0x11
 3798 01c8 01       		.uleb128 0x1
 3799 01c9 12       		.uleb128 0x12
 3800 01ca 06       		.uleb128 0x6
 3801 01cb 01       		.uleb128 0x1
 3802 01cc 13       		.uleb128 0x13
 3803 01cd 00       		.byte	0
 3804 01ce 00       		.byte	0
 3805 01cf 23       		.uleb128 0x23
 3806 01d0 34       		.uleb128 0x34
 3807 01d1 00       		.byte	0
 3808 01d2 03       		.uleb128 0x3
 3809 01d3 0E       		.uleb128 0xe
 3810 01d4 3A       		.uleb128 0x3a
 3811 01d5 0B       		.uleb128 0xb
 3812 01d6 3B       		.uleb128 0x3b
 3813 01d7 0B       		.uleb128 0xb
 3814 01d8 49       		.uleb128 0x49
 3815 01d9 13       		.uleb128 0x13
 3816 01da 3F       		.uleb128 0x3f
 3817 01db 19       		.uleb128 0x19
 3818 01dc 3C       		.uleb128 0x3c
 3819 01dd 19       		.uleb128 0x19
 3820 01de 00       		.byte	0
 3821 01df 00       		.byte	0
 3822 01e0 24       		.uleb128 0x24
 3823 01e1 2E       		.uleb128 0x2e
 3824 01e2 01       		.byte	0x1
 3825 01e3 3F       		.uleb128 0x3f
 3826 01e4 19       		.uleb128 0x19
 3827 01e5 03       		.uleb128 0x3
 3828 01e6 0E       		.uleb128 0xe
 3829 01e7 27       		.uleb128 0x27
 3830 01e8 19       		.uleb128 0x19
 3831 01e9 49       		.uleb128 0x49
 3832 01ea 13       		.uleb128 0x13
 3833 01eb 34       		.uleb128 0x34
 3834 01ec 19       		.uleb128 0x19
 3835 01ed 3C       		.uleb128 0x3c
 3836 01ee 19       		.uleb128 0x19
 3837 01ef 01       		.uleb128 0x1
 3838 01f0 13       		.uleb128 0x13
 3839 01f1 00       		.byte	0
 3840 01f2 00       		.byte	0
 3841 01f3 25       		.uleb128 0x25
 3842 01f4 05       		.uleb128 0x5
 3843 01f5 00       		.byte	0
 3844 01f6 49       		.uleb128 0x49
 3845 01f7 13       		.uleb128 0x13
 3846 01f8 00       		.byte	0
 3847 01f9 00       		.byte	0
 3848 01fa 26       		.uleb128 0x26
 3849 01fb 26       		.uleb128 0x26
 3850 01fc 00       		.byte	0
 3851 01fd 00       		.byte	0
 3852 01fe 00       		.byte	0
ARM GAS  /tmp/ccPZc5IS.s 			page 76


 3853 01ff 27       		.uleb128 0x27
 3854 0200 2E       		.uleb128 0x2e
 3855 0201 01       		.byte	0x1
 3856 0202 3F       		.uleb128 0x3f
 3857 0203 19       		.uleb128 0x19
 3858 0204 03       		.uleb128 0x3
 3859 0205 0E       		.uleb128 0xe
 3860 0206 3A       		.uleb128 0x3a
 3861 0207 0B       		.uleb128 0xb
 3862 0208 3B       		.uleb128 0x3b
 3863 0209 05       		.uleb128 0x5
 3864 020a 27       		.uleb128 0x27
 3865 020b 19       		.uleb128 0x19
 3866 020c 49       		.uleb128 0x49
 3867 020d 13       		.uleb128 0x13
 3868 020e 3C       		.uleb128 0x3c
 3869 020f 19       		.uleb128 0x19
 3870 0210 01       		.uleb128 0x1
 3871 0211 13       		.uleb128 0x13
 3872 0212 00       		.byte	0
 3873 0213 00       		.byte	0
 3874 0214 28       		.uleb128 0x28
 3875 0215 18       		.uleb128 0x18
 3876 0216 00       		.byte	0
 3877 0217 00       		.byte	0
 3878 0218 00       		.byte	0
 3879 0219 29       		.uleb128 0x29
 3880 021a 2E       		.uleb128 0x2e
 3881 021b 01       		.byte	0x1
 3882 021c 3F       		.uleb128 0x3f
 3883 021d 19       		.uleb128 0x19
 3884 021e 03       		.uleb128 0x3
 3885 021f 0E       		.uleb128 0xe
 3886 0220 3A       		.uleb128 0x3a
 3887 0221 0B       		.uleb128 0xb
 3888 0222 3B       		.uleb128 0x3b
 3889 0223 0B       		.uleb128 0xb
 3890 0224 27       		.uleb128 0x27
 3891 0225 19       		.uleb128 0x19
 3892 0226 49       		.uleb128 0x49
 3893 0227 13       		.uleb128 0x13
 3894 0228 3C       		.uleb128 0x3c
 3895 0229 19       		.uleb128 0x19
 3896 022a 01       		.uleb128 0x1
 3897 022b 13       		.uleb128 0x13
 3898 022c 00       		.byte	0
 3899 022d 00       		.byte	0
 3900 022e 2A       		.uleb128 0x2a
 3901 022f 2E       		.uleb128 0x2e
 3902 0230 01       		.byte	0x1
 3903 0231 3F       		.uleb128 0x3f
 3904 0232 19       		.uleb128 0x19
 3905 0233 03       		.uleb128 0x3
 3906 0234 0E       		.uleb128 0xe
 3907 0235 3A       		.uleb128 0x3a
 3908 0236 0B       		.uleb128 0xb
 3909 0237 3B       		.uleb128 0x3b
ARM GAS  /tmp/ccPZc5IS.s 			page 77


 3910 0238 05       		.uleb128 0x5
 3911 0239 6E       		.uleb128 0x6e
 3912 023a 0E       		.uleb128 0xe
 3913 023b 27       		.uleb128 0x27
 3914 023c 19       		.uleb128 0x19
 3915 023d 49       		.uleb128 0x49
 3916 023e 13       		.uleb128 0x13
 3917 023f 3C       		.uleb128 0x3c
 3918 0240 19       		.uleb128 0x19
 3919 0241 01       		.uleb128 0x1
 3920 0242 13       		.uleb128 0x13
 3921 0243 00       		.byte	0
 3922 0244 00       		.byte	0
 3923 0245 2B       		.uleb128 0x2b
 3924 0246 2E       		.uleb128 0x2e
 3925 0247 01       		.byte	0x1
 3926 0248 3F       		.uleb128 0x3f
 3927 0249 19       		.uleb128 0x19
 3928 024a 03       		.uleb128 0x3
 3929 024b 0E       		.uleb128 0xe
 3930 024c 3A       		.uleb128 0x3a
 3931 024d 0B       		.uleb128 0xb
 3932 024e 3B       		.uleb128 0x3b
 3933 024f 0B       		.uleb128 0xb
 3934 0250 6E       		.uleb128 0x6e
 3935 0251 0E       		.uleb128 0xe
 3936 0252 27       		.uleb128 0x27
 3937 0253 19       		.uleb128 0x19
 3938 0254 49       		.uleb128 0x49
 3939 0255 13       		.uleb128 0x13
 3940 0256 3C       		.uleb128 0x3c
 3941 0257 19       		.uleb128 0x19
 3942 0258 01       		.uleb128 0x1
 3943 0259 13       		.uleb128 0x13
 3944 025a 00       		.byte	0
 3945 025b 00       		.byte	0
 3946 025c 2C       		.uleb128 0x2c
 3947 025d 2E       		.uleb128 0x2e
 3948 025e 01       		.byte	0x1
 3949 025f 3F       		.uleb128 0x3f
 3950 0260 19       		.uleb128 0x19
 3951 0261 03       		.uleb128 0x3
 3952 0262 0E       		.uleb128 0xe
 3953 0263 3A       		.uleb128 0x3a
 3954 0264 0B       		.uleb128 0xb
 3955 0265 3B       		.uleb128 0x3b
 3956 0266 05       		.uleb128 0x5
 3957 0267 27       		.uleb128 0x27
 3958 0268 19       		.uleb128 0x19
 3959 0269 49       		.uleb128 0x49
 3960 026a 13       		.uleb128 0x13
 3961 026b 3C       		.uleb128 0x3c
 3962 026c 19       		.uleb128 0x19
 3963 026d 00       		.byte	0
 3964 026e 00       		.byte	0
 3965 026f 00       		.byte	0
 3966              		.section	.debug_loc,"",%progbits
ARM GAS  /tmp/ccPZc5IS.s 			page 78


 3967              	.Ldebug_loc0:
 3968              	.LLST0:
 3969 0000 00000000 		.4byte	.LVL0
 3970 0004 0A000000 		.4byte	.LVL1
 3971 0008 0100     		.2byte	0x1
 3972 000a 50       		.byte	0x50
 3973 000b 0A000000 		.4byte	.LVL1
 3974 000f 0C000000 		.4byte	.LVL2
 3975 0013 0400     		.2byte	0x4
 3976 0015 73       		.byte	0x73
 3977 0016 F900     		.sleb128 121
 3978 0018 9F       		.byte	0x9f
 3979 0019 0C000000 		.4byte	.LVL2
 3980 001d 18000000 		.4byte	.LVL3
 3981 0021 0100     		.2byte	0x1
 3982 0023 50       		.byte	0x50
 3983 0024 18000000 		.4byte	.LVL3
 3984 0028 1A000000 		.4byte	.LVL4
 3985 002c 0400     		.2byte	0x4
 3986 002e 73       		.byte	0x73
 3987 002f 8507     		.sleb128 901
 3988 0031 9F       		.byte	0x9f
 3989 0032 1A000000 		.4byte	.LVL4
 3990 0036 24000000 		.4byte	.LVL5
 3991 003a 0100     		.2byte	0x1
 3992 003c 50       		.byte	0x50
 3993 003d 24000000 		.4byte	.LVL5
 3994 0041 26000000 		.4byte	.LVL6
 3995 0045 0400     		.2byte	0x4
 3996 0047 73       		.byte	0x73
 3997 0048 C70A     		.sleb128 1351
 3998 004a 9F       		.byte	0x9f
 3999 004b 26000000 		.4byte	.LVL6
 4000 004f 30000000 		.4byte	.LVL7
 4001 0053 0100     		.2byte	0x1
 4002 0055 50       		.byte	0x50
 4003 0056 30000000 		.4byte	.LVL7
 4004 005a 34000000 		.4byte	.LFE32
 4005 005e 0400     		.2byte	0x4
 4006 0060 F3       		.byte	0xf3
 4007 0061 01       		.uleb128 0x1
 4008 0062 50       		.byte	0x50
 4009 0063 9F       		.byte	0x9f
 4010 0064 00000000 		.4byte	0
 4011 0068 00000000 		.4byte	0
 4012              	.LLST1:
 4013 006c 34000000 		.4byte	.LVL8
 4014 0070 39000000 		.4byte	.LVL9-1
 4015 0074 0100     		.2byte	0x1
 4016 0076 50       		.byte	0x50
 4017 0077 39000000 		.4byte	.LVL9-1
 4018 007b 3A000000 		.4byte	.LVL9
 4019 007f 0400     		.2byte	0x4
 4020 0081 F3       		.byte	0xf3
 4021 0082 01       		.uleb128 0x1
 4022 0083 50       		.byte	0x50
 4023 0084 9F       		.byte	0x9f
ARM GAS  /tmp/ccPZc5IS.s 			page 79


 4024 0085 3A000000 		.4byte	.LVL9
 4025 0089 3C000000 		.4byte	.LVL10
 4026 008d 0100     		.2byte	0x1
 4027 008f 50       		.byte	0x50
 4028 0090 3C000000 		.4byte	.LVL10
 4029 0094 3E000000 		.4byte	.LFE25
 4030 0098 0400     		.2byte	0x4
 4031 009a F3       		.byte	0xf3
 4032 009b 01       		.uleb128 0x1
 4033 009c 50       		.byte	0x50
 4034 009d 9F       		.byte	0x9f
 4035 009e 00000000 		.4byte	0
 4036 00a2 00000000 		.4byte	0
 4037              	.LLST2:
 4038 00a6 40000000 		.4byte	.LVL11
 4039 00aa 50000000 		.4byte	.LVL12
 4040 00ae 0100     		.2byte	0x1
 4041 00b0 50       		.byte	0x50
 4042 00b1 50000000 		.4byte	.LVL12
 4043 00b5 94000000 		.4byte	.LVL18
 4044 00b9 0100     		.2byte	0x1
 4045 00bb 54       		.byte	0x54
 4046 00bc 94000000 		.4byte	.LVL18
 4047 00c0 D8000000 		.4byte	.LFE28
 4048 00c4 0400     		.2byte	0x4
 4049 00c6 F3       		.byte	0xf3
 4050 00c7 01       		.uleb128 0x1
 4051 00c8 50       		.byte	0x50
 4052 00c9 9F       		.byte	0x9f
 4053 00ca 00000000 		.4byte	0
 4054 00ce 00000000 		.4byte	0
 4055              	.LLST3:
 4056 00d2 94000000 		.4byte	.LVL18
 4057 00d6 97000000 		.4byte	.LVL19-1
 4058 00da 0100     		.2byte	0x1
 4059 00dc 50       		.byte	0x50
 4060 00dd 97000000 		.4byte	.LVL19-1
 4061 00e1 C6000000 		.4byte	.LVL23
 4062 00e5 0100     		.2byte	0x1
 4063 00e7 54       		.byte	0x54
 4064 00e8 00000000 		.4byte	0
 4065 00ec 00000000 		.4byte	0
 4066              	.LLST4:
 4067 00f0 BA000000 		.4byte	.LVL21
 4068 00f4 BE000000 		.4byte	.LVL22
 4069 00f8 0800     		.2byte	0x8
 4070 00fa F5       		.byte	0xf5
 4071 00fb 4E       		.uleb128 0x4e
 4072 00fc 25       		.uleb128 0x25
 4073 00fd F7       		.byte	0xf7
 4074 00fe 2C       		.uleb128 0x2c
 4075 00ff F7       		.byte	0xf7
 4076 0100 00       		.uleb128 0
 4077 0101 9F       		.byte	0x9f
 4078 0102 BE000000 		.4byte	.LVL22
 4079 0106 D8000000 		.4byte	.LFE28
 4080 010a 0200     		.2byte	0x2
ARM GAS  /tmp/ccPZc5IS.s 			page 80


 4081 010c 90       		.byte	0x90
 4082 010d 4F       		.uleb128 0x4f
 4083 010e 00000000 		.4byte	0
 4084 0112 00000000 		.4byte	0
 4085              	.LLST5:
 4086 0116 D8000000 		.4byte	.LVL24
 4087 011a E6000000 		.4byte	.LVL25
 4088 011e 0100     		.2byte	0x1
 4089 0120 50       		.byte	0x50
 4090 0121 E6000000 		.4byte	.LVL25
 4091 0125 3E010000 		.4byte	.LVL33
 4092 0129 0100     		.2byte	0x1
 4093 012b 57       		.byte	0x57
 4094 012c 3E010000 		.4byte	.LVL33
 4095 0130 44010000 		.4byte	.LFE29
 4096 0134 0400     		.2byte	0x4
 4097 0136 F3       		.byte	0xf3
 4098 0137 01       		.uleb128 0x1
 4099 0138 50       		.byte	0x50
 4100 0139 9F       		.byte	0x9f
 4101 013a 00000000 		.4byte	0
 4102 013e 00000000 		.4byte	0
 4103              	.LLST6:
 4104 0142 D8000000 		.4byte	.LVL24
 4105 0146 E6000000 		.4byte	.LVL25
 4106 014a 0100     		.2byte	0x1
 4107 014c 51       		.byte	0x51
 4108 014d E6000000 		.4byte	.LVL25
 4109 0151 3E010000 		.4byte	.LVL33
 4110 0155 0100     		.2byte	0x1
 4111 0157 56       		.byte	0x56
 4112 0158 3E010000 		.4byte	.LVL33
 4113 015c 44010000 		.4byte	.LFE29
 4114 0160 0400     		.2byte	0x4
 4115 0162 F3       		.byte	0xf3
 4116 0163 01       		.uleb128 0x1
 4117 0164 51       		.byte	0x51
 4118 0165 9F       		.byte	0x9f
 4119 0166 00000000 		.4byte	0
 4120 016a 00000000 		.4byte	0
 4121              	.LLST7:
 4122 016e 30010000 		.4byte	.LVL30
 4123 0172 33010000 		.4byte	.LVL31-1
 4124 0176 0100     		.2byte	0x1
 4125 0178 50       		.byte	0x50
 4126 0179 33010000 		.4byte	.LVL31-1
 4127 017d 3E010000 		.4byte	.LVL33
 4128 0181 0100     		.2byte	0x1
 4129 0183 54       		.byte	0x54
 4130 0184 00000000 		.4byte	0
 4131 0188 00000000 		.4byte	0
 4132              	.LLST8:
 4133 018c 44010000 		.4byte	.LVL34
 4134 0190 50010000 		.4byte	.LVL35
 4135 0194 0100     		.2byte	0x1
 4136 0196 50       		.byte	0x50
 4137 0197 50010000 		.4byte	.LVL35
ARM GAS  /tmp/ccPZc5IS.s 			page 81


 4138 019b AA010000 		.4byte	.LVL43
 4139 019f 0100     		.2byte	0x1
 4140 01a1 56       		.byte	0x56
 4141 01a2 AA010000 		.4byte	.LVL43
 4142 01a6 B0010000 		.4byte	.LFE30
 4143 01aa 0400     		.2byte	0x4
 4144 01ac F3       		.byte	0xf3
 4145 01ad 01       		.uleb128 0x1
 4146 01ae 50       		.byte	0x50
 4147 01af 9F       		.byte	0x9f
 4148 01b0 00000000 		.4byte	0
 4149 01b4 00000000 		.4byte	0
 4150              	.LLST9:
 4151 01b8 9A010000 		.4byte	.LVL40
 4152 01bc 9D010000 		.4byte	.LVL41-1
 4153 01c0 0100     		.2byte	0x1
 4154 01c2 50       		.byte	0x50
 4155 01c3 9D010000 		.4byte	.LVL41-1
 4156 01c7 AA010000 		.4byte	.LVL43
 4157 01cb 0100     		.2byte	0x1
 4158 01cd 54       		.byte	0x54
 4159 01ce 00000000 		.4byte	0
 4160 01d2 00000000 		.4byte	0
 4161              	.LLST10:
 4162 01d6 B0010000 		.4byte	.LVL44
 4163 01da BE010000 		.4byte	.LVL45
 4164 01de 0100     		.2byte	0x1
 4165 01e0 50       		.byte	0x50
 4166 01e1 BE010000 		.4byte	.LVL45
 4167 01e5 38020000 		.4byte	.LVL58
 4168 01e9 0100     		.2byte	0x1
 4169 01eb 57       		.byte	0x57
 4170 01ec 38020000 		.4byte	.LVL58
 4171 01f0 3C020000 		.4byte	.LFE31
 4172 01f4 0400     		.2byte	0x4
 4173 01f6 F3       		.byte	0xf3
 4174 01f7 01       		.uleb128 0x1
 4175 01f8 50       		.byte	0x50
 4176 01f9 9F       		.byte	0x9f
 4177 01fa 00000000 		.4byte	0
 4178 01fe 00000000 		.4byte	0
 4179              	.LLST11:
 4180 0202 B0010000 		.4byte	.LVL44
 4181 0206 BE010000 		.4byte	.LVL45
 4182 020a 0100     		.2byte	0x1
 4183 020c 51       		.byte	0x51
 4184 020d BE010000 		.4byte	.LVL45
 4185 0211 38020000 		.4byte	.LVL58
 4186 0215 0100     		.2byte	0x1
 4187 0217 56       		.byte	0x56
 4188 0218 38020000 		.4byte	.LVL58
 4189 021c 3C020000 		.4byte	.LFE31
 4190 0220 0400     		.2byte	0x4
 4191 0222 F3       		.byte	0xf3
 4192 0223 01       		.uleb128 0x1
 4193 0224 51       		.byte	0x51
 4194 0225 9F       		.byte	0x9f
ARM GAS  /tmp/ccPZc5IS.s 			page 82


 4195 0226 00000000 		.4byte	0
 4196 022a 00000000 		.4byte	0
 4197              	.LLST12:
 4198 022e FE010000 		.4byte	.LVL50
 4199 0232 04020000 		.4byte	.LVL51
 4200 0236 0100     		.2byte	0x1
 4201 0238 50       		.byte	0x50
 4202 0239 04020000 		.4byte	.LVL51
 4203 023d 1C020000 		.4byte	.LVL54
 4204 0241 0100     		.2byte	0x1
 4205 0243 54       		.byte	0x54
 4206 0244 1C020000 		.4byte	.LVL54
 4207 0248 20020000 		.4byte	.LVL55
 4208 024c 0100     		.2byte	0x1
 4209 024e 50       		.byte	0x50
 4210 024f 20020000 		.4byte	.LVL55
 4211 0253 38020000 		.4byte	.LVL58
 4212 0257 0100     		.2byte	0x1
 4213 0259 54       		.byte	0x54
 4214 025a 00000000 		.4byte	0
 4215 025e 00000000 		.4byte	0
 4216              	.LLST13:
 4217 0262 3C020000 		.4byte	.LVL59
 4218 0266 44020000 		.4byte	.LVL60
 4219 026a 0100     		.2byte	0x1
 4220 026c 50       		.byte	0x50
 4221 026d 44020000 		.4byte	.LVL60
 4222 0271 F2020000 		.4byte	.LFE24
 4223 0275 0100     		.2byte	0x1
 4224 0277 54       		.byte	0x54
 4225 0278 00000000 		.4byte	0
 4226 027c 00000000 		.4byte	0
 4227              	.LLST14:
 4228 0280 3C020000 		.4byte	.LVL59
 4229 0284 46020000 		.4byte	.LVL61
 4230 0288 0100     		.2byte	0x1
 4231 028a 51       		.byte	0x51
 4232 028b 46020000 		.4byte	.LVL61
 4233 028f F2020000 		.4byte	.LFE24
 4234 0293 0100     		.2byte	0x1
 4235 0295 56       		.byte	0x56
 4236 0296 00000000 		.4byte	0
 4237 029a 00000000 		.4byte	0
 4238              	.LLST15:
 4239 029e A4020000 		.4byte	.LVL74
 4240 02a2 B0020000 		.4byte	.LVL76
 4241 02a6 0100     		.2byte	0x1
 4242 02a8 55       		.byte	0x55
 4243 02a9 EC020000 		.4byte	.LVL84
 4244 02ad F2020000 		.4byte	.LFE24
 4245 02b1 0100     		.2byte	0x1
 4246 02b3 55       		.byte	0x55
 4247 02b4 00000000 		.4byte	0
 4248 02b8 00000000 		.4byte	0
 4249              	.LLST16:
 4250 02bc 90020000 		.4byte	.LVL71
 4251 02c0 97020000 		.4byte	.LVL72-1
ARM GAS  /tmp/ccPZc5IS.s 			page 83


 4252 02c4 0100     		.2byte	0x1
 4253 02c6 50       		.byte	0x50
 4254 02c7 B0020000 		.4byte	.LVL76
 4255 02cb B2020000 		.4byte	.LVL77
 4256 02cf 0100     		.2byte	0x1
 4257 02d1 50       		.byte	0x50
 4258 02d2 00000000 		.4byte	0
 4259 02d6 00000000 		.4byte	0
 4260              	.LLST17:
 4261 02da F4020000 		.4byte	.LVL86
 4262 02de FF020000 		.4byte	.LVL88-1
 4263 02e2 0100     		.2byte	0x1
 4264 02e4 50       		.byte	0x50
 4265 02e5 FF020000 		.4byte	.LVL88-1
 4266 02e9 28030000 		.4byte	.LFE27
 4267 02ed 0100     		.2byte	0x1
 4268 02ef 54       		.byte	0x54
 4269 02f0 00000000 		.4byte	0
 4270 02f4 00000000 		.4byte	0
 4271              	.LLST18:
 4272 02f8 F4020000 		.4byte	.LVL86
 4273 02fc FC020000 		.4byte	.LVL87
 4274 0300 0100     		.2byte	0x1
 4275 0302 51       		.byte	0x51
 4276 0303 FC020000 		.4byte	.LVL87
 4277 0307 12030000 		.4byte	.LVL91
 4278 030b 0100     		.2byte	0x1
 4279 030d 55       		.byte	0x55
 4280 030e 12030000 		.4byte	.LVL91
 4281 0312 28030000 		.4byte	.LFE27
 4282 0316 0400     		.2byte	0x4
 4283 0318 F3       		.byte	0xf3
 4284 0319 01       		.uleb128 0x1
 4285 031a 51       		.byte	0x51
 4286 031b 9F       		.byte	0x9f
 4287 031c 00000000 		.4byte	0
 4288 0320 00000000 		.4byte	0
 4289              	.LLST19:
 4290 0324 F4020000 		.4byte	.LVL86
 4291 0328 12030000 		.4byte	.LVL91
 4292 032c 0200     		.2byte	0x2
 4293 032e 30       		.byte	0x30
 4294 032f 9F       		.byte	0x9f
 4295 0330 12030000 		.4byte	.LVL91
 4296 0334 14030000 		.4byte	.LVL92
 4297 0338 0800     		.2byte	0x8
 4298 033a 70       		.byte	0x70
 4299 033b 00       		.sleb128 0
 4300 033c 30       		.byte	0x30
 4301 033d 2E       		.byte	0x2e
 4302 033e 08       		.byte	0x8
 4303 033f FF       		.byte	0xff
 4304 0340 1A       		.byte	0x1a
 4305 0341 9F       		.byte	0x9f
 4306 0342 14030000 		.4byte	.LVL92
 4307 0346 28030000 		.4byte	.LFE27
 4308 034a 0800     		.2byte	0x8
ARM GAS  /tmp/ccPZc5IS.s 			page 84


 4309 034c 75       		.byte	0x75
 4310 034d 00       		.sleb128 0
 4311 034e 30       		.byte	0x30
 4312 034f 2E       		.byte	0x2e
 4313 0350 08       		.byte	0x8
 4314 0351 FF       		.byte	0xff
 4315 0352 1A       		.byte	0x1a
 4316 0353 9F       		.byte	0x9f
 4317 0354 00000000 		.4byte	0
 4318 0358 00000000 		.4byte	0
 4319              	.LLST20:
 4320 035c 28030000 		.4byte	.LVL95
 4321 0360 42030000 		.4byte	.LVL96
 4322 0364 0100     		.2byte	0x1
 4323 0366 50       		.byte	0x50
 4324 0367 42030000 		.4byte	.LVL96
 4325 036b C8030000 		.4byte	.LVL105
 4326 036f 0100     		.2byte	0x1
 4327 0371 56       		.byte	0x56
 4328 0372 C8030000 		.4byte	.LVL105
 4329 0376 CC030000 		.4byte	.LFE26
 4330 037a 0400     		.2byte	0x4
 4331 037c F3       		.byte	0xf3
 4332 037d 01       		.uleb128 0x1
 4333 037e 50       		.byte	0x50
 4334 037f 9F       		.byte	0x9f
 4335 0380 00000000 		.4byte	0
 4336 0384 00000000 		.4byte	0
 4337              	.LLST21:
 4338 0388 4E030000 		.4byte	.LVL97
 4339 038c C8030000 		.4byte	.LVL105
 4340 0390 0100     		.2byte	0x1
 4341 0392 55       		.byte	0x55
 4342 0393 00000000 		.4byte	0
 4343 0397 00000000 		.4byte	0
 4344              	.LLST22:
 4345 039b 4E030000 		.4byte	.LVL97
 4346 039f 50030000 		.4byte	.LVL98
 4347 03a3 0200     		.2byte	0x2
 4348 03a5 30       		.byte	0x30
 4349 03a6 9F       		.byte	0x9f
 4350 03a7 50030000 		.4byte	.LVL98
 4351 03ab C8030000 		.4byte	.LVL105
 4352 03af 0100     		.2byte	0x1
 4353 03b1 54       		.byte	0x54
 4354 03b2 00000000 		.4byte	0
 4355 03b6 00000000 		.4byte	0
 4356              	.LLST23:
 4357 03ba 28030000 		.4byte	.LVL95
 4358 03be 4E030000 		.4byte	.LVL97
 4359 03c2 0200     		.2byte	0x2
 4360 03c4 30       		.byte	0x30
 4361 03c5 9F       		.byte	0x9f
 4362 03c6 4E030000 		.4byte	.LVL97
 4363 03ca C8030000 		.4byte	.LVL105
 4364 03ce 0100     		.2byte	0x1
 4365 03d0 59       		.byte	0x59
ARM GAS  /tmp/ccPZc5IS.s 			page 85


 4366 03d1 00000000 		.4byte	0
 4367 03d5 00000000 		.4byte	0
 4368              	.LLST24:
 4369 03d9 28030000 		.4byte	.LVL95
 4370 03dd 4E030000 		.4byte	.LVL97
 4371 03e1 0200     		.2byte	0x2
 4372 03e3 30       		.byte	0x30
 4373 03e4 9F       		.byte	0x9f
 4374 03e5 4E030000 		.4byte	.LVL97
 4375 03e9 C8030000 		.4byte	.LVL105
 4376 03ed 0100     		.2byte	0x1
 4377 03ef 57       		.byte	0x57
 4378 03f0 00000000 		.4byte	0
 4379 03f4 00000000 		.4byte	0
 4380              	.LLST25:
 4381 03f8 7C000000 		.4byte	.LVL117
 4382 03fc 8E000000 		.4byte	.LVL118
 4383 0400 0100     		.2byte	0x1
 4384 0402 50       		.byte	0x50
 4385 0403 8E000000 		.4byte	.LVL118
 4386 0407 99000000 		.4byte	.LVL119-1
 4387 040b 0100     		.2byte	0x1
 4388 040d 51       		.byte	0x51
 4389 040e 99000000 		.4byte	.LVL119-1
 4390 0412 9A000000 		.4byte	.LVL119
 4391 0416 0100     		.2byte	0x1
 4392 0418 54       		.byte	0x54
 4393 0419 22010000 		.4byte	.LVL130
 4394 041d 26010000 		.4byte	.LVL131
 4395 0421 0100     		.2byte	0x1
 4396 0423 50       		.byte	0x50
 4397 0424 26010000 		.4byte	.LVL131
 4398 0428 2D010000 		.4byte	.LVL132-1
 4399 042c 0100     		.2byte	0x1
 4400 042e 51       		.byte	0x51
 4401 042f 30010000 		.4byte	.LVL133
 4402 0433 34010000 		.4byte	.LVL134
 4403 0437 0100     		.2byte	0x1
 4404 0439 50       		.byte	0x50
 4405 043a 34010000 		.4byte	.LVL134
 4406 043e 3B010000 		.4byte	.LVL135-1
 4407 0442 0100     		.2byte	0x1
 4408 0444 51       		.byte	0x51
 4409 0445 3E010000 		.4byte	.LVL136
 4410 0449 42010000 		.4byte	.LVL137
 4411 044d 0100     		.2byte	0x1
 4412 044f 50       		.byte	0x50
 4413 0450 42010000 		.4byte	.LVL137
 4414 0454 49010000 		.4byte	.LVL138-1
 4415 0458 0100     		.2byte	0x1
 4416 045a 51       		.byte	0x51
 4417 045b 00000000 		.4byte	0
 4418 045f 00000000 		.4byte	0
 4419              	.LLST26:
 4420 0463 9A000000 		.4byte	.LVL119
 4421 0467 9C000000 		.4byte	.LVL120
 4422 046b 0100     		.2byte	0x1
ARM GAS  /tmp/ccPZc5IS.s 			page 86


 4423 046d 54       		.byte	0x54
 4424 046e 9C000000 		.4byte	.LVL120
 4425 0472 CA000000 		.4byte	.LVL122
 4426 0476 0300     		.2byte	0x3
 4427 0478 74       		.byte	0x74
 4428 0479 7F       		.sleb128 -1
 4429 047a 9F       		.byte	0x9f
 4430 047b CA000000 		.4byte	.LVL122
 4431 047f D6000000 		.4byte	.LVL123
 4432 0483 0100     		.2byte	0x1
 4433 0485 54       		.byte	0x54
 4434 0486 EA000000 		.4byte	.LVL125
 4435 048a 22010000 		.4byte	.LVL130
 4436 048e 0300     		.2byte	0x3
 4437 0490 74       		.byte	0x74
 4438 0491 7F       		.sleb128 -1
 4439 0492 9F       		.byte	0x9f
 4440 0493 00000000 		.4byte	0
 4441 0497 00000000 		.4byte	0
 4442              		.section	.debug_aranges,"",%progbits
 4443 0000 24000000 		.4byte	0x24
 4444 0004 0200     		.2byte	0x2
 4445 0006 00000000 		.4byte	.Ldebug_info0
 4446 000a 04       		.byte	0x4
 4447 000b 00       		.byte	0
 4448 000c 0000     		.2byte	0
 4449 000e 0000     		.2byte	0
 4450 0010 00000000 		.4byte	.Ltext0
 4451 0014 CC030000 		.4byte	.Letext0-.Ltext0
 4452 0018 00000000 		.4byte	.LFB23
 4453 001c 4C010000 		.4byte	.LFE23-.LFB23
 4454 0020 00000000 		.4byte	0
 4455 0024 00000000 		.4byte	0
 4456              		.section	.debug_ranges,"",%progbits
 4457              	.Ldebug_ranges0:
 4458 0000 90020000 		.4byte	.LBB4
 4459 0004 98020000 		.4byte	.LBE4
 4460 0008 9A020000 		.4byte	.LBB7
 4461 000c 9C020000 		.4byte	.LBE7
 4462 0010 00000000 		.4byte	0
 4463 0014 00000000 		.4byte	0
 4464 0018 50030000 		.4byte	.LBB8
 4465 001c 66030000 		.4byte	.LBE8
 4466 0020 88030000 		.4byte	.LBB9
 4467 0024 A6030000 		.4byte	.LBE9
 4468 0028 00000000 		.4byte	0
 4469 002c 00000000 		.4byte	0
 4470 0030 00000000 		.4byte	.Ltext0
 4471 0034 CC030000 		.4byte	.Letext0
 4472 0038 00000000 		.4byte	.LFB23
 4473 003c 4C010000 		.4byte	.LFE23
 4474 0040 00000000 		.4byte	0
 4475 0044 00000000 		.4byte	0
 4476              		.section	.debug_line,"",%progbits
 4477              	.Ldebug_line0:
 4478 0000 EC010000 		.section	.debug_str,"MS",%progbits,1
 4478      0200D500 
ARM GAS  /tmp/ccPZc5IS.s 			page 87


 4478      00000201 
 4478      FB0E0D00 
 4478      01010101 
 4479              	.LASF106:
 4480 0000 7072696E 		.ascii	"printf\000"
 4480      746600
 4481              	.LASF11:
 4482 0007 5F5F6F66 		.ascii	"__off_t\000"
 4482      665F7400 
 4483              	.LASF20:
 4484 000f 5F494F5F 		.ascii	"_IO_read_ptr\000"
 4484      72656164 
 4484      5F707472 
 4484      00
 4485              	.LASF32:
 4486 001c 5F636861 		.ascii	"_chain\000"
 4486      696E00
 4487              	.LASF87:
 4488 0023 74657374 		.ascii	"test_pins\000"
 4488      5F70696E 
 4488      7300
 4489              	.LASF82:
 4490 002d 73686F72 		.ascii	"shorts\000"
 4490      747300
 4491              	.LASF9:
 4492 0034 73697A65 		.ascii	"size_t\000"
 4492      5F7400
 4493              	.LASF38:
 4494 003b 5F73686F 		.ascii	"_shortbuf\000"
 4494      72746275 
 4494      6600
 4495              	.LASF70:
 4496 0045 64697265 		.ascii	"direction\000"
 4496      6374696F 
 4496      6E00
 4497              	.LASF26:
 4498 004f 5F494F5F 		.ascii	"_IO_buf_base\000"
 4498      6275665F 
 4498      62617365 
 4498      00
 4499              	.LASF88:
 4500 005c 74696D65 		.ascii	"time_des_test\000"
 4500      5F646573 
 4500      5F746573 
 4500      7400
 4501              	.LASF8:
 4502 006a 6C6F6E67 		.ascii	"long long unsigned int\000"
 4502      206C6F6E 
 4502      6720756E 
 4502      7369676E 
 4502      65642069 
 4503              	.LASF73:
 4504 0081 74696D65 		.ascii	"time_des\000"
 4504      5F646573 
 4504      00
 4505              	.LASF94:
 4506 008a 636F6D70 		.ascii	"componentID\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 88


 4506      6F6E656E 
 4506      74494400 
 4507              	.LASF89:
 4508 0096 70726F62 		.ascii	"problem\000"
 4508      6C656D00 
 4509              	.LASF79:
 4510 009e 64657465 		.ascii	"detectProblems\000"
 4510      63745072 
 4510      6F626C65 
 4510      6D7300
 4511              	.LASF7:
 4512 00ad 6C6F6E67 		.ascii	"long long int\000"
 4512      206C6F6E 
 4512      6720696E 
 4512      7400
 4513              	.LASF5:
 4514 00bb 7369676E 		.ascii	"signed char\000"
 4514      65642063 
 4514      68617200 
 4515              	.LASF101:
 4516 00c7 5F5F6275 		.ascii	"__builtin_fwrite\000"
 4516      696C7469 
 4516      6E5F6677 
 4516      72697465 
 4516      00
 4517              	.LASF56:
 4518 00d8 616E5F66 		.ascii	"an_file\000"
 4518      696C6500 
 4519              	.LASF83:
 4520 00e0 74657374 		.ascii	"test_gpios\000"
 4520      5F677069 
 4520      6F7300
 4521              	.LASF33:
 4522 00eb 5F66696C 		.ascii	"_fileno\000"
 4522      656E6F00 
 4523              	.LASF21:
 4524 00f3 5F494F5F 		.ascii	"_IO_read_end\000"
 4524      72656164 
 4524      5F656E64 
 4524      00
 4525              	.LASF12:
 4526 0100 6C6F6E67 		.ascii	"long int\000"
 4526      20696E74 
 4526      00
 4527              	.LASF19:
 4528 0109 5F666C61 		.ascii	"_flags\000"
 4528      677300
 4529              	.LASF27:
 4530 0110 5F494F5F 		.ascii	"_IO_buf_end\000"
 4530      6275665F 
 4530      656E6400 
 4531              	.LASF36:
 4532 011c 5F637572 		.ascii	"_cur_column\000"
 4532      5F636F6C 
 4532      756D6E00 
 4533              	.LASF10:
 4534 0128 5F5F7175 		.ascii	"__quad_t\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 89


 4534      61645F74 
 4534      00
 4535              	.LASF0:
 4536 0131 646F7562 		.ascii	"double\000"
 4536      6C6500
 4537              	.LASF69:
 4538 0138 73657444 		.ascii	"setDirGPIO\000"
 4538      69724750 
 4538      494F00
 4539              	.LASF35:
 4540 0143 5F6F6C64 		.ascii	"_old_offset\000"
 4540      5F6F6666 
 4540      73657400 
 4541              	.LASF40:
 4542 014f 5F6F6666 		.ascii	"_offset\000"
 4542      73657400 
 4543              	.LASF61:
 4544 0157 70696E5F 		.ascii	"pin_number\000"
 4544      6E756D62 
 4544      657200
 4545              	.LASF108:
 4546 0162 73746167 		.ascii	"stage1_test.c\000"
 4546      65315F74 
 4546      6573742E 
 4546      6300
 4547              	.LASF62:
 4548 0170 76616C75 		.ascii	"value\000"
 4548      6500
 4549              	.LASF49:
 4550 0176 5F494F5F 		.ascii	"_IO_marker\000"
 4550      6D61726B 
 4550      657200
 4551              	.LASF90:
 4552 0181 73746469 		.ascii	"stdin\000"
 4552      6E00
 4553              	.LASF1:
 4554 0187 756E7369 		.ascii	"unsigned int\000"
 4554      676E6564 
 4554      20696E74 
 4554      00
 4555              	.LASF81:
 4556 0194 6F70656E 		.ascii	"opens\000"
 4556      7300
 4557              	.LASF99:
 4558 019a 66707269 		.ascii	"fprintf\000"
 4558      6E746600 
 4559              	.LASF4:
 4560 01a2 6C6F6E67 		.ascii	"long unsigned int\000"
 4560      20756E73 
 4560      69676E65 
 4560      6420696E 
 4560      7400
 4561              	.LASF63:
 4562 01b4 665F6770 		.ascii	"f_gpio\000"
 4562      696F00
 4563              	.LASF24:
 4564 01bb 5F494F5F 		.ascii	"_IO_write_ptr\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 90


 4564      77726974 
 4564      655F7074 
 4564      7200
 4565              	.LASF51:
 4566 01c9 5F736275 		.ascii	"_sbuf\000"
 4566      6600
 4567              	.LASF3:
 4568 01cf 73686F72 		.ascii	"short unsigned int\000"
 4568      7420756E 
 4568      7369676E 
 4568      65642069 
 4568      6E7400
 4569              	.LASF109:
 4570 01e2 2F686F6D 		.ascii	"/home/test\000"
 4570      652F7465 
 4570      737400
 4571              	.LASF28:
 4572 01ed 5F494F5F 		.ascii	"_IO_save_base\000"
 4572      73617665 
 4572      5F626173 
 4572      6500
 4573              	.LASF111:
 4574 01fb 5F5F6973 		.ascii	"__isoc99_fscanf\000"
 4574      6F633939 
 4574      5F667363 
 4574      616E6600 
 4575              	.LASF102:
 4576 020b 6E616E6F 		.ascii	"nanosleep\000"
 4576      736C6565 
 4576      7000
 4577              	.LASF39:
 4578 0215 5F6C6F63 		.ascii	"_lock\000"
 4578      6B00
 4579              	.LASF95:
 4580 021b 73707269 		.ascii	"sprintf\000"
 4580      6E746600 
 4581              	.LASF34:
 4582 0223 5F666C61 		.ascii	"_flags2\000"
 4582      67733200 
 4583              	.LASF46:
 4584 022b 5F6D6F64 		.ascii	"_mode\000"
 4584      6500
 4585              	.LASF96:
 4586 0231 73747263 		.ascii	"strcat\000"
 4586      617400
 4587              	.LASF103:
 4588 0238 5F5F6275 		.ascii	"__builtin_puts\000"
 4588      696C7469 
 4588      6E5F7075 
 4588      747300
 4589              	.LASF91:
 4590 0247 7374646F 		.ascii	"stdout\000"
 4590      757400
 4591              	.LASF80:
 4592 024e 636F6D70 		.ascii	"components\000"
 4592      6F6E656E 
 4592      747300
ARM GAS  /tmp/ccPZc5IS.s 			page 91


 4593              	.LASF75:
 4594 0259 64657465 		.ascii	"detectShort\000"
 4594      63745368 
 4594      6F727400 
 4595              	.LASF55:
 4596 0265 74765F6E 		.ascii	"tv_nsec\000"
 4596      73656300 
 4597              	.LASF14:
 4598 026d 73697A65 		.ascii	"sizetype\000"
 4598      74797065 
 4598      00
 4599              	.LASF67:
 4600 0276 6770696F 		.ascii	"gpio_value\000"
 4600      5F76616C 
 4600      756500
 4601              	.LASF78:
 4602 0281 73686F72 		.ascii	"shorted\000"
 4602      74656400 
 4603              	.LASF54:
 4604 0289 74765F73 		.ascii	"tv_sec\000"
 4604      656300
 4605              	.LASF16:
 4606 0290 5F5F7379 		.ascii	"__syscall_slong_t\000"
 4606      7363616C 
 4606      6C5F736C 
 4606      6F6E675F 
 4606      7400
 4607              	.LASF25:
 4608 02a2 5F494F5F 		.ascii	"_IO_write_end\000"
 4608      77726974 
 4608      655F656E 
 4608      6400
 4609              	.LASF68:
 4610 02b0 77726974 		.ascii	"writeGPIO\000"
 4610      65475049 
 4610      4F00
 4611              	.LASF110:
 4612 02ba 5F494F5F 		.ascii	"_IO_lock_t\000"
 4612      6C6F636B 
 4612      5F7400
 4613              	.LASF48:
 4614 02c5 5F494F5F 		.ascii	"_IO_FILE\000"
 4614      46494C45 
 4614      00
 4615              	.LASF64:
 4616 02ce 66696C65 		.ascii	"file_end\000"
 4616      5F656E64 
 4616      00
 4617              	.LASF76:
 4618 02d7 6770696F 		.ascii	"gpio1\000"
 4618      3100
 4619              	.LASF77:
 4620 02dd 6770696F 		.ascii	"gpio2\000"
 4620      3200
 4621              	.LASF71:
 4622 02e3 74657374 		.ascii	"testComponent\000"
 4622      436F6D70 
ARM GAS  /tmp/ccPZc5IS.s 			page 92


 4622      6F6E656E 
 4622      7400
 4623              	.LASF97:
 4624 02f1 666F7065 		.ascii	"fopen\000"
 4624      6E00
 4625              	.LASF52:
 4626 02f7 5F706F73 		.ascii	"_pos\000"
 4626      00
 4627              	.LASF31:
 4628 02fc 5F6D6172 		.ascii	"_markers\000"
 4628      6B657273 
 4628      00
 4629              	.LASF2:
 4630 0305 756E7369 		.ascii	"unsigned char\000"
 4630      676E6564 
 4630      20636861 
 4630      7200
 4631              	.LASF58:
 4632 0313 6368616E 		.ascii	"channel_str\000"
 4632      6E656C5F 
 4632      73747200 
 4633              	.LASF6:
 4634 031f 73686F72 		.ascii	"short int\000"
 4634      7420696E 
 4634      7400
 4635              	.LASF86:
 4636 0329 636F756E 		.ascii	"counter\000"
 4636      74657200 
 4637              	.LASF53:
 4638 0331 74696D65 		.ascii	"timespec\000"
 4638      73706563 
 4638      00
 4639              	.LASF37:
 4640 033a 5F767461 		.ascii	"_vtable_offset\000"
 4640      626C655F 
 4640      6F666673 
 4640      657400
 4641              	.LASF18:
 4642 0349 46494C45 		.ascii	"FILE\000"
 4642      00
 4643              	.LASF60:
 4644 034e 6368616E 		.ascii	"channel\000"
 4644      6E656C00 
 4645              	.LASF100:
 4646 0356 66736361 		.ascii	"fscanf\000"
 4646      6E6600
 4647              	.LASF59:
 4648 035d 6164635F 		.ascii	"adc_value\000"
 4648      76616C75 
 4648      6500
 4649              	.LASF107:
 4650 0367 474E5520 		.ascii	"GNU C 4.9.2 -march=armv7-a -mfloat-abi=hard -mfpu=v"
 4650      4320342E 
 4650      392E3220 
 4650      2D6D6172 
 4650      63683D61 
 4651 039a 66707633 		.ascii	"fpv3-d16 -mthumb -mtls-dialect=gnu -g -O2\000"
ARM GAS  /tmp/ccPZc5IS.s 			page 93


 4651      2D643136 
 4651      202D6D74 
 4651      68756D62 
 4651      202D6D74 
 4652              	.LASF17:
 4653 03c4 63686172 		.ascii	"char\000"
 4653      00
 4654              	.LASF98:
 4655 03c9 66636C6F 		.ascii	"fclose\000"
 4655      736500
 4656              	.LASF50:
 4657 03d0 5F6E6578 		.ascii	"_next\000"
 4657      7400
 4658              	.LASF13:
 4659 03d6 5F5F6F66 		.ascii	"__off64_t\000"
 4659      6636345F 
 4659      7400
 4660              	.LASF22:
 4661 03e0 5F494F5F 		.ascii	"_IO_read_base\000"
 4661      72656164 
 4661      5F626173 
 4661      6500
 4662              	.LASF92:
 4663 03ee 6D656D63 		.ascii	"memcpy\000"
 4663      707900
 4664              	.LASF30:
 4665 03f5 5F494F5F 		.ascii	"_IO_save_end\000"
 4665      73617665 
 4665      5F656E64 
 4665      00
 4666              	.LASF66:
 4667 0402 72656164 		.ascii	"readGPIO\000"
 4667      4750494F 
 4667      00
 4668              	.LASF41:
 4669 040b 5F5F7061 		.ascii	"__pad1\000"
 4669      643100
 4670              	.LASF42:
 4671 0412 5F5F7061 		.ascii	"__pad2\000"
 4671      643200
 4672              	.LASF43:
 4673 0419 5F5F7061 		.ascii	"__pad3\000"
 4673      643300
 4674              	.LASF44:
 4675 0420 5F5F7061 		.ascii	"__pad4\000"
 4675      643400
 4676              	.LASF45:
 4677 0427 5F5F7061 		.ascii	"__pad5\000"
 4677      643500
 4678              	.LASF15:
 4679 042e 5F5F7469 		.ascii	"__time_t\000"
 4679      6D655F74 
 4679      00
 4680              	.LASF47:
 4681 0437 5F756E75 		.ascii	"_unused2\000"
 4681      73656432 
 4681      00
ARM GAS  /tmp/ccPZc5IS.s 			page 94


 4682              	.LASF84:
 4683 0440 70696E73 		.ascii	"pins\000"
 4683      00
 4684              	.LASF93:
 4685 0445 6D656D73 		.ascii	"memset\000"
 4685      657400
 4686              	.LASF65:
 4687 044c 72656164 		.ascii	"readADC\000"
 4687      41444300 
 4688              	.LASF74:
 4689 0454 636F6D70 		.ascii	"component\000"
 4689      6F6E656E 
 4689      7400
 4690              	.LASF29:
 4691 045e 5F494F5F 		.ascii	"_IO_backup_base\000"
 4691      6261636B 
 4691      75705F62 
 4691      61736500 
 4692              	.LASF72:
 4693 046e 6770696F 		.ascii	"gpio\000"
 4693      00
 4694              	.LASF104:
 4695 0473 66777269 		.ascii	"fwrite\000"
 4695      746500
 4696              	.LASF105:
 4697 047a 70757473 		.ascii	"puts\000"
 4697      00
 4698              	.LASF85:
 4699 047f 6D61696E 		.ascii	"main\000"
 4699      00
 4700              	.LASF23:
 4701 0484 5F494F5F 		.ascii	"_IO_write_base\000"
 4701      77726974 
 4701      655F6261 
 4701      736500
 4702              	.LASF57:
 4703 0493 66696C65 		.ascii	"file_path\000"
 4703      5F706174 
 4703      6800
 4704              		.ident	"GCC: (Debian 4.9.2-10) 4.9.2"
 4705              		.section	.note.GNU-stack,"",%progbits
ARM GAS  /tmp/ccPZc5IS.s 			page 95


DEFINED SYMBOLS
                            *ABS*:00000000 stage1_test.c
     /tmp/ccPZc5IS.s:20     .text:00000000 $t
     /tmp/ccPZc5IS.s:24     .text:00000000 componentID.part.0
     /tmp/ccPZc5IS.s:78     .text:00000034 componentID
     /tmp/ccPZc5IS.s:104    .text:00000040 readADC
     /tmp/ccPZc5IS.s:1062   .rodata:00000000 .LANCHOR0
     /tmp/ccPZc5IS.s:1079   .rodata.str1.4:00000000 .LC2
     /tmp/ccPZc5IS.s:1082   .rodata.str1.4:00000008 .LC3
     /tmp/ccPZc5IS.s:1085   .rodata.str1.4:0000000c .LC4
     /tmp/ccPZc5IS.s:187    .text:000000c8 $d
     /tmp/ccPZc5IS.s:194    .text:000000d8 $t
     /tmp/ccPZc5IS.s:199    .text:000000d8 writeGPIO
     /tmp/ccPZc5IS.s:1088   .rodata.str1.4:00000010 .LC6
     /tmp/ccPZc5IS.s:1091   .rodata.str1.4:0000001c .LC7
     /tmp/ccPZc5IS.s:274    .text:00000140 $d
     /tmp/ccPZc5IS.s:278    .text:00000144 $t
     /tmp/ccPZc5IS.s:283    .text:00000144 readGPIO
     /tmp/ccPZc5IS.s:357    .text:000001ac $d
     /tmp/ccPZc5IS.s:361    .text:000001b0 $t
     /tmp/ccPZc5IS.s:366    .text:000001b0 setDirGPIO
     /tmp/ccPZc5IS.s:1094   .rodata.str1.4:00000020 .LC8
     /tmp/ccPZc5IS.s:1100   .rodata.str1.4:00000034 .LC10
     /tmp/ccPZc5IS.s:1097   .rodata.str1.4:00000030 .LC9
     /tmp/ccPZc5IS.s:466    .text:00000238 $d
     /tmp/ccPZc5IS.s:470    .text:0000023c $t
     /tmp/ccPZc5IS.s:475    .text:0000023c testComponent
     /tmp/ccPZc5IS.s:629    .text:000002f4 detectShort
     /tmp/ccPZc5IS.s:687    .text:00000328 detectProblems
     /tmp/ccPZc5IS.s:818    .text:000003c8 $d
     /tmp/ccPZc5IS.s:823    .text.startup:00000000 $t
     /tmp/ccPZc5IS.s:828    .text.startup:00000000 main
     /tmp/ccPZc5IS.s:1102   .rodata.str1.4:00000038 .LC11
     /tmp/ccPZc5IS.s:1113   .rodata.str1.4:000000cc .LC15
     /tmp/ccPZc5IS.s:1115   .rodata.str1.4:000000e0 .LC16
     /tmp/ccPZc5IS.s:965    .text.startup:000000b8 $d
     /tmp/ccPZc5IS.s:1127   .rodata.str1.4:00000108 .LC20
     /tmp/ccPZc5IS.s:1133   .rodata.str1.4:0000011c .LC22
     /tmp/ccPZc5IS.s:1124   .rodata.str1.4:000000fc .LC19
     /tmp/ccPZc5IS.s:1121   .rodata.str1.4:000000f0 .LC18
     /tmp/ccPZc5IS.s:1118   .rodata.str1.4:000000e8 .LC17
     /tmp/ccPZc5IS.s:1130   .rodata.str1.4:00000114 .LC21
     /tmp/ccPZc5IS.s:1105   .rodata.str1.4:00000054 .LC12
     /tmp/ccPZc5IS.s:1108   .rodata.str1.4:00000090 .LC13
     /tmp/ccPZc5IS.s:1110   .rodata.str1.4:000000ac .LC14
     /tmp/ccPZc5IS.s:1061   .rodata:00000000 $d
     /tmp/ccPZc5IS.s:1078   .rodata.str1.4:00000000 $d
                     .debug_frame:00000010 $d
     /tmp/ccPZc5IS.s:1144   .text:000003cc $t
     /tmp/ccPZc5IS.s:970    .text.startup:000000bd $d
     /tmp/ccPZc5IS.s:970    .text.startup:000000be $t

UNDEFINED SYMBOLS
memcpy
memset
sprintf
strcat
ARM GAS  /tmp/ccPZc5IS.s 			page 96


fopen
__isoc99_fscanf
fclose
fprintf
fwrite
nanosleep
printf
puts
