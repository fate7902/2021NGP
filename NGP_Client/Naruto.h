#pragma once
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <ctime>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "sound.h"
#include "map.h"
#include "network.h"

bool gamestart = true;
bool gameover = false;
bool version2 = false;
bool version3 = false;
bool perfectclear = false;

/*
나루토의 위치 : narutoCoord(컴그팀플.cpp drawScene에서 계속 해서 업데이트)
*/

//명령 관련
bool boolw = false;
bool boola = false;
bool boolgravity = false;
bool boold = false;
bool boolj = false;
bool boolj2 = false;
bool gameclear = false;
bool sound_6 = true;
bool sound_7 = true;
//주인공이동(x,y,z순)
float transj = 0.0f;
float pointx = 2.25f;
float pointz = 0.0f;

glm::vec3 narutoCoord[3]{ glm::vec3(0.0f, 0.0f, 0.0f) };

glm::vec3 wireCoord[2] = {
   glm::vec3(0.0f,0.0f,0.0f),
   glm::vec3(0.0f,0.0f,0.0f)
};      //와이어의 좌표 [0] : 발사된 손의 좌표, [2] : 부딪힌 곳의 좌표

float setat = 0.0f;      //이동시 다리가 움직이는 애니메이션을 하기위한 각도
float setam = 180.0f;      //몸통 회전을 맡음
float setaRighthand = 60.0f;
float setaLefthand = 60.0f;

bool map2 = false;

bool leftWire = false;      //왼쪽 와이어가 발사되었는지 확인
bool rightWire = false;     //우측 와이어가 발사되었는지 확인

void drawNaruto(GLuint program, int vertexCount, GLuint vao[], GLuint vbo[],glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int id) {
    //for (const auto& client : net.clients) {
          

        //행렬 
        glm::mat4 transformMatrix(1.0f);      //월드변환
        glm::mat4 basicChange(1.0f);         //기본축사용시
        glm::mat4 basicChange2(1.0f);         //기본축사용시

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

        //명령
        scalehandfoot = glm::scale(basicChange, glm::vec3(0.1f, 0.5f, 0.1f));         //손과발의 크기 축소(기본행렬)
        transMatrixpoint = glm::translate(transMatrixpoint, glm::vec3(narutoCoord[id].x, narutoCoord[id].y, narutoCoord[id].z));
        rotateMatrixm = glm::rotate(rotateMatrixm, glm::radians(setam), glm::vec3(0.0f, 1.0f, 0.0f));      //몸통 회전


        glUseProgram(program);
        //바디
        //월드변환 및 각 행렬 대입
        basicChange = glm::mat4(1.0f);      //초기화   
        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(1.0f, 0.5f, 1.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;


        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 0.65f, 0.0));      //바닥의 윗면으로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;      //난간위로 이동

        objColor = glm::vec3(1.0f, 0.2f, 0.2f);

        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);


        //머리
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));      //원점이동
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(0.7f, 0.3f, 0.7f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 1.05f, 0.0));      //1층의 윗면으로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.8f, 0.8f, 0.8f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);

        //코
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(0.3f, 0.1f, 0.1f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 1.05f, 0.355));      //1층의 윗면으로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.0f, 0.0f, 0.0f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);

        //우측손
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        transformMatrix = scalehandfoot * transformMatrix;      //손크기
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setaRighthand), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 팔의 움직임
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.55f, 0.9f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;         //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.2f, 0.2f, 1.0f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);

        //좌측쪽손
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        transformMatrix = scalehandfoot * transformMatrix;      //손크기
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::mat4(1.0f);
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setaLefthand), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 팔의 움직임
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.55f, 0.9f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.2f, 0.2f, 1.0f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);

        //왼쪽 발
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        transformMatrix = scalehandfoot * transformMatrix;      //크기
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::mat4(1.0f);
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(-setat), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 발의 움직임
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.25f, 0.5f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(1.0f, 1.0f, 0.2f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);

        //오른쪽 발
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        transformMatrix = scalehandfoot * transformMatrix;      //크기
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::mat4(1.0f);
        if (leftWire || rightWire)
            rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(-setat), glm::vec3(1.0f, 0.0f, 0.0f));
        else
            rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setat), glm::vec3(1.0f, 0.0f, 0.0f));

        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.25f, 0.5f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(1.0f, 1.0f, 0.2f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);



        //와이어
        if (leftWire || rightWire) {
            transformMatrix = glm::mat4(1.0f);

            objColor = glm::vec3(0.0f, 0.0f, 0.0f);

            transformLocation = glGetUniformLocation(program, "transform");
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
            objcolorLocation = glGetUniformLocation(program, "objectColor");
            glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
            glBindVertexArray(vao[2]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(wireCoord), wireCoord, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_LINES, 0, 2);

        }

}

