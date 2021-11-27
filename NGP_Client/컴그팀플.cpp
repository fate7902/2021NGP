#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "ReadObj.h"
#include "Map.h"
#include "Naruto.h"
#include "stb_image.h"
#include "sound.h"
#include "monster.h"
#include "object1.h"
#include "object2.h"
#include "behind.h"
#include  <string.h>
#include "text.h"
#include "network.h"

#include "..\NGP\NGP\protocol.h"
using namespace std;

GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
GLuint vertexshader, fragmentshader; // 세이더 객체
GLuint s_program[6];      //사용 세이더 프로그램      //0 : 바닥, 1 : 나루토, 2: 건물, 3: 배경

//함수의 모음
GLvoid reshape(int w, int h);
GLvoid drawScene();         //그려주는 함수
void make_vertexShader();      //버텍스 쉐이더 만듬
void make_fragmentShader();      //프래그먼트 쉐이더 만듬
void InitBuffer();      //버퍼 만드는 함수
void InitShader();      //프로그램 만드는 함수
void InitTexture();     //텍스쳐 함수
char* filetobuf(char* file);   //쉐이더를 사용하기 위해 파일을 가져오는 함수
GLvoid TimerFunction(int value);
GLvoid ObjectTimer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

void restart();

//텍스처
unsigned int texture1, texture2, texture3, texture4, texture5, texture6;

//변수의 모음
//객체관련
GLuint vao[5];      //0 바닥, 1: 정육면체, 2: 와이어, 3 : 바닥, 4: 시작버튼
GLuint vbo[3];
GLuint ebo;

//ReadObj관련
//사각형 read
float* vPosData;
float* vNormalData;
float* vTextureCoordinateData;
int* indexData;
int vertexCount;
int indexCount;
//구 read
float* vPosDatas;
float* vNormalDatas;
float* vTextureCoordinateDatas;
int* indexDatas;
int vertexCounts;
int indexCounts;

//명령 관련

bool boolt = false;
bool M_boolt = false; //몬스터 팔 회전 

float setac = 0.0f;   //카메라 공전
float setay = 0.0f;      //카메라 자전
float transz = 0.0f;       //카메라 축이동

float speed = 0.17;
float jumpspeedup = 0.5;
float jumpspeeddown = 0.05;
float mainwirespeed = 0.07;
float subwirespeed = 0.035;
float setatspeed = 3.0f;        //다리 회전속도

float mouseX;
float mouseY;
//float yy = 5.0f;

//장애물 위치 위한 변수 
int num = 0;
int num2 = 0;
bool left_Object = false;
bool right_Object = false;
bool mid_Object = false;
bool set_Object1 = false;
bool set_Object2 = false;

bool boolw2 = false;  //대각선 이동시 속도 줄이기 위함 
//타이머 1
unsigned int timer2 = 1000;
//바닥을 위한 좌표
GLfloat floorShape[6][3] = {
   {-1.0f,0.0f,-1.0f},
   {-1.0f,0.0f,1.0f},
   {1.0f,0.0f,-1.0f},
   {1.0f,0.0f,-1.0f},
   {-1.0f,0.0f,1.0f},
   {1.0f,0.0f,1.0f}
};
GLfloat floorColor[6][3] = {
   {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},
   {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},
   {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}
};

float backGround[] = {
    -1.0,1.0,0.0, 0.0f,1.0f,0.0f,  0.0,1.0,//6
    -1.0,-1.0,0.0, 0.0f,0.0f,0.5f, 0.0,0.0,//8
     1.0,1.0,0.0, 0.5f,0.0f,0.5f,  1.0,1.0,//5

     1.0,1.0,0.0, 0.5f,0.0f,0.5f,  1.0,1.0,//5
     -1.0,-1.0,0.0, 0.0f,0.0f,0.5f, 0.0,0.0,//8
      1.0,-1.0,0.0, 0.5f,0.0f,0.0f,  1.0,0.0//7
};
float startbutton[] = {
     -0.3,0.1,0.0, 0.0f,1.0f,0.0f,  0.0,1.0,//6
    -0.3,-0.1,0.0, 0.0f,0.0f,0.5f, 0.0,0.0,//8
     0.3,0.1,0.0, 0.5f,0.0f,0.5f,  1.0,1.0,//5

     0.3,0.1,0.0, 0.5f,0.0f,0.5f,  1.0,1.0,//5
     -0.3,-0.1,0.0, 0.0f,0.0f,0.5f, 0.0,0.0,//8
      0.3,-0.1,0.0, 0.5f,0.0f,0.0f,  1.0,0.0//7
};

