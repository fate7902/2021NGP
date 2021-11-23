#version 330
//--- ex_Color: 버텍스 세이더에서 입력받는 색상 값
//--- gl_FragColor: 출력할 색상의 값으로 응용 프로그램으로 전달 됨.

in vec3 ex_Color; //--- 버텍스 세이더에게서 전달 받음
in vec2 TexCoord;
in vec3 Normal; //--- 노멀값을 프래그먼트 세이더로 보낸다.

out vec4 gl_FragColor; //--- 색상 출력

uniform vec3 objectColor;		//컬러값이 주어지지 않았으면 백색으로 초기화
uniform sampler2D texture1;

void main(void)
{
	if(objectColor.x==-1.0f){
	gl_FragColor = texture(texture1,TexCoord);
	//l_FragColor.a=1.0f;
	}
	else{
	vec3 result=objectColor;
	gl_FragColor = vec4(result,1.0);
	}
}