void spiderNaruto() {
    if (!map2) {
        if (leftWire) {
            setaLefthand = -90.0f;
            setam = 225.0f;
            wireCoord[0] = glm::vec3(narutoCoord[0].x - 0.3, narutoCoord[0].y + 2.9, narutoCoord[0].z - 0.45);

            //와이어 충돌처리
            wireCoord[1] = glm::vec3(wireCoord[0].x - 6.0f, wireCoord[0].y, wireCoord[0].z - 3.0f);


            bool crushCheck = false;
            for (int i = 0; i < BUILDINGNUMBER; i++) {
                if (buildingCoord[i].x < 0) {
                    if (buildingCoord[i].z - 5.0f < wireCoord[1].z && wireCoord[1].z < buildingCoord[i].z + 5.0f)
                        crushCheck = true;
                }
            }

            if (!crushCheck) {
                leftWire = false;
                setaLefthand = 60.0f;
                setam = 180.0f;
            }
        }
        if (rightWire) {
            setaRighthand = -90.0f;
            setam = 135.0f;
            wireCoord[0] = glm::vec3(narutoCoord[0].x + 0.3, narutoCoord[0].y + 2.9, narutoCoord[0].z - 0.45);

            //와이어 충돌처리
            wireCoord[1] = glm::vec3(wireCoord[0].x + 6.0f, wireCoord[0].y, wireCoord[0].z - 3.0f);
            bool crushCheck = false;
            for (int i = 0; i < BUILDINGNUMBER; i++) {
                if (buildingCoord[i].x > 0) {
                    if (buildingCoord[i].z - 5.0f < wireCoord[1].z && wireCoord[1].z < buildingCoord[i].z + 5.0f)
                        crushCheck = true;
                }
            }

            if (!crushCheck) {
                rightWire = false;
                setaRighthand = 60.0f;
                setam = 180.0f;
            }
        }
    }
    else if(map2){
        if (leftWire) {
            setaLefthand = -90.0f;
            setam = 135.0f;
            wireCoord[0] = glm::vec3(narutoCoord[0].x + 0.45, narutoCoord[0].y + 2.9, narutoCoord[0].z - 0.3);

            //와이어 충돌처리
            wireCoord[1] = glm::vec3(wireCoord[0].x + 3.0f, wireCoord[0].y, wireCoord[0].z - 6.0);


            bool crushCheck = false;
            for (int i = 0; i < BUILDINGNUMBER; i++) {
                if (buildingCoord3[i].z < -11.5*6-8.0) {
                    if (buildingCoord3[i].x - 5.0f < wireCoord[1].x && wireCoord[1].x < buildingCoord3[i].x + 5.0f)
                        crushCheck = true;
                }
            }

            //건물이 없을때 원상복귀
            if (!crushCheck) {
                leftWire = false;
                setaLefthand = 60.0f;
                setam = 90.0f;
            }
        }
        if (rightWire) {
            setaRighthand = -90.0f;
            setam = 45.0f;
            wireCoord[0] = glm::vec3(narutoCoord[0].x + 0.45, narutoCoord[0].y + 2.9, narutoCoord[0].z + 0.3);

            //와이어 충돌처리
            wireCoord[1] = glm::vec3(wireCoord[0].x + 3.0f, wireCoord[0].y, wireCoord[0].z + 6.0);


            bool crushCheck = false;
            for (int i = 0; i < BUILDINGNUMBER; i++) {
                if (buildingCoord3[i].z > -11.5 * 6 - 8.0) {
                    if (buildingCoord3[i].x - 5.0f < wireCoord[1].x && wireCoord[1].x < buildingCoord3[i].x + 5.0f)
                        crushCheck = true;
                }
            }

            //건물이 없을때 원상복귀
            if (!crushCheck) {
                rightWire = false;
                setaRighthand = 60.0f;
                setam = 90.0f;
            }
        }
    }
}

