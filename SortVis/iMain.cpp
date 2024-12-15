#include "iGraphics.h"
#include<stdlib.h>
#include<time.h>
char s[1000]; int sindex =0;
/*
	function iDraw() is called again and again by the system.

	*/
int a[1000] = {9, 7,1,5,5, 12, 3,15,21, 8};
//10
//9 7 1 5 5 12 3 15 21 8
int n = 10;
int it_i = 0, it_j = -1, min, rl=0, rr=1000, piv = -1;
int tn =0, tbegin = 0;
int *t;

char logtxt[300] = {0};
int swapFrames = 0;
int colourState = 0;
int vSwapFrames = 0;
int InputState = 0;
int RestartReq = 0;
int PauseReq = 0;
int bufferReady = 0;
int SaveReady = 0;
int doRand = 0;
int mousePosX = 1020, mousePosY;
int ActveArea[4] = {0};
int tmpR[5], tmpG[5], tmpB[5];
double SPEED = 1.0, tmpSp;

typedef struct{
	int R;
	int G;
	int B;
}colour;
colour Bars[5];

enum Algo {Bubble, Selection, Quick, Merge};
enum state {RUNNING, IDLE};
enum state exec = IDLE;
enum state setting = IDLE;
enum Algo currAlgo;
enum state pause = IDLE;
enum state Menu = RUNNING;
enum state bg = RUNNING;

int isPaused;
void (*CallFunc)(void)={0};
UINT_PTR TimeRef;
static void  __stdcall iA(HWND,unsigned int, unsigned int, unsigned long){if(!isPaused)CallFunc();}

void formatArray(int a[], int n); //size of bar
void arrayRect(double a[], int n); //visualisation
void highlight(); //iteration
void swap();
void Vswap();
void arrayInp();
void Settings();
void CreateTimer(int msec, void (*f)(void));
void Pause();
void Resume();
void KillTime();
void Instructions();
void Restart(int x);
void pausRes();
void Logic(char opc);
void StartMenu();
FILE* source;
double show[1000], tshow[1000];

void CreateTimer(int msec, void (*f)(void)){
	CallFunc = f;
	isPaused = 0;
	TimeRef = SetTimer(0,0,msec, iA);
}
void Pause(){
	isPaused = 1;
}
void Resume(){
	isPaused = 0;
}
void KillTime(){
	KillTimer(0,TimeRef);
}

void pausRes(){
	if(!vSwapFrames && !swapFrames){
		PauseReq = 0;
		if(pause == IDLE){Pause(); pause = RUNNING;}
		else {Resume(); pause = IDLE;}
	}else PauseReq = 1;
}

