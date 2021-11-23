/*
11/24 으범이에게
    빌딩건물 추가시
        -#define BULDINGNUMBER 의 숫자 증가시키기
        -void InitMAP에서 buildingCoord(빌딩의 좌표)와 handrailCoord(난간의 좌표)추가시키기
*/

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

#define BUILDINGNUMBER 12        //빌딩(건물)의 개수

//건물의 좌표를 나타내줌
glm::vec3 buildingCoord[BUILDINGNUMBER];        //빌딩의 좌표
glm::vec3 handrailCoord[BUILDINGNUMBER];        //난간의 좌표

//가로 난간 한개 
glm::vec3 handrailCoord2[1];        //난간의 좌표
glm::vec3 clear[1];

//가로 건물과 난간 
#define BUILDINGNUMBER3 6        //빌딩(건물)의 개수
glm::vec3 buildingCoord3[BUILDINGNUMBER3];        //빌딩의 좌표
glm::vec3 handrailCoord3[BUILDINGNUMBER3];        //난간의 좌표

float nangansize = 3.0f;

void InitMap() {
    //0번 건물
    buildingCoord[0] = glm::vec3(5.5f, 1.5f, 0.0);
  //  handrailCoord[0] = glm::vec3(2.5f, 1.5f, 0.0);
    

    //1번 건물
    buildingCoord[1] = glm::vec3(-25.5f, 1.5f, 0.0);
  //  handrailCoord[1] = glm::vec3(0.0f, 1.5f, 0.1);
    
    //2번 건물
    buildingCoord[2] = glm::vec3(5.5f, 1.5f, -100 );
   // handrailCoord[2] = glm::vec3(-3.5f, 1.5f, 0);
    //3번 건물
    buildingCoord[3] = glm::vec3(-25.5f, 1.5f, -100 );
   // handrailCoord[3] = glm::vec3(-3.5f, 1.5f, 0);
    
    //4번 건물
    buildingCoord[4] = glm::vec3(5.5f, 1.5f, -200);
//    handrailCoord[4] = glm::vec3(2.5f, 1.5f, -11.5 * 4);
    //5번 건물
    buildingCoord[5] = glm::vec3(-25.5f, 1.5f, -200);
  //  handrailCoord[5] = glm::vec3(-3.5f, 1.5f, -11.5 * 5);

    //6번 건물
    buildingCoord[6] = glm::vec3(5.5f, 1.5f, -300);
    //7번 건물
    buildingCoord[7] = glm::vec3(-25.5f, 1.5f, -300);
    //8번 건물
    buildingCoord[8] = glm::vec3(5.5f, 1.5f, -400);
    //9번 건물
    buildingCoord[9] = glm::vec3(-25.5f, 1.5f, -400);
    //10번 건물
    buildingCoord[10] = glm::vec3(5.5f, 1.5f, -500);
    //11번 건물
    buildingCoord[11] = glm::vec3(-25.5f, 1.5f, -500);



    /**
    //6번 건물
    buildingCoord[6] = glm::vec3(5.5f, 1.5f, -11.5 * 6);
    handrailCoord[6] = glm::vec3(2.5f, 1.5f, -11.5 * 6);  */

    /*
    //가로전환 난간 1개
    handrailCoord2[0] = glm::vec3(4.5f, 1.5f, -11.5 * 6 - 5.5f);


    //8번 건물
    buildingCoord3[0] = glm::vec3(5.5f + 9.5, 1.5f, -11.5 * 6 - 2.5f);
    handrailCoord3[0] = glm::vec3(5.5f + 9.5, 1.5f, -11.5 * 6 - 5.5f);

    //9번 건물
    buildingCoord3[1] = glm::vec3(5.5f + 9.5 + 11.5, 1.5f, -11.5 * 6 - 2.5f - 11.0f);
    handrailCoord3[1] = glm::vec3(5.5f + 9.5 + 11.5, 1.5f, -11.5 * 6 - 5.5f - 5.0f);


    //10번 건물
    buildingCoord3[2] = glm::vec3(5.5f + 9.5 + 11.5 * 2, 1.5f, -11.5 * 6 - 2.5f - 11.0f);
    handrailCoord3[2] = glm::vec3(5.5f + 9.5 + 11.5 * 2, 1.5f, -11.5 * 6 - 5.5f - 5.0f);


    //11번 건물
    buildingCoord3[3] = glm::vec3(5.5f + 9.5 + 11.5 * 3, 1.5f, -11.5 * 6 - 2.5f);
    handrailCoord3[3] = glm::vec3(5.5f + 9.5 + 11.5 * 3, 1.5f, -11.5 * 6 - 5.5f);


    //12번 건물
    buildingCoord3[4] = glm::vec3(5.5f + 9.5 + 11.5 * 4, 1.5f, -11.5 * 6 - 2.5f - 11.0f);
    handrailCoord3[4] = glm::vec3(5.5f + 9.5 + 11.5 * 4, 1.5f, -11.5 * 6 - 5.5f - 5.0f);


    //13번 건물
    buildingCoord3[5] = glm::vec3(5.5f + 9.5 + 11.5 * 5, 1.5f, -11.5 * 6 - 2.5f);
    handrailCoord3[5] = glm::vec3(5.5f + 9.5 + 11.5 * 5, 1.5f, -11.5 * 6 - 5.5f);*/

    //클리어 
    clear[0] = glm::vec3(5.5f + 9.5 + 11.5 * 6, 1.5f, -11.5 * 6 - 7.0f);
}


