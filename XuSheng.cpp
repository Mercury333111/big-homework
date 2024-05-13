#include<graphics.h>
#include<stdio.h>
#include<assert.h>
#include<time.h>
#include<stdlib.h>
//初始界面
void beginLayout() {
	char beginString[] = "开始游戏";
	char closeString[] = "退出游戏";
	char tipsString[] = "W键跳跃，S键展示";
	//界面
	setfillcolor(RGB(65, 105, 225));
	solidrectangle(210, 100, 380, 150);
	solidrectangle(210, 160, 380, 210);
	solidrectangle(0, 360, 600, 400);
	//文字
	settextcolor(RGB(255, 192, 203));
	setbkmode(TRANSPARENT);//去掉文字背景
	settextstyle(40, 0, "黑体");
	outtextxy(218, 110, beginString);
	outtextxy(218, 170, closeString);
	outtextxy(150, 250, tipsString);
	//鼠标
	while (1) {
		MOUSEMSG m = GetMouseMsg();
		if (m.x >= 210 && m.x <= 380 && m.y >= 100 && m.y <= 150) {
			setlinecolor(RED);
			rectangle(205, 95, 385, 155);//开始
			if (m.uMsg == WM_LBUTTONDOWN) {
				break;
			}
		}
		else if (m.x >= 210 && m.x <= 380 && m.y >= 160 && m.y <= 210) {
			setlinecolor(RED);
			rectangle(205, 155, 385, 215); //结束
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(0);
			}
		}
		else {
			setlinecolor(RGB(205, 133, 63));
			rectangle(205, 95, 385, 155);
			rectangle(205, 155, 385, 215);
		}
	}

}
//定时
int Timer(time_t num, int id) {
	static time_t start[10];
	time_t end = clock();
	if (end - start[id] > num) {
		start[id] = end;
		return 1;
	}
	return 0;
}
//障碍
struct Wei {
	int x;
	int y;
	int width;
	int height;
};
struct Wei* createWei(int x, int y, int width, int height)
{
	struct Wei* cc = (struct Wei*)malloc(sizeof(struct Wei));
	assert(cc);//一个断言
	cc->x = x;
	cc->y = y;
	cc->width = width;
	cc->height = height;
	return cc;
}
//制作、移动障碍
void createWei(struct Wei* cc) {
	setfillcolor(RGB(176, 196, 222));
	solidrectangle(cc->x, cc->y, cc->x + cc->width, cc->y + cc->height);
}
void moveWei(struct Wei* cc) {
	cc->x -= 20;
}
struct Node {
	struct Wei* cc;
	struct Node* next;
};
struct Node* list = NULL;
struct Node* createList() {
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	assert(headNode);
	headNode->cc = NULL;
	headNode->next = NULL;
	return headNode;
}
struct Node* createNode(struct Wei* cc) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	assert(newNode);
	newNode->cc = cc;
	newNode->next = NULL;
	return newNode;
}
void insert(struct Node* headNode, struct Wei* cc) {
	struct Node* newNode = createNode(cc);
	newNode->next = headNode->next;
}
void printList(struct Node* headNode) {
	struct Node* cmove = headNode->next;
	while (cmove != NULL) {
		createWei(cmove->cc);
		moveWei(cmove->cc);
		cmove = cmove->next;
	}
}
//碰撞
int hitWei(int x, int y, int width, int height, struct Node* list) {
	struct Node* cmove = list->next;
	while (cmove != NULL) {
		if (cmove->cc->x >= (x - cmove->cc->width) && cmove->cc->x <= x + width) {
			if (cmove->cc->y >= (y - cmove->cc->height) && cmove->cc->y <= y + height) {
				return 1;
			}
		}
		cmove = cmove->next;
	}
	return 0;
}
//徐盛
IMAGE DOWN[5];
IMAGE JUMP[5];
IMAGE RUN[5];
HWND hwnd = NULL;
void loadResource() {
	for (int i = 1;i <= 5;i++) {
		char fileName[32] = "";
		sprintf(fileName, "JUMP%d.png", i);
		loadimage(JUMP + i - 1, fileName, 100, 100);
		sprintf(fileName, "RUN%d.png", i);
		loadimage(RUN + i - 1, fileName, 100, 100);
		sprintf(fileName, "DOWN%d.png", i);
		loadimage(DOWN + i - 1, fileName, 80, 50);
	}
}
//动作.RUN
void move(int FrameNum) {
	BeginBatchDraw();
	int i = 0;
	while (i < FrameNum) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, 260, RUN + i);
		if (hitWei(30, 260, 100, 100, list)) {
			MessageBox(hwnd, "GAME OVER","Exit",MB_OK);
			exit(0);
		}
		i++;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	EndBatchDraw();
}
//DOWN
void down(int FrameNum) {
	BeginBatchDraw();
	int i = 0;
	while (i < FrameNum) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, 310, DOWN + i);
		i++;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	EndBatchDraw();
}

//JUMP
void jump() { 
	BeginBatchDraw();
	int y = 260;
	for (int i = 0;i <= 5;i++) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, y, &JUMP[0]);
		y -= 30;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	for (int i = 0;i <= 5;i++) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, y, &JUMP[4]);
		y += 30;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	EndBatchDraw();
}
void keyDOWN() {
	if (GetAsyncKeyState('S')) {
		down(5);
	}
	if (GetAsyncKeyState('W')) {
		jump();
	}
}








int main() {
	list = createList();
	srand((unsigned int)time(0));
	loadResource();
	hwnd=initgraph(600, 400);
	setbkcolor(RGB(205, 133, 63));
	cleardevice();
	beginLayout();
	//Game
	while (1) {
		move(5);
		keyDOWN();
		if (Timer(1000, 0)) {
			insert(list, createWei(600, 315 - rand() % 100, 45, 45));
		}
	}
	closegraph();
	return 0;
}