void StartMenu(){
	iShowBMP(270,200,"bg.bmp");
	iShowBMP2(510,440,"logo.bmp",0xFFFFFF);
	iSetColor(255,255,255);
	iText(500,400,"S O R T I N G",GLUT_BITMAP_9_BY_15);
	iText(570,350,"A L G O R I T H M ",GLUT_BITMAP_9_BY_15);
	iText(640,300,"V I S U A L I S E R",GLUT_BITMAP_9_BY_15);
	iRectangle(20,30,420,690);
	iRectangle(450,30,430,690);
	iRectangle(890,30,400,690);
	iRectangle(590,130,150,50);
	iText(120,690,"I N S T R U C T I O N S", GLUT_BITMAP_9_BY_15);
	iText(40,620,"All Instructions can be found at the upper-left ");
	iText(100,590,"corner of the corresponding window");
	iText(1050,690,"A B O U T", GLUT_BITMAP_9_BY_15);
	iText(640,150,"START",GLUT_BITMAP_9_BY_15);

	iText(140, 510,"M A I N    M E N U");
	iText(110, 470, "Press B to edit array");
	iText(110, 440, "Press E to start sorting");
	iText(110, 410, "Press P to pause/resume");
	iText(110, 380, "Press E to restart");
	iText(110, 350, "Press X to stop and reset");
	iText(140, 250,"E D I T    M E N U");
	iText(110, 200, "Press B to reset array");
	iText(110, 170, "Press R to randomise values");
	iText(110, 140, "Press ENTER to continue");

	iText(940,500,"SORTING ALGORITHM VISUALISER IS DUE TO");
	iText(1030,420,"M AHSAF ABID",GLUT_BITMAP_HELVETICA_18);
	iText(1060,390,"2305026",GLUT_BITMAP_HELVETICA_18);
	iText(970,280,"CRAFTED UNDER THE SUPERVISION OF");
	iText(970,220,"ABDUR RASHID TUSHAR SIR",GLUT_BITMAP_HELVETICA_18);
}
void iDraw() {
	//place your drawing codes here
	iClear();
	if(Menu == RUNNING) StartMenu();
	else{
	if(RestartReq) Restart(RestartReq);
	if(PauseReq) pausRes();
	if(InputState){
		arrayInp();
		//iRectangle(60, 500, 800, 170); // input rect
	} 
	else{
		if(bg==RUNNING) iShowBMP(0,0,"waves.bmp");
		if(setting == RUNNING) Settings();
		else{
			iSetColor(255,255,255);
			char tabf[] = "PseudoSort0.txt";
			tabf[10] = currAlgo+49;
			FILE* pseudo = fopen(tabf,"r");
			char codedis[300], dis[300];
			int cnt = 0;
			while (fgets(codedis, 300, pseudo) != NULL){
				codedis[strcspn(codedis, "\n")] = '\0';
				iText(885,640-cnt*20,codedis, GLUT_BITMAP_9_BY_15);
				cnt++;
			}
			fclose(pseudo);
			iFilledRectangle(885,200,390,2);
			iText(885,180,"CURRENT PROCESS:");
				iText(895,160,logtxt,GLUT_BITMAP_9_BY_15);
			
		}
		if(currAlgo == 0) iSetColor(255,255,0);
		iText(350,700,"BUBBLE SORT");
		iRectangle(330, 690, 130, 30);
		iSetColor(255,255,255);
		

		if(currAlgo == 1) iSetColor(255,255,0);
		iText(510,700,"SELECTION SORT");
		iRectangle(500, 690, 130, 30);
		iSetColor(255,255,255);

		if(currAlgo == 2) iSetColor(255,255,0);
		iText(695,700,"QUICK SORT");
		iRectangle(670, 690, 130, 30);
		iSetColor(255,255,255);

		if(currAlgo == 3) iSetColor(255,255,0);
		iText(865,700,"MERGE SORT");
		iRectangle(840, 690, 130, 30);
		iSetColor(255,255,255);
		
		if(setting == RUNNING) iSetColor(255,255,0);
		iText(1190,700,"SETTINGS");
		iRectangle(1160, 690, 130, 30);
		iSetColor(255,255,255);

		iRectangle(60, 30, 800, 450); //bar rect
		iRectangle(60, 500, 800, 170); // input rect
		iRectangle(870,30, 420, 640); // pseudocode rect
		formatArray(a,n);
		arrayRect(show,n);
		if(swapFrames) swap();
		if(vSwapFrames) Vswap();
		//iText(40, 40, "Hi, I am iGraphics");
	}
	iSetColor(255,255,255);
	Instructions();
	}

}
void formatArray(int a[], int n){
	int m = a[0],i;
	for(i=0; i<n; i++) m = (m<a[i]) ? a[i] : m;
	for(i=0; i<n; i++){
		show[i] = (405.0*a[i])/m;
	}

	if(tn){
		for(i=0; i<tn; i++){
			tshow[i] = (160.0*t[i])/m;
		}
	}
}
void arrayRect(double a[], int n){
	iSetColor(255,255,255);
	double width = (795.0/n);
	int i;
	for(i=0; i<n; i++){
		if(swapFrames || vSwapFrames) iSetColor(Bars[0].R/5.0,Bars[0].G/5.0,Bars[0].B/5.0);
		else if(i< rl || i> rr) iSetColor(Bars[4].R,Bars[4].G,Bars[4].B); 
		else if(i == min) iSetColor(Bars[1].R,Bars[1].G,Bars[1].B);
		else if(i == piv) iSetColor(Bars[3].R,Bars[3].G,Bars[3].B);
		else if(i==it_j) iSetColor(Bars[2].R,Bars[2].G,Bars[2].B);
		else iSetColor(Bars[0].R,Bars[0].G,Bars[0].B);
		iFilledRectangle(65.0+(width*i), 35, (width-10.0), a[i]);
	}
	for(i=0; i<tn; i++){
		iSetColor(255, 165, 0);
		iFilledRectangle(65.0+(width*(i+tbegin)), 505, (width-10.0), tshow[i]);
	}
	

}
void highlight(){
	char opcomm;
	fscanf(source, "%c", &opcomm);
	if(opcomm == 'c') fscanf(source, "%d %d", &min, &it_j);
	else if(opcomm == 's') fscanf(source, "%d %d", &it_i, &min);
	else if(opcomm == 'h' || opcomm == 'r') fscanf(source, "%d %d", &rl, &rr);
	else if(opcomm == 'z') {rl = n+1; rr = -1; it_j = -1; min = -1; exec = IDLE;}
	else if(opcomm == 'p') fscanf(source,"%d",&piv);
	else if(opcomm == 'b'){
		fscanf(source, "%d %d", &tn, &tbegin);
		t = (int*) malloc(tn*sizeof(int));
		memset(t, 0, tn*sizeof(int));
	}else if(opcomm == 'i'){
		int ti, tV;
		fscanf(source, "%d %d",&ti, &tV);
		*(t+ti) = tV;
	}else if(opcomm == 'd'){
		int I;
		fscanf(source, "%d", &tbegin);
		vSwapFrames++;
	}
		if(opcomm=='s'){
			swapFrames = 1;
		}
		Logic(opcomm);
		if(it_i==n){
			Pause();
			fclose(source);
			return;
		}
}
void Logic(char opc){
	if(opc == 'c') sprintf(logtxt,"Compare between %d and %d  %d <= %d",a[it_j], a[min],a[min], a[it_j]);
	if(opc == 's') sprintf(logtxt,"Swap the values %d and %d",a[it_i], a[min]);
	if(opc == 'p') sprintf(logtxt,"Move PivotIndex to Index %d",piv);
	if(opc == 'd') sprintf(logtxt,"Copy values from temp array",piv);
	if(opc == 'i') sprintf(logtxt,"Insert smaller value to Temp",piv);
	if(opc == 'h' && currAlgo >=2) sprintf(logtxt,"Move to (%d,%d) partition",rl,rr);
	if(opc == 'r' && currAlgo >=2) sprintf(logtxt,"Backtrack to (%d,%d)",rl,rr);
	if(opc == 'z') sprintf(logtxt,"Sorting Complete!");
}