void drawMap(GLuint program, int vertexCount, GLuint vao[], glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    glm::mat4 transformMatrix(1.0f);      //월드변환

    glm::mat4 basicChange(1.0f);         //기본축사용시
    glm::mat4 basicChange2(1.0f);         //기본축사용시

    glm::mat4 transMatrixj(1.0f);         //명령j/J
    glm::mat4 rotateMatrixm(1.0f);         //명령m/M
    glm::mat4 rotateMatrixt(1.0f);         //명령t/T
    glm::mat4 scalehandfoot(1.0f);         //손과 발의 사이즈 책정
    glm::mat4 transMatrixpoint(1.0f);      //몸통의 이동


    //색깔
    glm::vec3 objColor(1.0f, 1.0f, 1.0f);
    //버텍스 연결(유니폼 연결)
    unsigned int transformLocation;
    unsigned int viewLocation;
    unsigned int projectionLocation;
    unsigned int objcolorLocation;


    glUseProgram(program);
    for (int i = 0; i < 2; i++) {
        //건물본체

        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화   
        basicChange = glm::translate(basicChange, glm::vec3(1.50f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)   Y -0.5
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(5.0f, 20.0f, 100.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord[i].x, buildingCoord[i].y, buildingCoord[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.7f, 0.5f, 0.0f);
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

       /*//난간
        basicChange = glm::mat4(1.0f);      //초기화   
        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));   //원점으로 이동(정육면체의 중앙이 원점)
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(2.0f, 1.0f, nangansize));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(handrailCoord[i].x, handrailCoord[i].y, handrailCoord[i].z));   //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.3f, 0.3f, 0.3f);
        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        viewLocation = glGetUniformLocation(program, "viewTransform");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        projectionLocation = glGetUniformLocation(program, "projectionTransform");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);*/
    }

    for (int i = 2; i < 4; i++) {
        //건물본체

        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화   
        basicChange = glm::translate(basicChange, glm::vec3(1.50f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)   Y -0.5
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(5.0f, 20.0f, 100.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord[i].x, buildingCoord[i].y, buildingCoord[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.0f, 0.8f, 0.5f);
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

 
    }
    for (int i =4; i < 6; i++) {
        //건물본체

        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화   
        basicChange = glm::translate(basicChange, glm::vec3(1.50f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)   Y -0.5
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(5.0f, 20.0f, 100.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord[i].x, buildingCoord[i].y, buildingCoord[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.0f, 0.0f, 0.5f);
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


    }
    for (int i = 6; i < 8; i++) {
        //건물본체

        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화   
        basicChange = glm::translate(basicChange, glm::vec3(1.50f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)   Y -0.5
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(5.0f, 20.0f, 100.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord[i].x, buildingCoord[i].y, buildingCoord[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.7f, 0.5f, 0.0f);
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
    }


    for (int i = 8; i < 10; i++) {
        //건물본체

        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화   
        basicChange = glm::translate(basicChange, glm::vec3(1.50f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)   Y -0.5
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(5.0f, 20.0f, 100.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord[i].x, buildingCoord[i].y, buildingCoord[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.0f, 0.8f, 0.5f);
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


    }
    for (int i = 10; i < 12; i++) {
        //건물본체

        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화   
        basicChange = glm::translate(basicChange, glm::vec3(1.50f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)   Y -0.5
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(5.0f, 20.0f, 100.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord[i].x, buildingCoord[i].y, buildingCoord[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.0f, 0.0f, 0.5f);
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


    }
    /*
    //회전부분 난간
    for (int i = 0; i < 1; i++) {


        //난간
        basicChange = glm::mat4(1.0f);      //초기화   
        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));   //원점으로 이동(정육면체의 중앙이 원점)
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화

        basicChange = glm::scale(basicChange, glm::vec3(6.0f, 1.0f, 2.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(handrailCoord2[i].x, handrailCoord2[i].y, handrailCoord2[i].z));   //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.3f, 0.3f, 0.3f);
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
    }

    for (int i = 0; i < BUILDINGNUMBER3; i++) {


        //건물본체


        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)
        transformMatrix = basicChange * transformMatrix;

        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(10.0f, 20.0f, 5.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기

        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(buildingCoord3[i].x, buildingCoord3[i].y, buildingCoord3[i].z));     //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.7f, 0.7f, 0.7f);
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


        //난간
        basicChange = glm::mat4(1.0f);      //초기화   
        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));   //원점으로 이동(정육면체의 중앙이 원점)
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(nangansize, 1.0f, 2.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;   //크기
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(handrailCoord3[i].x, handrailCoord3[i].y, handrailCoord3[i].z));   //건물이 있어야 하는 위치로 이동
        transformMatrix = basicChange * transformMatrix;

        objColor = glm::vec3(0.3f, 0.3f, 0.3f);
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
    }*/

    //클리어 공간 

    transformMatrix = glm::mat4(1.0f);   //초기화
    basicChange = glm::mat4(1.0f);      //초기화
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));  //원점으로 이동(정육면체의 중앙이 원점)
    transformMatrix = basicChange * transformMatrix;

    basicChange = glm::mat4(1.0f);      //초기화
    basicChange = glm::scale(basicChange, glm::vec3(10.0f, 1.0f, 5.0f));      //사이즈 조절
    transformMatrix = basicChange * transformMatrix;   //크기

    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(clear[0].x, clear[0].y, clear[0].z));     //건물이 있어야 하는 위치로 이동
    transformMatrix = basicChange * transformMatrix;

    objColor = glm::vec3(0.0f, 0.0f, 1.0f);
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
}
