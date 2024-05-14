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
int Timer(DWORD num, int id) {
	static DWORD start[10];
	DWORD end = GetTickCount();
	if (end - start[id] > num) {
		start[id] = end;
		return 1;
	}
	return 0;
}
// 障碍物结构体
typedef struct {
	int x;
	int y;
	int width;
	int height;
} Obstacle;

Obstacle* createObstacle(int x, int y, int width, int height) {
	Obstacle* ob = (Obstacle*)malloc(sizeof(Obstacle));
	assert(ob);
	ob->x = x;
	ob->y = y;
	ob->width = width;
	ob->height = height;
	return ob;
}

void drawObstacle(Obstacle* ob) {
	setfillcolor(RGB(176, 196, 222));
	solidrectangle(ob->x, ob->y, ob->x + ob->width, ob->y + ob->height);
}

void moveObstacle(Obstacle* ob) {
	ob->x -= 20;
}

typedef struct Node {
	Obstacle* ob;
	struct Node* next;
} Node;

Node* createList() {
	Node* head = (Node*)malloc(sizeof(Node));
	assert(head);
	head->ob = NULL;
	head->next = NULL;
	return head;
}

void insert(Node* head, Obstacle* ob) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	assert(newNode);
	newNode->ob = ob;
	newNode->next = head->next;
	head->next = newNode;
}

void printList(Node* head) {
	Node* curr = head->next;
	while (curr != NULL) {
		drawObstacle(curr->ob);
		moveObstacle(curr->ob);
		curr = curr->next;
	}
}

int hitObstacle(int x, int y, int width, int height, Node* list) {
	Node* curr = list->next;
	while (curr != NULL) {
		if (curr->ob->x >= (x - curr->ob->width) && curr->ob->x <= x + width) {
			if (curr->ob->y >= (y - curr->ob->height) && curr->ob->y <= y + height) {
				return 1;
			}
		}
		curr = curr->next;
	}
	return 0;
}

//徐盛
IMAGE DOWN[5];
IMAGE JUMP[5];
IMAGE RUN[5];
IMAGE DOWNB[5];
IMAGE JUMPB[5];
IMAGE RUNB[5];
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
		sprintf(fileName, "JUMPB%d.png", i);
		loadimage(JUMP + i - 1, fileName, 100, 100);
		sprintf(fileName, "RUNB%d.png", i);
		loadimage(RUN + i - 1, fileName, 100, 100);
		sprintf(fileName, "DOWNB%d.png", i);
		loadimage(DOWN + i - 1, fileName, 80, 50);
	}
}
//动作.RUN
void move(Node*list,int FrameNum) {
	BeginBatchDraw();
	int i = 0;
	while (i < FrameNum) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, 260, RUN + i,SRCAND);
		putimage(50, 260, RUNB + i,SRCPAINT);
		if (hitObstacle(30, 260, 100, 100,list)) {
			MessageBox(hwnd, "GAME OVER","GAME OVER",MB_OK);
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
void down(Node* list, int FrameNum) {
	BeginBatchDraw();
	int i = 0;
	while (i < FrameNum) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, 310, DOWN + i,SRCAND);
		putimage(50, 310, DOWNB + i,SRCPAINT);
		i++;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	EndBatchDraw();
}

//JUMP
void jump(Node* list) {
	BeginBatchDraw();
	int y = 260;
	for (int i = 0;i <= 5;i++) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, y, &JUMP[0],SRCAND);
		putimage(50, y, &JUMPB[0],SRCPAINT);
		y -= 30;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	for (int i = 0;i <= 5;i++) {
		cleardevice();
		solidrectangle(0, 360, 600, 400);
		putimage(50, y, &JUMP[4],SRCAND);
		putimage(50, y, &JUMPB[4],SRCPAINT);
		y += 30;
		printList(list);
		Sleep(50);
		FlushBatchDraw();
	}
	EndBatchDraw();
}
void keyDOWN(Node* list) {
	if (GetAsyncKeyState('S')) {
		down(list,5);
	}
	if (GetAsyncKeyState('W')) {
		jump(list);
	}
}
//主体
int main() {
	Node* obstacleList = createList();
	srand((unsigned int)time(0));
	loadResource();
	HWND hwnd=initgraph(600, 400);
	setbkcolor(RGB(205, 133, 63));
	cleardevice();
	beginLayout();
	//Game
	while (1) {
		move(obstacleList,5);
		keyDOWN(obstacleList);
		if (Timer(1000, 0)) {
			insert(obstacleList, createObstacle(600, 315 - rand() % 100, 45, 45));
		}
	}
	closegraph();
	return 0;
}