void swap(){
	Pause();
	double width = (795.0/n);
	double ini_y = 65+(width*min);
	double ini_x = 65+(width*it_i);
	double div_x = width*(min - it_i)*SPEED*(4.0 + (!bg)*5.0)/350;
	double div_y = -div_x;
	iSetColor(255, 165, 0);
	iFilledRectangle(ini_x + swapFrames*div_x, 35, (width-10.0), show[it_i]);
	iFilledRectangle(ini_y + swapFrames*div_y, 35, (width-10.0), show[min]);
	swapFrames++;
	if(swapFrames>=(362/((4.0 + (!bg)*5.0)*SPEED))) {
		swapFrames = 0; 
		Resume();
		int t = a[min]; a[min]=a[it_i]; a[it_i] = t;
		Sleep(350/SPEED);
	}
}

void Vswap(){
	Pause();
	double div_y = (double)(505-35)*SPEED*(4.0 + (!bg)*5.0)/350.0;
	double width = (795.0/n);
	iSetColor(255, 165, 0);
	for(int i=0; i<tn; i++){ 
		double div_x = SPEED*(4.0 + (!bg)*5.0)*(405.0/160.0 - 1)*tshow[i]/350.0;
		iFilledRectangle(65.0+(width*(i+tbegin)), 505 - vSwapFrames*div_y, (width-10.0), tshow[i]+div_x*vSwapFrames);
	}
	vSwapFrames++;
	if(vSwapFrames>(483.2/div_y)) {
		vSwapFrames = 0; 
		for(int I=0; I<tn; I++){
				*(a+I+tbegin) = *(t+I);
			}
		free(t); t = 0; tn = 0;
		Resume();
		Sleep(350/SPEED);
	}
}
void Instructions(){
	if(InputState){
		iSetColor(255,255,255);
		iText(60, 715, "Press B to reset array");
		iText(60, 740, "Press R to randomise values");
		iText(60, 690, "Press ENTER to continue");
	}else if(exec == IDLE){
		iSetColor(255,255,255);
		iText(60, 720, "Press B to edit array");
		iText(60, 690, "Press E to start sorting");
	}
	else if(exec == RUNNING){
		iSetColor(255,255,255);
		iText(60, 715, "Press P to pause/resume");
		iText(60, 690, "Press E to restart");
		iText(60,740, "Press X to stop and reset");
	}
}
void arrayInp(){
	iShowBMP(270,10,"bg.bmp");
	iText(70,630,"Enter Number of Elements: ");
	if(InputState==1) iSetColor(255,255,0);
	else iSetColor(255,255,255);
	iRectangle(280,620,30,30);
	FILE* inp;
	FILE* read;
	if(InputState==1) inp = fopen("InputArray.txt", "w+");
	else inp = fopen("InputArray.txt", "r+");
	fseek(inp,0,SEEK_END);
	if(InputState == 1){
		if(doRand){
			fprintf(inp,"%d\n", 1+rand()%30);
			memset(s,'\0',sizeof s);
			sindex = 0;
			InputState++;
		}else{
		if(bufferReady){ 
			fprintf(inp,"\n%s\n",s);
			bufferReady = 0;
			memset(s,'\0',sizeof s);
			sindex = 0;
		}
		}
		iSetColor(255,255,255);
		iText(282,630,s);
	}
	if(InputState ==2){
		read = fopen("InputArray.txt", "r");
		fscanf(read, "%d",&n);
		fclose(read);
		if(n>30){
			n=30;
			read = fopen("InputArray.txt", "w");
			fprintf(read, "%d\n",n);
			//iSetColor(255,255,255);
			//iText(300,630,"Input size exceeds maximum allowed size");
			fclose(read);
		}
	}
	if(InputState >1){
		int i;
		char tmpS [10];
		sprintf(tmpS, "%d",n);
		iSetColor(255,255,255);
		iText(282,630,tmpS);
		for(i=0; i<n; i++){
			if(InputState==(i+2)) iSetColor(255,255,0);
			else iSetColor(255,255,255);
			iRectangle(70 + i*40,570,30,30);
			if(doRand && InputState < (n+2)){
			fprintf(inp,"%d\n", 1+rand()%30);
			memset(s,'\0',sizeof s);
			sindex = 0;
			InputState++;
			}else{
			if(bufferReady){ 
				fprintf(inp,"%s\n",s);
				bufferReady = 0;
				memset(s,'\0',sizeof s);
				sindex = 0;
			}
			}
		}
		iSetColor(255,255,255);
		iText(72+(InputState-2)*40,580,s);
	}
	fclose(inp);
	if(InputState > 2){
			read = fopen("InputArray.txt", "r");
			char tmpS[10];
			fscanf(read,"%d",&n);
			int i;
			int minit = ((InputState-2) < n) ? (InputState-2) : n;
			for(i=0; i<minit; i++){
				fscanf(read,"%d",&a[i]);
				sprintf(tmpS, "%d",a[i]);
				iSetColor(255,255,255);
				iText(72+(i)*40,580,tmpS);
			}
			fclose(read);
		}
	if(InputState > (n+1)){
			if(!doRand)InputState = 0;
			read = fopen("InputArray.txt", "r");
			fscanf(read, "%d", &n);
			int i;
			for(i=0; i<n; i++)
				fscanf(read,"%d",&a[i]);
			fclose(read);
		} 
}