void COLL_CHECK(glm::vec3 user, glm::vec3 obj)
{
   
   if ((user.z -0.5 > obj.z - 1 && user.z + 0.5 < obj.z +1) && (user.x  - 0.5> obj.x - 2 && user.x +0.5 < obj.x + 2))
    {
      gameover = true;

    }
}

Network net;

//함수들 작성의 시작
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
    sound2();
    //--- 윈도우 생성하기
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("NGP2021");
    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;      //openGL extention 정보를 가져올 수 있도록 설정
    if (glewInit() != GLEW_OK) {            //glew 초기화
        std::cerr << "Unable to initialize GLEW" << std::endl;         //초기화 할수 없을때 에러 출력
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW LInitialized\n";

    net.network();

    ReadObj("cube.obj", vPosData, vNormalData, vTextureCoordinateData, indexData, vertexCount, indexCount, false);
    ReadObj("sphere.obj", vPosDatas, vNormalDatas, vTextureCoordinateDatas, indexDatas, vertexCounts, indexCounts, true);
    my_init();
    InitMap();
    InitBuffer();
    InitShader();
    InitTexture();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutTimerFunc(timer2, TimerFunction, 0);
    glutTimerFunc(3000, ObjectTimer, 0);  //1초

    glutMainLoop();

}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
    //--- 변경된 배경색 설정
    glClearColor(1.0, 1.0, 1.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //행렬 
    glm::mat4 transformMatrix(1.0f);      //월드변환
    glm::mat4 viewMatrix(1.0f);            //뷰변환
    glm::mat4 projectionMatrix(1.0f);      //투영변환
    glm::mat4 basicChange(1.0f);         //기본축사용시
    glm::mat4 basicChange2(1.0f);         //기본축사용시

    glm::mat4 transMatrixj(1.0f);         //명령j/J
    glm::mat4 rotateMatrixm(1.0f);         //명령m/M
    glm::mat4 rotateMatrixt(1.0f);         //명령t/T
    glm::mat4 scalehandfoot(1.0f);         //손과 발의 사이즈 책정
    glm::mat4 transMatrixpoint(1.0f);      //몸통의 이동

    glm::mat4 charTerrace(1.0f);
    charTerrace = glm::translate(charTerrace, glm::vec3(0.0f, 2.0f, 0.0f));

    //색깔
    glm::vec3 objColor(1.0f, 1.0f, 1.0f);
    //버텍스 연결(유니폼 연결)
    unsigned int transformLocation;
    unsigned int viewLocation;
    unsigned int projectionLocation;
    unsigned int objcolorLocation;

    if (!gameover && !gamestart && !gameclear) {
        glDisable(GL_DEPTH_TEST);
        //배경-------------------------------
        glUseProgram(s_program[3]);
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::mat4(1.0f);  //맵크기?
        transformLocation = glGetUniformLocation(s_program[3], "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objColor = glm::vec3(-1.0f, 1.0f, 1.0f);
        objcolorLocation = glGetUniformLocation(s_program[3], "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);

        glBindVertexArray(vao[3]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //--------------------------------------
        //뷰
        glm::vec3 cameraPos(1.0f);
        if (!(map2))
            cameraPos = glm::vec3((10.0f + transz) * sin(glm::radians(setac)), 8.0f, (11.0f + transz + 3.0f) * cos(glm::radians(setac)) + pointz);
        else
            cameraPos = glm::vec3(narutoCoord[net.getMyId()].x - 11.0f, 5.0f, -11.5 * 6 - 2.5f - 5.5f);
        glm::vec4 ang(0.0f, 0.0f, 0.0f, 0.0f);
        ang.x = ang.x - ((10.0f + transz) * sin(glm::radians(setac)));
        ang.z = ang.z - ((10.0f + transz) * cos(glm::radians(setac)));

        glm::mat4 ang2(1.0f);
        ang2 = glm::rotate(ang2, glm::radians(setay), glm::vec3(0.0f, 1.0f, 0.0f));
        ang = ang2 * ang;

        ang.x = ang.x + ((10.0f + transz) * sin(glm::radians(setac)));
        ang.z = ang.z + ((10.0f + transz) * cos(glm::radians(setac)));


        glm::vec3 cameraDirection(1.0f);
        if (!(map2))
            cameraDirection = glm::vec3(ang.x, 3.0f, ang.z + pointz - 0.2f);
        else
            cameraDirection = glm::vec3(ang.x + pointx, 0.0f, -11.5 * 6 - 2.5f - 5.5f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        viewMatrix = glm::lookAt(cameraPos, cameraDirection, cameraUp);


        //투영
        projectionMatrix = glm::perspective(glm::radians(45.0f), (float)600.0f / (float)600.0f, 0.1f, 50.0f);

        // 오브젝트 좌표
        //명령 //그릴 위치 계속 지정 
        
        for (const auto& client : net.clients) {
            if (client.alive) {
                narutoCoord[client.id] = glm::vec3(client.x, client.y , client.z);        //캐릭터의 위치
            }
        }

        MonsterCoord = glm::vec3(M_pointx - 1.4, 0.0, M_pointz-20);        //괴물 위치
        Object1Coord = glm::vec3(O1_pointx, 0.0, O1_pointz -40);
        Object2Coord = glm::vec3(O2_pointx, O2_pointy, O2_pointz -40);
        BehindCoord = glm::vec3(B_pointx, 0.0, B_pointz);

    
        scalehandfoot = glm::scale(basicChange, glm::vec3(0.1f, 0.5f, 0.1f));         //손과발의 크기 축소(기본행렬)
        rotateMatrixm = glm::rotate(rotateMatrixm, glm::radians(setam), glm::vec3(0.0f, 1.0f, 0.0f));      //몸통 회전

        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        //바닥
        glUseProgram(s_program[0]);
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::scale(basicChange, glm::vec3(10.0f, 0.0f, 1000.0f));      //사이즈 조절  1000은 약 1분36초 
        basicChange2 = glm::mat4(1.0f);
        basicChange2 = glm::translate(basicChange2, glm::vec3(0.0f, -2.0f, 0.0f));
        transformMatrix = basicChange2 * basicChange * transformMatrix;

        objColor = glm::vec3(0.7f, 0.7f, 1.0f);

        transformLocation = glGetUniformLocation(s_program[0], "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(s_program[0], "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        //바디
        for (const auto& client : net.clients) {
            if (client.alive) {
                drawNaruto(s_program[1], vertexCount, vao, vbo, viewMatrix, projectionMatrix, client.id);
            }
        }

        drawMonster(s_program[1], vertexCount, vao, vbo, viewMatrix, projectionMatrix);
        drawObject1(s_program[1], vertexCount, vao, vbo, viewMatrix, projectionMatrix);
        drawObject2(s_program[1], vertexCount, vao, vbo, viewMatrix, projectionMatrix);
        drawBehind(s_program[1], vertexCount, vao, vbo, viewMatrix, projectionMatrix);

        //건물그리기
        drawMap(s_program[2], vertexCount, vao, viewMatrix, projectionMatrix);
    }

    if (gamestart) {
        glUseProgram(s_program[3]);
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::mat4(1.0f);
        transformLocation = glGetUniformLocation(s_program[3], "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objColor = glm::vec3(-1.0f, 1.0f, 1.0f);
        objcolorLocation = glGetUniformLocation(s_program[3], "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);

        glBindVertexArray(vao[3]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //startbutton
        glUseProgram(s_program[4]);
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::mat4(1.0f);
        transformLocation = glGetUniformLocation(s_program[4], "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(s_program[4], "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        projectionLocation = glGetUniformLocation(s_program[4], "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objColor = glm::vec3(-1.0f, 1.0f, 1.0f);
        objcolorLocation = glGetUniformLocation(s_program[4], "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);

        glBindVertexArray(vao[4]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    if (gameover) {//gameover일때 종료
        glUseProgram(s_program[3]);
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::mat4(1.0f);
        transformLocation = glGetUniformLocation(s_program[3], "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objColor = glm::vec3(-1.0f, 1.0f, 1.0f);
        objcolorLocation = glGetUniformLocation(s_program[3], "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);

        glBindVertexArray(vao[3]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }

    if (gameclear) {
        if (perfectclear) {
            glUseProgram(s_program[3]);
            viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            transformMatrix = glm::mat4(1.0f);
            transformLocation = glGetUniformLocation(s_program[3], "transform");
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
            viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
            projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
            objColor = glm::vec3(-1.0f, 1.0f, 1.0f);
            objcolorLocation = glGetUniformLocation(s_program[3], "objectColor");
            glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);

            glBindVertexArray(vao[3]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture5);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        //startbutton
        glUseProgram(s_program[4]);
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::mat4(1.0f);
        transformLocation = glGetUniformLocation(s_program[4], "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(s_program[4], "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        projectionLocation = glGetUniformLocation(s_program[4], "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objColor = glm::vec3(-1.0f, 1.0f, 1.0f);
        objcolorLocation = glGetUniformLocation(s_program[4], "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);

        glBindVertexArray(vao[4]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture6);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    draw_stuff();  //텍스트

    glutSwapBuffers(); // 화면에 출력하기
    glutPostRedisplay();
}

GLvoid reshape(int w, int h) {
    GLdouble size;
    GLdouble aspect;

    /* Use the whole window. */
    glViewport(0, 0, w, h);

    /* We are going to do some 2-D orthographic drawing. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    size = (GLdouble)((w >= h) ? w : h) / 2.0;
    if (w <= h) {
        aspect = (GLdouble)h / (GLdouble)w;
        glOrtho(-size, size, -size * aspect, size * aspect, -100000.0, 100000.0);
    }
    else {
        aspect = (GLdouble)w / (GLdouble)h;
        glOrtho(-size * aspect, size * aspect, -size, size, -100000.0, 100000.0);
    }

    /* Make the world and window coordinates coincide so that 1.0 in */
    /* model space equals one pixel in window space.                 */
    glScaled(aspect, aspect, 1.0);

    /* Now determine where to draw things. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   // glViewport(0, 0, w, h);
}

char* filetobuf(const char* file) {
    FILE* fptr;
    long length;
    char* buf;
    fopen_s(&fptr, file, "rb"); // Open file for reading
    if (!fptr) // Return NULL on failure
        return NULL;
    fseek(fptr, 0, SEEK_END); // Seek to the end of the file
    length = ftell(fptr); // Find out how many bytes into the file we are
    buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
    fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
    fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
    fclose(fptr); // Close the file
    buf[length] = 0; // Null terminator
    return buf; // Return the buffer
}

void make_vertexShader() {
    vertexsource = filetobuf("vertex.glsl");
    //--- 버텍스 세이더 객체 만들기
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    //--- 세이더 코드를 세이더 객체에 넣기
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    //--- 버텍스 세이더 컴파일하기
    glCompileShader(vertexshader);
    //--- 컴파일이 제대로 되지 않은 경우: 에러 체크
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
        cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
        //return false;
        //return;
        exit(0);
    }
}

void make_fragmentShader()
{
    fragmentsource = filetobuf("fragment.glsl");
    //--- 프래그먼트 세이더 객체 만들기
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    //--- 세이더 코드를 세이더 객체에 넣기
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    //--- 프래그먼트 세이더 컴파일
    glCompileShader(fragmentshader);
    //--- 컴파일이 제대로 되지 않은 경우: 컴파일make_fragmentShader 에러 체크
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
        cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
        //return false;
        //return;
        exit(0);
    }
}

void InitBuffer()
{
    glGenVertexArrays(5, vao); //--- VAO 를 지정하고 할당하기

    //바닥
    glBindVertexArray(vao[0]); //--- VAO를 바인드하기
    glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorShape), floorShape, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorColor), floorColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    //정육면체
    glBindVertexArray(vao[1]); //--- VAO를 바인드하기
    glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, vPosData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, vNormalData, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData, GL_STATIC_DRAW);




    //배경
    glBindVertexArray(vao[3]); //--- VAO를 바인드하기
    glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backGround), backGround, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말값 속성
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
    glEnableVertexAttribArray(2);

    //배경
    glBindVertexArray(vao[4]); //--- VAO를 바인드하기
    glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(startbutton), startbutton, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말값 속성
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
    glEnableVertexAttribArray(2);
}

void InitShader()
{
    make_vertexShader();
    make_fragmentShader();
    //-- shader Program
    for (int i = 0; i < 6; i++) {
        s_program[i] = glCreateProgram();
        glAttachShader(s_program[i], vertexshader);
        glAttachShader(s_program[i], fragmentshader);
        glLinkProgram(s_program[i]);
        //checkCompileErrors(s_program, "PROGRAM");
        //--- 세이더 삭제하기
        glDeleteShader(vertexshader);
        glDeleteShader(fragmentshader);
        //--- Shader Program 사용하기
        glUseProgram(s_program[i]);
    }
}

void InitTexture() {

    int width, height, nrChannels;
    unsigned char* data;
    int tLocation;
    //--- texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("sky.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glUseProgram(s_program[3]);
    tLocation = glGetUniformLocation(s_program[3], "texture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
    glUniform1i(tLocation, 6);
    stbi_image_free(data);

    //--- texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("samurai_gameover.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glUseProgram(s_program[3]);
    tLocation = glGetUniformLocation(s_program[3], "texture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
    glUniform1i(tLocation, 6);
    stbi_image_free(data);

    //--- texture 3
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("background.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glUseProgram(s_program[3]);
    tLocation = glGetUniformLocation(s_program[3], "texture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
    glUniform1i(tLocation, 6);
    stbi_image_free(data);

    //--- texture 4
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("start2.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glUseProgram(s_program[4]);
    tLocation = glGetUniformLocation(s_program[4], "texture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
    glUniform1i(tLocation, 6);
    stbi_image_free(data);

    //--- texture 5
    glGenTextures(1, &texture5);
    glBindTexture(GL_TEXTURE_2D, texture5);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("클리어.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glUseProgram(s_program[3]);
    tLocation = glGetUniformLocation(s_program[3], "texture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
    glUniform1i(tLocation, 6);
    stbi_image_free(data);

    //--- texture 6
    glGenTextures(1, &texture6);
    glBindTexture(GL_TEXTURE_2D, texture6);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("next.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glUseProgram(s_program[4]);
    tLocation = glGetUniformLocation(s_program[4], "texture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
    glUniform1i(tLocation, 6);
    stbi_image_free(data);
}

void Mouse(int button, int state, int x, int y)
{
    mouseX = (float)(x - (float)600 / 2.0) * (float)(1.0 / (float)(600 / 2.0));
    mouseY = -(float)(y - (float)600 / 2.0) * (float)(1.0 / (float)(600 / 2.0));

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (mouseX > -0.3 && mouseX < 0.3 && mouseY>-0.1 && mouseY < 0.1) {
            gamestart = false;
     
        }
    }
}

GLvoid ObjectTimer(int value) {

    if (net.getStart()) {
        //COLL_CHECK(narutoCoord, Object2Coord);
        //COLL_CHECK(narutoCoord, Object1Coord);

        num = rand() % 3 + 1; //1~3 

        if (set_Object1 == false && O1_pointz != M_pointz - 2.0f) {
            switch (num)
            {
            case 1:
                set_Object1 = true;
                O1_pointx = M_pointx - 8;
                left_Object = true;
                break;
            case 2:
                set_Object1 = true;
                O1_pointx = M_pointx - 2;
                mid_Object = true;
                break;
            case 3:
                set_Object1 = true;
                O1_pointx = M_pointx + 4;
                right_Object = true;
                break;
            }

        }
        if (set_Object2 == false && O2_pointz != M_pointz - 2.0f) {
            num2 = rand() % 2 + 1;
            if (left_Object == true) {
                switch (num2)
                {
                case 1:
                    set_Object2 = true;
                    O2_pointx = M_pointx + 3;
                    mid_Object = true;
                    break;
                case 2:
                    set_Object2 = true;
                    O2_pointx = M_pointx + 7;
                    right_Object = true;
                    break;
                }
            }
            else if (mid_Object == true) {
                switch (num2)
                {
                case 1:
                    set_Object2 = true;
                    O2_pointx = M_pointx - 4;
                    left_Object = true;
                    break;
                case 2:
                    set_Object2 = true;
                    O2_pointx = M_pointx + 7;
                    right_Object = true;
                    break;
                }
            }
            else if (right_Object == true) {
                switch (num2)
                {
                case 1:
                    set_Object2 = true;
                    O2_pointx = M_pointx - 4;
                    left_Object = true;
                    break;
                case 2:
                    set_Object2 = true;
                    O2_pointx = M_pointx + 3;
                    mid_Object = true;
                    break;
                }
            }
        }
        glutTimerFunc(1, ObjectTimer, value); // 타이머함수 재 설정
    }
}

bool object2_Height = true;  //위아래 튕기기 판단 위한 함수

GLvoid TimerFunction(int value) {
    if (net.getStart()) {
        //장애물1
        if (set_Object1 == false) {
            O1_pointz = M_pointz - 2.0f;
        }
        else
        {
            O1_pointz += 0.2f;
            if (O1_pointz >= 0.0)
            {
                O1_pointz = M_pointz - 2.0f;

                if (O1_pointx == M_pointx - 8)
                    left_Object = false;
                else if (O1_pointx = M_pointx - 2)
                    mid_Object = false;
                else if (O1_pointx = M_pointx + 4)
                    right_Object = false;

                set_Object1 = false;
            }
        }

        //장애물2
        if (set_Object2 == false) {
            O2_pointz = M_pointz - 2.0f;
            O2_pointy = 0.0f;
        }
        else
        {
            O2_pointz += 0.2f;
            if (object2_Height == true) {
                O2_pointy += 0.1f;
                if (O2_pointy >= 4.0f)
                    object2_Height = false;
            }
            else {
                O2_pointy -= 0.1f;
                if (O2_pointy <= 0.0f)
                    object2_Height = true;

            }

            if (O2_pointz >= 0.0)
            {
                O2_pointz = M_pointz - 2.0f;

                if (O2_pointx == M_pointx - 4)
                    left_Object = false;
                else if (O2_pointx = M_pointx + 3)
                    mid_Object = false;
                else if (O2_pointx = M_pointx + 7)
                    right_Object = false;

                set_Object2 = false;
            }

        }



        // 몬스터 이동 
        M_pointz -= 0.3f;
        if (M_boolt) {  //다리 회전 
            M_setat += setatspeed;
            if (M_setat >= 60.0f) {
                M_setat = 60.0f;
                M_boolt = false;
            }
        }
        else {
            M_setat -= setatspeed;
            if (M_setat <= -60.0f) {
                M_setat = -60.0f;
                M_boolt = true;
            }
        }
        //---------------------------------
        //뒤 장애물 이동 
        B_pointz -= 0.1f;



        if (boolj == true) {
            transj += jumpspeedup; //노트북 0.005
            if (transj > 2.3f)
                boolj = false;
        }

        else if (transj != 0.0f && boolj2) {
            transj -= jumpspeeddown;    //노트북 0.005
            if (transj < 0.0f) {
                transj = 0.0f;
                boolj2 = false;
            }
        }



        if (boolw) { //전진
            //pointz -= 0.07f;       //노트북 0.007
            pointz -= 0.2;
            
        }
        else if (boolw2) {
            pointz -= 0.1;
        }

   
        else if (boola || boolw || boold) {  //팔회전 
            if (boolt) {
                setat += setatspeed;
                if (setat >= 60.0f) {
                    setat = 60.0f;
                    boolt = false;
                }
            }
            else {
                setat -= setatspeed;
                if (setat <= -60.0f) {
                    setat = -60.0f;
                    boolt = true;
                }
            }
        }

    
        if (gameover == false && transj < -5) {      // 아래로 떨어졌을 때,사망 사운드
            //glEnd();
            sound5();
            gameover = true;
        }


     
        glutTimerFunc(1, TimerFunction, value); // 타이머함수 재 설정
    }
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    if (net.getStart()) {
        net.CS_MOVE(key);

    //    switch (key) {
    //    case 'W':
    //    case 'w':
    //        setam = 180.0f;
    //        boolw = true;
    //        boola = false;
    //        boold = false;
    //       // pointz -= 0.5;
    //        break;
    //    case 'S':
    //    case 's':
    //        setam = -180.0f;
    //        boolw = false;
    //        boolw2 = false;
    //        boola = false;
    //        boold = false;

    //        break;
    //    case 'A':
    //    case 'a':
    //        if(boolw == true || boolw2 == true)
    //            setam = -135.0f;
    //        else 
    //            setam = -90.0f;
    //    //    boolw = false;
    //        boola = true;
    //        boold = false;
    //        boolw = false;
    //        boolw2 = true;
    //        pointx -= 0.2;        //코딩용
    //        if (pointx <= -5.5) {
    //            pointx = -5.5;
    //        }
    //        break;
    //    case 'd':
    //    case 'D':
    //        if (boolw == true || boolw2 == true)
    //            setam = 135.0f;
    //        else
    //            setam = 90.0f;
    //   //     boolw = false;
    //        boola = false;
    //        boold = true;
    //        boolw = false;
    //        boolw2 = true;
    //        pointx += 0.2;        //코딩용
    //        if (pointx >= 5.5) {
    //            pointx = 5.5;
    //        }
    //        break;
    //    case 'I':
    //    case 'i':
    //        boolw = false;
    //        boold = false;
    //        break;
    //    case 'j':
    //    case 'J':
    //        sound3();
    //        if ( !boolj2) {
    //            if (transj >= 0.0) {
    //                boolj = true;
    //                boolj2 = true;
    //            }
    //        }
    //        break;

    //    case 'P':
    //    case 'p':
    //        glutLeaveMainLoop();
    //        break;
    //    case 27:    /* Esc - Quits the program. */
    //        printf("done.\n");
    //        exit(1);
    //        break;

    //    case ' ':    /* Space - toggles mode.     */
    //        mode = (mode == MODE_BITMAP) ? MODE_STROKE : MODE_BITMAP;
    //        font_index = 0;
    //        glutPostRedisplay();
    //        break;

    //    case '1':
    //    case '2':
    //    case '3':
    //    case '4':
    //    case '5':
    //    case '6':
    //    case '7':
    //        if (mode == MODE_BITMAP || key == '1' || key == '2') {
    //            font_index = key - '1';
    //        }
    //        glutPostRedisplay();
    //        break;
    //    }
    //    glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
    //}
    //else {
    //    if (key == 'r' || key == 'R') {
    //        if (gameover)
    //            restart();
    //    }
    }
}



void restart() {
    gameover = false;
    bool boolt = false;

    float setac = 0.0f;   //카메라 공전
    float setay = 0.0f;      //카메라 자전
    float transz = 0.0f;       //카메라 축이동


    //명령 관련
    boolw = false;
    boola = false;
    boolgravity = false;
    boold = false;
    boolj = false;
    boolj2 = false;
    gameclear = false;
    sound_6 = true;
    //주인공이동(x,y,z순)
    transj = 0.0f;
    pointx = 2.25f;
    pointz = 0.0f;

    narutoCoord[0] = glm::vec3(2.25f, 0.0f, 0.0f);


    setat = 0.0f;      //이동시 다리가 움직이는 애니메이션을 하기위한 각도
    setam = 180.0f;      //몸통 회전을 맡음
    setaRighthand = 60.0f;
    setaLefthand = 60.0f;

    map2 = false;
    version2 = false;
    version3 = false;


    speed = 0.17;
    jumpspeedup = 0.5;
    jumpspeeddown = 0.05;
    mainwirespeed = 0.07;
    subwirespeed = 0.035;
    setatspeed = 3.0f;        //다리 회전속도
}