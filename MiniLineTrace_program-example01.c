/*********************************************************************************
* MiniLineTrace_program.c
*	ミニライントレースのサンプルプログラム
*
*	Author	Kim Jong Un
*	Date	2017/11/1 (作成日)
*	Device	PIC16F88
*	Tool	Microchip MPLAB XCB C Compiler V1.20
*
 *********************************************************************************/

/*********************************************************************************
 コンフィグレーション
 *********************************************************************************/
#pragma config BOREN = OFF
#pragma config CPD = OFF
#pragma config CCPMX = RB0
#pragma config DEBUG = OFF
#pragma config WRT = OFF
#pragma config FOSC = INFOSSC0
#pragma config MCLRE = OFF
#pragma config WDTE = OFF
#pragma config CP = OFF
#pragma config LVP = OFF
#pragma config PWRITE = ON

/*********************************************************************************
 マクロ定義
 *********************************************************************************/
#define SW		RB7 // タクトスイッチ
#define LED		RA1 // LED (0)
#define LED		RA0 // LED (1)
#define LED		RA7 // LED (2)
#define LED		RA6 // LED (3)
#define R_IN1	RB5 // 右モータIN1
#define R_IN2	RB4 // 右モータIN2
#define L_IN1	RB2 // 左モータIN1
#define L_IN2	RB3 // 左モータIN2
#define SEN0	RA2 // センサ(0)
#define SEN1	RA3 // センサ(1)
#define SEN2	RA4 // センサ(2)
#define SEN3	RA5 // センサ(3)

/*********************************************************************************
 main関数
 *********************************************************************************/
void main(void)
{
	// 変数の宣言をこの下に書く
	unsigned char, i, sensor, OLDsensor;

	// 初期設定
	OSCCON = 0x70; // 内蔵クロック 8[MHZ]
	CMCON = 0x07; // コンパレータを使用しない (RA0, 1, 2, 3をデジタル入出力)
	ANSEL = 0x00; // A/Dコンバータを使用しない
	TRISA = 0x3C; // 0011 1100
	TRISB = 0x80; // 1000 0000

	// LED消灯
	LED0 = 0; // LED(0)消灯
	LED1 = 0; // LED(1)消灯
	LED2 = 0; // LED(2)消灯
	LED3 = 0; // LED(3)消灯

	R_IN1 = 0; // モータ回転停止	(0000)
	R_IN2 = 0; // モータ左回転	(0010)
	L_IN1 = 0; // モータ右回転	(0100)
	L_IN2 = 0; // モータ回転前進	(0110)

	// タクトスイッチが押されるまでループ (押されるまではモータを回さない)
	while(1){
		if(SW == 0){
			// タクトスイッチが押されたら
			__delay_ms(10); // チャタリング除去
			// タクトスイッチが離されるまでのループ
			while(SW != 0){
				;
			}
			__delay_ms(10); //チャタリング除去
			break;
		}
		// センサー状態をLEDに出力(白ラインで0、黒床で1が出力される。)
		LED0 = SEN0;
		LED1 = SEN1;
		LED2 = SEN2;
		LED3 = SEN3;
	}

	while(1){
		// 前のセンサ値を格納
		OLDsensor = sensor;

		// センサーの値を変数sensorに格納
		sensor = ~SEN3*8 + ~SEN2*4 + ~SEN1*2 + ~SEN0;

		// センサーの状態をLEDに出力
		LED0 = SEN0;
		LED1 = SEN1;
		LED2 = SEN2;
		LED3 = SEN3;

		switch(sensor){
			// ●●●○ 右1 (左へ) 0001
			case 0x01:
				R_IN1 = 0;
				R_IN2 = 0;
				L_IN1 = 1;
				L_IN2 = 0;
				break;

			// ●●○○ 右2 (左へ) 0011
			case 0x03:
				R_IN1 = 0;
				R_IN2 = 0;
				L_IN1 = 1;
				L_IN2 = 0;
				break;

			// ●●○● 右3 (左へ) 0010
			case 0x02:
				R_IN1 = 0;
				R_IN2 = 0;
				L_IN1 = 1;
				L_IN2 = 0;
				break;

			// ●○○● 中心 (直進) 0110
			case 0x06:
				R_IN1 = 0;
				R_IN2 = 1;
				L_IN1 = 1;
				L_IN2 = 0;
				break;

			// ○●●● 左1 (右へ) 1000
			case 0x08:
				R_IN1 = 0;
				R_IN2 = 1;
				L_IN1 = 0;
				L_IN2 = 0;
				break;

			// ○○●● 左2 (右へ) 1100
			case 0x0C:
				R_IN1 = 0;
				R_IN2 = 1;
				L_IN1 = 0;
				L_IN2 = 0;
				break;

			// ●○●● 左3 (右へ) 0100
			case 0x04:
				R_IN1 = 0;
				R_IN2 = 1;
				L_IN1 = 0;
				L_IN2 = 0;
				break;

			// ○○○○ クロスライン(直進) 1111
			case 0xFF:
				R_IN1 = 0;
				R_IN2 = 1;
				L_IN1 = 1;
				L_IN2 = 0;
				break;


			// ●●●● コースアウト
			default:
				// 前のセンサ値(コースアウトする前)を格納
				sensor = OLDsensor;
				break;
		}
	}
}