void Settings(){
	int seek = 0;
	int mpos[4];
	char tval[20];
	char names [5][50] = {"Default", "Compare 1", "Compare 2", "Pivot (QuickSort)", "Idle Range"};
	if(SaveReady>32){
		if(SaveReady-32 ==1){
			SPEED = tmpSp;
			KillTime();
			CreateTimer(350.0/SPEED, highlight);
			printf("UPDATED SPEED = %.3lf\n",SPEED);
			SaveReady = 0;
			ActveArea[0] = 0; ActveArea[1] = 0; ActveArea[2] = 0; ActveArea[3] = 0;
		}
	}
	if(mousePosY < 623 && mousePosY > 607 && mousePosX>800+60 && mousePosX<1210+60) {
		seek = 1; SaveReady = 1;
		ActveArea[0] = 800+60; ActveArea[1] = 1210+60; ActveArea[2] = 607; ActveArea[3] = 623;
	}else
		if(mousePosY < 280 && mousePosY > 263 && mousePosX>890+60 && mousePosX<1150+60) {
			seek = 2;
			ActveArea[0] = 850+60; ActveArea[1] = 1190+60; ActveArea[2] = 263; ActveArea[3] = 280;
		}else if(mousePosY < 250 && mousePosY > 233 && mousePosX>890+60 && mousePosX<1150+60) {
			seek = 3;
			ActveArea[0] = 850+60; ActveArea[1] = 1190+60; ActveArea[2] = 233; ActveArea[3] = 250;
		}else if (mousePosY < 223 && mousePosY > 203 && mousePosX>890+60 && mousePosX<1150+60) {
			seek = 4;
			ActveArea[0] = 850+60; ActveArea[1] = 1190+60; ActveArea[2] = 203; ActveArea[3] = 223;
		}else {seek = 0; SaveReady = 0;}
	
	iSetColor(255,255,255);
	iText(880+60,650,"Speed");
	iText(880+60,630,"SLOW");
	iText(1130+60,630,"FAST");
	iFilledRectangle(890+60,615,260,3); 
	tmpSp = SPEED; 
	if(tmpSp < 1.0) mpos[0] = 1020.0+60.0 - 130.0*(1/tmpSp -1)/3.0;
	else mpos[0] = 1020+60.0 + 130.0*(tmpSp -1)/3.0;

	if(seek == 1){
		mpos[0] = mousePosX;
		if(mousePosX<890+60) mpos[0] = 890+60;
		if(mousePosX>1150+60) mpos[0] = 1150+60;
		if(mousePosX<1020+60) tmpSp = 1.0/(1 + ((1020.0+60.0-mpos[0])*3.0/130.0));
		else tmpSp = 1 + ((mpos[0]-1020.0-60.0)*3.0/130.0);
	}

	if(colourState){
		mpos[1] = 910+60.0 + tmpR[colourState-1]*210.0/255;
		mpos[2] = 910+60.0 + tmpG[colourState-1]*210.0/255;
		mpos[3] = 910+60.0 + tmpB[colourState-1]*210.0/255;
	}
	if(seek==2){
		mpos[1] = mousePosX;
		if(mpos[1]<910+60) mpos[1] = 910+60;
		if(mpos[1]>1120+60) mpos[1] = 1120+60;
		tmpR[colourState-1] = (mpos[1] - 910-60)*255/210;
	}else if(seek == 3){
		mpos[2] = mousePosX;
		if(mpos[2]<910+60) mpos[2] = 910+60;
		if(mpos[2]>1120+60) mpos[2] = 1120+60;
		tmpG[colourState-1] = (mpos[2] - 910-60)*255/210;
	}else if(seek ==4){
		mpos[3] = mousePosX;
		if(mpos[3]<910+60) mpos[3] = 910+60;
		if(mpos[3]>1120+60) mpos[3] = 1120+60;
		tmpB[colourState-1] = (mpos[3] - 910-60)*255/210;
	}
	iFilledRectangle(mpos[0],608,2,15); //seek cursor
	sprintf(tval, "%.2lf",tmpSp);
	iText(930+60,650,tval);
	
	for(int i=0; i<5; i++){
		iSetColor(255,255,255);
		iText(880+60, 570-i*30,names[i]);
		iSetColor(Bars[i].R,Bars[i].G,Bars[i].B);
		iFilledRectangle(1050+60, 568-i*30, 100, 15);
	}
	if(colourState){
		iSetColor(tmpR[colourState-1],tmpG[colourState-1],tmpB[colourState-1]);
		iFilledRectangle(960+60,300,120,120);

		iSetColor(255,255,255);
		iText(890+60,267,"R");
		iFilledRectangle(910+60,270,210,3);
		sprintf(tval,"%d",tmpR[colourState-1]);
		iText(1130+60,267,tval);
		iFilledRectangle(mpos[1],264,2,15);

		iText(890+60,237,"G");
		iFilledRectangle(910+60,240,210,3);
		sprintf(tval,"%d",tmpG[colourState-1]);
		iText(1130+60,237,tval);
		iFilledRectangle(mpos[2],234,2,15);

		iText(890+60,207,"B");
		iFilledRectangle(910+60,210,210,3);
		sprintf(tval,"%d",tmpB[colourState-1]);
		iText(1130+60,207,tval);
		iFilledRectangle(mpos[3],204,2,15);
	}
		iSetColor(255,255,255);
		if(bg == RUNNING) iText(960+60,145,"Hide Background",GLUT_BITMAP_9_BY_15);
		else iText(960+60,145,"Show Background",GLUT_BITMAP_9_BY_15);
		iRectangle(940+60,130,170,40);
		iText(945+60,95,"Restore to Default",GLUT_BITMAP_9_BY_15);
		iRectangle(940+60,80,170,40);
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	//printf("x = %d, y= %d\n",mx,my);
	mousePosX = mx;
	mousePosY = my;
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	//printf("%d %d\n",mx,my);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if(mx>=590 && mx<=740 && my <=180 && my >= 130){
			if(Menu == RUNNING) Menu = IDLE;
		}
		if((mx>=1160 && mx<=1290) && (my >=690 && my <= 720)){
			if(setting== IDLE) setting = RUNNING;
			else if(setting == RUNNING){
				setting = IDLE;
				FILE *setnow = fopen("SettingsDat.txt","w");
				for(int i=0; i<5; i++){
					fprintf(setnow, "%d %d %d\n",Bars[i].R, Bars[i].G ,Bars[i].B);
				}
				fclose(setnow);
			} 
		}
		if(exec == IDLE){
			if((mx>=330 && mx<=460) && (my >=690 && my <= 720)) currAlgo = Bubble;
			if((mx>=500 && mx<=630) && (my >=690 && my <= 720)) currAlgo = Selection;
			if((mx>=670 && mx<=800) && (my >=690 && my <= 720)) currAlgo = Quick;
			if((mx>=840 && mx<=970) && (my >=690 && my <= 720)) currAlgo = Merge;
		}
		if(setting == RUNNING){
			if(colourState){
				if(mx<927 || mx>1217 || my<197 || my>437){
					Bars[colourState-1].R = tmpR[colourState-1];
					Bars[colourState-1].G = tmpG[colourState-1];
					Bars[colourState-1].B = tmpB[colourState-1];
					SaveReady = 0;
					colourState=0;
				} 
			}else{
			for(int i=0; i<5; i++){
				if((mx>=1050+60 && mx<=1150+60) && (my>=(568-i*30) && my<=(583-i*30))){
					colourState = i+1;
					tmpR[colourState-1] = Bars[colourState-1].R;
					tmpG[colourState-1] = Bars[colourState-1].G;
					tmpB[colourState-1] = Bars[colourState-1].B;
					mousePosY = 0;
				} 
			}
			}
			if((ActveArea[0] && (mx< ActveArea[0])) || (ActveArea[1] && (mx > ActveArea[1])) || (ActveArea[2] && (my< ActveArea[2])) || (ActveArea[3] && (my > ActveArea[3]))) SaveReady += 32;
			if(mx>=1000 && mx<= 1170 && my>=80 && my <=120){
				SaveReady = 1;
				mousePosY = 0;
				SPEED = 1.000;
				tmpSp = 1.000;
				KillTime();
				CreateTimer(350.0/SPEED, highlight);
				//printf("%.2lf\n",SPEED);
				Bars[0].R = 100; Bars[0].G = 20; Bars[0].B = 150;
				Bars[1].R = 170; Bars[1].G = 120; Bars[1].B = 20;
				Bars[2].R = 60; Bars[2].G = 150; Bars[2].B = 10;
				Bars[3].R = 252; Bars[3].G = 15; Bars[3].B = 192;
				Bars[4].R = 80; Bars[4].G = 0; Bars[4].B = 10;
			}
			if((mx>=1000 && mx<= 1170 && my>=130 && my <=170)){
				if(bg == RUNNING) bg = IDLE;
				else bg = RUNNING;
			}
		}
		//printf("%d %d\n",colourState, SaveReady);
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//place your codes here

	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if((key >= '0' && key <= '9' && sindex <3)){
		s[sindex] = key;
		sindex++;
	}
	if(key == '\b' && sindex>0){
		s[--sindex] = '\0';
	}
	if(key == '\r'){
		if((doRand && InputState >= n+2))
			InputState=0;
		if((s[0] >='0' && s[0] <= '9')){
			bufferReady = 1;
			InputState++;
		}
	} 
	if (key == 'q') {
		exit(0);
	}
	if(Menu == IDLE){
	if(key == 'p')
		pausRes();
	if(key == 'e'){
		Restart(1);
	}
	if(key == 'x'){
		Restart(2);
	}
	if(key=='b' && exec == IDLE){
		doRand = 0;
		InputState = 1;
		//n = 0;
	}
	if(key=='r' && InputState){
		doRand = 1;
	}
	}
}
void Restart(int x){
	if(!vSwapFrames && !swapFrames){
			RestartReq = 0;
		FILE* comand = fopen("cmdhandler.txt", "r+");
		source = fopen("SortData.txt", "r");

		it_j = 0;
		min = 0;
		char cmd[100];
		sprintf(cmd, "gcc SortCode%d.c -o SortCode%d && SortCode%d.exe <InputArray.txt >SortData.txt", currAlgo+1,currAlgo+1,currAlgo+1);
		 if (system(cmd)==0) {
        system("cmdhandler.txt");
    	} else {
        puts("Couldn't compile");
    	}
		fclose(comand);

		FILE* read;
		read = fopen("InputArray.txt", "r");
			fscanf(read, "%d", &n);
			int i;
			for(i=0; i<n; i++)
				fscanf(read,"%d",&a[i]);
			fclose(read);
		if(x == 1){
			Resume();
			exec = RUNNING;
		}else{
			Pause();
			exec = IDLE;
		}
		} else RestartReq = x;
}
	//place your codes for other keys here

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}
	//place your codes for other keys here
}


int main(){
	//place your own initialization codes here.
	srand(time(0));
	CreateTimer(350,highlight);
	FILE* ini = fopen("InputArray.txt", "w");
	fprintf(ini, "%d\n", n);
	for(int i=0; i<n; i++)
		fprintf(ini, "%d ",a[i]);
	fprintf(ini,"\n");
	fclose(ini);
	Bars[0].R = 100; Bars[0].G = 20; Bars[0].B = 150;
	Bars[1].R = 170; Bars[1].G = 120; Bars[1].B = 20;
	Bars[2].R = 60; Bars[2].G = 150; Bars[2].B = 10;
	Bars[3].R = 252; Bars[3].G = 15; Bars[3].B = 192;
	Bars[4].R = 80; Bars[4].G = 0; Bars[4].B = 10;
	iInitialize(1300, 760, "Sorting Algorithm Visualiser");

	return 0;
}
