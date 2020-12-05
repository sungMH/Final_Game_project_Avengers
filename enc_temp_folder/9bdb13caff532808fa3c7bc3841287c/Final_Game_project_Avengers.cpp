#include <bangtal>
#include <ctime>
#include <iostream>
#include <string>

using namespace bangtal;
using namespace std;

//파워스톤, 스페이스,타임,리얼리티,마인드, 소울
//bool stones[6] = {false,false, false, false, false, false};
bool stones[6] = { true,true, true, true, true, true };


bool have5Stone(bool stones[6]) {
    int nu = 0;
    for (int i = 0; i < 5; i++) { nu++; }
    return nu == 5;
}

ObjectPtr makeNextButton(ScenePtr &nowScene, ScenePtr &nextScene, int x ,int y, bool isNext =true,
    bool defaultFilename =true, const char * inputFilename = "\0") {
    char filenames[][100] = { "images/nextButton.png","images/previousButton.png" };
    int index = 0;
    if (!isNext) { index = 1; };
    ObjectPtr nextButton;
    if (defaultFilename) {
        nextButton = Object::create(filenames[index], nowScene, x, y);//게임 버튼 추가하기
    }
    else {
        nextButton = Object::create(inputFilename, nowScene, x, y);
    }
    nextButton->setOnMouseCallback([&](ObjectPtr o,int,int,MouseAction)->bool {

            nextScene->enter();
    return true;
    });
    return nextButton;
}

ObjectPtr xandarObject(int x, int y, ScenePtr scene,const char* filename) {
    auto xandarobject = Object::create(filename, scene, x, y);
    return xandarobject;
}

class Queue30Object {
    int front = 0, rear = 1;
    int maxSize = 29;
    int nowPointerX[30];
    ObjectPtr object[30];
    int num = 0;
    int key[30];

public:
    void inputKey(int k) {
        key[rear] = k;
    }
    int outputKey() {
        return key[front+1];
    }
    bool isEmpty() {
        return num == 0;
    };
    bool isFull() {
        return num == 29;
    }
    void Enqueue(int x, ObjectPtr o) {
        if (!isFull()) {
            nowPointerX[rear] = x;
            object[rear] = o;
            rear = (rear + 1) % maxSize;
            num++;
        }
    };
    void moveAll(int y, ScenePtr s, int stepSize) {
        int f = front, nowNum = 0;
        while (nowNum < num && num != 0) {
            f = (f + 1) % maxSize;
            nowNum++;
            nowPointerX[f] = nowPointerX[f] + stepSize;
            object[f]->locate(s, nowPointerX[f], y);
            if (nowPointerX[f] < 0) { int x = Dequeue(); };
        }
        
    }
    void moveAllYAXIS(int x, ScenePtr s, int stepSize) {
        int f = front, nowNum = 0;
        while (nowNum < num && num != 0) {
            f = (f + 1) % maxSize;
            nowNum++;
            nowPointerX[f] = nowPointerX[f] + stepSize;
            object[f]->locate(s, x,nowPointerX[f]);
            if (nowPointerX[f] < 0) { int x = Dequeue(); };
        }

    }
    int firstElem() {
        if (!isEmpty()){
            int f = front;
            f = (f + 1) % maxSize;
            return nowPointerX[f];
        }
        return -1;
    }
    int Dequeue() {
        if (!isEmpty()) {
            front = (front + 1) % maxSize;
            num--;
            object[front]->hide();
            return nowPointerX[front];
        }
        return -1;
    }
    bool isStrike(int myX){
        int f = front, nowNum = 0;
        while (nowNum < num && num != 0){
            f = (f + 1) % maxSize;
            nowNum++;
            if (-30<=nowPointerX[f] - myX && nowPointerX[f] - myX <=30) {
                return true;
            }
        }
        return false;
    }
    bool isStrikeYAXIS(int myY) {
        int f = front, nowNum = 0;
        while (nowNum < num && num != 0) {
            f = (f + 1) % maxSize;
            nowNum++;
            if (-50<= nowPointerX[f]- myY && nowPointerX[f] - myY <=50) {
                return true;
            }
        }
        return false;
    }
    void allRevmove() {
        int f = front, nowNum = 0;
        while (nowNum < num && num != 0) {
            f = (f + 1) % maxSize;
            nowNum++;
            object[f]->hide();
        }
        num = 0;
        front = 0;
        rear = 1;
    }
};


void xandarGenerater(Queue30Object q[5], bool isGenerate[5][2],const char *filename,ScenePtr scene,int &pC){
    int randomG[5] = {0,0,0,0,0};
    cout << "xandar G" << endl;
    for (int i = 0; i < 5; i++) {randomG[rand() % 5] = 1;};
    for (int i = 0; i < 5; i++) {
        if (randomG[i] == 1) {
            auto xandarob = Object::create(filename, scene, 1152, 120*i);
            q[i].Enqueue(1152, xandarob);
        }
    };
}


