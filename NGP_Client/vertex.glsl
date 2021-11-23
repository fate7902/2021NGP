#version 330

layout (location=0) in vec3 in_Position;		//위치 속성
layout (location=1) in vec3 in_Color;		//색상 속성
layout (location=2) in vec2 in_texture;		//텍스처 좌표

out vec3 ex_Color;
out vec2 TexCoord;
out vec3 Normal; //--- 노멀값을 프래그먼트 세이더로 보낸다.

//layout (location = 0) in vec3 vPos;


uniform mat4 transform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(){
	gl_Position = projectionTransform * viewTransform * transform * vec4(in_Position.x,in_Position.y,in_Position.z,1.0);
	ex_Color = in_Color;
	TexCoord=in_texture;	//텍스처값을 프래그먼트 세이더로 보낸다
};