void gravityNaruto() {
    if (!boolj2) {
        if (!map2) {
            boolgravity = true;
            for (int i = 0; i < BUILDINGNUMBER; i++) {
                if (handrailCoord[i].z - (nangansize/2.0) < narutoCoord[0].z && narutoCoord[0].z < handrailCoord[i].z + (nangansize / 2.0)) { //캐릭터가 건물 난간위에 있을때
                    if (handrailCoord[i].x > 0) {
                        if (narutoCoord[0].x >= 2.25-0.6 && narutoCoord[0].x <= 2.25 + 0.6) {
                            boolgravity = false;
                        }
                    }
                    else {
                        if (narutoCoord[0].x >= -2.25 - 0.6 && narutoCoord[0].x <= -2.25 + 0.6) {
                            boolgravity = false;
                        }
                    }
                }
            }

            for (int i = 0; i < 1; i++) {
                if (handrailCoord2[i].x - 3.0f < narutoCoord[0].x && narutoCoord[0].x < handrailCoord2[i].x + 3.0f) { //캐릭터가 건물 난간위에 있을때
                    if (handrailCoord3[i].z > -11.5 * 6 - 8) {
                        if (narutoCoord[0].z <= -11.5 * 6 - 4.5 + 0.3 && narutoCoord[0].z >= -11.5 * 6 - 5.75 - 0.6) {
                            boolgravity = false;
                        }
                    }
                }
            }

            if (leftWire || rightWire)
                boolgravity = false;
        }
        else {
            boolgravity = true;

            for (int i = 0; i < 1; i++) {
                if (handrailCoord2[i].x - 3.0f < narutoCoord[0].x && narutoCoord[0].x < handrailCoord2[i].x + 3.0f) { //캐릭터가 건물 난간위에 있을때
                    if (handrailCoord3[i].z > -11.5 * 6 - 8) {
                        if (narutoCoord[0].z <= -11.5 * 6 - 5.75 + 0.6 && narutoCoord[0].z >= -11.5 * 6 - 5.75 - 0.6) {
                            boolgravity = false;
                        }
                    }
                    else {
                        if (narutoCoord[0].z <= -11.5 * 6 - 10.25 + 0.6 && narutoCoord[0].z >= -11.5 * 6 - 10.25 - 0.6) {
                            boolgravity = false;
                        }
                    }
                }
            }


            for (int i = 0; i < BUILDINGNUMBER3; i++) {
                if (handrailCoord3[i].x - (nangansize / 2.0) < narutoCoord[0].x && narutoCoord[0].x < handrailCoord3[i].x + (nangansize / 2.0)) { //캐릭터가 건물 난간위에 있을때
                    if (handrailCoord3[i].z > -11.5*6-8) {
                        if (narutoCoord[0].z <= -11.5*6-5.75+0.6 && narutoCoord[0].z >= -11.5 * 6 - 5.75 - 0.6) {
                            boolgravity = false;
                        }
                    }
                    else {
                        if (narutoCoord[0].z <= -11.5 * 6 - 10.25+0.6 && narutoCoord[0].z >= -11.5 * 6 - 10.25 - 0.6) {
                            boolgravity = false;
                        }
                    }
                }
            }

            if (clear[0].x - 1.5f < narutoCoord[0].x && narutoCoord[0].x < clear[0].x + 1.5f ) {
                boolgravity = false;
                boolw = false;
                gameclear = true;
                if (version3) {

                    perfectclear = true;
                    if (sound_7) {
                        sound7();
                        sound_7 = false;
                    }

         
                }
                if (sound_6 && version3 == false) {
                    sound6();
                    sound_6 = false;
                }
            }
            if (leftWire || rightWire)
                boolgravity = false;
        }
    }
    if (transj < 0.0) {
        boolgravity = true;
     
    }
}