int main()
{
    //######################### 시작 화면 ############################################
    auto startScene = Scene::create("어벤져스", "images/startScene.png");

    srand((unsigned)time(NULL));

    auto startS1 = Scene::create("게임설명1", "images/gameManual1.png");//여기 화면 만들어야 함
    auto nextbuttonS1 = makeNextButton(startScene, startS1, 1000, 50);//위치 조절
    auto prevbuttonS1 = makeNextButton(startS1, startScene, 900, 50,false);//위치 조절

    auto startS2 = Scene::create("게임설명2", "images/gameManual2.png");//여기 화면 만들어야 함
    auto nextbuttonS2 = makeNextButton(startS1, startS2, 1000, 50);
    auto prevbuttonS2 = makeNextButton(startS2, startS1, 900, 50, false);


    auto xF = Scene::create("잔다르패배", "images/잔다르패배.png");
    auto aF = Scene::create("아스가르드패배", "images/아스가르드패배.png");
    auto eF = Scene::create("뉴욕생텀패배", "images/지구패배.png");
    auto nF = Scene::create("노웨어패배", "images/노웨어패배.png");
    auto dF = Scene::create("도르마무패배", "images/도르마무패배.png");

    //상황봐서 게임 설명 화면 더추가


 //#############################게임맵 생성#######################################

    auto gameMap = Scene::create("게임 맵", "images/gameMap.png");
    auto nextbuttonM2 = makeNextButton(startS2, gameMap,1000, 50);
    auto prevbuttonM2 = makeNextButton(gameMap, startS2, 900, 50, false);


    

    //게임 맵에 행성들 추가

    //################1.잔다르 행성

    //게임메뉴얼
    auto xandarMenual1 = Scene::create("잔다르설명1", "images/잔다르메뉴얼.png");//메뉴얼 만들기
  //  auto xandarMenual2 = Scene::create("잔다르설명2", "images/xandarMenual2.png");//메뉴얼 만들기
    auto xandarGameScene = Scene::create("잔다르행성", "images/xandarGameScene.png");
    auto xandarGameSceneV = Scene::create("잔다르행성", "images/잔다르승리.png");
    xandarGameSceneV->setOnEnterCallback([&](ScenePtr p)->bool {
        auto xvo = makeNextButton(xandarGameSceneV, gameMap, 800, 50, true, false, "images/메인으로.png");
        return true; });

    //다음 이전 장 가는 버튼
    auto xandar = makeNextButton(gameMap, xandarMenual1, 100, 100,true,false, "images/xandar.png");
    auto nextBX1 = makeNextButton(xandarMenual1, xandarGameScene, 1000, 10);
    auto prevBX1 = makeNextButton(xandarMenual1, gameMap, 900, 10, false);
    //auto nextBX2 = makeNextButton(xandarMenual2, xandarGameScene, 1000, 10);
    //auto prevBX2 = makeNextButton(xandarMenual2, xandarMenual1, 900, 10, false);

    auto powerS = Object::create("images/파워스톤.png", gameMap, 100, 100, false);
    powerS->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool {
        o->pick();
        return true; });

    //게임 구현
    int nowX = 0, nowY = 240;
     //타이머 정의
    float xandarGenerateTime = 1.5f, level1 =0.015f, level2 = 5.0f, level3 = 2.5f, setFireTime = 1.5f, moveFireTime = 0.2f;
    auto xandarGenerateTimer = Timer::create(xandarGenerateTime);
    auto xandarMoveTimer = Timer::create(level1);
    auto moveFireTimer = Timer::create(moveFireTime);
    auto setFireTimer = Timer::create(setFireTime);

    auto gun = Object::create("images/총장전.png",xandarGameScene, 700, 600);
    bool gunReady = false;
  

    //좌표 정의
    auto my = Object::create("images/스타로드.png", xandarGameScene,0,240);
    int xandarStartPointer[5][2] = { {1280,0},{1280,120},{1280,240},{1280,360},{1280,480} };

    //총알 공격로 5개 설정
    bool fireReady = false;
    Queue30Object fireRoot[5];
    for (int i = 0; i < 5; i++) { fireRoot[i] = Queue30Object(); };



    //잔다르 비행기 5개 공격로 설정
    bool isGenerate[5][2] = { {false, false},{false, false},{false, false},{false, false},{false, false} }; //지나갈 길은 만들고 생성하기 위해
    Queue30Object goRoot[5];
    int pC = 0;
    for (int i = 0; i < 5; i++) { goRoot[i] = Queue30Object(); };


    //다음줄을 타이머 마다 생성하면 비행기 적절한 경로로 생성
    xandarGenerateTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        xandarGenerater(goRoot, isGenerate, "images/잔다르전투기.png", xandarGameScene,pC);
        t->set(xandarGenerateTime);
        t->start();
        return true;
     });

    setFireTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        gunReady = true;
        gun->setImage("images/총발사.png");
        setFireTimer->set(setFireTime);
        setFireTimer->stop();
        return true;
        });

    //총알 이랑 전투기 부딪히면 둘다 삭제
    moveFireTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        for (int i = 0; i < 5; i++) {
            fireRoot[i].moveAll(120 * i+30, xandarGameScene, 47);
            if ((fireRoot[i].firstElem() != -1) && (goRoot[i].firstElem() != -1)
                && (fireRoot[i].firstElem() >= goRoot[i].firstElem())) {
                int trash1 = fireRoot[i].Dequeue(), trash2 = goRoot[i].Dequeue();
            }
        }
        moveFireTimer->set(moveFireTime);
        moveFireTimer->start();
        return true;
        });

    xandarMoveTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        for (int i = 0; i < 5; i++) {
            goRoot[i].moveAll(120*i, xandarGameScene, -5);
            
            if (nowY / 120 == i) {
                if (goRoot[i].isStrike(nowX)) {
                    showMessage("잔다르 비행기의 공격을 받으셨습니다. 탈출 실패");
                    xandarGenerateTimer->stop();
                    xandarMoveTimer->stop();
                    moveFireTimer->stop();
                    setFireTimer->stop();
                    xF->enter();
                }
            }
        }
        t->set(level1);
        t->start();
        return true;
        });

    //키보드 누르면 총알 발사
    xandarGameScene->setOnKeyboardCallback([&](ScenePtr s, int key, bool pressed)->bool {
        if (gunReady) {
            //총알경로 생성
            int y = nowY / 120;
            auto fireObject = Object::create("images/총알.png", xandarGameScene, nowX + 128, nowY + 30);
            fireRoot[y].Enqueue(nowX + 128, fireObject);
            setFireTimer->set(setFireTime); setFireTimer->start();
            gunReady = false;
            gun->setImage("images/총장전.png");
        }
        return true;
        });

    //캐릭터 이동 버튼 부착
    auto up1 = Object::create("images/up.png", xandarGameScene, 41, 40);
    auto down1 = Object::create("images/down.png", xandarGameScene, 41, 0);
    auto left1 = Object::create("images/left.png", xandarGameScene, 0, 0);
    auto right1 = Object::create("images/right.png", xandarGameScene, 82, 0);
    ObjectPtr moveKey[4] = { up1, down1,left1, right1 };
    //캐릭터 이동시에 내 위치 업데이트
    for (int i = 0; i < 4; i++) {
        moveKey[i]->setOnMouseCallback([&,i](ObjectPtr o, int x, int y, MouseAction)->bool {
            int moveStep[4][2] = { {0,120},{0,-120},{-64,0},{64,0} };
            if (nowX + moveStep[i][0] >= 0 && nowY + moveStep[i][1] <= 480 && 0<=nowY + moveStep[i][1]) {
                my->locate(xandarGameScene, nowX + moveStep[i][0], nowY + moveStep[i][1]);
                if (1280 <= nowX + moveStep[i][0]) {
                    showMessage("잔다르 행성 탈출 성공!!!");
                    xandarGenerateTimer->stop();
                    xandarMoveTimer->stop();
                    moveFireTimer->stop();
                    setFireTimer->stop();
                    stones[0] = true;
                    xandar->hide();
                    powerS->show();
                    stones[0] = true;
                    xandarGameSceneV->enter();
                }
                nowX += moveStep[i][0]; nowY += moveStep[i][1];
                cout << nowX << endl << nowY << endl;
                if (goRoot[nowY / 120].isStrike(nowX)) {
                    showMessage("적군에 비행기에 부딪혔습니다! GAME OVER");
                    xandarGenerateTimer->stop();
                    xandarMoveTimer->stop();
                    moveFireTimer->stop();
                    setFireTimer->stop();
                    xF->enter();
                }

            }
            return true;
            });
    }

    //게임화면 들어오면 타이머 시작하는 콜백 붙이기
    xandarGameScene->setOnEnterCallback([&](ScenePtr s)->bool {
        nowX = 0; nowY = 240;
        my->locate(xandarGameScene, 0, 240);
        fireReady = false;
        for (int i = 0; i < 5; i++) { fireRoot[i].allRevmove(); };
        pC = 0;
        for (int i = 0; i < 5; i++) { goRoot[i].allRevmove(); };
        for (int i = 0; i < 5; i++) { isGenerate[i][0] = false; isGenerate[i][1] = false; };
        gun->setImage("images/총장전.png");
        gunReady = false;
        xandarGenerateTimer->set(0);
        xandarGenerateTimer->start();
        xandarMoveTimer->start();
        moveFireTimer->start();
        setFireTimer->start();
        return true;
        });


    //################2.아스가르드 행성
    //게임메뉴얼
    auto asgardMenual1 = Scene::create("아스가르드설명1", "images/아스가르드메뉴얼.png");//메뉴얼 만들기
    //auto asgardMenual2 = Scene::create("아스가르드설명2", "images/asgardMenual2.png");//메뉴얼 만들기
    auto asgardGameScene = Scene::create("아스가르드행성", "images/asgardGameScene.png");

    //다음 이전 장 가는 버튼
    auto asgard = makeNextButton(gameMap, asgardMenual1, 300, 420, true,false, "images/asgard.png");
    auto nextBA1 = makeNextButton(asgardMenual1, asgardGameScene, 1000, 10);
    auto prevBA1 = makeNextButton(asgardMenual1, gameMap, 900, 10, false);
    //auto nextBA2 = makeNextButton(asgardMenual2, asgardGameScene, 1000, 10);
    //auto prevBA2 = makeNextButton(asgardMenual2, asgardMenual1, 900, 10, false);

    auto spaceS = Object::create("images/스페이스스톤.png", gameMap, 300, 420, false);
    spaceS->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool {
        o->pick();
        return true; });

    //게임 구현
    //헬라공격 피하고 영원의 불꽃 불피워서 수르트로 변신하기

    auto LokiAndThor = Object::create("images/LokiAndThor.png", asgardGameScene,0,0);//로키 -영원의 불꽃가진상태
    auto Hella = Object::create("images/Hella.png", asgardGameScene, 1100, 0);//헬라구현
    auto hp = Object::create("images/hp.png", asgardGameScene, 100, 600);//칼에 맞을 수 있는 횟수 10개정도?
    hp->setScale(2.0f);
    int LTX = 0, LTY = 0;//현재나의 위치
    auto spaceStack = Object::create("images/frame.png",asgardGameScene, 400, 600);
    spaceStack->setScale(2.0f);
    int spaceStackNum = 0;
    auto sL10 = Object::create("images/0.png", asgardGameScene, 500, 600);
    auto sL1 = Object::create("images/0.png", asgardGameScene, 550, 600);



      //캐릭터 이동 버튼 부착
    auto up2 = Object::create("images/up.png", asgardGameScene, 41, 40);
    auto down2 = Object::create("images/down.png", asgardGameScene, 41, 0);
    auto left2 = Object::create("images/left.png", asgardGameScene, 0, 0);
    auto right2 = Object::create("images/right.png", asgardGameScene, 82, 0);
    ObjectPtr moveKey1[4] = { up2, down2,left2, right2 };
    //캐릭터 이동시에 내 위치 업데이트
    for (int i = 0; i < 4; i++) {
        moveKey1[i]->setOnMouseCallback([&, i](ObjectPtr o, int x, int y, MouseAction)->bool {
            int moveStep[4][2] = { {0,120},{0,-120},{-128,0},{128,0} };
            if (LTX + moveStep[i][0] >= 0 && LTY + moveStep[i][1] <= 120 && 0 <= LTY + moveStep[i][1] && LTX + moveStep[i][0] < 1000) {
                LokiAndThor->locate(asgardGameScene, LTX + moveStep[i][0], LTY + moveStep[i][1]);
                LTX = LTX + moveStep[i][0];
                LTY = LTY + moveStep[i][1];
            }
            return true;
            });
    }
    
    //헬라공격 지평에 수평한 공격
    Queue30Object HellaAttack1;
    Queue30Object HellaAttack2[9];

    int aLife = 20;
    auto aL10 = Object::create("images/2.png", asgardGameScene, 200, 600);
    auto aL1 = Object::create("images/0.png", asgardGameScene, 250, 600);



    float HellaTime = 1.0f, AttackTime = 0.1f; 
    auto HellaTimer = Timer::create(HellaTime);
    auto AttackTimer = Timer::create(AttackTime);
    AttackTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        HellaAttack1.moveAll(40, asgardGameScene, -30);
        if (HellaAttack1.isStrike(LTX) && LTY == 0) {
            aLife--;
            int x10 = aLife / 10;
            int x1 = aLife % 10;
            string xs10 = "images/" + to_string(x10) + ".png";
            string xs1 = "images/" + to_string(x1) + ".png";
            const char* xsc10 = xs10.c_str();
            const char* xsc1 = xs1.c_str();
            aL10->setImage(xsc10);
            aL1->setImage(xsc1);

            if (aLife <= 0) {
                showMessage("헬라에게 패배하셨습니다!");
                aF->enter();
            }
            int a = HellaAttack1.Dequeue();
        }
        if (HellaAttack1.firstElem() <= 0) { int a = HellaAttack1.Dequeue(); }
        for (int i = 1; i < 10; i++) {         
                HellaAttack2[i - 1].moveAllYAXIS(128 * (i - 1), asgardGameScene, -30);
                if (HellaAttack2[i - 1].isStrikeYAXIS(LTY) && LTX == 128 * (i - 1)) {
                    //$$라이프 감소 구현
                    int a = HellaAttack2[i - 1].Dequeue();
                    aLife--;
                    int x10 = aLife / 10;
                    int x1 = aLife % 10;
                    string xs10 = "images/" + to_string(x10) + ".png";
                    string xs1 = "images/" + to_string(x1) + ".png";
                    const char* xsc10 = xs10.c_str();
                    const char* xsc1 = xs1.c_str();
                    aL10->setImage(xsc10);
                    aL1->setImage(xsc1);
                    if (aLife <= 0) {
                        HellaTimer->stop();
                        AttackTimer->stop();
                        showMessage("헬라에게 패배하셨습니다!");
                        aF->enter();
                    }
                }          
            if (HellaAttack2[i - 1].firstElem() <= 0) { int a = HellaAttack2[i - 1].Dequeue(); }
        }
        AttackTimer->set(AttackTime);
        AttackTimer->start();
        return true;
        });

    HellaTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        int attacks[10];
        for (int i = 0; i < 10; i++) {
            attacks[i] = rand() % 5;
        }

        if (attacks[0] != 0) {
            auto HellaAttack = Object::create("images/HellaAttack.png", asgardGameScene,1152, 40);
            HellaAttack1.Enqueue(1280, HellaAttack);
        }

        for (int i = 1; i < 10; i++) {
            if (attacks[i] == 0) {
                auto HellaAttack = Object::create("images/HellaAttack2.png", asgardGameScene, 128 * (i - 1), 700);
                HellaAttack2[i - 1].Enqueue(760, HellaAttack);
            }
        }
        HellaTimer->set(HellaTime);
        HellaTimer->start();
        return true;
    });
        
    auto surtSkill = Object::create("images/수르트스킬.png", asgardGameScene, 700, 500,false);

    //스페이스 바 누르면 게이지 채워짐(99개)
    asgardGameScene->setOnKeyboardCallback([&](ScenePtr s ,int key, bool pressed)->bool {
        if (spaceStackNum >= 99) {
            showMessage("불의 거인 수르트 소환!, 우측 상단 스킬을 사용하시면 승리합니다.");
            LokiAndThor->locate(asgardGameScene,0,0);
            LokiAndThor->setImage("images/surt.png");//$$수르트 이미지 넣기
            surtSkill->show();//수르트 스킬 활성화
            aLife = 99999;
            sL10->hide();
            sL1->hide();
            aL10->hide();
            aL1->hide();

        }
        else {
            spaceStackNum++;
            int s10 = spaceStackNum / 10;
            int s1 = spaceStackNum % 10;
            string ss10 = "images/" + to_string(s10) + ".png";
            string ss1 = "images/" + to_string(s1) + ".png";
            const char* ssc10 = ss10.c_str();
            const char* ssc1 = ss1.c_str();
            sL10->setImage(ssc10);
            sL1->setImage(ssc1);
        
        }
        return true; });

    auto asgardSurt = Scene::create("아스가르드", "images/수르트공격.png");

    surtSkill->setOnMouseCallback([&](ObjectPtr, int, int, MouseAction)->bool {
        HellaTimer->stop();
        AttackTimer->stop();
        asgardSurt->enter();
        showMessage("아스가르드 행성을 파괴하여 헬라를 처리하였습니다. 스페이스 스톤을 얻으셨습니다.");
        asgard->hide();
        spaceS->show();
        stones[1] = true;
        auto temp = makeNextButton(asgardSurt, gameMap, 800, 50, true, false, "images/메인으로.png");
        return true;
        });

    asgardGameScene->setOnEnterCallback([&](ScenePtr s)->bool {
        LokiAndThor->locate(asgardGameScene,0,0);
        spaceStackNum = 0;
        LTX = 0; LTY = 0;
        sL10->setImage("images/0.png");
        sL1->setImage("images/0.png");
        aLife = 20;
        HellaAttack1.allRevmove();
        for (int i = 0; i < 9; i++) { HellaAttack2[i].allRevmove(); };
        aL10->setImage("images/2.png");
        aL1->setImage("images/0.png");
        HellaTimer->set(HellaTime);
        AttackTimer->set(AttackTime);
        HellaTimer->start();
        AttackTimer->start();
        return true;
        });


    //################3.뉴욕생텀(지구 행성)
    //게임메뉴얼
    auto earthMenual1 = Scene::create("생텀설명1", "images/지구메뉴얼.png");//메뉴얼 만들기
    //auto earthMenual2 = Scene::create("생텀설명2", "images/earthMenual2.png");//메뉴얼 만들기
    auto earthGameScene = Scene::create("뉴욕생텀(지구)", "images/뉴욕생텀.png");
    auto hulk = Object::create("images/헐크.png", earthGameScene, 0, 0);
    auto earthGameSceneV = Scene::create("뉴욕생텀(지구)", "images/뉴욕승리.png");
    earthGameSceneV->setOnEnterCallback([&](ScenePtr p)->bool {
        auto xvo = makeNextButton(earthGameSceneV, gameMap, 800, 50, true, false, "images/메인으로.png");
        return true; });

    

    //다음 이전 장 가는 버튼
    auto earth = makeNextButton(gameMap, earthMenual1, 700, 120, true,false,"images/earth.png");
    auto nextBE1 = makeNextButton(earthMenual1, earthGameScene, 1000, 10);
    auto prevBE1 = makeNextButton(earthMenual1, gameMap, 900, 10, false);
   // auto nextBE2 = makeNextButton(earthMenual2, earthGameScene, 1000, 10);
    //auto prevBE2 = makeNextButton(earthMenual2, earthMenual1, 900, 10, false);
    bool solReady = false;
    int okSeqenece = 0;//순서 맞춘 갯수
    
    //게임 구현
    auto timeS = Object::create("images/타임스톤.png", gameMap, 700, 120, false);
    timeS->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool {
        o->pick();
        return true; });

    int appearance = 5;
    int nowAppearNum = 0;
    float appearTime = 0.5f, solTime = 30.0f;
    auto acientOneTimer = Timer::create(appearTime);
    auto solTimer = Timer::create(solTime);
    int sequenceGame[5][3][4] = { 0 };// 5는 게임횟수
    int aX, aY;
    auto lastterm = Timer::create(0.5f);

 
    int acientOnePointer[3][4][2];
    ObjectPtr acientOneBlock[3][4];
    ObjectPtr acientOne[3][4];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            acientOnePointer[i][j][0] = 300 + j * 200;
            acientOnePointer[i][j][1] =  i * 200;
            cout << acientOnePointer[i][i][0] << " " << acientOnePointer[i][j][1] << endl;
            acientOneBlock[i][j] = Object::create("images/마법진.png", earthGameScene, acientOnePointer[i][j][0], acientOnePointer[i][j][1]);
            acientOne[i][j] = Object::create("images/에이션트원.png", earthGameScene, acientOnePointer[i][j][0], acientOnePointer[i][j][1], false);
            acientOneBlock[i][j]->setOnMouseCallback([&](ObjectPtr o, int x, int y ,MouseAction)->bool {
                if (solReady) {
                    for (int k = 0; k < 3; k++) {
                        for (int l = 0; l < 4; l++) {
                            if (o == acientOneBlock[k][l]) {
                                if (sequenceGame[okSeqenece][k][l] == 1) {
                                    okSeqenece++;
                                    solTimer->stop();
                                    break;
                                }
                                else {
                                    okSeqenece = 0;
                                    showMessage("순서가 틀렸습니다! 게임 패배");
                                    acientOneTimer->stop();
                                    solTimer->stop();
                                    hideTimer();
                                    eF->enter();

                                }
                            }
                        }
                    }
                    if (okSeqenece >= 5) {//승리
                        showMessage("승리! 에이션트 원이 타임스톤을 주었습니다!");
                        acientOneTimer->stop();
                        solTimer->stop();
                        hideTimer();
                        earth->hide();
                        stones[2] = true;
                        timeS->show();
                        earthGameSceneV->enter();
                    }
                   
                }
                return true; });
        }
    }   
    acientOneTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        if (nowAppearNum >= 1) {
            acientOne[aY][aX]->hide();
            while (sequenceGame[nowAppearNum - 1][aY][aX] == 1) {
                aX = rand() % 4;
                aY = rand() % 3;
            }
        }
        else {
            aX = rand() % 4;
            aY = rand() % 3;
        }
        nowAppearNum++;
        sequenceGame[nowAppearNum - 1][aY][aX] = 1;
        acientOne[aY][aX]->show();

        if (nowAppearNum >= 5) {
            showMessage("에이션트 원이 나타난 마법진을 순서대로 클릭하세요! 시간제한 있습니다!");
            showTimer(solTimer);
            lastterm->start();
            solTimer->start();
            solReady = true;
        }
        else {
            acientOneTimer->set(appearTime);
            acientOneTimer->start();
        }
        return true; });

    lastterm->setOnTimerCallback([&](TimerPtr t)->bool {
        acientOne[aY][aX]->hide();
        return true;
        });

    solTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        showMessage("시간종료! 다시 도전 하시십시요!");
        acientOneTimer->stop();
        solTimer->stop();
        hideTimer();
        eF->enter();
        return true; });

    earthGameScene->setOnEnterCallback([&](ScenePtr s)->bool {
        {
            solReady = false;
            okSeqenece = 0;
            appearance = 5;
            nowAppearNum = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 4; k++) {
                        sequenceGame[i][j][k] = 0;
                    }
                }
            }
        }
        acientOneTimer->set(appearTime);
        acientOneTimer->start();
        lastterm->set(0.5f);
        solTimer->set(solTime);
        return true; });


    //################4.노웨어 행성
    //게임메뉴얼
    auto nowhereMenual1 = Scene::create("노웨어설명1", "images/노웨어메뉴얼.png");//메뉴얼 만들기
    //auto nowhereMenual2 = Scene::create("노웨어설명2", "images/nowhereMenual2.png");//메뉴얼 만들기
    auto nowhereGameScene = Scene::create("노웨어행성", "images/노웨어게임1.png");
    auto nowhereGameSceneV = Scene::create("노웨어행성", "images/노웨어승리.png");
    nowhereGameSceneV->setOnEnterCallback([&](ScenePtr p)->bool {
        auto xvo = makeNextButton(nowhereGameSceneV, gameMap, 800, 50, true, false, "images/메인으로.png");
        return true; });

    //다음 이전 장 가는 버튼
    auto nowhere = makeNextButton(gameMap, nowhereMenual1, 730, 420, true,false,"images/nowhere.png");
    auto nextBN1 = makeNextButton(nowhereMenual1, nowhereGameScene, 1000, 10);
    auto prevBN1 = makeNextButton(nowhereMenual1, gameMap, 900, 10, false);
    //auto nextBN2 = makeNextButton(nowhereMenual2, nowhereGameScene, 1000, 10);
    //auto prevBN2 = makeNextButton(nowhereMenual2, nowhereMenual1, 900, 10, false);


    
    //게임 구현
    
    auto realityS = Object::create("images/리얼리티스톤.png", gameMap, 730, 420, false);
    realityS->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool {
        o->pick();
        return true; });

    float playerTime = 0.1f, playerGenerateTime = 0.5f, gameTime1 = 60.0f;
    auto gameTimer1 = Timer::create(gameTime1);
    auto playerTimer = Timer::create(playerTime);
    auto playerGTimer = Timer::create(playerGenerateTime);
    auto noS = Sound::create("images/노웨어.mp3");
    Queue30Object gameKey;
    char filenames[4][20] = { "images/상키.png","images/하키.png" ,"images/좌키.png" ,"images/우키.png" };
    auto sc10 = Object::create("images/0.png", nowhereGameScene, 200, 620);
    auto sc1 = Object::create("images/0.png", nowhereGameScene, 300, 620);
    

    int correctPress = 0;//맞게 누른 키

    gameTimer1->setOnTimerCallback([&](TimerPtr t)->bool {
        showMessage("시간초과 게임패배!");
        gameTimer1->stop();
        playerTimer->stop();
        playerGTimer->stop();
        noS->stop();
        hideTimer();
        nF->enter();
        return true; });

    playerGTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        int i = rand() % 4;
        int ks;
        if (i == 0) { ks = 84; }
        else if (i == 1) { ks = 85; }
        else if (i == 2) { ks=82; }
        else { ks = 83; }
        
        auto gameB = Object::create(filenames[i], nowhereGameScene, 1152, 316);
        gameKey.inputKey(ks);
        gameKey.Enqueue(1152, gameB);
        playerGTimer->set(playerGenerateTime);
        playerGTimer->start();
        return true; });

    playerTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        gameKey.moveAll(316, nowhereGameScene, -128);
        if (gameKey.firstElem() < 0) {
            int a = gameKey.Dequeue();
        }
        playerTimer->set(playerTime);
        playerTimer->start();
        return true; });

    nowhereGameScene->setOnEnterCallback([&](ScenePtr s)->bool {
        gameKey.allRevmove();
        sc10->setImage("images/0.png"); sc1->setImage("images/0.png");
        correctPress = 0;
        showMessage("나와 거래를 할 것인가? 스톤과 너의 목숨 어때?");
        noS->play(true);
        playerTimer->set(playerTime);
        playerTimer->start();
        playerGTimer->set(playerGenerateTime);
        playerGTimer->start();
        gameTimer1->set(gameTime1);
        showTimer(gameTimer1);
        gameTimer1->start();
        return true; });
    nowhereGameScene->setOnKeyboardCallback([&](ScenePtr s,int key,bool pressed)->bool {
        if (gameKey.firstElem() <= 200) {
            if (key == gameKey.outputKey()){
                int a =gameKey.Dequeue();
                correctPress++;
                int ind10 = correctPress / 10;
                int ind1 = correctPress % 10;
                string fn10 = "images/" + to_string(ind10) + ".png";
                string fn1 = "images/" + to_string(ind1) + ".png";
                const char* fnc10 = fn10.c_str();
                const char* fnc1 = fn1.c_str();
                sc10->setImage(fnc10);
                sc1->setImage(fnc1);
                if (correctPress >= 40) {//40개 맞추면 승리
                    showMessage("승리! 컬렉터가 스톤을 주었습니다!");
                    gameTimer1->stop();
                    playerTimer->stop();
                    playerGTimer->stop();
                    noS->stop();
                    gameTimer1->stop();
                    hideTimer();
                    nowhere->hide();
                    realityS->show();
                    stones[3] = true;
                    nowhereGameSceneV->enter();
                }
            }
        }
        return true; });


    //################5.도르마무
    //게임메뉴얼
    auto darkMenual1 = Scene::create("다크디멘젼설명1", "images/도르마무메뉴얼.png");//메뉴얼 만들기
    //auto darkMenual2 = Scene::create("다크디멘젼설명2", "images/darkMenual2.png");//메뉴얼 만들기
    auto darkGameScene = Scene::create("다크디멘젼", "images/도르마무게임.png");
    auto darkGameSceneV = Scene::create("다크디멘젼", "images/도르마무승리.png");

    //다음 이전 장 가는 버튼
    auto wakanda = makeNextButton(gameMap, darkMenual1 , 950, 220, true,false,"images/도르마무.png");
    auto nextBW1 = makeNextButton(darkMenual1, darkGameScene, 1000, 10);
    auto prevBW1 = makeNextButton(darkMenual1, gameMap, 900, 10, false);
    //auto nextBW2 = makeNextButton(darkMenual2, darkGameScene, 1000, 10);
   // auto prevBW2 = makeNextButton(darkMenual2, darkMenual1, 900, 10, false);

    //게임 구현
    auto mindS = Object::create("images/마인드스톤.png", gameMap, 950, 220, false);

    
    auto myC = Object::create("images/닥터스트레인지.png", darkGameScene, 0, 200);
    int magicalXP, magicalXidx;
    int nowMyX=0, nowMyY = 200;

    float gameTime2 = 30.0f, magicTime = 0.5f, DATime = 1.0f, fireTime = 0.05f;
    auto fireTimer = Timer::create(fireTime);
    auto gameTimer2 = Timer::create(gameTime2);
    auto magicTimer = Timer::create(magicTime);
    auto DATimer = Timer::create(DATime);
    ObjectPtr gameUse[5];
    ObjectPtr diU[2];

    for(int i = 0; i < 4; i++) {
        gameUse[i + 1] = Object::create("images/화염.png", darkGameScene, i * 320, 150,false);
    }

    darkGameSceneV->setOnEnterCallback([&](ScenePtr p)->bool {
        auto xvo = makeNextButton(darkGameSceneV, gameMap, 800, 50, true, false, "images/메인으로.png");
        fireTimer->stop();
        gameTimer2->stop();
        magicTimer->stop();
        DATimer->stop();
        return true; });

    mindS->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool {
        o->pick();
        return true; });
    
     diU[0] = Object::create("images/좌키.png", darkGameScene, 512, 0);
     diU[0]->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool{
         if (nowMyX >= 320) {
             nowMyX -= 320;
             myC->locate(darkGameScene, nowMyX, nowMyY);
         
         }
         return true; });
     diU[1] = Object::create("images/우키.png", darkGameScene, 640, 0);
     diU[1]->setOnMouseCallback([&](ObjectPtr o, int, int, MouseAction)->bool {
         if (nowMyX <= 940) {
             nowMyX += 320;
             myC->locate(darkGameScene, nowMyX, nowMyY);

         }
         return true; });

    fireTimer->setOnTimerCallback([&](TimerPtr t)->bool {
        gameUse[0]->hide();
        for (int i = 0; i < 4; i++) {
            if (i != magicalXidx) {
                gameUse[i + 1]->hide();
            }
        }
        fireTimer->set(fireTime);
        magicTimer->start();
        return true; });

    gameTimer2->setOnTimerCallback([&](TimerPtr t)->bool {
        showMessage("도르마무와의 거래를 성공하셨습니다!.");
        for (int k = 0; k < 5; k++) {
            gameUse[k]->hide();
        }
        hideTimer();
        stones[4] = true;
        wakanda->hide();
        mindS->show();
        darkGameSceneV->enter();
        return true; });

    magicTimer->setOnTimerCallback([&](TimerPtr t)->bool{
        magicalXidx = rand() % 4;
        gameUse[0] = Object::create("images/마법진2.png", darkGameScene, 320 * magicalXidx, 150);
        magicalXP = 320 * magicalXidx;
        magicTimer->set(magicTime);
        DATimer->start();
        return true; });

    DATimer->setOnTimerCallback([&](TimerPtr t)->bool {
        for(int i = 0; i < 4; i++) {
            if (i != magicalXidx) {
                gameUse[i + 1]->show();
                if (nowMyX == i * 320) {
                    for (int k = 0; k < 5; k++) {
                        gameUse[k]->hide();
                    }
                    showMessage("게임패배! 도르마무와의 협상과 실패");
                    fireTimer->stop();
                    gameTimer2->stop();
                    magicTimer->stop();
                    DATimer->stop();
                    fireTimer->set(fireTime);
                    gameTimer2->set(gameTime2);
                    magicTimer->set(magicTime);
                    DATimer->set(DATime);
                    hideTimer();
                    dF->enter();
                    return true;
                }
            }
        }
        DATimer->set(DATime);
        fireTimer->start();
        return true; });

    darkGameScene->setOnEnterCallback([&](ScenePtr s)->bool {
        nowMyX = 0; nowMyY = 200;
        myC->locate(darkGameScene, 0, 200);
        fireTimer->set(fireTime);
        gameTimer2->set(gameTime2);
        magicTimer->set(magicTime);
        DATimer->set(DATime);
        showTimer(gameTimer2);
        gameTimer2->start();
        magicTimer->start();
        showTimer(gameTimer2);
        return true; });



    auto xFo = makeNextButton(xF, gameMap, 800, 50, true, false, "images/메인으로.png");
    auto aFo = makeNextButton(aF, gameMap, 800, 50, true, false, "images/메인으로.png");
    auto eFo = makeNextButton(eF, gameMap, 800, 50, true, false, "images/메인으로.png");
    auto nFo = makeNextButton(nF, gameMap, 800, 50, true, false, "images/메인으로.png");
    auto dFo = makeNextButton(dF, gameMap, 800, 50, true, false, "images/메인으로.png");

    //6. 보스 타노스

    auto tanosGame = Scene::create("타노스", "images/타노스스테이지.png");
    auto tanosM1 = Scene::create("타노스", "images/타노스등장1.png");
    auto tanosM2 = Scene::create("타노스", "images/타노스설명.png");
    auto tanosGS = Scene::create("최종보스 타노스", "images/타노스게임1.png");
    int TANOS_LIFE = 1;
    int IRON_LIFE = 1;

    auto ILO = Object::create("images/아이언맨목숨.png", tanosGS,500,600);
    auto TNO = Object::create("images/타노스목숨.png", tanosGS,820,600);

    auto ILO100 = Object::create("images/9.png", tanosGS, 580, 550);
    auto ILO10 = Object::create("images/9.png", tanosGS, 640, 550);
    auto ILO1 = Object::create("images/9.png", tanosGS, 720, 550);

    auto TNO100 = Object::create("images/2.png", tanosGS, 880, 550);
    auto TNO10 = Object::create("images/2.png", tanosGS, 960, 550);
    auto TNO1 = Object::create("images/2.png", tanosGS, 1040, 550);

    bool qb = true, wb = true, eb = true, rb = true, tb = true;
    auto QB = Object::create("images/Q.png", tanosGS, 30, 650,false);
    auto WB = Object::create("images/W.png", tanosGS, 110, 650, false);
    auto EB = Object::create("images/E.png", tanosGS, 180, 650, false);
    auto RB = Object::create("images/R.png", tanosGS, 250, 650, false);
    auto TB = Object::create("images/T.png", tanosGS, 320, 650, false);
    auto QSKILL = Scene::create("스킬!", "images/Qskill.png");
    auto WSKILL = Scene::create("스킬!", "images/Wskill.png");
    auto ESKILL = Scene::create("스킬!", "images/Eskill.png");
    auto RSKILL = Scene::create("스킬!", "images/Rskill.png");
    auto TSKILL = Scene::create("스킬!", "images/Tskill.png");

    tanosGame->setOnEnterCallback([&](ScenePtr)->bool {
        showMessage("타노스 등장!");
        return true; });
    
    auto nextTX0 = makeNextButton(tanosGame, tanosM1, 1000, 10);
    auto nextTX1 = makeNextButton(tanosM1, tanosM2, 1000, 10);
    auto prevTX1 = makeNextButton(tanosM1, tanosGame, 900, 10, false);
    auto nextTX2 = makeNextButton(tanosM2, tanosGS, 1000, 10);
    auto prevTX2 = makeNextButton(tanosM2, tanosM1, 900, 10, false);

    auto Tanos = Object::create("images/타노스스탠딩.png", tanosGS, 1000, 0);
    auto ironM = Object::create("images/아이언맨.png", tanosGS, 0, 0);
    auto TanosSkill = tanosGS; 

    
    auto V1 = Scene::create("6개의 인피니티 스톤","images/승리1.png");
    auto V2 = Scene::create("핑거스냅!", "images/승리2.png");
    auto VG1 = Object::create("images/인피니티건틀렛.png", V1, 1000, 0);
    VG1->setOnMouseCallback([&](ObjectPtr,int,int,MouseAction)->bool{
        V2->enter();
        return true;
        });

    auto F1 = Scene::create("패배", "images/패배1.png");

    bool laserS = false, skillS = false, TSkill =false, nowTanosTime = false;
    float TanosWalk = 0.05f, TanosAttack = 0.1f;
    float laserTime = 0.25f, laserSet = 1.5f, skillTimer =0.2f, SKILLT = 0.75f, TSkillTime = 10.0f, TSutill = 3.0f, meG = 0.25f, meM =0.00001f;
    auto TanosWalkTimer = Timer::create(TanosWalk);
    auto TanosAttackTimer = Timer::create(TanosAttack);
    auto laserTimer = Timer::create(laserTime);
    auto laserSetTimer = Timer::create(laserSet);
    auto skillTimer1 = Timer::create(skillTimer);
    auto SKILLTtimer = Timer::create(SKILLT);
    auto TSkillTimer = Timer::create(TSkillTime);
    auto TSutillTimer = Timer::create(TSutill);
    auto MeteoMT = Timer::create(meM);
    auto MeteoGT = Timer::create(meG);
    auto LS = Object::create("images/레이져빔아이콘.png", tanosGS, 400, 650,false);
    auto laser = Object::create("images/레이져빔.png", tanosGS, 0, 0, false);
    int IX = 0, IY = 0;
    int TX = 1000, TY = 0;
    int TStep = 5;
    Queue30Object meteos[4];
    int meLine;
    ObjectPtr safeZone;

    auto goHome = Object::create("images/처음으로.png", V2, 1000, 0);
    auto reTry = Object::create("images/다시도전.png", F1, 950, 0);
    goHome->setOnMouseCallback([&](ObjectPtr, int, int, MouseAction)->bool{
        startScene->enter();
        return true; });

    reTry->setOnMouseCallback([&](ObjectPtr, int, int, MouseAction)->bool {
        tanosGame->enter();
        return true; });
    V1->setOnEnterCallback([&](ScenePtr)->bool {
        IX = 0, IY = 0;
        TX = 1000, TY = 0;
        Tanos->locate(tanosGS, TX, TY);
        ironM->locate(tanosGS, IX, IY);
        QB->show();
        WB->show();
        EB->show();
        RB->show();
        TB->show();
        TANOS_LIFE = 222;
        IRON_LIFE = 999;
        qb = true, wb = true, eb = true, rb = true, tb = true;
        showMessage("타노스가 가지고 있는 소울 스톤을 가져왔습니다! 건틀렛을 눌러주세요!");
        TanosWalkTimer->set(TanosWalk); TanosWalkTimer->stop();
        TanosAttackTimer->set(TanosAttack); TanosAttackTimer->stop();
        laserTimer->set(laserTime); laserTimer->stop();
        laserSetTimer->set(laserSet); laserSetTimer->stop();
        skillTimer1->set(skillTimer); skillTimer1->stop();
        SKILLTtimer->set(SKILLT); SKILLTtimer->stop();
        TSkillTimer->set(TSkillTime); TSkillTimer->stop();
        TSutillTimer->set(TSutill); TSutillTimer->stop();
        MeteoMT->set(meM); MeteoMT->stop();
        MeteoGT->set(meG); MeteoGT->stop();
        return true;
        });
    F1->setOnEnterCallback([&](ScenePtr)->bool {
        IX = 0, IY = 0;
        TX = 1000, TY = 0;
        Tanos->locate(tanosGS, TX, TY);
        ironM->locate(tanosGS, IX, IY);
        QB->show();
        WB->show();
        EB->show();
        RB->show();
        TB->show();
        TANOS_LIFE = 222;
        IRON_LIFE = 999;
        qb = true, wb = true, eb = true, rb = true, tb = true;
        showMessage("게임패배! 인피니티 스톤들을 빼았겼습니다!");
        TanosWalkTimer->set(TanosWalk); TanosWalkTimer->stop();
        TanosAttackTimer->set(TanosAttack); TanosAttackTimer->stop();
        laserTimer->set(laserTime); laserTimer->stop();
        laserSetTimer->set(laserSet); laserSetTimer->stop();
        skillTimer1->set(skillTimer); skillTimer1->stop();
        SKILLTtimer->set(SKILLT); SKILLTtimer->stop();
        TSkillTimer->set(TSkillTime); TSkillTimer->stop();
        TSutillTimer->set(TSutill); TSutillTimer->stop();
        MeteoMT->set(meM); MeteoMT->stop();
        MeteoGT->set(meG); MeteoGT->stop();
        return true;
        });

    TanosWalkTimer->setOnTimerCallback([&](TimerPtr)->bool {

        if (TX - IX >= 0) {
            TX -= TStep;
            Tanos->locate(tanosGS, TX, 0);
            Tanos->setImage("images/타노스스탠딩1.png");
        }
        else {
            TX += TStep;
            Tanos->locate(tanosGS, TX, 0);
            Tanos->setImage("images/타노스스탠딩.png");
        
        }

        if (-30 <= TX - IX && TX - IX <= 30){
            Tanos->setImage("images/타노스공격.png");
            IRON_LIFE -= 5;
            int temp100 = IRON_LIFE / 100, temp10 = (IRON_LIFE % 100) / 10, temp1 = IRON_LIFE % 10;
            string temps100 = "images/" + to_string(temp100) + ".png";
            string temps10 = "images/" + to_string(temp10) + ".png";
            string temps1 = "images/" + to_string(temp1) + ".png";
            const char* tempc100 = temps100.c_str();
            const char* tempc10 = temps10.c_str();
            const char* tempc1 = temps1.c_str();
            ILO100->setImage(tempc100); ILO10->setImage(tempc10); ILO1->setImage(tempc1);
            if (IRON_LIFE <= 0) {
                F1->enter();
            
            }
        }
        else {
            Tanos->setImage("images/타노스스탠딩.png"); 
        }

        TanosWalkTimer->set(TanosWalk);
        TanosWalkTimer->start();
        return true; });

    TanosSkill->setOnEnterCallback([&](ScenePtr s)->bool {
        MeteoGT->start();
        MeteoMT->start();
        return true; });

    MeteoGT->setOnTimerCallback([&](TimerPtr)->bool {
        for (int i = 0; i < 4; i++) {
            if (i != meLine) {
                auto meO = Object::create("images/메테오.png", TanosSkill, 320 * i, 350);
                meteos[i].Enqueue(350, meO);
            }
        }
        MeteoGT->set(meG);
        MeteoGT->start();
        return true; });

    MeteoMT->setOnTimerCallback([&](TimerPtr)->bool {
            for (int i = 0; i < 4; i++) {
                if (i != meLine) {
                    meteos[i].moveAllYAXIS(320 * i, TanosSkill, -10);
                }
            }
            if (!(320* meLine <= IX && IX <= 320 * meLine+320)) {
                IRON_LIFE -= 1;
                int temp100 = IRON_LIFE / 100, temp10 = (IRON_LIFE % 100) / 10, temp1 = IRON_LIFE % 10;
                string temps100 = "images/" + to_string(temp100) + ".png";
                string temps10 = "images/" + to_string(temp10) + ".png";
                string temps1 = "images/" + to_string(temp1) + ".png";
                const char* tempc100 = temps100.c_str();
                const char* tempc10 = temps10.c_str();
                const char* tempc1 = temps1.c_str();
                ILO100->setImage(tempc100); ILO10->setImage(tempc10); ILO1->setImage(tempc1);
                if (IRON_LIFE <= 0) {
                    F1->enter();
                }
            }
        MeteoMT->set(meM);
        MeteoMT->start();
        return true; });

    TSkillTimer->setOnTimerCallback([&](TimerPtr)->bool {
        nowTanosTime = true;
        Tanos->setImage("images/타노스메테오.png");
        TanosWalkTimer->stop();
        meLine = rand() % 4;
        safeZone = Object::create("images/마법진2.png", TanosSkill, 320 * meLine, 0);
        TSkillTimer->set(TSkillTime);
        TSkillTimer->stop();
        MeteoGT->start();
        MeteoMT->start();
        TSutillTimer->set(TSutill);
        TSutillTimer->start();
        TanosSkill->enter();
        return true; });

    TSutillTimer->setOnTimerCallback([&](TimerPtr)->bool {
        for (int i = 0; i < 5; i++) {
            if (i != meLine) {
                meteos[i].allRevmove();
            }
        }
        MeteoGT->set(meG);
        MeteoMT->set(meM);
        MeteoGT->stop();
        MeteoMT->stop();
        safeZone->hide();
        TSutillTimer->stop();
        TSutillTimer->set(TSutill);
        TSkillTimer->start();
        tanosGS->enter();
        TanosWalkTimer->start();
        nowTanosTime = false;
        return true; });

    SKILLTtimer->setOnTimerCallback([&](TimerPtr)->bool {
        Tanos->setImage("images/타노스스탠딩.png");
        Tanos->locate(tanosGS, TX, TY);
        tanosGS->enter();
        SKILLTtimer->set(SKILLT);
        SKILLTtimer->stop();
        return true; });

    skillTimer1->setOnTimerCallback([&](TimerPtr)->bool {
        skillS = true;
        if (qb) { QB->show(); }
        if (wb) { WB->show(); }
        if (eb) { EB->show(); }
        if (rb) { RB->show(); }
        if (tb) { TB->show(); }
        skillTimer1->set(laserSet); skillTimer1->start();
        return true; });


    laserSetTimer->setOnTimerCallback([&](TimerPtr)->bool {
        laserS = true;
        LS->show();
        laserSetTimer->stop();
        laserSetTimer->set(laserSet);
        return true; });

    laserTimer->setOnTimerCallback([&](TimerPtr)->bool {
        laser->hide();
        laserTimer->stop();
        laserTimer->set(laserTime);

        return true; });

    int IDXYGround[4][2] = { {-20,0},{20,0},{0,100},{0,-100}};
    int IDXY[4][2];
    int IDXYFlying[4][2] = { {-60,0},{60,0},{0,100},{0,-100} };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            IDXY[i][j] = IDXYGround[i][j];
        }
    }
    int nowIndex =1;
    tanosGS->setOnKeyboardCallback([&](ScenePtr s, int k, int p)->bool {
        if (IY == 0) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 2; j++) {
                    IDXY[i][j] = IDXYGround[i][j]; 
                }
            }     
        }
        else {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 2; j++) {
                    IDXY[i][j] = IDXYFlying[i][j];
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            cout << k <<"  " << IX + IDXY[i][0] <<"  "<< IY + IDXY[i][0]<<endl;
            if (k - 82 == i &&(IX+ IDXY[i][0]>=0)&& (IX + IDXY[i][0] <= 1030)&&(IY + IDXY[i][1]>=0) && (IY + IDXY[i][1] <= 300) ){
                nowIndex = i;
                IX += IDXY[i][0];
                IY += IDXY[i][1];
                if (IY > 0) {
                    if (IDXY[i][0] < 0) {
                        ironM->setImage("images/아이언맨공중폼1.png");
                    }
                    else {
                        ironM->setImage("images/아이언맨공중폼.png");
                    }
                }
                else {
                    if (IDXY[i][0] < 0) {
                        ironM->setImage("images/아이언맨1.png");
                    }
                    else {
                        ironM->setImage("images/아이언맨.png");
                    }
                
                
                }
                ironM->locate(tanosGS, IX, IY);
            }
        }
        if (!nowTanosTime) {
            if (k == 75 && laserS) {

                if (IDXY[nowIndex][0] < 0) {
                    ironM->setImage("images/아이언맨레이져빔1.png");
                    laser->setImage("images/레이져빔.png");
                    laser->locate(tanosGS, -1280 + IX, IY + 55);
                }
                else {
                    ironM->setImage("images/아이언맨레이져빔.png");
                    laser->setImage("images/레이져빔1.png");
                    laser->locate(tanosGS, IX + 250, IY + 55);
                }
                laser->show();
                laserTimer->start();
                laserSetTimer->start();
                laserS = false;
                LS->hide();
                TANOS_LIFE -= 1;
                if (TANOS_LIFE <= 0) {
                    V1->enter();
                }
                int temp100 = TANOS_LIFE / 100, temp10 = (TANOS_LIFE % 100) / 10, temp1 = TANOS_LIFE % 10;
                string temps100 = "images/" + to_string(temp100) + ".png";
                string temps10 = "images/" + to_string(temp10) + ".png";
                string temps1 = "images/" + to_string(temp1) + ".png";
                const char* tempc100 = temps100.c_str();
                const char* tempc10 = temps10.c_str();
                const char* tempc1 = temps1.c_str();
                TNO100->setImage(tempc100); TNO10->setImage(tempc10); TNO1->setImage(tempc1);
            }
            if (k == 17) {
                if (skillS && qb) {
                    TSkillTimer->set(TSkillTime);
                    TSutillTimer->set(TSutill);
                    TSkillTimer->stop();
                    TSutillTimer->stop();
                    TanosWalkTimer->stop();
                    TSkillTimer->stop();
                    QSKILL->enter();
                    TANOS_LIFE -= 40;
                    if (TANOS_LIFE <= 0) {
                        V1->enter();
                    }
                    int temp100 = TANOS_LIFE / 100, temp10 = (TANOS_LIFE % 100) / 10, temp1 = TANOS_LIFE % 10;
                    string temps100 = "images/" + to_string(temp100) + ".png";
                    string temps10 = "images/" + to_string(temp10) + ".png";
                    string temps1 = "images/" + to_string(temp1) + ".png";
                    const char* tempc100 = temps100.c_str();
                    const char* tempc10 = temps10.c_str();
                    const char* tempc1 = temps1.c_str();
                    TNO100->setImage(tempc100); TNO10->setImage(tempc10); TNO1->setImage(tempc1);
                    qb = false;
                    QB->hide();
                    skillTimer1->set(skillTimer);
                    skillTimer1->start();
                    SKILLTtimer->start();
                    Tanos->setImage("images/타노스데미지.png");
                    Tanos->locate(QSKILL, 350, 100);
                }
            }
            else if (k == 23) {
                if (skillS && wb) {
                    TSkillTimer->set(TSkillTime);
                    TSutillTimer->set(TSutill);
                    TSkillTimer->stop();
                    TSutillTimer->stop();
                    TanosWalkTimer->stop();
                    TSkillTimer->stop();
                    WSKILL->enter();
                    TANOS_LIFE -= 40;
                    if (TANOS_LIFE <= 0) {
                        V1->enter();
                    }
                    int temp100 = TANOS_LIFE / 100, temp10 = (TANOS_LIFE % 100) / 10, temp1 = TANOS_LIFE % 10;
                    string temps100 = "images/" + to_string(temp100) + ".png";
                    string temps10 = "images/" + to_string(temp10) + ".png";
                    string temps1 = "images/" + to_string(temp1) + ".png";
                    const char* tempc100 = temps100.c_str();
                    const char* tempc10 = temps10.c_str();
                    const char* tempc1 = temps1.c_str();
                    TNO100->setImage(tempc100); TNO10->setImage(tempc10); TNO1->setImage(tempc1);
                    wb = false;
                    WB->hide();
                    skillTimer1->set(skillTimer);
                    skillTimer1->start();
                    SKILLTtimer->start();
                    Tanos->setImage("images/타노스데미지.png");
                    Tanos->locate(WSKILL, 350, 100);
                }
            }
            else if (k == 5) {
                if (skillS && eb) {
                    TSkillTimer->set(TSkillTime);
                    TSutillTimer->set(TSutill);
                    TSkillTimer->stop();
                    TSutillTimer->stop();
                    TanosWalkTimer->stop();
                    TSkillTimer->stop();
                    ESKILL->enter();
                    TANOS_LIFE -= 40;
                    if (TANOS_LIFE <= 0) {
                        V1->enter();
                    }
                    int temp100 = TANOS_LIFE / 100, temp10 = (TANOS_LIFE % 100) / 10, temp1 = TANOS_LIFE % 10;
                    string temps100 = "images/" + to_string(temp100) + ".png";
                    string temps10 = "images/" + to_string(temp10) + ".png";
                    string temps1 = "images/" + to_string(temp1) + ".png";
                    const char* tempc100 = temps100.c_str();
                    const char* tempc10 = temps10.c_str();
                    const char* tempc1 = temps1.c_str();
                    TNO100->setImage(tempc100); TNO10->setImage(tempc10); TNO1->setImage(tempc1);
                    eb = false;
                    EB->hide();
                    skillTimer1->set(skillTimer);
                    skillTimer1->start();
                    SKILLTtimer->start();
                    Tanos->setImage("images/타노스데미지.png");
                    Tanos->locate(ESKILL, 350, 100);
                }
            }
            else if (k == 18) {
                if (skillS && rb) {
                    TSkillTimer->set(TSkillTime);
                    TSutillTimer->set(TSutill);
                    TSkillTimer->stop();
                    TSutillTimer->stop();
                    TanosWalkTimer->stop();
                    TSkillTimer->stop();
                    RSKILL->enter();
                    TANOS_LIFE -= 40;
                    if (TANOS_LIFE <= 0) {
                        V1->enter();
                    }
                    int temp100 = TANOS_LIFE / 100, temp10 = (TANOS_LIFE % 100) / 10, temp1 = TANOS_LIFE % 10;
                    string temps100 = "images/" + to_string(temp100) + ".png";
                    string temps10 = "images/" + to_string(temp10) + ".png";
                    string temps1 = "images/" + to_string(temp1) + ".png";
                    const char* tempc100 = temps100.c_str();
                    const char* tempc10 = temps10.c_str();
                    const char* tempc1 = temps1.c_str();
                    TNO100->setImage(tempc100); TNO10->setImage(tempc10); TNO1->setImage(tempc1);
                    rb = false;
                    RB->hide();
                    skillTimer1->set(skillTimer);
                    skillTimer1->start();
                    SKILLTtimer->start();
                    Tanos->setImage("images/타노스데미지.png");
                    Tanos->locate(RSKILL, 350, 100);
                }
            }
            else if (k == 20) {
                if (skillS && tb) {
                    TSkillTimer->set(TSkillTime);
                    TSutillTimer->set(TSutill);
                    TSkillTimer->stop();
                    TSutillTimer->stop();
                    TanosWalkTimer->stop();
                    TSkillTimer->stop();
                    TSKILL->enter();
                    TANOS_LIFE -= 40;
                    if (TANOS_LIFE <= 0) {
                        V1->enter();
                    }
                    int temp100 = TANOS_LIFE / 100, temp10 = (TANOS_LIFE % 100) / 10, temp1 = TANOS_LIFE % 10;
                    string temps100 = "images/" + to_string(temp100) + ".png";
                    string temps10 = "images/" + to_string(temp10) + ".png";
                    string temps1 = "images/" + to_string(temp1) + ".png";
                    const char* tempc100 = temps100.c_str();
                    const char* tempc10 = temps10.c_str();
                    const char* tempc1 = temps1.c_str();
                    TNO100->setImage(tempc100); TNO10->setImage(tempc10); TNO1->setImage(tempc1);
                    tb = false;
                    TB->hide();
                    skillTimer1->set(skillTimer);
                    skillTimer1->start();
                    SKILLTtimer->start();
                    Tanos->setImage("images/타노스데미지.png");
                    Tanos->locate(TSKILL, 350, 100);
                }
            }
        }


        return true;
        });

    tanosGS->setOnEnterCallback([&](ScenePtr)->bool {
        TanosWalkTimer->start();
        TSkillTimer->start();
        laserSetTimer->start();
        skillTimer1->start();
        return true; });


    //$$타이머 다 멈춤
    gameMap->setOnEnterCallback([&](ScenePtr s)->bool {
        //1잔다르 타이머들
        xandarGenerateTimer->stop();
        xandarMoveTimer->stop();
        moveFireTimer->stop();
        setFireTimer->stop();
        
        //2아스가르드 타이머들
        HellaTimer->stop();
        AttackTimer->stop();

        //3뉴욕지구 타이머들
        acientOneTimer->stop();
        solTimer->stop();

        //4노웨어 타이머들
        gameTimer1->stop();
        playerTimer->stop();
        playerGTimer->stop();

        //5도르마무 타이머들
        fireTimer->stop();
        gameTimer2->stop();
        magicTimer->stop();
        DATimer->stop();

        if (have5Stone(stones)) {
            tanosGame->enter();
        }

        return true; });

    //$$모든 초기화 할것 여기에 적기
    startScene->setOnEnterCallback([&](ScenePtr s)->bool {
        for (int i = 0; i < 5; i++) {
            stones[i] = false;
        }
        return true; });

    startGame(startScene);
}
